package com.dev.javier.redsensores;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Spinner;
import android.widget.TextView;

import com.dev.javier.redsensores.Nodo;

public class BaseActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener  {

    private Menu menu;
    int NodoSel;
    public final static String EXTRA_MESSAGE = "NodoSel";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
    protected  void onStart() {
        super.onStart();
    //Muestra la barra superior y el menu
    Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
    setSupportActionBar(toolbar);

    DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
    ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
            this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
    toggle.syncState();

    final NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
    navigationView.setNavigationItemSelectedListener(this);

}
    //Funcion pulsar el boton "atras"
    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            Intent intent = new Intent(this, MainActivity.class);
            startActivity(intent);
        }
    }
    //Elementos del menu
    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.R1NCentral) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=0;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.R1NUno) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=1;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.R1NDos) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=2;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.R1NTres) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=3;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.R1NCuatro) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=4;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.R1NCinco) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=5;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.R1NSeis) {
            Intent intent = new Intent(this, Nodo.class);
            NodoSel=6;
            intent.putExtra(EXTRA_MESSAGE, NodoSel);
            startActivity(intent);

        } else if (id == R.id.Ajustes) {
            Intent intent = new Intent(this, Settings.class);
            startActivity(intent);
        } else if (id == R.id.Conexion) {
            Intent intent = new Intent(this, MainActivity.class);
            startActivity(intent);
        }
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }


    /**Modificar valores elementos menu lateral*/
    //Cambiar nombre red
    public void SetNombreRed (String NombreRed){
        final NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        View header=navigationView.getHeaderView(0);
        TextView NombRed = (TextView) header.findViewById(R.id.Nombre_Red);
        NombRed.setText(NombreRed);
    }
    //Cambiar nombre opcion
    public void SetNombreOpcionMenu(int id, String string)
    {   NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        menu = navigationView.getMenu();
        MenuItem item = menu.findItem(id);
        item.setTitle(string);
    }
    //Ocultar opcion
    public void hideOpcionMenu(int id)
    {   NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        menu = navigationView.getMenu();
        MenuItem item = menu.findItem(id);
        item.setVisible(false);
    }
    //Mostrar opcion
    public void showOpcionMenu(int id)
    {   NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        menu = navigationView.getMenu();
        MenuItem item = menu.findItem(id);
        item.setVisible(true);
    }
    //Eliminar opcion
    public void deleteOpcionMenu(int id)
    {   NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        menu = navigationView.getMenu();
        menu.removeItem(id);
    }
}
