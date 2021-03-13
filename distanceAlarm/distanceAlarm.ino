#define TRIG_PIN 12
#define ECHO_PIN 11
#define PULSE_TIMEOUT 150000L
#define GREEN_PIN 10
#define RED_PIN 9
#define LOOP_FREQUENCY 100 //millisec
#define DISTANCE_THRESHOLD 10 //cm

void setup() {
  Serial.begin(9600);
  //SR04 init
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  //SEMAPHORE LED init
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(RED_PIN,OUTPUT);
}

void loop() {
  long dist=distance();
  Serial.print(dist);
  Serial.println("cm");
  if(dist<DISTANCE_THRESHOLD){
    digitalWrite(GREEN_PIN,LOW);
    digitalWrite(RED_PIN,HIGH);    
   }else{
    digitalWrite(GREEN_PIN,HIGH);
    digitalWrite(RED_PIN,LOW);
   }
  delay(LOOP_FREQUENCY);
}

long distance() {
    long distance = 0;
    long duration = 0;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    duration = pulseIn(ECHO_PIN, HIGH, PULSE_TIMEOUT);
    distance = microsecondsToCentimeter(duration);
    delay(25);
    return distance;
}

long microsecondsToCentimeter(long duration) {
    return (duration * 100) / 5882;
}
