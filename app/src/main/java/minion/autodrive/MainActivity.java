package minion.autodrive;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;

public class MainActivity extends AppCompatActivity {
    public final static String EXTRA_MESSAGE="minion.autodrive.MESSAGE";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        View.OnClickListener handler = new View.OnClickListener(){
            public void onClick(View view) {
                //Switching action to be performed
                switch (view.getId()) {

                    case R.id.car_connect: //toast will be shown
                        openCarConnect();
                        break;
                    case R.id.map_mode: //program will end
                        openMapMode(view);
                        break;
                }
            }
        };

        //Setting the listeners
        findViewById(R.id.car_connect).setOnClickListener(handler);
        findViewById(R.id.map_mode).setOnClickListener(handler);
        //findViewById(R.id.help).setOnClickListener(handler);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        MenuInflater inflater=getMenuInflater();
        inflater.inflate(R.menu.main_activity_actions, menu);
        return super.onCreateOptionsMenu(menu);
    }

    private void openCarConnect(){
        Intent intent = new Intent(this, BluetoothActivity.class);
        startActivity(intent);

    }


    private void openMapMode(View view) {
        Intent intent = new Intent(this, MapsActivity1.class);
        startActivity(intent);

    }
}


