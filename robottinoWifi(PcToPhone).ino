#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 8

#define In1 2 //Pin D2 dell'Arduino e' collegato al Input 1 del ponte H
#define In2 4 //Pin D4 dell'Arduino e' collegato al Input 2 del ponte H
#define In3 6 //Pin D6 dell'Arduino e' collegato al Input 3 del ponte H
#define In4 7 //Pin D7 dell'Arduino e' collegato al Input 4 del ponte H
#define EnM1 5 //M1 motore di sinistra
#define EnM2 9 //M2 motore destro


//Sensore Ultrasuoni( dichiarazione variabli)

int trigPin = 11;
int echoPin = 12;
long duration;
float distance;


// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

String check4answer(){
    String str = "";
    //while (mySerial.available() == 0) {}
    while (mySerial.available() > 0) {
      char c = mySerial.read();
      str += String(c);
    }
    Serial.println(str);
    return str;
}


String esp01cmd(String cmd) {
  Serial.println("sending: " + cmd);
  mySerial.println(cmd);
  delay(10);
  return check4answer();
}

void setup()  {
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    Serial.begin(9600);

// nel caso in cui ESP01 sia settato alla velocità di trasmissione 11500
// che per la softserial risulta troppo veloce, possiamo andare a settare
// la velocità a 9600. basterà eseguire il seguente codice una volta solamente.
//    mySerial.begin(115200);
//    delay(100);
//    esp01cmd("AT+UART=9600,8,1,0,0");
//    delay(1000);
//    mySerial.end();
//    delay(1000);


    mySerial.begin(9600);
    delay(1000);
    esp01cmd("AT");
    delay(1000);
    esp01cmd("AT+CWMODE=2");
    delay(1000);
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4");
    delay(1000);
    esp01cmd("AT+CIFSR"); //show AP IP address
    esp01cmd("AT+CIPMUX=1"); //allow up to 1 connections at the time
    
    
    Serial.println("ESP-01 Configuration Completed");

     //Motori
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(EnM1, HIGH);
  digitalWrite(EnM2, HIGH);
  
  //Sensore Ultrasuoni
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Output Sensore si accede alla funzione serial 
  //per il funzionamento dell'output del monitor seriale
  Serial.begin(9600);

}

void loop() {
    Serial.println("loop...");
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") {
      Serial.println("no connections so far... still waiting");
      delay(1000);
    }

    String str = esp01cmd("AT+CWLIF");
    int startOfSTR = str.indexOf(',',18);  //IP finsce prima della virgola
    String cellphoneIP = str.substring(11,startOfSTR);
    Serial.println(cellphoneIP);
    Serial.println("Connection from remote device was Established!!!");

    // AT+CIPSTART=<id>,<type>,<remote address>,<remote port>[,(<local port>),(<mode>)]
    // AT+CIPSEND=[<id>,]<length>[,<ip>,<port>]
    // AT+CIPCLOSE=<id>
    
    //Socket Server: server in ascolto, pronto a ricevere pacchetti UDP da WIFI
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPCLOSE=3"); //close socket if for any reason it was already open
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server 

    
    //esp01cmd("AT+CIPSTART=1,\"UDP\",\""+cellphoneIP+"\",1234"); //starting UDP Socket Client 
    
    delay(3000);

    while(true) {

        String res;
      // dati ricevuti da Modulo WIFI
      str = mySerial.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str);
        Serial.println("Message: "+str.substring(startOfSTR));
        
       String time=str.substring(1);

        if(str.substring(startOfSTR).indexOf(0)=='w'||str.substring(startOfSTR).indexOf(0)=='W'){

         res="Il robot andrà avanti per " + time + " millisecondi";

           /*digitalWrite(In1, LOW);
            digitalWrite(In2, HIGH);
            digitalWrite(In3, LOW);
            digitalWrite(In4, HIGH);

            delay(time);

            digitalWrite(In1, LOW);
            digitalWrite(In2, LOW);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);*/
        }
        //gira a destra
        if(str.substring(startOfSTR).indexOf(0)=='d'||str.substring(startOfSTR).indexOf(0)=='D'){

            /*digitalWrite(In1, HIGH);
            digitalWrite(In2, HIGH);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);

            delay(time);

            digitalWrite(In1, LOW);
            digitalWrite(In2, LOW);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);*/
        }
        if(str.substring(startOfSTR).indexOf(0)=='a'||str.substring(startOfSTR).indexOf(0)=='A'){

           
            /*digitalWrite(In1, HIGH);
            digitalWrite(In2, HIGH);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);

            delay(time);

            digitalWrite(In1, LOW);
            digitalWrite(In2, LOW);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);*/
        }
        if(str.substring(startOfSTR).indexOf(0)=='s'||str.substring(startOfSTR).indexOf(0)=='S'){

            /*digitalWrite(In1, HIGH);
            digitalWrite(In2, HIGH);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);

            delay(time);

            digitalWrite(In1, LOW);
            digitalWrite(In2, LOW);
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);*/
        }
      }

      // dati ricevuti da Monitor Seriale
      str = Serial.readString(); 
      if(str != "") {
        Serial.println("Received from Serial Monitor: "+str);
        //String str1 = "AT+CIPSEND=1," + str.length(); NOT WORKING??? bug???
        String str1 = "AT+CIPSEND=3,";
        str1 = str1 + str.length() + ",\"" + cellphoneIP + "\",1234";
        //str1.concat(str.length());
        //Serial.println(str1);
        esp01cmd(str1);
        esp01cmd(res); 
            
      }

    }
}
