package com.example.sushmajdeshpande.bluetooth;

/*import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}*/


        import android.os.Bundle;
        import android.app.Activity;
        import android.bluetooth.BluetoothAdapter;
        import android.bluetooth.BluetoothDevice;
        //import android.bluetooth.BluetoothSocket;
        import android.content.BroadcastReceiver;
        import android.content.Context;

        //import java.io.IOException;
        //import java.io.InputStream;
        //import java.io.OutputStream;
        //import java.util.ArrayList;
        import java.util.Set;
        //import java.util.UUID;

        import android.content.Intent;
        import android.content.IntentFilter;
        //import android.os.Handler;
        //import android.os.Bundle;
        //import android.os.Message;
        import android.view.View;
        //import android.view.View.OnClickListener;
        //import android.widget.AdapterView;
        import android.widget.AdapterView;
        import android.widget.ArrayAdapter;
        import android.widget.Button;
        import android.widget.ListView;
        import android.widget.TextView;
        import android.widget.Toast;


public class MainActivity extends Activity {

    private static final int REQUEST_ENABLE_BT = 1;
    private Button onBtn;
    private Button offBtn;
    private Button listBtn;
    private Button findBtn;
    private TextView text;
    private BluetoothAdapter myBluetoothAdapter;
    private Set<BluetoothDevice> pairedDevices;
    private ListView myListView;
    private ArrayAdapter<String> BTArrayAdapter;
    //public static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    /*
public class Main_Activity extends Activity implements AdapterView.OnItemClickListener {
        ArrayAdapter<String> listAdapter;
        ListView ListView;
        BluetoothAdapter btAdapter;
        Set<BluetoothDevice>devicesArray;
        ArrayList<String>pairedDevices;
        public static final UUID MY_UUID=UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
        protected static final int SUCCESS_CONNECT=0;
        protected static final int MESSAGE_READ=1;
        IntentFilter filter;
        BroadcastReceiver receiver;
        String tag="debugging";
        Handler mHandler= new Handler();
        {
            //@Override
                    public void handleMessage(Message msg)
            {
                mHandler.handleMessage(msg);
                switch(msg.what) {

                    case SUCCESS_CONNECT:
                        ConnectedThread connectedThread = new ConnectedThread((BluetoothSocket) msg.obj);
                        Toast.makeText(getApplicationContext(), "CONNECT", Toast.LENGTH_SHORT).show();
                        break;
                    case MESSAGE_READ:
                    break;
                }


            }
            }
        }

}
*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // take an instance of BluetoothAdapter - Bluetooth radio
        myBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(myBluetoothAdapter == null) {
            onBtn.setEnabled(false);
            offBtn.setEnabled(false);
            listBtn.setEnabled(false);
            findBtn.setEnabled(false);
            text.setText("Status: not supported");
            Toast.makeText(getApplicationContext(),"Your device does not support Bluetooth",
                    Toast.LENGTH_LONG).show();
        } else
            {
            text = (TextView) findViewById(R.id.text);
            onBtn = (Button)findViewById(R.id.turnOn);
            onBtn.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    // TODO Auto-generated method stub
                    on(v);
                }

            });

            offBtn = (Button)findViewById(R.id.turnOff);
            offBtn.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    // TODO Auto-generated method stub
                    off(v);
                }
            });

            listBtn = (Button)findViewById(R.id.paired);
            listBtn.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    // TODO Auto-generated method stub
                    list(v);
                }
            });
            findBtn = (Button)findViewById(R.id.search);
            findBtn.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View v) {
                    // TODO Auto-generated method stub
                    find(v);
                }

            });
            myListView = (ListView)findViewById(R.id.listView1);

            // create the arrayAdapter that contains the BTDevices, and set it to the ListView
            BTArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
            myListView.setAdapter(BTArrayAdapter);


        }

    }


    public void on(View view){
        if (!myBluetoothAdapter.isEnabled()) {
            Intent discoverableIntent = new
                    Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
            //Intent turnOnIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            //startActivityForResult(turnOnIntent, REQUEST_ENABLE_BT);

            Toast.makeText(getApplicationContext(),"Bluetooth turned on" ,
                    Toast.LENGTH_LONG).show();

        }
        else{
            Toast.makeText(getApplicationContext(),"Bluetooth is already on",
                    Toast.LENGTH_LONG).show();
        }
    }
/*
    private class ConnectThread extends Thread {

        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {
            // Use a temporary object that is later assigned to mmSocket,
            // because mmSocket is final
            BluetoothSocket tmp = null;
            mmDevice = device;

            // Get a BluetoothSocket to connect with the given BluetoothDevice
            try {
                // MY_UUID is the app's UUID string, also used by the server code
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) { }
            mmSocket = tmp;
        }

        public void run() throws IOException {
            // Cancel discovery because it will slow down the connection
            BTArrayAdapter.cancelDiscovery();

            try {
                // Connect the device through the socket. This will block
                // until it succeeds or throws an exception
                mmSocket.connect();
            } catch (IOException connectException) {
                // Unable to connect; close the socket and get out
                mmSocket.close();
                return;


            }

            // Do work to manage the connection (in a separate thread)
            manageConnectedSocket(mmSocket);
        }

        *//** Will cancel an in-progress connection, and close the socket *//*
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }*/

    //private void manageConnectedSocket(BluetoothSocket mmSocket) {

    //}

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // TODO Auto-generated method stub
        if(requestCode == REQUEST_ENABLE_BT){
            if(myBluetoothAdapter.isEnabled()) {
                text.setText("Status: Enabled");
            } else {
                text.setText("Status: Disabled");
            }

        }
    }


    public void list(View view){
        // get paired devices
        pairedDevices = myBluetoothAdapter.getBondedDevices();

        // put it's one to the adapter
        for(BluetoothDevice device : pairedDevices)
        BTArrayAdapter.add(device.getName()+ "\n" + device.getAddress());
        Toast.makeText(getApplicationContext(),"Show Paired Devices",
                Toast.LENGTH_SHORT).show();
    }

    final BroadcastReceiver bReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                // add the name and the MAC address of the object to the arrayAdapter
                BTArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                BTArrayAdapter.notifyDataSetChanged();
            }

        }

    };



    public void find(View view) {
        if (myBluetoothAdapter.isDiscovering()) {
            // the button is pressed when it discovers, so cancel the discovery
            myBluetoothAdapter.cancelDiscovery();
        }
        else {
            BTArrayAdapter.clear();
            myBluetoothAdapter.startDiscovery();
            registerReceiver(bReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        }
    }

    /*private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);
                    // Send the obtained bytes to the UI activity
                    mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                            .sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }*/

        /* Call this from the main activity to send data to the remote device */
       /* public void write(byte[] bytes) {
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }

        /* Call this from the main activity to shutdown the connection */
        /*public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }*/
    public void off(View view){
        myBluetoothAdapter.disable();
        text.setText("Status: Disconnected");
        Toast.makeText(getApplicationContext(),"Bluetooth turned off",
                Toast.LENGTH_LONG).show();
    }
    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        unregisterReceiver(bReceiver);
    }

}

