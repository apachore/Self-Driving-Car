/*===========================================================================================
 Name        : gps.cpp
 Author      : Amit Pachore
 Version     : 1.0
 Description : Program to Get current coordinates form GPS Module,
               Calculate Distance,
=============================================================================================
*/

#include "gps.hpp"
#include "handlers.hpp"

#define FetchCheckpointDistance 20
#define ZeroCoordinate 0.0
#define CheckPointDistance 200

extern bool BootReplySent;
extern uint8_t Received_Checkpoint_Count;
extern uint16_t Total_Distance_To_Travel;
uint16_t current_bearing = 0;


//Commented printf statements are added for testing purpose

gpsTask::gpsTask(uint8_t priority) : scheduler_task("gps", 4*512, priority),
gps_uart(Uart3::getInstance()),
gps_data_q(NULL),
midpoint_q(NULL)
{
    gps_uart.init(gps_uart_BaudRate,gps_rx_q_size,1);
}

bool gpsTask::init(void)
{
    logger_init(PRIORITY_LOW);
    gps_data_q = xQueueCreate(2, sizeof(coordinates));
    addSharedObject("gps_queue", gps_data_q);
    midpoint_q = xQueueCreate(2, sizeof(coordinates));
    addSharedObject("Middle_Checkpoint_q", midpoint_q);
    return (NULL != gps_data_q);
}

float /*gpsTask::*/ToRadians(float InDegrees)
{
    return(InDegrees*(M_PI/180));
}

float /*gpsTask::*/ToDegrees(float InRadians)
{
    return(InRadians*(180/M_PI));
}

uint16_t /*gpsTask::*/calculateBearing(coordinates point_2,coordinates point_1)
{
    float x,y;
    uint16_t bearing;
    y = sin(ToRadians(point_2.longitude-point_1.longitude))*cos(ToRadians(point_2.latitude));
    x = (cos(ToRadians(point_1.latitude))*sin(ToRadians(point_2.latitude))) - (sin(ToRadians(point_1.latitude))*cos(ToRadians(point_2.latitude))*cos(ToRadians(point_2.longitude-point_1.longitude)));
    bearing = (int)(ToDegrees(atan2(y,x))+360)%360;
    return(bearing);

}
uint16_t /*gpsTask::*/calculateCheckpointDistance(coordinates point_2,coordinates point_1)
{
    float a,c;
    uint16_t distance;
    a = pow(sin(ToRadians(point_2.latitude-point_1.latitude)/2),2) + (cos(ToRadians(point_1.latitude))*cos(ToRadians(point_2.latitude))*pow(sin(ToRadians(point_2.latitude-point_1.latitude)/2),2));
    c = 2 * atan2(sqrt(a),sqrt(1-a));
    distance = Radius * c;
    return(distance);
}

coordinates getCheckpoint_AtCheckPointDistance(coordinates checkpoint,uint16_t bearing)
{
    coordinates midCheckpoint;
    midCheckpoint.latitude = ToDegrees(asin((sin(ToRadians(checkpoint.latitude))*cos((CheckPointDistance/Radius)))+(cos(ToRadians(checkpoint.latitude))*sin((CheckPointDistance/Radius))*cos(ToRadians(bearing)))));
    //λ2 = λ1 + atan2( sin θ ⋅ sin δ ⋅ cos φ1, cos δ − sin φ1 ⋅ sin φ2 )
    midCheckpoint.longitude = ToDegrees(ToRadians(checkpoint.longitude) + atan2(sin(ToRadians(bearing))*sin((CheckPointDistance/Radius))*cos(ToRadians(checkpoint.latitude)),(cos((CheckPointDistance/Radius))-sin(ToRadians(checkpoint.latitude))*sin(ToRadians(midCheckpoint.latitude)))));
    return midCheckpoint;
}

