#include <SoftwareSerial.h>
//libreria che mi fa accedere alla classe SoftwareSerial
//attributi: pinTx e pin Rx
//metodi: begin(), available(), read(), print(), println()
//begin() usato per configurare la velocità della porta seriale
//print() e println() usati per inviare i dati da Arduino a modulo WIFI
//available() usato per verificare presenza dati trasmessi da moduloWIFI per Arduino
//read() usato per ricevere i dati provenienti da moduloWIFI

#define rxPin 10
#define txPin 8
// Set up a new SoftwareSerial object
SoftwareSerial serialModuloWIFI =  SoftwareSerial(rxPin, txPin);

