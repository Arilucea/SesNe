/////////////////////////NODO 0///////////////////////////////
#include <SPI.h> // Libreria de SPI para la comunicacion con el modulo nRF24L01
//Librerias para el control del modulo nRF
#include <nRF24L01.h> 
#include <RF24.h>
#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  


RF24 radio(9, 10);//Declaracion de los pines de control CE y CSN para el modulo, se define "radio"
SoftwareSerial BT(6,7);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth


//Se crean las 6 direcciones posibles para las Pipes P0 - P5; 
//Las direcciones es un numero de 40bits para este caso se usa uint_64_t de 64bits
const uint64_t direccion = 0x7878787878LL;

//Variable para almacenar el numero de nodo recibido desde el bluetooth en formato ASCI
String Check="0"; 

//Nodo seleccionado enviar para solicitar informacion sensores
int activ[1];

//Variables nodo 0
int LongitudCadenaN0=12; //Numero de sensores *3
String RecibidaN0[12]; //Array enteros convertida en Strings
String CadenaCompletaN0=""; // Array Strings convertida en una Cadena
String NombresNodo0[]={"Temperatura","Luminosidad","Humedad","Viento"};

//Variables nodo 1
int LongitudCadenaN1=9; //Numero de sensores *3
int RecibidoN1[9]; //Array enteros recibida del Nodo 1
String RecibidaN1[9]; //Array enteros convertida en Strings
String CadenaCompletaN1; // Array Strings convertida en una Cadena
String NombresNodo1[]={"Humedadsuelo","Temperatura","Distancia"};

//Variables nodo 2
int LongitudCadenaN2=12; //Numero de sensores *3
int RecibidoN2[12]; //Array enteros recibida del Nodo 2
String RecibidaN2[12]; //Array enteros convertida en Strings
String CadenaCompletaN2; // Array Strings convertida en una Cadena
String NombresNodo2[]={"Tension","Corriente","Temperatura", "AlturaAgua"};

int Recibido[18]; //Cadena para almacenar los datos recibidos del nodo 1
int contenvio=0;

//Temporizadores deteccion actividad
int temp_conexionI=0,temp_conexionF=0;

//Variables Bluetooth
int enviar=0;
int comenzar=0;
String temp;
String NodoSel;

//Identificadores de la Red
String Nombre_Red="Vid";
String Nombre_Nodo_0="Ambiente";
String Nombre_Nodo_1="Cultivo";
String Nombre_Nodo_2="Bombas riego";
String NRed="";
String Numero_Nodos="3";

//Otras variables
int ciclo_primero=0;

void setup() {  
  BT.begin(9600);       // Inicializamos el puerto serie BT que hemos creado
  // put your setup code here, to run once:

 Serial.begin(9600);
 Serial.println("Serial begin");

//Configuracion de la comunicacion
  radio.begin(); //Inicio del modulo nRF24L01+
  //radio.setRetries(15, 15);// Cinfiguracion del numero maximo de reintentos
  radio.setPALevel(RF24_PA_MAX);  // Se configura a la maxima potencia
  radio.setChannel(100); // Apertura del canal especifico
  
  //Apertura de las lineas de comunicacion con un maximo de 6 direcciones
  radio.openReadingPipe(0, direccion);
  radio.startListening(); //Se inicia en modo de escucha, a la espera de alertas de las alarmas
}

    

