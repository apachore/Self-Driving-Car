package minion.autodrive;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PolylineOptions;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.lang.reflect.Array;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

import javax.xml.transform.Source;


public class MapsActivity1 extends FragmentActivity implements OnMapReadyCallback {

    private GoogleMap mMap; // Might be null if Google Play services APK is not available.
    private static final int REQUEST_ENABLE_BT = 1;
    private boolean startflag=false;
    private double latitude_s=0;
    private double longitude_s=0;
    private String source=null;
    private String lon_string;
    private String lat_string;
    ArrayList<LatLng> markerPoints;
    String lat_current;
    String lon_current;
    String sensor;
    String sensor1;
    String sensor2;
    String sensor3;
    TextView text;
    TextView textf;
    TextView textr;
    TextView textrr;


    //TextView source_l;

    Handler bluetoothIn;

    final int handlerState = 0;                        //used to identify handler message
    private StringBuilder recDataString = new StringBuilder();


    BluetoothAdapter bluetoothAdapter;
    ArrayList<BluetoothDevice> pairedDeviceArrayList;

    TextView textInfo;
    ListView listViewPairedDevice;
    LinearLayout inputPane;

    ToggleButton runstop_btn;
    Button monitor_btn;
    Button kill_btn;
    Button set_btn;
    Button get_btn;

    Button controls;

    ArrayAdapter<BluetoothDevice> pairedDeviceAdapter;
    private UUID myUUID;
    private final String UUID_STRING_WELL_KNOWN_SPP =
            "00001101-0000-1000-8000-00805F9B34FB";

