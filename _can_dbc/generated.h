/// DBC file: Group2.dbc    Self node: MASTER
/// This file should be included by a source file, for example: #include "generated.c"
#include <stdbool.h>
#include <stdint.h>



/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} msg_hdr_t; 

static const msg_hdr_t MASTER_TX_MOTORIO_CMD_HDR =            {  220, 4 };
static const msg_hdr_t ANDROID_TX_KILL_REQUEST_HDR =          {  010, 0 };
static const msg_hdr_t MASTER_TX_KILL_MESSAGE_HDR =           {  020, 0 };
static const msg_hdr_t ANDROID_TX_STOP_REQUEST_HDR =          {  030, 0 };
static const msg_hdr_t MASTER_TX_STOP_COMMAND_HDR =           {  040, 0 };
static const msg_hdr_t SENSOR_TX_SONARS_HDR =                 {  210, 4 };
static const msg_hdr_t ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_HDR = {  230, 4 };
static const msg_hdr_t ANDROID_TX_SEND_CHECKPOINTS_HDR =      {  240, 8 };
static const msg_hdr_t GEO_TX_DISTANCE_HDR =                  {  250, 4 };
static const msg_hdr_t GEO_TX_TURN_ANGLE_DIRECTION_HDR =      {  260, 2 };
static const msg_hdr_t ANDROID_TX_RUN_PAUSE_HDR =             {  270, 0 };
static const msg_hdr_t MOTORIO_TX_HEARTBEAT__HDR =            {  280, 0 };
static const msg_hdr_t GEO_TX_HEARTBEAT_HDR =                 {  281, 0 };
static const msg_hdr_t ANDROID_TX_HEARTBEAT_HDR =             {  282, 0 };
static const msg_hdr_t SENSOR_TX_HEARTBEAT_HDR =              {  283, 0 };
static const msg_hdr_t MASTER_TX_DESTINATION_REACHED_HDR =    {  410, 0 };
static const msg_hdr_t SENSOR_TX_BATTERY_STATUS_HDR =         {  420, 1 };
static const msg_hdr_t MASTER_TX_BOOT_REQUEST_HDR =           {  610, 0 };
static const msg_hdr_t MOTORIO_TX_BOOT_REPLY_MOTORIO_HDR =    {  620, 0 };
static const msg_hdr_t SENSOR_TX_BOOT_REPLY_SENSOR_HDR =      {  621, 0 };
static const msg_hdr_t GEO_TX_BOOT_REPLY_GEO_HDR =            {  622, 0 };
static const msg_hdr_t ANDROID_TX_BOOT_REPLY_ANDROID_HDR =    {  623, 0 };
static const msg_hdr_t MASTER_TX_BOOT_STATUS_HDR =            {  630, 0 };
static const msg_hdr_t GEO_TX_SOURCE_COORDINATE_HDR =         {  650, 8 };


/// Message: MOTORIO_CMD from 'MASTER', DLC: 4 byte(s), MID: 220
typedef struct {
    uint8_t MOTORIO_CMD_LeftRightdirection; ///< B7:0  Min: 0 Max: 2   Destination: MOTORIO
    uint8_t MOTORIO_CMD_LevelOfDirection; ///< B15:8  Min: 0 Max: 5   Destination: MOTORIO
    uint8_t MOTORIO_CMD_FrontBackDirection; ///< B23:16  Min: 0 Max: 2   Destination: MOTORIO
    uint8_t MOTORIO_CMD_LevelOfSpeed;    ///< B31:24  Min: 0 Max: 5   Destination: MOTORIO

    mia_info_t mia_info;
} MASTER_TX_MOTORIO_CMD_t;