void loop() {
  DatosSimulados();
    
  if (comenzar==0){
    ComienzoBluetooth();
  }else{

    if (ciclo_primero==1){
    temp=BT.read();}

    if(temp.equals("70")){
      Serial.println("45655555555556541656161");
     }else{
      NodoSel=temp;
     }
     
    Check = NodoSel;
    if (enviar==1){
    Enviar();
    
    } 
    delay(10);  
  }  
  //temp_conexionF=millis();

  //if (temp_conexionF-temp_conexionI>(2*1000*60)){ //Interrumpe la comunicacion si no se detecta actividad
 //   activ[0]=0;
  //}
}
////////////////////////////////////////////////////////////////
void Enviar(){
  Serial.println("Enviar");
  Serial.println("+++++++++++++++++++++");
  if (Check.equals("48")){
    activ[0]=1;  //Valor activacion nodo 0
  }
  
  if (Check.equals("49")){
    activ[0]=10;  //Valor activacion nodo 1
  }
  if (Check.equals("50")){
    activ[0]=20;  //Valor activacion nodo 2
  }
   Serial.println(activ[0]);
  bool ok = 1;
  /* bool ok = 0;
  radio.stopListening(); // Paramos la escucha para poder escribir
  radio.openWritingPipe(direccion); //Se abre el puerto de escritura para la direccion del modulo 1
  ok = radio.write(activ, sizeof(activ));    //Envia el valor de activacion al nodo 1 y comprueba is se hace correctamente
  Serial.println(ok);*/
  if (ok && (activ[0]==10 || activ[0]==20))
      {
        radio.stopListening(); // Paramos la escucha para poder escribir
        radio.openWritingPipe(direccion); //Se abre el puerto de escritura para la direccion del modulo 1
        ok = radio.write(activ, sizeof(activ));    //Envia el valor de activacion al nodo 1 y comprueba is se hace correctamente
        Serial.println(ok);
        Serial.println("eviado...");
        radio.startListening(); //Se inicia el modo de escucha
        Recibir(); //Ejecuta la funcion recibir
      }else
      {
        if (activ[0]==1){
          Convertir();
        }
      }
  }
////////////////////////////////////////////////////////////////
void Recibir(){
Serial.println("Recibiendo");
delay(200); //Espera para que comience el envio en el otro nodo
  //Lee el dato recibido
  if (radio.available()) //Si hay un emisor
  {
    bool done = false;
    while(!done) //Recibe los valores
    {
    radio.read(Recibido, sizeof(Recibido)); //Recibe los valores del nodo 1
    Serial.println(Recibido[0]);
    Convertir(); //Ejecuta la funcion "Convertir"
    
    if (Recibido[6] != 0) done=true;  //Se comprueba que se han recibido valores
    }
  }else
  {    
      Serial.println("No radio available");
  }
  //if (activ[0]==10){Serial.println(CadenaCompletaN1);EnviarBluetooth(CadenaCompletaN1);}
  //if (activ[0]==20){Serial.println(CadenaCompletaN2);EnviarBluetooth(CadenaCompletaN2);}

}
  