    ThreadConnectBTdevice myThreadConnectBTdevice;
    ThreadConnected myThreadConnected;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps1);

        textInfo = (TextView)findViewById(R.id.info);
        text=(TextView)findViewById(R.id.sensor);
        textf=(TextView)findViewById(R.id.sensor2);
        textr=(TextView)findViewById(R.id.sensor3);
        textrr=(TextView)findViewById(R.id.sensor4);

        listViewPairedDevice = (ListView)findViewById(R.id.pairedlist);
        inputPane = (LinearLayout)findViewById(R.id.inputpane);

        runstop_btn=(ToggleButton)findViewById(R.id.run_stop_toggle);
        monitor_btn=(Button)findViewById(R.id.monitor);
        kill_btn=(Button)findViewById(R.id.kill);
        set_btn=(Button)findViewById(R.id.setdest);
        controls=(Button)findViewById(R.id.controls);

        View.OnClickListener handler = new View.OnClickListener(){
            public void onClick(View view) {
                //Switching action to be performed
                switch (view.getId()) {

                    case R.id.run_stop_toggle: //toast will be shown
                        onToggleClicked(view);
                        break;
                    case R.id.monitor: //program will end
                        openMonitor();
                        break;
                    case R.id.kill:
                        sendKillSignal();
                        break;
                    case R.id.setdest:
                        setDest();
                        break;
                    case R.id.controls:
                        backToControls();
                        break;
                }
            }
        };

        //Setting the listeners
        findViewById(R.id.run_stop_toggle).setOnClickListener(handler);
        findViewById(R.id.monitor).setOnClickListener(handler);
        findViewById(R.id.kill).setOnClickListener(handler);
        findViewById(R.id.setdest).setOnClickListener(handler);
        findViewById(R.id.controls).setOnClickListener(handler);


        //source_l = (TextView) findViewById(R.id.source);

        //monitor=(Button)findViewById(R.id.monitor);

        markerPoints = new ArrayList<LatLng>();

        SupportMapFragment mapFragment=(SupportMapFragment)getSupportFragmentManager().findFragmentById(R.id.map);
        mMap=mapFragment.getMap();

        if(mMap!=null){

            // Enable MyLocation Button in the Map
            mMap.setMyLocationEnabled(true);

            // Setting onclick event listener for the map
            mMap.setOnMapClickListener(new GoogleMap.OnMapClickListener() {

                @Override
                public void onMapClick(LatLng point) {

                    // Already two locations
                    if(markerPoints.size()>1){
                        markerPoints.clear();
                        mMap.clear();
                    }

                    // Adding new item to the ArrayList
                    markerPoints.add(point);

                    // Creating MarkerOptions
                    MarkerOptions options = new MarkerOptions();

                    // Setting the position of the marker
                    options.position(point);

                    /**
                     * For the start location, the color of marker is GREEN and
                     * for the end location, the color of marker is RED.
                     */

                    if(markerPoints.size()==1){
                        options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN));
                    }

                    else if(markerPoints.size()==2){
                        options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED));
                    }

                    // Add new marker to the Google Map Android API V2
                    mMap.addMarker(options);

                    // Checks, whether start and end locations are captured
                    if(markerPoints.size() >= 2){
                        LatLng origin = markerPoints.get(0);
                        LatLng dest = markerPoints.get(1);

                        // Getting URL to the Google Directions API
                        String url = getDirectionsUrl(origin, dest);

                        DownloadTask downloadTask = new DownloadTask();

                        // Start downloading json data from Google Directions API
                        downloadTask.execute(url);
                    }
                }
            });
        }


        mMap.moveCamera( CameraUpdateFactory.newLatLngZoom(new LatLng(37.336079,-121.880453) , 14.0f) );

        myUUID = UUID.fromString(UUID_STRING_WELL_KNOWN_SPP);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            // Toast.makeText(this,"Bluetooth is not supported on this hardware platform", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

        //mapFragment.getMapAsync(this);
        //setUpMapIfNeeded();
    }

    Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            byte[] writeBuf = (byte[]) msg.obj;
            int begin = (int)msg.arg1;
            int end = (int)msg.arg2;

            switch(msg.what) {
                case 1:
                    String readMessage = new String(writeBuf);
                    String id=readMessage.substring(begin,end);
                    //final String data=readMessage.substring(begin+1, end);


                    if(id.contains("x")){
                        lat_string = readMessage.substring(begin+1, end);

                    }

                    if(id.contains("y")){
                        lon_string = readMessage.substring(begin+1, end);

                    }

                    if(id.contains("f")){
                        sensor=readMessage.substring(begin+1, end);
                        text.setText(sensor);
                    }

                    if(id.contains("l")){
                        sensor1=readMessage.substring(begin+1, end);
                        textf.setText(sensor1);
                    }

                    if(id.contains("s")){
                        sensor2=readMessage.substring(begin+1, end);
                        textr.setText(sensor2);
                    }

                    if(id.contains("r")){
                        sensor3=readMessage.substring(begin+1, end);
                        textrr.setText(sensor3);
                    }

                    if(id.contains("y")) {
                        LatLng lat_lng_current = new LatLng(Double.parseDouble(lat_string), Double.parseDouble(lon_string));
                        MarkerOptions options1 = new MarkerOptions();
                        options1.position(lat_lng_current);
                        options1.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_BLUE));
                        mMap.addMarker(options1);
                    }


                    if(id.contains("y") && markerPoints.size()==0) {
                        LatLng lat_lng = new LatLng(Double.parseDouble(lat_string), Double.parseDouble(lon_string));

                        markerPoints.add(0, lat_lng);
                        MarkerOptions options = new MarkerOptions();
                        options.position(lat_lng);

                        if (markerPoints.size() == 1) {
                            options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_GREEN));
                        }

                        mMap.addMarker(options);
                    }
                    //final String finalWriteMessage1 = finalWriteMessage;
                    runOnUiThread(new Runnable() {
                        public void run() {

                        }
                    });

                    break;

            }
        }
    };

    private String getDirectionsUrl(LatLng origin,LatLng dest){

        // Origin of route
        String str_origin = "origin="+origin.latitude+","+origin.longitude;

        // Destination of route
        String str_dest = "destination="+dest.latitude+","+dest.longitude;

        // Sensor enabled
        String sensor = "sensor=false";

        // Building the parameters to the web service
        String parameters = str_origin+"&"+str_dest+"&"+sensor+"&mode=bicycling";

        // Output format
        String output = "json";

        // Building the url to the web service
        String url = "https://maps.googleapis.com/maps/api/directions/"+output+"?"+parameters;

        return url;
    }
    /** A method to download json data from url */
    private String downloadUrl(String strUrl) throws IOException {
        String data = "";
        InputStream iStream = null;
        HttpURLConnection urlConnection = null;
        try{
            URL url = new URL(strUrl);

            // Creating an http connection to communicate with url
            urlConnection = (HttpURLConnection) url.openConnection();

            // Connecting to url
            urlConnection.connect();

            // Reading data from url
            iStream = urlConnection.getInputStream();

            BufferedReader br = new BufferedReader(new InputStreamReader(iStream));

            StringBuffer sb = new StringBuffer();

            String line = "";
            while( ( line = br.readLine()) != null){
                sb.append(line);
            }

            data = sb.toString();

            br.close();

        }catch(Exception e){
            Log.d("Exception", e.toString());
        }finally{
            iStream.close();
            urlConnection.disconnect();
        }
        return data;
    }

    // Fetches data from url passed
    private class DownloadTask extends AsyncTask<String, Void, String> {

        // Downloading data in non-ui thread
        @Override
        protected String doInBackground(String... url) {

            // For storing data from web service
            String data = "";

            try{
                // Fetching the data from web service
                data = downloadUrl(url[0]);
            }catch(Exception e){
                Log.d("Background Task",e.toString());
            }
            return data;
        }

        // Executes in UI thread, after the execution of
        // doInBackground()
        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);

            ParserTask parserTask = new ParserTask();

            // Invokes the thread for parsing the JSON data
            parserTask.execute(result);
        }
    }

    /** A class to parse the Google Places in JSON format */
    private class ParserTask extends AsyncTask<String, Integer, List<List<HashMap<String,String>>> >{

        // Parsing the data in non-ui thread
        @Override
        protected List<List<HashMap<String, String>>> doInBackground(String... jsonData) {

            JSONObject jObject;
            List<List<HashMap<String, String>>> routes = null;

            try{
                jObject = new JSONObject(jsonData[0]);
                DirectionsJSONParser parser = new DirectionsJSONParser();

                // Starts parsing data
                routes = parser.parse(jObject);
            }catch(Exception e){
                e.printStackTrace();
            }
            return routes;
        }

        // Executes in UI thread, after the parsing process
        @Override
        protected void onPostExecute(List<List<HashMap<String, String>>> result) {
            ArrayList<LatLng> points = null;
            PolylineOptions lineOptions = null;
            MarkerOptions markerOptions = new MarkerOptions();

            // Traversing through all the routes
            for(int i=0;i<result.size();i++){
                points = new ArrayList<LatLng>();
                lineOptions = new PolylineOptions();

                // Fetching i-th route
                List<HashMap<String, String>> path = result.get(i);

                // Fetching all the points in i-th route
                int size_of_path=path.size();
                String noofpoints= String.valueOf(size_of_path);

                Toast.makeText(MapsActivity1.this, noofpoints, Toast.LENGTH_SHORT).show();

                for(int j=0;j<path.size();j++){
                    HashMap<String,String> point = path.get(j);

                    double lat = Double.parseDouble(point.get("lat"));
                    double lng = Double.parseDouble(point.get("lng"));
                    LatLng position = new LatLng(lat, lng);

                    points.add(position);

                    // Adding new item to the ArrayList
                    markerPoints.add(position);

                    // Creating MarkerOptions
                    MarkerOptions options = new MarkerOptions();

                    // Setting the position of the marker
                    options.position(position);

                    options.title(position.latitude + " : " + position.longitude);

                    /**
                     * For the start location, the color of marker is GREEN and
                     * for the end location, the color of marker is RED.
                     */
                    options.icon(BitmapDescriptorFactory.defaultMarker(BitmapDescriptorFactory.HUE_RED));

                    // Add new marker to the Google Map Android API V2
                    mMap.addMarker(options);



                }

                // Adding all the points in the route to LineOptions

                for (int size=0;size<points.size();size++) {
                    LatLng checkpoints=points.get(size);

                    String latinput = Double.toString(checkpoints.latitude);
                    String loninput = Double.toString(checkpoints.longitude);
                    String coordinate = "f" + "," + latinput + "," + loninput;
                    byte[] coordinateBytes = coordinate.getBytes();
                    myThreadConnected.write(coordinateBytes);
                    try {
                        Thread.sleep(1500);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

                lineOptions.addAll(points);
                lineOptions.width(10);
                lineOptions.color(Color.BLUE);

            }

            // Drawing polyline in the Google Map for the i-th route
            mMap.addPolyline(lineOptions);
        }
    }

    public void onToggleClicked(View view) {
        // Is the toggle on?
        boolean on = ((ToggleButton) view).isChecked();
       /* if (latitude_s != 0 && longitude_s != 0)
            {*/
        if (on) {

            if (myThreadConnected != null) {

                if (startflag == false) {

                    String startId = "a";
                    byte[] bytesToSend = startId.getBytes();
                    myThreadConnected.write(bytesToSend);
                    //myThreadConnected.write(latbytes);
                    //myThreadConnected.write(lonbytes);

                    startflag = true;
                } else {

                    String startId = "b";
                    byte[] bytesToSend = startId.getBytes();
                    myThreadConnected.write(bytesToSend);
                }

                Toast.makeText(this, "Start", Toast.LENGTH_SHORT).show();

            }


        } else {

            String stopId = "c";
            byte[] bytesToSend = stopId.getBytes();
            myThreadConnected.write(bytesToSend);
            Toast.makeText(this, "Stop", Toast.LENGTH_SHORT).show();
        }

            /*}
        else {

            Toast.makeText(this, "Destination needed", Toast.LENGTH_LONG).show();

        }*/

    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        LatLng sydney = new LatLng(37.3353, 121.8813);
        googleMap.addMarker(new MarkerOptions().position(sydney).title("Marker in Sydney"));
        googleMap.moveCamera(CameraUpdateFactory.newLatLng(sydney));
    }

    public void openMonitor(){
        runstop_btn.setVisibility(View.INVISIBLE);
        kill_btn.setVisibility(View.INVISIBLE);
        monitor_btn.setVisibility(View.INVISIBLE);
        set_btn.setVisibility(View.INVISIBLE);
        controls.setVisibility(View.VISIBLE);
    }

    public void sendKillSignal(){
        String killId="d";
        byte[] bytesToSend = killId.getBytes();
        myThreadConnected.write(bytesToSend);
        Toast.makeText(this, "Kill Switch pressed", Toast.LENGTH_SHORT).show();

    }

    public void backToControls(){
        controls.setVisibility(View.INVISIBLE);
        runstop_btn.setVisibility(View.VISIBLE);
        kill_btn.setVisibility(View.VISIBLE);
        monitor_btn.setVisibility(View.VISIBLE);
        set_btn.setVisibility(View.VISIBLE);
    }
    public void setDest(){

        if(markerPoints.size()>=0){
            markerPoints.clear();
            mMap.clear();
        }
    }

    @Override
    protected void onStart() {
        super.onStart();

        //Turn ON BlueTooth if it is OFF
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        }

        setup();
    }

    private void setup() {
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            pairedDeviceArrayList = new ArrayList<BluetoothDevice>();

            for (BluetoothDevice device : pairedDevices) {
                pairedDeviceArrayList.add(device);
            }

            pairedDeviceAdapter = new ArrayAdapter<BluetoothDevice>(this,
                    android.R.layout.simple_list_item_1, pairedDeviceArrayList);

            listViewPairedDevice.setAdapter(pairedDeviceAdapter);

            listViewPairedDevice.setOnItemClickListener(new AdapterView.OnItemClickListener() {

                @Override
                public void onItemClick(AdapterView<?> parent, View view,
                                        int position, long id) {
                    BluetoothDevice device =
                            (BluetoothDevice) parent.getItemAtPosition(position);

                    myThreadConnectBTdevice = new ThreadConnectBTdevice(device);
                    myThreadConnectBTdevice.start();
                }
            });
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if(myThreadConnectBTdevice!=null){
            myThreadConnectBTdevice.cancel();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(requestCode==REQUEST_ENABLE_BT){
            if(resultCode == Activity.RESULT_OK){
                setup();
            }else{
                Toast.makeText(this,
                        "BlueTooth NOT enabled",
                        Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    //Called in ThreadConnectBTdevice once connect successed
    //to start ThreadConnected
    private void startThreadConnected(BluetoothSocket socket){

        myThreadConnected = new ThreadConnected(socket);
        myThreadConnected.start();
    }

    /*
    ThreadConnectBTdevice:
    Background Thread to handle BlueTooth connecting
    */
    private class ThreadConnectBTdevice extends Thread {

        private BluetoothSocket bluetoothSocket = null;
        private final BluetoothDevice bluetoothDevice;


        private ThreadConnectBTdevice(BluetoothDevice device) {
            bluetoothDevice = device;

            try {
                bluetoothSocket = device.createRfcommSocketToServiceRecord(myUUID);
                //textStatus.setText("bluetoothSocket: \n" + bluetoothSocket);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            boolean success = false;
            try {
                bluetoothSocket.connect();
                success = true;
                //inputPane.setVisibility(View.GONE);
            } catch (IOException e) {
                e.printStackTrace();

                final String eMessage = e.getMessage();
                runOnUiThread(new Runnable() {

                    @Override
                    public void run() {
                        //textStatus.setText("something wrong bluetoothSocket.connect(): \n" + eMessage);
                    }
                });

                try {
                    bluetoothSocket.close();
                } catch (IOException e1) {
                    // TODO Auto-generated catch block
                    e1.printStackTrace();
                }
            }

            if(success){
                runOnUiThread(new Runnable() {

                    @Override
                    public void run() {

                        Toast.makeText(MapsActivity1.this,"Connection On",Toast.LENGTH_SHORT).show();

                    }
                });


                startThreadConnected(bluetoothSocket);

            }else{
                //fail
            }
        }

        public void cancel() {

            Toast.makeText(getApplicationContext(),
                    "Connection Off",
                    Toast.LENGTH_LONG).show();

            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

        }

    }

    /*
    ThreadConnected:
    Background Thread to handle Bluetooth data communication
    after connected
     */
    public class ThreadConnected extends Thread {
        private final BluetoothSocket connectedBluetoothSocket;
        private final InputStream connectedInputStream;
        private final OutputStream connectedOutputStream;

        public ThreadConnected(BluetoothSocket socket) {
            connectedBluetoothSocket = socket;
            InputStream in = null;
            OutputStream out = null;

            try {
                in = socket.getInputStream();
                out = socket.getOutputStream();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

            connectedInputStream = in;
            connectedOutputStream = out;
        }

        @Override
        public void run() {
            byte[] buffer = new byte[1024];
            int bytes = 0;
            int begin = 0;

            while (true) {
                try {

                    bytes += connectedInputStream.read(buffer, bytes, buffer.length - bytes);
                    for(int i = begin; i < bytes; i++) {
                        if (buffer[i] == "#".getBytes()[0]) {
                            mHandler.obtainMessage(1, begin, i, buffer).sendToTarget();
                            begin = i + 1;
                            if (i == bytes - 1) {
                                bytes = 0;
                                begin = 0;
                            }
                        }
                    }

                    //bytes = connectedInputStream.read(buffer);

                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();

                    final String msgConnectionLost = "Connection lost:\n"
                            + e.getMessage();
                    runOnUiThread(new Runnable(){

                        @Override
                        public void run() {
                            //textStatus.setText(msgConnectionLost);
                        }});
                }
            }
        }

        public void write(byte[] buffer) {
            try {
                connectedOutputStream.write(buffer);
                connectedOutputStream.flush();
                //Thread.sleep(1000);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        public void cancel() {
            try {
                connectedBluetoothSocket.close();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }

}

