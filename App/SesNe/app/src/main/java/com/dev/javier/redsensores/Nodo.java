package com.dev.javier.redsensores;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.view.Menu;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import com.dev.javier.redsensores.apicall;
/**
 * Created by Javier on 08/07/2017.
 */

public class Nodo  extends BaseActivity {

    @BindView(R.id.boton_recibir)
    Button button2;
    @BindView(R.id.tabla)
    TableLayout tabla;

    //Variables recibir
    LocalService mService;
    boolean mBound = false;
    String recib;
    String s="";
    String s2="";
    int lon=0;
    Handler mHandlerRecibir;

    int NodoSel=0;

    //Separar-crear filas
    String NSensor=""; //Variable para almacenar el nombre del sensor
    String VSensor=""; //Variable para almacenar la magnitud medida por el sensor
    String USensor=""; //Variable para almacenar el tipo del sensor

    int Ncompletado=0;
    int Vcompletado=0;
    int Ucompletado=0;

    int CharDiv=0; //Caracter indica barra divisora
    int CharDiv2=0; //Caracter indica barra divisora

    String cadInicial="";

    //Strings configuracion menu
    String NumNodo=""; //Numero de nodos
    String NomNodo=""; //Nombre de los nodos
    int Contador_Nodos=0;
    int Menu_config=0;

    private int Menu_IDs[]={R.id.R1NUno,R.id.R1NDos,R.id.R1NTres,R.id.R1NCuatro,R.id.R1NCinco,R.id.R1NSeis};
    private String Menu_N[]={"0","0","0","0","0","0","0","0","0","0","0"};
    private int NumNodoI=0; //Numero de los nodos en int


    //Lectura datos
    String Nom_Red="";
    String Nom_Cent="";

    //Auxiliar unidades
    String Unidades="";

