//what the project does:

//At certain temp the button will work.
//When on, a light will be green
//and the fan will go through a burst of movement.
//When off, no fan movement and the light will be red.
//When the temperture reaches the right level a light will be yellow.
//Display will show current temperature (in °F).

#include <Arduino.h>
#include <TM1637Display.h>

const int pinRed = 6;
const int pinBlue = 5;
const int pinGreen = 3;
const int ledYellow = 13;
const int buttonPin = 8;
const int motorPin = 9;
const int tmpPin = A0;
const byte CLK_PIN = 11;
const byte DIO_PIN = 10;

TM1637Display display = TM1637Display(CLK_PIN, DIO_PIN);

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(pinRed, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  display.setBrightness(7); //sets how bright the numbers on the display will be
  Serial.begin(9600);
}

void loop()
{ 
  display.clear();
  int reading = analogRead(tmpPin);
  int tmpPinValue = analogRead(tmpPin);
  
  //get the voltage
  float voltage = tmpPinValue * (5.0 / 1023.0);
  
  //convert to °Celsius
  float temperatureC = (voltage - 0.5) * 100.0;
  
  //convert to °Fahrenheit
  float temperatureF = (temperatureC * 9.0/5.0) + 32;
  
  int temp = (int)temperatureF; //when saying 'temp', it means 'temperatureF'
  Serial.print("Temperature (F): "); //specify that the temperature is in Fahrenheit before the number is printed in the serial monitor
  Serial.println(temperatureF); //print the temerature in the serial monitor
  display.showNumberDec(temp); //display the temperature on the display
  delay(1000); //pause for 1 second
  display.clear(); //clear the display
  
  if(temperatureF >= 70.0){//set the temperature for the yellow LED to turn on, and the button to work
    digitalWrite(ledYellow, HIGH);//turns yellow LED on
    if(digitalRead(buttonPin) == HIGH){ //if button is pressed
      for (int speed = 0; speed <= 60; speed++) {
        //starts up the motor

        //RED
        analogWrite(pinRed, 0);
        //BLUE (but actually green, even though everything seems to be pulgged in correctly)
        analogWrite(pinBlue, 255);
        //GREEN (but actually blue, even though everything seems to be pulgged in correctly)
        analogWrite(pinGreen, 0);//

        analogWrite(motorPin, speed);
        delay(50);//wait for 50 milliseconds
      } 
      for (int speed = 60; speed >= 0; speed--) {
        //slows down the motor

        //RED
        analogWrite(pinRed, 255);//make the LED red
        //BLUE
        analogWrite(pinBlue, 0);
        //GREEN
        analogWrite(pinGreen, 0);

        analogWrite(motorPin, speed);
        delay(50);//wait for 50 milliseconds
      }
    }
  } else { //otherwise
     digitalWrite(ledYellow, LOW);//has the yellow LED be off
    
     analogWrite(motorPin, 0);//turn off the motor
    
     //RED
     analogWrite(pinRed, 255);//makes the LED red
     //BLUE
     analogWrite(pinBlue, 0);
     //GREEN
     analogWrite(pinGreen, 0);
   }
}