/// Message: KILL_REQUEST from 'ANDROID', DLC: 0 byte(s), MID: 010
typedef struct {
    uint8_t KILL_REQUEST;                ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_KILL_REQUEST_t;


/// Message: KILL_MESSAGE from 'MASTER', DLC: 0 byte(s), MID: 020
typedef struct {
    uint8_t KILL_REQUEST;                ///< B-1:0   Destination: MOTORIO,SENSOR,GEO,ANDROID

    mia_info_t mia_info;
} MASTER_TX_KILL_MESSAGE_t;


/// Message: STOP_REQUEST from 'ANDROID', DLC: 0 byte(s), MID: 030
typedef struct {
    uint8_t STOP_REQUEST;                ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_STOP_REQUEST_t;


/// Message: STOP_COMMAND from 'MASTER', DLC: 0 byte(s), MID: 040
typedef struct {
    uint8_t ANDROID_HEARTBEAT_cmd;       ///< B-1:0   Destination: MOTORIO

    mia_info_t mia_info;
} MASTER_TX_STOP_COMMAND_t;


/// Message: SONARS from 'SENSOR', DLC: 4 byte(s), MID: 210
typedef struct {
    uint8_t SENSOR_SONARS_FrontDistance; ///< B7:0   Destination: MASTER,MOTORIO
    uint8_t SENSOR_SONARS_LeftDistance;  ///< B15:8   Destination: MASTER,MOTORIO
    uint8_t SENSOR_SONARS_RightDistance; ///< B23:16   Destination: MASTER,MOTORIO
    uint16_t SENSOR_SONARS_RearDistance; ///< B35:24   Destination: MASTER,MOTORIO

    mia_info_t mia_info;
} SENSOR_TX_SONARS_t;


/// Not generating 'ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'ANDROID_TX_SEND_CHECKPOINTS_t' since we are not the sender or a recipient of any of its signals

/// Message: DISTANCE from 'GEO', DLC: 4 byte(s), MID: 250
typedef struct {
    uint16_t DISTANCE_FinalDistance;     ///< B15:0   Destination: MASTER
    uint16_t DISTANCE_CheckpointDistance; ///< B31:16   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_DISTANCE_t;


/// Message: TURN_ANGLE_DIRECTION from 'GEO', DLC: 2 byte(s), MID: 260
typedef struct {
    uint8_t TURN_ANGLE_DIRECTION_TurnAngle; ///< B7:0   Destination: MASTER
    uint8_t TURN_ANGLE_DIRECTION_TurnDirection; ///< B15:8   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_TURN_ANGLE_DIRECTION_t;


/// Message: RUN_PAUSE from 'ANDROID', DLC: 0 byte(s), MID: 270
typedef struct {
    uint8_t RUN_PAUSE;                   ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_RUN_PAUSE_t;


/// Message: HEARTBEAT_ from 'MOTORIO', DLC: 0 byte(s), MID: 280
typedef struct {
    uint8_t MOTORIO_HEARTBEAT_cmd;       ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} MOTORIO_TX_HEARTBEAT__t;


/// Message: HEARTBEAT from 'GEO', DLC: 0 byte(s), MID: 281
typedef struct {
    uint8_t GEO_HEARTBEAT_cmd;           ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_HEARTBEAT_t;


/// Message: HEARTBEAT from 'ANDROID', DLC: 0 byte(s), MID: 282
typedef struct {
    uint8_t ANDROID_HEARTBEAT_cmd;       ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_HEARTBEAT_t;


/// Message: HEARTBEAT from 'SENSOR', DLC: 0 byte(s), MID: 283
typedef struct {
    uint8_t SENSOR_HEARTBEAT_cmd;        ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} SENSOR_TX_HEARTBEAT_t;


/// Message: DESTINATION_REACHED from 'MASTER', DLC: 0 byte(s), MID: 410
typedef struct {
    uint8_t SENSOR_HEARTBEAT_cmd;        ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} MASTER_TX_DESTINATION_REACHED_t;


/// Message: BATTERY_STATUS from 'SENSOR', DLC: 1 byte(s), MID: 420
typedef struct {
    uint8_t BATTERY_STATUS_VALUE;        ///< B-1:0   Destination: MASTER,MOTORIO

    mia_info_t mia_info;
} SENSOR_TX_BATTERY_STATUS_t;


/// Message: BOOT_REQUEST from 'MASTER', DLC: 0 byte(s), MID: 610
typedef struct {
    uint8_t BATTERY_STATUS_VALUE;        ///< B-1:0   Destination: MOTORIO,SENSOR,ANDROID,GEO

    mia_info_t mia_info;
} MASTER_TX_BOOT_REQUEST_t;


/// Message: BOOT_REPLY_MOTORIO from 'MOTORIO', DLC: 0 byte(s), MID: 620
typedef struct {
    uint8_t BOOT_REPLY_MOTORIO;          ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} MOTORIO_TX_BOOT_REPLY_MOTORIO_t;


/// Message: BOOT_REPLY_SENSOR from 'SENSOR', DLC: 0 byte(s), MID: 621
typedef struct {
    uint8_t BOOT_REPLY_SENSOR;           ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} SENSOR_TX_BOOT_REPLY_SENSOR_t;


/// Message: BOOT_REPLY_GEO from 'GEO', DLC: 0 byte(s), MID: 622
typedef struct {
    uint8_t BOOT_REPLY_GEO;              ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} GEO_TX_BOOT_REPLY_GEO_t;


/// Message: BOOT_REPLY_ANDROID from 'ANDROID', DLC: 0 byte(s), MID: 623
typedef struct {
    uint8_t BOOT_REPLY_ANDROID;          ///< B-1:0   Destination: MASTER

    mia_info_t mia_info;
} ANDROID_TX_BOOT_REPLY_ANDROID_t;


/// Message: BOOT_STATUS from 'MASTER', DLC: 0 byte(s), MID: 630
typedef struct {
    uint8_t BOOT_STATUS;                 ///< B-1:0   Destination: MOTORIO,ANDROID

    mia_info_t mia_info;
} MASTER_TX_BOOT_STATUS_t;


/// Not generating 'GEO_TX_SOURCE_COORDINATE_t' since we are not the sender or a recipient of any of its signals

/// These 'externs' need to be defined in a source file of your project
extern const uint32_t MOTORIO_CMD__MIA_MS;
extern const MASTER_TX_MOTORIO_CMD_t MOTORIO_CMD__MIA_MSG;
extern const uint32_t KILL_REQUEST__MIA_MS;
extern const ANDROID_TX_KILL_REQUEST_t KILL_REQUEST__MIA_MSG;
extern const uint32_t KILL_MESSAGE__MIA_MS;
extern const MASTER_TX_KILL_MESSAGE_t KILL_MESSAGE__MIA_MSG;
extern const uint32_t STOP_REQUEST__MIA_MS;
extern const ANDROID_TX_STOP_REQUEST_t STOP_REQUEST__MIA_MSG;
extern const uint32_t STOP_COMMAND__MIA_MS;
extern const MASTER_TX_STOP_COMMAND_t STOP_COMMAND__MIA_MSG;
extern const uint32_t SONARS__MIA_MS;
extern const SENSOR_TX_SONARS_t SONARS__MIA_MSG;
extern const uint32_t TOTAL_DISTANCE_TO_TRAVEL__MIA_MS;
extern const ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_t TOTAL_DISTANCE_TO_TRAVEL__MIA_MSG;
extern const uint32_t SEND_CHECKPOINTS__MIA_MS;
extern const ANDROID_TX_SEND_CHECKPOINTS_t SEND_CHECKPOINTS__MIA_MSG;
extern const uint32_t DISTANCE__MIA_MS;
extern const GEO_TX_DISTANCE_t DISTANCE__MIA_MSG;
extern const uint32_t TURN_ANGLE_DIRECTION__MIA_MS;
extern const GEO_TX_TURN_ANGLE_DIRECTION_t TURN_ANGLE_DIRECTION__MIA_MSG;
extern const uint32_t RUN_PAUSE__MIA_MS;
extern const ANDROID_TX_RUN_PAUSE_t RUN_PAUSE__MIA_MSG;
extern const uint32_t HEARTBEAT___MIA_MS;
extern const MOTORIO_TX_HEARTBEAT__t HEARTBEAT___MIA_MSG;
extern const uint32_t HEARTBEAT__MIA_MS;
extern const GEO_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG;
extern const uint32_t HEARTBEAT__MIA_MS;
extern const ANDROID_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG;
extern const uint32_t HEARTBEAT__MIA_MS;
extern const SENSOR_TX_HEARTBEAT_t HEARTBEAT__MIA_MSG;
extern const uint32_t DESTINATION_REACHED__MIA_MS;
extern const MASTER_TX_DESTINATION_REACHED_t DESTINATION_REACHED__MIA_MSG;
extern const uint32_t BATTERY_STATUS__MIA_MS;
extern const SENSOR_TX_BATTERY_STATUS_t BATTERY_STATUS__MIA_MSG;
extern const uint32_t BOOT_REQUEST__MIA_MS;
extern const MASTER_TX_BOOT_REQUEST_t BOOT_REQUEST__MIA_MSG;
extern const uint32_t BOOT_REPLY_MOTORIO__MIA_MS;
extern const MOTORIO_TX_BOOT_REPLY_MOTORIO_t BOOT_REPLY_MOTORIO__MIA_MSG;
extern const uint32_t BOOT_REPLY_SENSOR__MIA_MS;
extern const SENSOR_TX_BOOT_REPLY_SENSOR_t BOOT_REPLY_SENSOR__MIA_MSG;
extern const uint32_t BOOT_REPLY_GEO__MIA_MS;
extern const GEO_TX_BOOT_REPLY_GEO_t BOOT_REPLY_GEO__MIA_MSG;
extern const uint32_t BOOT_REPLY_ANDROID__MIA_MS;
extern const ANDROID_TX_BOOT_REPLY_ANDROID_t BOOT_REPLY_ANDROID__MIA_MSG;
extern const uint32_t BOOT_STATUS__MIA_MS;
extern const MASTER_TX_BOOT_STATUS_t BOOT_STATUS__MIA_MSG;
extern const uint32_t SOURCE_COORDINATE__MIA_MS;
extern const GEO_TX_SOURCE_COORDINATE_t SOURCE_COORDINATE__MIA_MSG;

/// Encode MASTER's 'MOTORIO_CMD' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_MOTORIO_CMD_encode(uint64_t *to, MASTER_TX_MOTORIO_CMD_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    if(from->MOTORIO_CMD_LeftRightdirection < 0) { from->MOTORIO_CMD_LeftRightdirection = 0; }
    if(from->MOTORIO_CMD_LeftRightdirection > 2) { from->MOTORIO_CMD_LeftRightdirection = 2; }
    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_LeftRightdirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    if(from->MOTORIO_CMD_LevelOfDirection < 0) { from->MOTORIO_CMD_LevelOfDirection = 0; }
    if(from->MOTORIO_CMD_LevelOfDirection > 5) { from->MOTORIO_CMD_LevelOfDirection = 5; }
    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_LevelOfDirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    if(from->MOTORIO_CMD_FrontBackDirection < 0) { from->MOTORIO_CMD_FrontBackDirection = 0; }
    if(from->MOTORIO_CMD_FrontBackDirection > 2) { from->MOTORIO_CMD_FrontBackDirection = 2; }
    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_FrontBackDirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16

    if(from->MOTORIO_CMD_LevelOfSpeed < 0) { from->MOTORIO_CMD_LevelOfSpeed = 0; }
    if(from->MOTORIO_CMD_LevelOfSpeed > 5) { from->MOTORIO_CMD_LevelOfSpeed = 5; }
    raw_signal = ((uint64_t)(((from->MOTORIO_CMD_LevelOfSpeed - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[3] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B24

    return MASTER_TX_MOTORIO_CMD_HDR;
}


/// Encode ANDROID's 'KILL_REQUEST' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_KILL_REQUEST_encode(uint64_t *to, ANDROID_TX_KILL_REQUEST_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->KILL_REQUEST - (0)) / 1.0) + 0.5)) & 0x00;

    return ANDROID_TX_KILL_REQUEST_HDR;
}


/// Encode MASTER's 'KILL_MESSAGE' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_KILL_MESSAGE_encode(uint64_t *to, MASTER_TX_KILL_MESSAGE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->KILL_REQUEST - (0)) / 1.0) + 0.5)) & 0x00;

    return MASTER_TX_KILL_MESSAGE_HDR;
}


/// Encode ANDROID's 'STOP_REQUEST' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_STOP_REQUEST_encode(uint64_t *to, ANDROID_TX_STOP_REQUEST_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->STOP_REQUEST - (0)) / 1.0) + 0.5)) & 0x00;

    return ANDROID_TX_STOP_REQUEST_HDR;
}


/// Encode MASTER's 'STOP_COMMAND' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_STOP_COMMAND_encode(uint64_t *to, MASTER_TX_STOP_COMMAND_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->ANDROID_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return MASTER_TX_STOP_COMMAND_HDR;
}


/// Encode SENSOR's 'SONARS' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_SONARS_encode(uint64_t *to, SENSOR_TX_SONARS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_FrontDistance - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_LeftDistance - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_RightDistance - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_RearDistance - (0)) / 1.0) + 0.5)) & 0xfff;
    bytes[3] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B24
    bytes[4] |= (((uint8_t)(raw_signal >> 8) & 0x0f) << 0); ///< 4 bit(s) to B32

    return SENSOR_TX_SONARS_HDR;
}


/// Encode ANDROID's 'TOTAL_DISTANCE_TO_TRAVEL' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_encode(uint64_t *to, ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->TOTAL_DISTANCE_TO_TRAVEL - (0)) / 1.0) + 0.5)) & 0xffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8

    return ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_HDR;
}


/// Encode ANDROID's 'SEND_CHECKPOINTS' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_SEND_CHECKPOINTS_encode(uint64_t *to, ANDROID_TX_SEND_CHECKPOINTS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SEND_CHECKPOINTS_Latitude - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->SEND_CHECKPOINTS_Longitude - (0)) / 1.0) + 0.5)) & 0xffffffffffffffff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B40
    bytes[6] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B48
    bytes[7] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B56
    bytes[8] |= (((uint8_t)(raw_signal >> 32) & 0xff) << 0); ///< 8 bit(s) to B64
    bytes[9] |= (((uint8_t)(raw_signal >> 40) & 0xff) << 0); ///< 8 bit(s) to B72
    bytes[10] |= (((uint8_t)(raw_signal >> 48) & 0xff) << 0); ///< 8 bit(s) to B80
    bytes[11] |= (((uint8_t)(raw_signal >> 56) & 0xff) << 0); ///< 8 bit(s) to B88

    return ANDROID_TX_SEND_CHECKPOINTS_HDR;
}


/// Encode GEO's 'DISTANCE' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_DISTANCE_encode(uint64_t *to, GEO_TX_DISTANCE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DISTANCE_FinalDistance - (0)) / 1.0) + 0.5)) & 0xffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8

    raw_signal = ((uint64_t)(((from->DISTANCE_CheckpointDistance - (0)) / 1.0) + 0.5)) & 0xffff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B24

    return GEO_TX_DISTANCE_HDR;
}


/// Encode GEO's 'TURN_ANGLE_DIRECTION' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_TURN_ANGLE_DIRECTION_encode(uint64_t *to, GEO_TX_TURN_ANGLE_DIRECTION_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->TURN_ANGLE_DIRECTION_TurnAngle - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->TURN_ANGLE_DIRECTION_TurnDirection - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    return GEO_TX_TURN_ANGLE_DIRECTION_HDR;
}


/// Encode ANDROID's 'RUN_PAUSE' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_RUN_PAUSE_encode(uint64_t *to, ANDROID_TX_RUN_PAUSE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->RUN_PAUSE - (0)) / 1.0) + 0.5)) & 0x00;

    return ANDROID_TX_RUN_PAUSE_HDR;
}


