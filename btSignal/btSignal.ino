#include <SoftwareSerial.h>
#define Rxpin 12 // Tx per Bluetooth Rx per Arduino
#define Txpin 13 // Rx per Bluetooth Tx per Arduino
#define LEDpin 10 // Pin per accendere/spegnere il led
char  BluetoothBuffer;
char  SerialBuffer;
int ledStatus;
SoftwareSerial Bluetooth(Rxpin, Txpin);

void setup() {
  pinMode(LEDpin, OUTPUT);
  ledStatus = 0;
  Serial.begin(9600);
  Bluetooth.begin(9600);
}

void loop() {
  if (Bluetooth.available() > 0) {
    BluetoothBuffer = Bluetooth.read();
    Serial.print(BluetoothBuffer);
    if(ledStatus==0 && BluetoothBuffer=='1'){
      ledStatus=1;
      digitalWrite(LEDpin,HIGH);
      Bluetooth.print(ledStatus);  
    }
    if(ledStatus==1 && BluetoothBuffer=='0'){
      ledStatus=0;
      digitalWrite(LEDpin,LOW);
      Bluetooth.print(ledStatus);  
    }
  }
  if (Serial.available() > 0) {
    SerialBuffer = Serial.read();
    Bluetooth.print(SerialBuffer);
  }
}