    //Almacenar datos blockchain
    boolean blockAlmacenar = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_nodo);
        ButterKnife.bind(this);

        Intent intent = getIntent();
        NodoSel = intent.getIntExtra("NodoSel",0);

        cadInicial = Integer.toString(NodoSel) + "F";

        SharedPreferences settings = getSharedPreferences("Menu", 0);
        NumNodoI = settings.getInt("NumNodoI", 0);
        Nom_Red = settings.getString("NomRed", "Red");
        Nom_Cent = settings.getString("NomCent", "Nodo 0");
        SetNombreRed(Nom_Red);
        SetNombreOpcionMenu(R.id.R1NCentral,Nom_Cent);

        for (int i=0; i<NumNodoI-1; i++){
            String NomGuardar = "Menu_N" + Integer.toString(i+1);
            String Nom_Nodo = settings.getString(NomGuardar, "");
            SetNombreOpcionMenu(Menu_IDs[i],Nom_Nodo);
            Menu_N[i+1]=Nom_Nodo;
            showOpcionMenu(Menu_IDs[i]);
        }
        if (NodoSel == 0){
            setTitle(Nom_Cent);
        } else {
            setTitle(Menu_N[NodoSel]);
        }


        SharedPreferences blockchain = getSharedPreferences("Block", 0);
        blockAlmacenar = blockchain.getBoolean("Send", false);
    }

    //Envia el numero del nodo seleccionado y comienza a recibir datos
    @OnClick(R.id.boton_recibir)
    public void RecibirDatos() {
        mService.Enviar(cadInicial);
        mHandlerRecibir = new Handler();
        mHandlerRecibir.postDelayed(mAction, 500);
    }

    //Runnable para recibir los datos
    Runnable mAction = new Runnable() {
        @Override
        public void run() {
            //mService.Enviar(cadInicial);
            recib = mService.getReadMessage();
            lon = recib.length();
            s2 = recib.substring(1, lon);
            separar();
            mHandlerRecibir.postDelayed(mAction, 3000);

        }
    };

    void separar(){
        tabla.removeAllViews();
        //Separa la cadena de nombres de los nodos
        if (s2.substring(0, 1).equals("%")){ //Comprueba que se recibe el caracter inicial
            //Define el sistema de almacenamiento
            SharedPreferences settings = getSharedPreferences("Menu", 0);
            SharedPreferences.Editor editor = settings.edit();
            if (Menu_config==0) {   //Comprueba que no se ha creado el menu todavia
                s2 = s2.substring(2, s2.length());
                for (int i = 0; i < s2.length(); i++) {
                    if (s2.substring(i, i + 1).equals("/")) {
                        Ncompletado = 1;
                        CharDiv = 1;
                    } else {
                        if (Ncompletado == 0) {
                            NumNodo = NumNodo + s2.substring(i, i + 1);
                        }
                    }
                    if (Ncompletado == 1) {
                        if (s2.substring(i, i + 1).equals("%")) {
                            Vcompletado = 1;
                        } else {
                            if (Ncompletado == 1 && Vcompletado == 0 && CharDiv == 0)
                                NomNodo = NomNodo + s2.substring(i, i + 1);
                        }
                    }
                    NumNodoI = Integer.parseInt(NumNodo);
                    editor.putInt("NumNodoI", NumNodoI);
                    editor.commit();
                    NumNodoI = NumNodoI + 1;

                    if (Vcompletado == 1 && NumNodoI > 0) {
                        if (Contador_Nodos == 0) {
                            Nom_Red = NomNodo;
                            SetNombreRed(NomNodo);
                                editor.putString("NomRed", NomNodo);
                                editor.commit();
                        }else{
                            if (Contador_Nodos == 1){
                                SetNombreOpcionMenu(R.id.R1NCentral,NomNodo);
                                editor.putString("NomCent", NomNodo);
                                editor.commit();
                            }else{
                                SetNombreOpcionMenu(Menu_IDs[Contador_Nodos-2],NomNodo);
                                Menu_N[Contador_Nodos-2]=NomNodo;
                                String NomGuardar = "Menu_N" + Integer.toString(Contador_Nodos-1);
                                editor.putString(NomGuardar, NomNodo);
                                editor.commit();
                            }
                        }
                        NomNodo = "";
                        Vcompletado = 0;
                        Contador_Nodos = Contador_Nodos + 1;
                        NumNodoI = NumNodoI - 1;
                    }
                    CharDiv = 0;
                }
                Menu_config=1;
            }
            Contador_Nodos=0;
            //Muestra los nodos disponibles
            for (int i=0; i<NumNodoI-1; i++){
                showOpcionMenu(Menu_IDs[i]);
            }
        }else{////////////////////////////////////////////////////////////////////////////////////////
            for (int i=0;i<s2.length(); i++){
                if (s2.substring(i, i+1).equals("|")) {
                    Ncompletado=1;
                    CharDiv=1;
                }else {
                    if (Ncompletado == 0) {
                        NSensor = NSensor + s2.substring(i, i+1);
                    }
                }
                if (s2.substring(i, i+1).equals("/")){
                    Vcompletado=1;
                    CharDiv2=1;
                }else{
                    if (Ncompletado==1 && Vcompletado==0 && CharDiv==0)
                        VSensor=VSensor+s2.substring(i,i+1);
                }
                if (s2.substring(i, i+1).equals("!")){
                    Ucompletado=1;
                }else{
                    if (Ncompletado==1 && Vcompletado==1 && CharDiv2==0)
                        USensor=USensor+s2.substring(i,i+1);
                }

                if (Ucompletado==1){
                    USensor = Unidades(USensor);
                    AddTabla(USensor,NSensor,VSensor);

                    if (blockAlmacenar == true) {
                        String nodo = (String) getTitle();
                        String rsp = apicall.Api(this, Nom_Red,  nodo, VSensor, NSensor, USensor);
                    }
                    Ncompletado=0;
                    Vcompletado=0;
                    Ucompletado=0;
                    VSensor="";
                    NSensor="";
                    USensor="";
                }
                CharDiv=0;
                if (Vcompletado==1) {CharDiv2=0;}
            }
            Ncompletado=0;
        }
    }
    //Cambia el valor recibido de las unidades por su correspondiente
    String Unidades (String sel) {
        String unidades = "-";
        if (sel.equals("0")){ unidades = "C";
        }else if(sel.equals("1")){unidades = "m";
        }else if(sel.equals("2")){unidades = "cm";
        }else if(sel.equals("3")){unidades = "bar";
        }else if(sel.equals("4")){unidades = "mL";
        }else if(sel.equals("5")){unidades = "Lm";
        }else if(sel.equals("6")){unidades = "RH%";
        }else if(sel.equals("7")){unidades = "V";
        }else if(sel.equals("8")){unidades = "mV";
        }else if(sel.equals("9")){unidades = "A";
        }else if(sel.equals("10")){unidades = "mA";
        }else if(sel.equals("11")){unidades = "True";
        }else if(sel.equals("12")){unidades = "False";
        }else if(sel.equals("13")){unidades = "m/s";
        }else if(sel.equals("14")){unidades = "g";
        }else if(sel.equals("15")){unidades = "kg";
        }else {unidades = "-";
        }
        return unidades;
    }

    void AddTabla(String Unidades, String Nombre, String Valor){
        //Declaracion campos
        TableRow row = (TableRow) getLayoutInflater().inflate(R.layout.fila, null);
        TextView nombreSensor,valorSensor,unidadesSensor;

        //Escribe el nombre del sensor
        nombreSensor = (TextView) row.findViewById(R.id.nombreSensor);
        nombreSensor.setTextColor(Color.rgb(255,255,255));
        nombreSensor.setText(Nombre);

        //Escribe el valor del sensor
        valorSensor = (TextView) row.findViewById(R.id.valorSensor);
        valorSensor.setTextColor(Color.rgb(255,255,255));
        valorSensor.setText(Valor);

        //Escribe las unidades
        unidadesSensor = (TextView) row.findViewById(R.id.Unidades);
        unidadesSensor.setTextColor(Color.rgb(255,255,255));
        unidadesSensor.setText(Unidades);

        //Añade la fila
        tabla.addView(row);

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
        mHandlerRecibir.removeCallbacks(mAction);
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


}