/// Encode MOTORIO's 'HEARTBEAT_' message
/// @returns the message header of this message
static msg_hdr_t MOTORIO_TX_HEARTBEAT__encode(uint64_t *to, MOTORIO_TX_HEARTBEAT__t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->MOTORIO_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return MOTORIO_TX_HEARTBEAT__HDR;
}


/// Encode GEO's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_HEARTBEAT_encode(uint64_t *to, GEO_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GEO_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return GEO_TX_HEARTBEAT_HDR;
}


/// Encode ANDROID's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_HEARTBEAT_encode(uint64_t *to, ANDROID_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->ANDROID_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return ANDROID_TX_HEARTBEAT_HDR;
}


/// Encode SENSOR's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_HEARTBEAT_encode(uint64_t *to, SENSOR_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SENSOR_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return SENSOR_TX_HEARTBEAT_HDR;
}


/// Encode MASTER's 'DESTINATION_REACHED' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_DESTINATION_REACHED_encode(uint64_t *to, MASTER_TX_DESTINATION_REACHED_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SENSOR_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0x00;

    return MASTER_TX_DESTINATION_REACHED_HDR;
}


/// Encode SENSOR's 'BATTERY_STATUS' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_BATTERY_STATUS_encode(uint64_t *to, SENSOR_TX_BATTERY_STATUS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BATTERY_STATUS_VALUE - (0)) / 1.0) + 0.5)) & 0x00;

    return SENSOR_TX_BATTERY_STATUS_HDR;
}