bool gpsTask::run(void *p)
{
    //printf("Inside GPS Task Run\n");
    if( gps_uart.gets(GPS_Data_Receive, sizeof(GPS_Data_Receive),1000))
    {
        //printf("Inside Receive IF Statement\n");
        //printf("Data from GPS: %s\n",GPS_Data_Receive);

        uint8_t i=0;
        coordinates degrees;
        char *token;

        //Parse Received String and get string data in variables
        token = strtok(GPS_Data_Receive,",");
        while (token != NULL)
        {
            GPS_parsed_data[i] = token;
            token = strtok(NULL,",");
            i++;
        }

        //Get number of satellite locks
        sscanf(GPS_parsed_data[7],"%d",&satelite_connections);
        //LD.setNumber(satelite_connections);

        /* If number satellite locks are valid then only proceed with coordinate
         * conversion to degrees and sending it over queue*/
        if (satelite_connections>2)
        {
            // Condition inserted before setting up module to Only GGA messages
            //if (!strcmp(GPS_parsed_data[0],"$GPGGA"))
            //{
                //LE.toggle(1);
                //printf("GPS data:%s\n",GPS_Data_Receive);
                //printf("GPS Message type: %s\n",GPS_parsed_data[0]);

                sscanf(GPS_parsed_data[2],"%f",&gps_readings.latitude);
                sscanf(GPS_parsed_data[4],"%f",&gps_readings.longitude);
                //printf("GPS reading Latitude: %f\n",gps_readings.latitude);
                //printf("GPS reading Longitude: %f\n",gps_readings.longitude);

                /*Readings are in 3721.1831,N,12153.5919,W format in which
                 *3721.1831 = 37 degrees and 21.1831 minutes
                 *To convert degree minute format to degrees below calculations are programmed
                 *e.g gps_readings = 3721.1831/100 = 37.211831
                 *floor will give 37 degrees
                 *current_gps_data.latitude = degrees.latitude + (((gps_readings.latitude-degrees.latitude)*100)/60);
                 *current_gps_data.latitude = 37 + (((37.211831 - 37)*100)/60) = 37.353050*/
                gps_readings.latitude /=100;
                gps_readings.longitude /=100;
                degrees.latitude = floor(gps_readings.latitude);
                degrees.longitude = floor(gps_readings.longitude);
                current_gps_data.latitude = degrees.latitude + (((gps_readings.latitude-degrees.latitude)*100)/60);
                current_gps_data.longitude = degrees.longitude + (((gps_readings.longitude-degrees.longitude)*100)/60);

                if (*GPS_parsed_data[3] == 'S')
                    current_gps_data.latitude *= -1;
                if (*GPS_parsed_data[5] == 'W')
                    current_gps_data.longitude *= -1;
//                printf("Latitude: %f\n",current_gps_data.latitude);
//                printf("Longitude: %f\n",current_gps_data.longitude);

                CANTransmit(TSourceCoordinates,(uint8_t*)&current_gps_data,sizeof(coordinates));

                if (!xQueueSend(gps_data_q, &current_gps_data, 0))
                {
                // unexpected led
                   LOG_ERROR("Error in Writing current GPS data to queue\n");
                }
        }

    }
    else {
//        // error case
    }
    return true;
}



