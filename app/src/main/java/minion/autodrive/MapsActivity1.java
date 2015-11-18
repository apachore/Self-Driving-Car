package minion.autodrive;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;


public class MapsActivity1 extends FragmentActivity implements OnMapReadyCallback {

    private GoogleMap mMap; // Might be null if Google Play services APK is not available.
    private static final int REQUEST_ENABLE_BT = 1;
    private boolean startflag=false;
    private float latitude_s=0;
    private float longitude_s=0;

    BluetoothAdapter bluetoothAdapter;
    ArrayList<BluetoothDevice> pairedDeviceArrayList;

    TextView textInfo;
    ListView listViewPairedDevice;
    LinearLayout inputPane;

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
        listViewPairedDevice = (ListView)findViewById(R.id.pairedlist);
        inputPane = (LinearLayout)findViewById(R.id.inputpane);

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
                }
            }
        };

        //Setting the listeners
        findViewById(R.id.run_stop_toggle).setOnClickListener(handler);
        findViewById(R.id.monitor).setOnClickListener(handler);
        findViewById(R.id.kill).setOnClickListener(handler);
        findViewById(R.id.setdest).setOnClickListener(handler);

        //monitor=(Button)findViewById(R.id.monitor);

        SupportMapFragment mapFragment=(SupportMapFragment)getSupportFragmentManager().findFragmentById(R.id.map);
        mMap=mapFragment.getMap();

        mMap.moveCamera( CameraUpdateFactory.newLatLngZoom(new LatLng(37.336079,-121.880453) , 14.0f) );

        mMap.setOnMapClickListener(new GoogleMap.OnMapClickListener() {
            @Override
            public void onMapClick(LatLng latLng) {
                // Creating a marker
                MarkerOptions markerOptions = new MarkerOptions();

                // Setting the position for the marker
                markerOptions.position(latLng);

                // Setting the title for the marker.
                // This will be displayed on taping the marker
                markerOptions.title(latLng.latitude + " : " + latLng.longitude);

                System.out.println("Latitude " + latLng.latitude + " Info " + latLng.toString());
                latitude_s=(float)latLng.latitude;
                longitude_s=(float)latLng.longitude;

                // Clears the previously touched position
                mMap.clear();

                // Animating to the touched position
                mMap.animateCamera(CameraUpdateFactory.newLatLng(latLng));

                // Placing a marker on the touched position
                mMap.addMarker(markerOptions);
            }
        });

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

    public void onToggleClicked(View view) {
        // Is the toggle on?
        boolean on = ((ToggleButton) view).isChecked();
        if (latitude_s != 0 && longitude_s != 0)
            {
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
    }
        else {

            Toast.makeText(this, "Destination needed", Toast.LENGTH_LONG).show();

        }

    }


    /**
     * This is where we can add markers or lines, add listeners or move the camera. In this case, we
     * just add a marker near Africa.
     * <p/>
     * This should only be called once and when we are sure that {@link #mMap} is not null.
     */

    @Override
    public void onMapReady(GoogleMap googleMap) {
        LatLng sydney = new LatLng(37.3353, 121.8813);
        googleMap.addMarker(new MarkerOptions().position(sydney).title("Marker in Sydney"));
        googleMap.moveCamera(CameraUpdateFactory.newLatLng(sydney));
    }

    public void openMonitor(){
        String monitorId="e";
        byte[] bytesToSend = monitorId.getBytes();
        myThreadConnected.write(bytesToSend);
        Toast.makeText(this, "Monitor pressed", Toast.LENGTH_SHORT).show();
    }

    public void sendKillSignal(){
        String killId="d";
        byte[] bytesToSend = killId.getBytes();
        myThreadConnected.write(bytesToSend);
        Toast.makeText(this, "Kill Switch pressed", Toast.LENGTH_SHORT).show();

    }

    public void setDest(){
        String destId="f";

        String latinput = Float.toString(latitude_s);
        String loninput = Float.toString(longitude_s);

        String coordinate=destId+","+latinput+","+loninput;

        byte[] coordinateBytes = coordinate.getBytes();

        myThreadConnected.write(coordinateBytes);

        Toast.makeText(this, "Destination Set", Toast.LENGTH_SHORT).show();

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
                //connect successful
     /*           final String msgconnected = "connect successful:\n"
                        + "BluetoothSocket: " + bluetoothSocket + "\n"
                        + "BluetoothDevice: " + bluetoothDevice;
*/
                runOnUiThread(new Runnable() {

                    @Override
                    public void run() {
                        //textStatus.setText("");
                        //textByteCnt.setText("");
                        //Toast.makeText(MainActivity.this, msgconnected, Toast.LENGTH_LONG).show();

                        Toast.makeText(MapsActivity1.this,"Paired successfully",Toast.LENGTH_SHORT).show();

                        //listViewPairedDevice.setVisibility(View.GONE);
                        //textInfo.setVisibility(View.GONE);
                        //inputPane.setVisibility(View.VISIBLE);
                        //dashboard.setVisibility(View.VISIBLE);
                    }
                });

                startThreadConnected(bluetoothSocket);

            }else{
                //fail
            }
        }

        public void cancel() {

            Toast.makeText(getApplicationContext(),
                    "close bluetoothSocket",
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
    private class ThreadConnected extends Thread {
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
            int bytes;

            String strRx = "";

            while (true) {
                try {
                    bytes = connectedInputStream.read(buffer);
                    final String strReceived = new String(buffer, 0, bytes);
                    final String strByteCnt = String.valueOf(bytes) + " bytes received.\n";

                    runOnUiThread(new Runnable(){

                        @Override
                        public void run() {
                            //textStatus.append(strReceived);
                            //textByteCnt.append(strByteCnt);
                        }});

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