/// Encode MASTER's 'BOOT_REQUEST' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_BOOT_REQUEST_encode(uint64_t *to, MASTER_TX_BOOT_REQUEST_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BATTERY_STATUS_VALUE - (0)) / 1.0) + 0.5)) & 0x00;

    return MASTER_TX_BOOT_REQUEST_HDR;
}


/// Encode MOTORIO's 'BOOT_REPLY_MOTORIO' message
/// @returns the message header of this message
static msg_hdr_t MOTORIO_TX_BOOT_REPLY_MOTORIO_encode(uint64_t *to, MOTORIO_TX_BOOT_REPLY_MOTORIO_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BOOT_REPLY_MOTORIO - (0)) / 1.0) + 0.5)) & 0x00;

    return MOTORIO_TX_BOOT_REPLY_MOTORIO_HDR;
}


/// Encode SENSOR's 'BOOT_REPLY_SENSOR' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_BOOT_REPLY_SENSOR_encode(uint64_t *to, SENSOR_TX_BOOT_REPLY_SENSOR_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BOOT_REPLY_SENSOR - (0)) / 1.0) + 0.5)) & 0x00;

    return SENSOR_TX_BOOT_REPLY_SENSOR_HDR;
}


/// Encode GEO's 'BOOT_REPLY_GEO' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_BOOT_REPLY_GEO_encode(uint64_t *to, GEO_TX_BOOT_REPLY_GEO_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BOOT_REPLY_GEO - (0)) / 1.0) + 0.5)) & 0x00;

    return GEO_TX_BOOT_REPLY_GEO_HDR;
}