Distance_Data GPS_Calculations()
{
    //gpsTask gpsTaskInstance()
    QueueHandle_t gps_data_q = scheduler_task::getSharedObject("gps_queue");
    QueueHandle_t Checkpoint_q = scheduler_task::getSharedObject("CheckpointsQueue");
    QueueHandle_t Mid_Checkppoint_q = scheduler_task::getSharedObject("Middle_Checkpoint_q");
    coordinates current_gps_data;
    Distance_Data Current_Distances;
    //uint16_t Current_Checkpoint_Distance,Total_Distance_Remaining;

    static uint16_t Previous_Checkpoint_Distnace=0;
    static uint16_t Total_Checkpoint_Distnace=0;
    static uint16_t Total_Distance_Traveled=0;
    static coordinates checkpoint;
    static bool Fetch_Checkpoint = 1;
    static bool first_after_cp_fetch;
    static uint8_t Fetched_Checkpoint_Count;

    /*printf("Total_Distance_Traveled:%d \nTotal_Checkpoint_Distnace:%d \nPrevious_Checkpoint_Distnace:%d \nTotal_Distance_To_Travel:%d \n",
            Total_Distance_Traveled,Total_Checkpoint_Distnace,Previous_Checkpoint_Distnace,Total_Distance_To_Travel);*/
    if (NULL == gps_data_q)
    {
        //light_up_the_project_blown_led();
    }
    else if (xQueueReceive(gps_data_q, &current_gps_data, 0))
    {
       if (BootReplySent)
        {
            LOG_INFO("%f %f ",current_gps_data.latitude,current_gps_data.longitude);
            //LE.toggle(2);
              printf("%f  %f\n",current_gps_data.latitude,current_gps_data.longitude);
            //printf("Longitude: %f\n",current_gps_data.longitude);

            if (Fetch_Checkpoint)
            {
                if (xQueueReceive(Mid_Checkppoint_q, &checkpoint, 0))
                {
                    first_after_cp_fetch = 1;
                    Fetch_Checkpoint = 0;
                }
                else if (xQueueReceive(Checkpoint_q, &checkpoint, 0))
                {
                    Fetch_Checkpoint = 0;
                    Fetched_Checkpoint_Count++;
                    first_after_cp_fetch = 1;
                }
                else Fetch_Checkpoint = 1;
            }
            //LD.setNumber(Received_Checkpoint_Count/*-Fetched_Checkpoint_Count*/);
            if((Received_Checkpoint_Count - Fetched_Checkpoint_Count) == 0)
                Current_Distances.is_last_checkpoint = 1;

            if((checkpoint.latitude != ZeroCoordinate) && (checkpoint.longitude != ZeroCoordinate))
            {
                Current_Distances.Current_Checkpoint_Distance = calculateCheckpointDistance(checkpoint,current_gps_data);
                //LOG_INFO("Current Distance: %d",Current_Distances.Current_Checkpoint_Distance);
                //printf("Current Checkpoint Distance: %d feet\n",Current_Distances.Current_Checkpoint_Distance);
                current_bearing = calculateBearing(checkpoint,current_gps_data);
                //LOG_INFO("Current Bearing: %d",current_bearing);
                //printf("Current Bearing: %d degrees\n",current_bearing);
                if(!first_after_cp_fetch)
                {
                    Total_Distance_Traveled = Total_Distance_Traveled + (Previous_Checkpoint_Distnace - Current_Distances.Current_Checkpoint_Distance);
                }
                if (first_after_cp_fetch)
                {
                    Previous_Checkpoint_Distnace = Current_Distances.Current_Checkpoint_Distance;
                    Total_Checkpoint_Distnace = Current_Distances.Current_Checkpoint_Distance;
                    if(Current_Distances.Current_Checkpoint_Distance>CheckPointDistance)
                    {
                        if (!xQueueSend(Mid_Checkppoint_q, &checkpoint, 0))
                        {
                            LOG_ERROR("Mid_Checkpoint Queue Send Failed");
                        }
                        checkpoint = getCheckpoint_AtCheckPointDistance(current_gps_data,current_bearing);
                        LOG_INFO("Calc CP:%f  %f\n",checkpoint.latitude,checkpoint.longitude);
                    }
                    first_after_cp_fetch = 0;
                }

                //printf("Total Checkpoint Distance: %d feet\n",previous_checkpoint_distnace);
                if (Current_Distances.Current_Checkpoint_Distance < FetchCheckpointDistance)
                    Fetch_Checkpoint = 1;


                //LOG_INFO("Total Distance Traveled: %d",Total_Distance_Traveled);
                //printf("Total Distance Traveled: %d",Total_Distance_Traveled);

                Previous_Checkpoint_Distnace = Current_Distances.Current_Checkpoint_Distance;
                Current_Distances.Total_Travelled_Distance = /*Total_Distance_To_Travel - */Total_Distance_Traveled;

                //printf("Total Distance Remaining: %d",Current_Distances.Total_Distance_Remaining);

                LOG_INFO("CheckPoint:%f  %f\n",checkpoint.latitude,checkpoint.longitude);
                LOG_INFO("GPS:%d  %d  %d\n",Current_Distances.Total_Travelled_Distance,
                        Current_Distances.Current_Checkpoint_Distance,current_bearing);
                printf("%d  %d  %d\n",Current_Distances.Total_Travelled_Distance,
                       Current_Distances.Current_Checkpoint_Distance,current_bearing);

                return Current_Distances;
                //CANTransmit(TFinalAndNextCheckpointDistance,(uint8_t*)&Current_Distances,sizeof(Current_Distances));
            }
        }
    }
}


/*
void copyLogtoMMCard()
{
    unsigned int readTimeMs = 0;
    unsigned int writeTimeMs = 0;
    unsigned int bytesTransferred = 0;
    FRESULT copyStatus = Storage::copy("0:log.csv", "1:log.csv",
            &readTimeMs, &writeTimeMs, &bytesTransferred);

    if(FR_OK != copyStatus) {
        printf("Error %u copying |%s| -> |%s|\n", copyStatus, "0:log.csv", "1:log.csv");
    }
    else {
        printf("Finished!  Read: %u Kb/sec, Write: %u Kb/sec\n",
                bytesTransferred/(0 == readTimeMs  ? 1 : readTimeMs),
                bytesTransferred/(0 == writeTimeMs ? 1 : writeTimeMs));
    }
    LOG_FLUSH();
    //printf("Delete '%s' : %s\n","0:log.csv", (FR_OK == f_unlink("0:log.csv")) ? "OK" : "ERROR");
    //crtNewLogFile();

}

bool crtNewLogFile()
{
//    const char end_file = '~';
//        int timeout_ms = OS_MS(10 * 10);

        FIL file;
        if (FR_OK != f_open(&file, "log.csv", FA_WRITE | FA_CREATE_ALWAYS)) {
           // printf("Unable to open '%s' to write the file\n", "log.csv");
            return true;
        }

        char c = 0;
        UINT bw = 0;
        //printf("End the file by using %c character.  %i is the timeout\n", end_file, timeout_ms);
        //printf("Sorry, no backspace support :(\n");

        //while (getChar(&c, timeout_ms) && c != end_file) {
            if (FR_OK != f_write(&file, &c, 1, &bw) || 1 != bw) {
                //printf("Error occurred while writing the file\n");
            }
//            else {
//                putChar(c);
//            }
//        }

        f_close(&file);
}
*/
