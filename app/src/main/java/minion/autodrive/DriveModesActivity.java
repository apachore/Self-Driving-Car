package minion.autodrive;


import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

public class DriveModesActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drive_modes);

        View.OnClickListener handler = new View.OnClickListener(){
            public void onClick(View view) {
                //Switching action to be performed
                switch (view.getId()) {

                    case R.id.map_mode: //toast will be shown
                        openMapMode(view);
                        break;
                    case R.id.free_run_mode: //program will end
                        openFreeRunMode(view);
                        break;
                }
            }
        };

        //Setting the listeners
        findViewById(R.id.map_mode).setOnClickListener(handler);
        findViewById(R.id.free_run_mode).setOnClickListener(handler);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_drive_modes, menu);
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

    private void openMapMode(View view) {
        //Toast.makeText(this, "Search button pressed", Toast.LENGTH_SHORT).show();
        Intent intent = new Intent(this, MapsActivity.class);
        //EditText editText = (EditText) findViewById(R.id.edit_message);
        //String message = editText.getText().toString();
        //intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);

    }

    private void openFreeRunMode(View view) {
        //Toast.makeText(this, " button pressed", Toast.LENGTH_SHORT).show();
        Intent intent = new Intent(this, FreeRunActivity.class);
        //EditText editText = (EditText) findViewById(R.id.edit_message);
        //String message = editText.getText().toString();
        //intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);
    }

}
