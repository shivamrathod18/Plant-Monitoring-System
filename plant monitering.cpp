#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <dht.h>        // Include library
#define outPin 12        // Defines pin number to which the sensor is connected
#define sensorPower 7
#define sensorPin 8
#define sensorPowerA 2
#define sensorPin1 A0
#define sensorPowerB 4
#define sensorPin2 A1
#define soilWet 300   // Define max value we consider soil 'wet'
#define soilDry 650   // Define min value we consider soil 'dry'
#define soilWet2 300   // Define max value we consider soil 'wet'
#define soilDry2 650   
// Define min value we consider soil 'dry'
dht DHT;                // Creates a DHT object
int servoPin = 9;
Servo servo;
int angle = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  servo.attach(servoPin);
    
  pinMode(sensorPower, OUTPUT);
  // Initially keep the sensor OFF    #rain
  digitalWrite(sensorPower, LOW);
 
  
  pinMode(sensorPowerA, OUTPUT);
  // Initially keep the sensor OFF    #soil 1
  digitalWrite(sensorPowerA, LOW);

  
  pinMode(sensorPowerB, OUTPUT);
  // Initially keep the sensor OFF    #soil 2
  digitalWrite(sensorPowerB, LOW);

  Serial.begin(9600);
}

void loop() {
  int readData = DHT.read11(outPin);
  
  int t = DHT.temperature;
  int h = DHT.humidity;
  int n=0,m=0;
  int sec;


  
  
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print((char)223); //shows degrees character
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("%");
  Serial.println(h);
    //get the reading from the function below and print it
  int val = readSensor();
  Serial.print("Digital Output: ");
  Serial.println(val);
  Serial.print("Analog output: ");
  Serial.println(readSensor1());

  lcd.setCursor(7,0);
  lcd.print("S1:");
  lcd.setCursor(7,1);
  lcd.print("S2:");

  
  
  // Determine status of rain

  Serial.println();
 
  
  //get the reading from the function below and print it
  int moisture = readSensor1();
  Serial.print("Analog Output: ");
  Serial.println(moisture);

  // Determine status of our soil
  if (moisture < soilWet) {
    Serial.println("Status: Soil is too wet");
    lcd.setCursor(10,0);
    lcd.print(" Wet");
    n =1;
  } else if (moisture >= soilWet && moisture < soilDry) {
    Serial.println("Status: Soil moisture is perfect");
    lcd.setCursor(10,0);
    lcd.print("Stable");
     n=1;
  } else {
    Serial.println("Status: Soil is too dry - time to water!");
    lcd.setCursor(10,0);
    lcd.print(" Dry");
     n =0;
  }
  Serial.println();


   //get the reading from the function below and print it
  int moisture2 = readSensor2();
  Serial.print("Analog Output: ");
  Serial.println(moisture2);

  // Determine status of our soil
  if (moisture2 < soilWet2) {
    Serial.println("Status: Soil is too wet");
    lcd.setCursor(10,1);
    lcd.print(" Wet");
    m =1;
  } else if (moisture2 >= soilWet2 && moisture2 < soilDry2) {
    Serial.println("Status: Soil moisture is perfect");
    lcd.setCursor(10,1);
    lcd.print("Stable");
    m =1;
  } else {
    Serial.println("Status: Soil is too dry - time to water!");
    lcd.setCursor(10,1);
    lcd.print(" Dry");
    m=0; 
  }
  delay(2000);
    
    
    
    if (n == 1 && m == 1){
      servo.write(90);
    }
    else if ( n == 0 && m == 1){
      servo.write(170);
    }
    else if ( n == 1 && m == 0){
      servo.write(5);
    }
    else if ( n ==0 && m == 0){
      servo.write(170);
      delay(2000);
      servo.write(5);
      delay(2000);
    }


  
    if (val) {
    Serial.println("Status: Clear");
    
  } else {
    Serial.println("Status: It's raining");
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Raining");
      delay(2000);
      lcd.clear();
    
  }
    Serial.println("\n ------------------------------------------------------------");
    sec = sec+1;
    if (sec == 30){
      lcd.clear();
      sec = 0;
    }
}









//  This function returns the sensor output
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = digitalRead(sensorPin); // Read the sensor output
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return the value
}


int readSensor1() {
  digitalWrite(sensorPowerA, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin1);  // Read the analog value form sensor
  digitalWrite(sensorPowerA, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}


int readSensor2() {
  digitalWrite(sensorPowerB, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin2);  // Read the analog value form sensor
  digitalWrite(sensorPowerB, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}
