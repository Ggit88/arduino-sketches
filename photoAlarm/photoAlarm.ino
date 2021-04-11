#define PHOTO_PIN 0
#define LOOP_FREQUENCY 1000 //millisec
#define LIGHT_THRESHOLD 250
#define BUZZER_PIN 8

void activateBuzzer(){
  unsigned char i;
  for(i=0;i<80;i++){
    digitalWrite(BUZZER_PIN,HIGH);
    delay(1);//wait for 1ms
    digitalWrite(BUZZER_PIN,LOW);
    delay(1);//wait for 1ms
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN,OUTPUT);//initialize the buzzer
}

void loop() {
  int photoReading = analogRead(PHOTO_PIN);
  Serial.print("Photo value - ");
  Serial.println(photoReading);
  if(LIGHT_THRESHOLD < photoReading){
    Serial.println("Over threshold!");
    activateBuzzer();
  }
  delay(LOOP_FREQUENCY); 
}
