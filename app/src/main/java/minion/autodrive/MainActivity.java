package minion.autodrive;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Display;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    public final static String EXTRA_MESSAGE="com.example.akshay.myfirstapp.MESSAGE";

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
                    case R.id.drive_mode: //program will end
                        openDriveModes(view);
                        break;
                    case R.id.help:
                        openHelp();
                        break;
                }
            }
        };

        //Setting the listeners
        findViewById(R.id.car_connect).setOnClickListener(handler);
        findViewById(R.id.drive_mode).setOnClickListener(handler);
        findViewById(R.id.help).setOnClickListener(handler);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        MenuInflater inflater=getMenuInflater();
        inflater.inflate(R.menu.main_activity_actions,menu);
        return super.onCreateOptionsMenu(menu);
        //getMenuInflater().inflate(R.menu.menu_my, menu);
        //return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        switch (item.getItemId()){
            case R.id.action_search:
                openSearch();
                return true;
            case R.id.action_settings:
                openSettings();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
   /*     int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
   */
    }

    //Perform action when user sends message
    /*public void sendMessage(View view){
        Intent intent= new Intent(this, DisplayMessageActivity.class);
        EditText editText=(EditText) findViewById(R.id.edit_message);
        String message= editText.getText().toString();
        intent.putExtra(EXTRA_MESSAGE,message);
        startActivity(intent);
    }
*/
    //Perform action when user clicks on Map
    public void openMaps(View view) {
        Intent intent = new Intent(this, MapsActivity.class);
        //EditText editText = (EditText) findViewById(R.id.edit_message);
        //String message = editText.getText().toString();
        //intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);
    }

    private void openCarConnect(){
        Toast.makeText(this, "Car Connect button pressed", Toast.LENGTH_SHORT).show();
    }

    private void openDriveModes(View view){
        Intent intent = new Intent(this, DriveModesActivity.class);
        startActivity(intent);
    }

    private void openHelp(){
        Toast.makeText(this, "Help button pressed", Toast.LENGTH_SHORT).show();
    }


    private void openSearch() {
        Toast.makeText(this, "Search button pressed", Toast.LENGTH_SHORT).show();
    }

    private void openSettings() {
        Toast.makeText(this, "Settings button pressed", Toast.LENGTH_SHORT).show();
    }

}


