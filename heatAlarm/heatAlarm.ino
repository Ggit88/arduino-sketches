#define TEMP_PIN 0
#define LOOP_FREQUENCY 1000 //millisec
#define SH_REG_LATCH_PIN 11   // (11) ST_CP [RCK] on 74HC595
#define SH_REG_CLOCK_PIN 9     // (9) SH_CP [SCK] on 74HC595
#define SH_REG_DATA_PIN 12    // (12) DS [S1] on 74HC595

byte leds = 0;

void updateShiftRegister(){
   digitalWrite(SH_REG_LATCH_PIN, LOW);
   shiftOut(SH_REG_DATA_PIN, SH_REG_CLOCK_PIN, MSBFIRST, leds);
   digitalWrite(SH_REG_LATCH_PIN, HIGH);
}

float readTempCelsius(){
  int tempReading = analogRead(TEMP_PIN);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celsius
  return tempC;
}

int calcNumsLed(float tempC){
  int numLEDSLit = 8 - (35-tempC);
  if(numLEDSLit<1)numLEDSLit = 1;
  if(numLEDSLit>8)numLEDSLit = 8;
  return numLEDSLit;   
}

void setLeds(int numLEDSLit){
  leds = 0;   // no LEDs lit to start
  for (int i = 0; i < numLEDSLit; i++) {
    leds = leds + (1 << i);  // sets the i'th bit
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(SH_REG_LATCH_PIN, OUTPUT);
  pinMode(SH_REG_CLOCK_PIN, OUTPUT);  
  pinMode(SH_REG_DATA_PIN, OUTPUT);
}

void loop(){
  float tempC = readTempCelsius();
  int numLEDSLit = calcNumsLed(tempC);
  Serial.print(tempC);
  Serial.print(" Celsius - ");
  Serial.print(numLEDSLit);
  Serial.println("/8 Active Leds");
  setLeds(numLEDSLit);
  updateShiftRegister();
  delay(LOOP_FREQUENCY);
}