/// Encode ANDROID's 'BOOT_REPLY_ANDROID' message
/// @returns the message header of this message
static msg_hdr_t ANDROID_TX_BOOT_REPLY_ANDROID_encode(uint64_t *to, ANDROID_TX_BOOT_REPLY_ANDROID_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BOOT_REPLY_ANDROID - (0)) / 1.0) + 0.5)) & 0x00;

    return ANDROID_TX_BOOT_REPLY_ANDROID_HDR;
}


/// Encode MASTER's 'BOOT_STATUS' message
/// @returns the message header of this message
static msg_hdr_t MASTER_TX_BOOT_STATUS_encode(uint64_t *to, MASTER_TX_BOOT_STATUS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->BOOT_STATUS - (0)) / 1.0) + 0.5)) & 0x00;

    return MASTER_TX_BOOT_STATUS_HDR;
}


/// Encode GEO's 'SOURCE_COORDINATE' message
/// @returns the message header of this message
static msg_hdr_t GEO_TX_SOURCE_COORDINATE_encode(uint64_t *to, GEO_TX_SOURCE_COORDINATE_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SOURCE_COORDINATE_LATITUDE - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->SOURCE_COORDINATE_LONGITUDE - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B40
    bytes[6] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B48
    bytes[7] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B56

    return GEO_TX_SOURCE_COORDINATE_HDR;
}


