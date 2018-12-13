package com.dev.javier.redsensores;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.StrictMode;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

public class apicall {

    public static String Api(Context context, String Nom_Red, String Nom_Nodo, String Lectura, String Unidades, String Nom_sensor) {

        SharedPreferences blockchain = context.getSharedPreferences("Block", 0);
        String Address = blockchain.getString("Address", "0x0");
        String Key = blockchain.getString("Key", "00000");

        String json = "";
        String sql = "http://ip:port/sensores/guardar?address=" + Address + "&clave=" + Key + "&red=" + Nom_Red + "&nodo=" + Nom_Nodo + "&sensornombre=" + Nom_sensor + "&unidades=" + Unidades + "&medida=" + Lectura;

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        URL url = null;
        HttpURLConnection conn;

        try {
            url = new URL(sql);
            conn = (HttpURLConnection) url.openConnection();

            conn.setRequestMethod("GET");

            conn.connect();

            BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));

            String inputLine;

            StringBuffer response = new StringBuffer();


            while ((inputLine = in.readLine()) != null) {
                response.append(inputLine);
            }

            json = response.toString();
            Log.d("a", json);


        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return (json);
    }
}
