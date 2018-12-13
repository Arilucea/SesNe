package com.dev.javier.redsensores;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.text.Editable;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

/**
 * Created by Javier on 08/07/2017.
 */

public class Settings extends BaseActivity {

    //Variables recibir
    LocalService mService;
    boolean mBound = false;

    //Almacenar datos blockchain
    boolean blockAlmacenar = false;
    @BindView(R.id.boton_config)
    Button botonConfig;
    @BindView(R.id.TextAddress)
    EditText TextAddress;
    @BindView(R.id.TextKey)
    EditText TextKey;
    @BindView(R.id.BoolSaveData)
    CheckBox BoolSaveData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
        ButterKnife.bind(this);

        SharedPreferences blockchain = getSharedPreferences("Block", 0);
        String Address = blockchain.getString("Address", "0x0");
        blockAlmacenar = blockchain.getBoolean("Send", false);

        BoolSaveData.setChecked(blockAlmacenar);
        TextAddress.setText(Address);
    }

    @Override
    protected void onStart() {
        super.onStart();
        // Bind to LocalService
        Intent intent = new Intent(this, LocalService.class);
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        super.onStop();
        // Unbind from the service
      /*  if (mBound) {
            unbindService(mConnection);
            mBound = false;
        }*/
    }

    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className,
                                       IBinder service) {
            // We've bound to LocalService, cast the IBinder and get LocalService instance
            LocalService.LocalBinder binder = (LocalService.LocalBinder) service;
            mService = binder.getService();
            mBound = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0) {
            mBound = false;
        }
    };

    //Muestra mensajes en pantalla
    private void msg(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    @OnClick(R.id.boton_config)
    public void SetConfig() {
        Editable key = TextKey.getText();
        Editable address = TextAddress.getText();

        Boolean Send =  BoolSaveData.isChecked();
        SharedPreferences blockchain = getSharedPreferences("Block", 0);
        SharedPreferences.Editor editor = blockchain.edit();
        editor.putString("Address", address.toString());
        editor.putString("Key", key.toString());
        editor.putBoolean("Send", Send);
        editor.commit();

        msg("Saved");
    }
}
