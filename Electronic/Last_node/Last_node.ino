/////////////////////////NODO 2///////////////////////////////
#include <SPI.h> //libreria para el uso del modulo SPI
#include "nRF24L01.h" //Libreria del modulos nRF24L01+
#include "RF24.h"

int sendd=0;

RF24 radio(48, 49); //Declaracion de los pines de control CE y CSN para el modulo, se define el objeto "radio"

//Comunicacion con el nodo 1
const uint64_t direccion = 0xB3B4B5B6F1LL; // En esta parte LL significa LongLong para asegurar el dato de 64bits
int activ[1]; //Valor de activacion recibido del nodo 1
int Enviar[15]; //Array para almacenar los valores de los sensores a mandar al nodo 1 


int i=0;

String Sensores[]={"T","D","P","I","V","Q"};


void setup() {
  Serial.begin(9600);
  Serial.println("Serial begin");
  
  radio.begin(); //Inicio del modulo nRF24L01+
  radio.setPALevel(RF24_PA_LOW);  // Configuracion en modo de baja potencia
  radio.setChannel(100); // Apertura del canal de comunicacion
  radio.openReadingPipe(0, direccion); 
  radio.startListening(); // iniciamos en modo de escucha

}

void loop() {
  radio.startListening();
  
  if ( radio.available() && sendd==0)//Si hay un emisor y no se ha recibido todavia empieza a recibir
  {
    bool done = false;
    while (!done) //Escucha mensajes hasta que se reciba algo 
    {
     radio.read(activ, sizeof(activ)); //Recibe el mensaje y lo guarda en "activ"
     Serial.println(activ[0]);
     if (activ[0] != 0) done=true; //Confirma que se ha recibido algo
     sendd=1;
    }
  }
  else
  {    
      Serial.println("No radio available");
  }
  if (activ[0] == 20 && sendd==1) //Apagar alarma, pero siempre esta armada
        {
        radio.stopListening(); //Paramos el modo de escucha para poder escribir
        radio.openWritingPipe(direccion); //Se abre el puerto de escritura
        //Sensor 1
        Enviar[0]=1; //Numero de sensor
        Enviar[1]=random(200,250);
        Enviar[2]=7; //Identificador unidades
        //Sensor 2
        Enviar[3]=2;
        Enviar[4]=random(5000,6000);
        Enviar[5]=10;
        //Sensor 3
        Enviar[6]=3;
        Enviar[7]=random(20,25); 
        Enviar[8]=0;
        //Sensor 4
        Enviar[9]=3;
        Enviar[10]=random(400,600);
        Enviar[11]=2;
        //Caracter final      
        Enviar[12]=9;  
              
        radio.write(Enviar, sizeof(Enviar)); //Manda el valor alamacenado en la variable Enviar
        sendd=0; //Se pone "sendd" a cero para volver a escuchar   
        i=i+1;
        Serial.println("0000000000000000000000000000");
        }
}