/// Decode MASTER's 'MOTORIO_CMD' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_MOTORIO_CMD_decode(MASTER_TX_MOTORIO_CMD_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_MOTORIO_CMD_HDR.dlc || hdr->mid != MASTER_TX_MOTORIO_CMD_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_LeftRightdirection = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_LevelOfDirection = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_FrontBackDirection = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 0);
    to->MOTORIO_CMD_LevelOfSpeed = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'KILL_REQUEST' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_KILL_REQUEST_decode(ANDROID_TX_KILL_REQUEST_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_KILL_REQUEST_HDR.dlc || hdr->mid != ANDROID_TX_KILL_REQUEST_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->KILL_REQUEST = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MASTER's 'KILL_MESSAGE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_KILL_MESSAGE_decode(MASTER_TX_KILL_MESSAGE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_KILL_MESSAGE_HDR.dlc || hdr->mid != MASTER_TX_KILL_MESSAGE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->KILL_REQUEST = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'STOP_REQUEST' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_STOP_REQUEST_decode(ANDROID_TX_STOP_REQUEST_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_STOP_REQUEST_HDR.dlc || hdr->mid != ANDROID_TX_STOP_REQUEST_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->STOP_REQUEST = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MASTER's 'STOP_COMMAND' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_STOP_COMMAND_decode(MASTER_TX_STOP_COMMAND_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_STOP_COMMAND_HDR.dlc || hdr->mid != MASTER_TX_STOP_COMMAND_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->ANDROID_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'SONARS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SONARS_decode(SENSOR_TX_SONARS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SONARS_HDR.dlc || hdr->mid != SENSOR_TX_SONARS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_FrontDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_LeftDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    to->SENSOR_SONARS_RightDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[4] >> 0) & 0x0f); ///< 4 bit(s) from B32
    raw_signal    |= (bits_from_byte << 8);
    to->SENSOR_SONARS_RearDistance = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'TOTAL_DISTANCE_TO_TRAVEL' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_decode(ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_HDR.dlc || hdr->mid != ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 8);
    to->TOTAL_DISTANCE_TO_TRAVEL = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'SEND_CHECKPOINTS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_SEND_CHECKPOINTS_decode(ANDROID_TX_SEND_CHECKPOINTS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_SEND_CHECKPOINTS_HDR.dlc || hdr->mid != ANDROID_TX_SEND_CHECKPOINTS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 8);
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 16);
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 24);
    to->SEND_CHECKPOINTS_Latitude = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[4] >> 0) & 0xff); ///< 8 bit(s) from B32
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[5] >> 0) & 0xff); ///< 8 bit(s) from B40
    raw_signal    |= (bits_from_byte << 8);
    bits_from_byte = ((bytes[6] >> 0) & 0xff); ///< 8 bit(s) from B48
    raw_signal    |= (bits_from_byte << 16);
    bits_from_byte = ((bytes[7] >> 0) & 0xff); ///< 8 bit(s) from B56
    raw_signal    |= (bits_from_byte << 24);
    bits_from_byte = ((bytes[8] >> 0) & 0xff); ///< 8 bit(s) from B64
    raw_signal    |= (bits_from_byte << 32);
    bits_from_byte = ((bytes[9] >> 0) & 0xff); ///< 8 bit(s) from B72
    raw_signal    |= (bits_from_byte << 40);
    bits_from_byte = ((bytes[10] >> 0) & 0xff); ///< 8 bit(s) from B80
    raw_signal    |= (bits_from_byte << 48);
    bits_from_byte = ((bytes[11] >> 0) & 0xff); ///< 8 bit(s) from B88
    raw_signal    |= (bits_from_byte << 56);
    to->SEND_CHECKPOINTS_Longitude = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'DISTANCE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_DISTANCE_decode(GEO_TX_DISTANCE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_DISTANCE_HDR.dlc || hdr->mid != GEO_TX_DISTANCE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 8);
    to->DISTANCE_FinalDistance = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 8);
    to->DISTANCE_CheckpointDistance = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'TURN_ANGLE_DIRECTION' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_TURN_ANGLE_DIRECTION_decode(GEO_TX_TURN_ANGLE_DIRECTION_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_TURN_ANGLE_DIRECTION_HDR.dlc || hdr->mid != GEO_TX_TURN_ANGLE_DIRECTION_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    to->TURN_ANGLE_DIRECTION_TurnAngle = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 0);
    to->TURN_ANGLE_DIRECTION_TurnDirection = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'RUN_PAUSE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_RUN_PAUSE_decode(ANDROID_TX_RUN_PAUSE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_RUN_PAUSE_HDR.dlc || hdr->mid != ANDROID_TX_RUN_PAUSE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->RUN_PAUSE = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MOTORIO's 'HEARTBEAT_' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MOTORIO_TX_HEARTBEAT__decode(MOTORIO_TX_HEARTBEAT__t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MOTORIO_TX_HEARTBEAT__HDR.dlc || hdr->mid != MOTORIO_TX_HEARTBEAT__HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->MOTORIO_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_HEARTBEAT_decode(GEO_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_HEARTBEAT_HDR.dlc || hdr->mid != GEO_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->GEO_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_HEARTBEAT_decode(ANDROID_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_HEARTBEAT_HDR.dlc || hdr->mid != ANDROID_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->ANDROID_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_HEARTBEAT_decode(SENSOR_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_HEARTBEAT_HDR.dlc || hdr->mid != SENSOR_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->SENSOR_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MASTER's 'DESTINATION_REACHED' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_DESTINATION_REACHED_decode(MASTER_TX_DESTINATION_REACHED_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_DESTINATION_REACHED_HDR.dlc || hdr->mid != MASTER_TX_DESTINATION_REACHED_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->SENSOR_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'BATTERY_STATUS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_BATTERY_STATUS_decode(SENSOR_TX_BATTERY_STATUS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_BATTERY_STATUS_HDR.dlc || hdr->mid != SENSOR_TX_BATTERY_STATUS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BATTERY_STATUS_VALUE = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MASTER's 'BOOT_REQUEST' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_BOOT_REQUEST_decode(MASTER_TX_BOOT_REQUEST_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_BOOT_REQUEST_HDR.dlc || hdr->mid != MASTER_TX_BOOT_REQUEST_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BATTERY_STATUS_VALUE = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MOTORIO's 'BOOT_REPLY_MOTORIO' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MOTORIO_TX_BOOT_REPLY_MOTORIO_decode(MOTORIO_TX_BOOT_REPLY_MOTORIO_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MOTORIO_TX_BOOT_REPLY_MOTORIO_HDR.dlc || hdr->mid != MOTORIO_TX_BOOT_REPLY_MOTORIO_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BOOT_REPLY_MOTORIO = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode SENSOR's 'BOOT_REPLY_SENSOR' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_BOOT_REPLY_SENSOR_decode(SENSOR_TX_BOOT_REPLY_SENSOR_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_BOOT_REPLY_SENSOR_HDR.dlc || hdr->mid != SENSOR_TX_BOOT_REPLY_SENSOR_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BOOT_REPLY_SENSOR = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'BOOT_REPLY_GEO' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_BOOT_REPLY_GEO_decode(GEO_TX_BOOT_REPLY_GEO_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_BOOT_REPLY_GEO_HDR.dlc || hdr->mid != GEO_TX_BOOT_REPLY_GEO_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BOOT_REPLY_GEO = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode ANDROID's 'BOOT_REPLY_ANDROID' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool ANDROID_TX_BOOT_REPLY_ANDROID_decode(ANDROID_TX_BOOT_REPLY_ANDROID_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != ANDROID_TX_BOOT_REPLY_ANDROID_HDR.dlc || hdr->mid != ANDROID_TX_BOOT_REPLY_ANDROID_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BOOT_REPLY_ANDROID = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode MASTER's 'BOOT_STATUS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool MASTER_TX_BOOT_STATUS_decode(MASTER_TX_BOOT_STATUS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != MASTER_TX_BOOT_STATUS_HDR.dlc || hdr->mid != MASTER_TX_BOOT_STATUS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    to->BOOT_STATUS = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GEO's 'SOURCE_COORDINATE' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GEO_TX_SOURCE_COORDINATE_decode(GEO_TX_SOURCE_COORDINATE_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GEO_TX_SOURCE_COORDINATE_HDR.dlc || hdr->mid != GEO_TX_SOURCE_COORDINATE_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    uint64_t bits_from_byte;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    bits_from_byte = ((bytes[0] >> 0) & 0xff); ///< 8 bit(s) from B0
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[1] >> 0) & 0xff); ///< 8 bit(s) from B8
    raw_signal    |= (bits_from_byte << 8);
    bits_from_byte = ((bytes[2] >> 0) & 0xff); ///< 8 bit(s) from B16
    raw_signal    |= (bits_from_byte << 16);
    bits_from_byte = ((bytes[3] >> 0) & 0xff); ///< 8 bit(s) from B24
    raw_signal    |= (bits_from_byte << 24);
    to->SOURCE_COORDINATE_LATITUDE = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    bits_from_byte = ((bytes[4] >> 0) & 0xff); ///< 8 bit(s) from B32
    raw_signal    |= (bits_from_byte << 0);
    bits_from_byte = ((bytes[5] >> 0) & 0xff); ///< 8 bit(s) from B40
    raw_signal    |= (bits_from_byte << 8);
    bits_from_byte = ((bytes[6] >> 0) & 0xff); ///< 8 bit(s) from B48
    raw_signal    |= (bits_from_byte << 16);
    bits_from_byte = ((bytes[7] >> 0) & 0xff); ///< 8 bit(s) from B56
    raw_signal    |= (bits_from_byte << 24);
    to->SOURCE_COORDINATE_LONGITUDE = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Handle the MIA for MASTER's 'MOTORIO_CMD' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_MOTORIO_CMD_handle_mia(MASTER_TX_MOTORIO_CMD_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= MOTORIO_CMD__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = MOTORIO_CMD__MIA_MSG;
        msg->mia_info.mia_counter_ms = MOTORIO_CMD__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'KILL_REQUEST' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_KILL_REQUEST_handle_mia(ANDROID_TX_KILL_REQUEST_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= KILL_REQUEST__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = KILL_REQUEST__MIA_MSG;
        msg->mia_info.mia_counter_ms = KILL_REQUEST__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's 'KILL_MESSAGE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_KILL_MESSAGE_handle_mia(MASTER_TX_KILL_MESSAGE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= KILL_MESSAGE__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = KILL_MESSAGE__MIA_MSG;
        msg->mia_info.mia_counter_ms = KILL_MESSAGE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'STOP_REQUEST' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_STOP_REQUEST_handle_mia(ANDROID_TX_STOP_REQUEST_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= STOP_REQUEST__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = STOP_REQUEST__MIA_MSG;
        msg->mia_info.mia_counter_ms = STOP_REQUEST__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's 'STOP_COMMAND' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_STOP_COMMAND_handle_mia(MASTER_TX_STOP_COMMAND_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= STOP_COMMAND__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = STOP_COMMAND__MIA_MSG;
        msg->mia_info.mia_counter_ms = STOP_COMMAND__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'SONARS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SONARS_handle_mia(SENSOR_TX_SONARS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SONARS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SONARS__MIA_MSG;
        msg->mia_info.mia_counter_ms = SONARS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'TOTAL_DISTANCE_TO_TRAVEL' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_handle_mia(ANDROID_TX_TOTAL_DISTANCE_TO_TRAVEL_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= TOTAL_DISTANCE_TO_TRAVEL__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = TOTAL_DISTANCE_TO_TRAVEL__MIA_MSG;
        msg->mia_info.mia_counter_ms = TOTAL_DISTANCE_TO_TRAVEL__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'SEND_CHECKPOINTS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_SEND_CHECKPOINTS_handle_mia(ANDROID_TX_SEND_CHECKPOINTS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SEND_CHECKPOINTS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SEND_CHECKPOINTS__MIA_MSG;
        msg->mia_info.mia_counter_ms = SEND_CHECKPOINTS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'DISTANCE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_DISTANCE_handle_mia(GEO_TX_DISTANCE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= DISTANCE__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = DISTANCE__MIA_MSG;
        msg->mia_info.mia_counter_ms = DISTANCE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'TURN_ANGLE_DIRECTION' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_TURN_ANGLE_DIRECTION_handle_mia(GEO_TX_TURN_ANGLE_DIRECTION_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= TURN_ANGLE_DIRECTION__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = TURN_ANGLE_DIRECTION__MIA_MSG;
        msg->mia_info.mia_counter_ms = TURN_ANGLE_DIRECTION__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'RUN_PAUSE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_RUN_PAUSE_handle_mia(ANDROID_TX_RUN_PAUSE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= RUN_PAUSE__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = RUN_PAUSE__MIA_MSG;
        msg->mia_info.mia_counter_ms = RUN_PAUSE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTORIO's 'HEARTBEAT_' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MOTORIO_TX_HEARTBEAT__handle_mia(MOTORIO_TX_HEARTBEAT__t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT___MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT___MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT___MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_HEARTBEAT_handle_mia(GEO_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_HEARTBEAT_handle_mia(ANDROID_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_HEARTBEAT_handle_mia(SENSOR_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's 'DESTINATION_REACHED' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_DESTINATION_REACHED_handle_mia(MASTER_TX_DESTINATION_REACHED_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= DESTINATION_REACHED__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = DESTINATION_REACHED__MIA_MSG;
        msg->mia_info.mia_counter_ms = DESTINATION_REACHED__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'BATTERY_STATUS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_BATTERY_STATUS_handle_mia(SENSOR_TX_BATTERY_STATUS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BATTERY_STATUS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BATTERY_STATUS__MIA_MSG;
        msg->mia_info.mia_counter_ms = BATTERY_STATUS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's 'BOOT_REQUEST' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_BOOT_REQUEST_handle_mia(MASTER_TX_BOOT_REQUEST_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BOOT_REQUEST__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BOOT_REQUEST__MIA_MSG;
        msg->mia_info.mia_counter_ms = BOOT_REQUEST__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MOTORIO's 'BOOT_REPLY_MOTORIO' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MOTORIO_TX_BOOT_REPLY_MOTORIO_handle_mia(MOTORIO_TX_BOOT_REPLY_MOTORIO_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BOOT_REPLY_MOTORIO__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BOOT_REPLY_MOTORIO__MIA_MSG;
        msg->mia_info.mia_counter_ms = BOOT_REPLY_MOTORIO__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for SENSOR's 'BOOT_REPLY_SENSOR' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_BOOT_REPLY_SENSOR_handle_mia(SENSOR_TX_BOOT_REPLY_SENSOR_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BOOT_REPLY_SENSOR__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BOOT_REPLY_SENSOR__MIA_MSG;
        msg->mia_info.mia_counter_ms = BOOT_REPLY_SENSOR__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'BOOT_REPLY_GEO' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_BOOT_REPLY_GEO_handle_mia(GEO_TX_BOOT_REPLY_GEO_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BOOT_REPLY_GEO__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BOOT_REPLY_GEO__MIA_MSG;
        msg->mia_info.mia_counter_ms = BOOT_REPLY_GEO__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for ANDROID's 'BOOT_REPLY_ANDROID' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool ANDROID_TX_BOOT_REPLY_ANDROID_handle_mia(ANDROID_TX_BOOT_REPLY_ANDROID_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BOOT_REPLY_ANDROID__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BOOT_REPLY_ANDROID__MIA_MSG;
        msg->mia_info.mia_counter_ms = BOOT_REPLY_ANDROID__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for MASTER's 'BOOT_STATUS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool MASTER_TX_BOOT_STATUS_handle_mia(MASTER_TX_BOOT_STATUS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= BOOT_STATUS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = BOOT_STATUS__MIA_MSG;
        msg->mia_info.mia_counter_ms = BOOT_STATUS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GEO's 'SOURCE_COORDINATE' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GEO_TX_SOURCE_COORDINATE_handle_mia(GEO_TX_SOURCE_COORDINATE_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= SOURCE_COORDINATE__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SOURCE_COORDINATE__MIA_MSG;
        msg->mia_info.mia_counter_ms = SOURCE_COORDINATE__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}