////////////////////////////////////////////////////////////////
void Convertir(){     //Adapta los valores recibidos para mandarlos al dispositivo
  int cont=0;
  contenvio = 0;
  Serial.println("-------------------");
  Serial.println(Recibido[15]);
  Serial.println(Recibido[16]);
  Serial.println(Recibido[17]);
  
  // Recibido del nodo 1 --------------------------------------------------
  if (activ[0]==10){
    CadenaCompletaN1="";  //Vacia la cadena a Enviar
    for (int t=0; t<LongitudCadenaN1; t=t+3){ //Convierte los numero a los parametros medidos
        RecibidaN1[t]=NombresNodo1[t/3];
    }
    for (int t=1; t<LongitudCadenaN1; t=t+3){ //Convierte los numeros en Strings
      RecibidaN1[t]=String(Recibido[t]);
    }

    for (int t=2; t<LongitudCadenaN1; t=t+3){
      RecibidaN1[t]=String(Recibido[t]);
    }
  
    for (int t=0; t<LongitudCadenaN1; t++){ //Forma la cadena final
      CadenaCompletaN1 +=  RecibidaN1[t];
      if (cont==0){CadenaCompletaN1 +=  "|";}
      if (cont==1){CadenaCompletaN1 +=   "/";}
      if (cont==2){CadenaCompletaN1 +=   "!";}
      cont=cont+1;
      if (cont==3){
        cont=0;
        //Serial.print("....................");Serial.println(CadenaCompletaN1);
        EnviarBluetoothFila(CadenaCompletaN1, contenvio);
        CadenaCompletaN1="";
        if (contenvio==0){contenvio=1;}
      }
    }
    BT.write("#");
  }
  

  // Recibido del nodo 2 --------------------------------------------------
  if (activ[0]==20){
    CadenaCompletaN2="";  //Vacia la cadena a Enviar
    for (int t=0; t<LongitudCadenaN2; t=t+3){ //Convierte los numero a los parametros medidos
        RecibidaN2[t]=NombresNodo2[t/3];
    }
    for (int t=1; t<LongitudCadenaN2; t=t+3){ //Convierte los numeros en Strings
      RecibidaN2[t]=String(Recibido[t]);
    }

    for (int t=2; t<LongitudCadenaN2; t=t+3){
      RecibidaN2[t]=String(Recibido[t]);
    }
  
    for (int t=0; t<LongitudCadenaN2; t++){ //Forma la cadena final
      CadenaCompletaN2 +=  RecibidaN2[t];
      if (cont==0){CadenaCompletaN2 +=  "|";}
      if (cont==1){CadenaCompletaN2 +=   "/";}
      if (cont==2){CadenaCompletaN2 +=   "!";}
      cont=cont+1;
      if (cont==3){
        cont=0;
        Serial.println(CadenaCompletaN2);
        EnviarBluetoothFila(CadenaCompletaN2, contenvio);
        CadenaCompletaN2="";
        if (contenvio==0){contenvio=1;}
      }
    }
    BT.write("#");
  }
  
  // Lectura del nodo 0 --------------------------------------------------
  if (activ[0]==1){        
    CadenaCompletaN0="";  //Vacia la cadena a Enviar
    for (int t=0; t<LongitudCadenaN0; t=t+3){ //Convierte los numero a los parametros medidos
      RecibidaN0[t]=NombresNodo0[t/3];
    }
    for (int t=1; t<LongitudCadenaN0; t=t+3){ //Convierte los numeros en Strings
      RecibidaN0[t]=String(Recibido[t]);
    }
    for (int t=2; t<LongitudCadenaN0; t=t+3){
      RecibidaN0[t]=String(Recibido[t]);
    }
    for (int t=0; t<LongitudCadenaN0; t++){ //Forma la cadena final
      CadenaCompletaN0 +=  RecibidaN0[t];
      if (cont==0){CadenaCompletaN0 +=  "|";}
      if (cont==1){CadenaCompletaN0 +=   "/";}
      if (cont==2){CadenaCompletaN0 +=   "!";}
      cont=cont+1;
      if (cont==3){
        cont=0;
        Serial.println(CadenaCompletaN0);
        EnviarBluetoothFila(CadenaCompletaN0, contenvio);
        CadenaCompletaN0="";
        if (contenvio==0){contenvio=1;}
      }
    }
    BT.write("#");
  }
}

void EnviarBluetoothFila(String Cad, int contenvio){
      //Serial.println(Cad);
      char charBuf[100];
      Cad.toCharArray(charBuf, 100);
      if (contenvio == 0){BT.write("0");}
      BT.write(charBuf);
}

void EnviarBluetooth(String Cad){
      char charBuf[200];
      Cad.toCharArray(charBuf, 200);
      BT.write("0");
      BT.write(charBuf);
      BT.write("#"); //Caracter final

      ciclo_primero=1;
}


//Tiene que haber una cadena ha recibir al principio para que el bluetooth no se cierre
//Cuando se recibe una "F", se deja de enviar esta cadena, se sale de este bucle, y se 
//pone enviar a 0, con esto se empieza a enviar la cadena
void ComienzoBluetooth(){
  NRed="%%" + Numero_Nodos + "/" + Nombre_Red + "%" + Nombre_Nodo_0 + "%" + Nombre_Nodo_1 + "%" + Nombre_Nodo_2 +"%";
  do{
    EnviarBluetooth(NRed);
    temp=BT.read();
    Serial.println(temp);
     if(temp.equals("70")){
      comenzar=1;
      enviar=1;
     }else{
      NodoSel=temp;
     }
     delay(500);
  }while(comenzar==0);
}

void DatosSimulados() {
    Recibido[0]=1; //Numero de sensor
    Recibido[1]=random(20,25);//Valor
    Recibido[2]=0; //Identificador unidades
    //Sensor 2
    Recibido[3]=2;
    Recibido[4]=random(2000,5000);
    Recibido[5]=5;
    //Sensor 3
    Recibido[6]=3;
    Recibido[7]=random(40,60); 
    Recibido[8]=6;
    //Sensor 4
    Recibido[9]=4; 
    Recibido[10]=random(2,10);
    Recibido[11]=13; 
    //Caracter final      
    Recibido[12]=9; 
}

