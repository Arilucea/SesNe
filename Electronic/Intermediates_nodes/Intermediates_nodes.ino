/////////////////////////NODO 1///////////////////////////////
#include <SPI.h> //libreria para el uso del modulo SPI
#include "nRF24L01.h" //Libreria del modulos nRF24L01+
#include "RF24.h"

#include <NewPing.h>

#define  TRIGGER_PIN  6
#define  ECHO_PIN     7
#define MAX_DISTANCE 200 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

//Unidades
//1-Temperatura
//2-Presion
//3-Caudal

int sendd=0;
int tmp = 0;

RF24 radio(9, 10); //Declaracion de los pines de control CE y CSN para el modulo, se define el objeto "radio"

//Comunicacion con el nodo 0
const uint64_t direccion = 0x7878787878LL; // Direccion comunicacion con el nodo 0
int EnviarN1[9]; //Array para almacenar los valores de los sensores del nodo 1 a mandar al nodo 0 
int activRec[1]; //Valor de activacion recibido del nodo 0

//Comunicacion con el nodo 2
const uint64_t direccion2 = 0xB3B4B5B6F1LL; //Direccion comunicacion con el nodo 2
int EnviarN2[15]; //Array para almacenar los valores de los sensores del nodo 2 a mandar al nodo 0 
int activEnv[1]; //Valor de activacion enviado al nodo 2
int RecibidoN2[15]; //Array enteros recibida del Nodo 2
int LongitudCadenaN2=15; //Numero de sensores en el nodo 2 *2

String Recibida[15]; //Array enteros convertida en Strins
String CadenaCompleta; // Array Strings convertida en una Cadena



String Sensores[]={"T","D","P","I","V","Q"};
String NombresNodo2[]={"Longitud","Humedad","Caudal","Velocidad","V","Q"};

void setup() {
  Serial.begin(9600);
  Serial.println("Serial begin");
  
  radio.begin(); //Inicio del modulo nRF24L01+
  radio.setPALevel(RF24_PA_LOW);  // Configuracion en modo de baja potencia
  radio.setChannel(100); // Apertura del canal de comunicacion
  radio.openReadingPipe(0, direccion); 
  radio.openReadingPipe(1, direccion2);
  radio.startListening(); // iniciamos en modo de escucha
}

void loop() {
  radio.startListening(); //Comienza a escuchar
  
  if ( radio.available() && sendd==0) //Si hay un emisor y no se ha recibido todavia empieza a recibir
  {
    bool Recibido = false;
    while (!Recibido) //Escucha mensajes hasta que se reciba algo 
    {
     radio.read(activRec,sizeof(activRec)); //Recibe el mensaje y lo guarda en "activRec"
     Serial.println("--------------------");
     Serial.println(activRec[0]);
     if (activRec[0] != 0) Recibido=true;  //Confirma que se ha recibido algo
     sendd=1;
    }
  }
  else
  {    
      Serial.println("No radio available"); 
  }
  if (activRec[0] == 10 && sendd==1) //Si se recibe el valor de activacion del nodo 1, lee los sensores y responde.
        {
        radio.stopListening(); //Deja de escuchar para poder escribir
        radio.openWritingPipe(direccion); //Crea una conexion con+ la direccion en modo escritura 
        //Sensor 1
        EnviarN1[0]=1; //Numero de sensor
        EnviarN1[1]=random(70,99);//Valor
        EnviarN1[2]=6; //Identificador unidades
        //Sensor 2
        EnviarN1[3]=2;
        EnviarN1[4]=tmp;
        EnviarN1[5]=0;
        //Sensor 3
        EnviarN1[6]=3;
        EnviarN1[7]=sonar.ping_cm(); 
        EnviarN1[8]=2;
        //Caracter final      
        EnviarN1[9]=9;  
        tmp = tmp +1;
        radio.write(EnviarN1, sizeof(EnviarN1));    //Manda el valor alamacenado en la variable Enviar
        sendd=0; //Se pone "sendd" a cero para volver a escuchar
        Serial.println(EnviarN1[16]);
        Serial.println(EnviarN1[17]);
        Serial.println(EnviarN1[7]);
        }
   if (activRec[0] == 20 && sendd==1) //Si se recibe el valor de activacion del nodo 2, se comunica con el nodo 2
        {    
           Enviar2();  
           sendd=0;
        }
}

/////////////////////////////////////////////////////
void Enviar2(){
  Serial.println("Enviar");
  //activEnv[0]=20; //Valor de activacion del nodo 2
  activEnv[0]=activRec[0];
  bool ok = 0;
  radio.stopListening(); // Paramos la escucha para poder escribir
  radio.openWritingPipe(direccion2); //Se abre el puerto de escritura para la direccion del modulo 1
  ok = radio.write(activEnv, sizeof(activEnv));   //Envia el valor de activacion al nodo 2 y comprueba is se hace correctamente

  if (ok)
      {
        Serial.println("eviado...");
        radio.startListening(); //Se inicia el modo de escucha
        Recibir(); //Ejecuta la funcion recibir
        radio.stopListening(); //Deja de escuchar para poder escribir
        radio.openWritingPipe(direccion); //Crea una conexion con la direccion en modo escritura 
        
        for (int i=0; i<LongitudCadenaN2; i++){
            EnviarN2[i]=RecibidoN2[i];      //Los valores recibidos del nodo 2 se pasan al array "EnviarN2" para ser enviado al nodo 0
        }     
        
        radio.write(EnviarN2, sizeof(EnviarN2));  //Se envian los valores del nodo 2 al nodo 0
        
      }else
      {
        Serial.println("fallo en envio!");
      }
  }
////////////////////////////////////////////////////////////////
void Recibir(){
Serial.println("Recibiendo");
delay(50); //Espera para que comience el envio en el otro nodo
  //Lee el dato recibido
  if (radio.available()) //Si hay un emisor
  {
    bool done = false;
    while(!done) //Recibe todos los valores
    {
    radio.read(RecibidoN2, sizeof(RecibidoN2)); //Recibe los valores del nodo 2
    Convertir();    //Ejecuta la funcion "Convertir"
    
    if (RecibidoN2[6] != 0) done=true; //Se comprueba que se han recibido valores
    }
  }else
  {    
      Serial.println("No radio available");
  }
}


////////////////////////////////////////////////////////////
void Convertir(){
  CadenaCompleta="";
  int cont=0;
  for (int t=1; t<LongitudCadenaN2; t=t+3){
    Recibida[t]=String(RecibidoN2[t]);
  }
  for (int t=0; t<LongitudCadenaN2; t=t+3){
    Recibida[t]=NombresNodo2[t/3];
  }
  for (int t=2; t<LongitudCadenaN2; t=t+3){
    Recibida[t]=String(RecibidoN2[t]);
  }

  for (int t=0; t<LongitudCadenaN2; t++){
    CadenaCompleta = CadenaCompleta + Recibida[t];
    if (cont==0){CadenaCompleta = CadenaCompleta + "|";}
    if (cont==1){CadenaCompleta = CadenaCompleta + "/";}
    if (cont==2){CadenaCompleta = CadenaCompleta + "!";}

    cont=cont+1;
    if (cont==3){cont=0;}
  }
  Serial.println(CadenaCompleta);
}

