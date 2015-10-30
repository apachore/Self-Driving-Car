package minion.autodrive;


import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

public class FreeRunActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_free_run);

        View.OnClickListener handler = new View.OnClickListener(){
            public void onClick(View view) {
                //Switching action to be performed
                switch (view.getId()) {

                    case R.id.car_start: //toast will be shown
                        carStart();
                        break;
                    case R.id.car_pause: //program will end
                        carPause();
                        break;
                    case R.id.car_stop:
                        carStop();
                        break;
                    case R.id.monitor:
                        openMonitor();
                        break;
                }
            }
        };

        //Setting the listeners
        findViewById(R.id.car_start).setOnClickListener(handler);
        findViewById(R.id.car_pause).setOnClickListener(handler);
        findViewById(R.id.car_stop).setOnClickListener(handler);
        findViewById(R.id.monitor).setOnClickListener(handler);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_free_run, menu);
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


    private void carStart(){
        Toast.makeText(this, "Car Start", Toast.LENGTH_SHORT).show();
    }


    private void carPause() {
        Toast.makeText(this, "Car Pause", Toast.LENGTH_SHORT).show();
    }

    private void carStop() {
        Toast.makeText(this, "Car Stop", Toast.LENGTH_SHORT).show();
    }

    private void openMonitor() {
        Toast.makeText(this, "Monitor", Toast.LENGTH_SHORT).show();
    }

}
