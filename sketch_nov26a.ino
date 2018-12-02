#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
  // these constants won't change:
  // const int ledPin = 13;      // LED connected to digital pin 13
  const int knockSensor = A0; // the piezzo is connected to analog pin 0
  const int threshold = 450;  // threshold value to decide when the detected sound is a knock or not

  const int ledPin = 13;

  // TO BE CHANGED
  const int deltaThreshold = 6;
  const int dissipatedEnergy = -700;


  // these variables will change:
  int sensorReading = 0;      // variable to store the value read from the sensor pin
  // int ledState = LOW;         // variable used to store the last LED status, to toggle the light
  bool isKnocked = false;
  bool beginLooping = false;
  int loopCount = 0;
  int numSteps = 0; 
  int energy = 0;
  

  int previousVolt;
  int currVolt;

  void setup() {
    lcd.begin(16, 2);
    lcd.print("Energy");
    lcd.setCursor( 8, 0 );
    lcd.print( energy );
    lcd.setCursor( 0, 1 );
    lcd.print("Steps");
    lcd.setCursor( 8, 1 );
    lcd.print( numSteps );
    
    previousVolt = analogRead(knockSensor);
    // pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
    Serial.begin(9600);       // use the serial port
  }

  void loop() {
    //
    int deltaV;

    // read the sensor and store it in the variable sensorReading:
    sensorReading = analogRead(knockSensor);
    
    currVolt = sensorReading;
    deltaV = currVolt - previousVolt;
    //Serial.println(deltaV);

    if (deltaV <= dissipatedEnergy){
      digitalWrite(ledPin, HIGH);
      beginLooping = true;
    }

    if (beginLooping) {
      loopCount++;
    }

    if (loopCount >= 5) {
      digitalWrite(ledPin, LOW);
      energy = 0;
      lcd.setCursor(8, 0);
      lcd.print(energy);
      lcd.setCursor(9, 0);
      lcd.print("  ");
      beginLooping = false;
      loopCount = 0;
    }

    // if the sensor reading is greater than the threshold:
    Serial.println( sensorReading ); 

    if (sensorReading >= threshold && !isKnocked) {
    // toggle the status of the ledPin:
    // ledState = !ledState;
    //numSteps = numSteps + 1; 
    // update the LED pin itself:
    // digitalWrite(ledPin, ledState);
    // send the string "Knock!" back to the computer, followed by newline
    Serial.println("Knock!");
    //lcd.setCursor(0, 1);
    //lcd.print( sensorReading );
    isKnocked = true;
    } else if (isKnocked && sensorReading < threshold) {
      isKnocked = false;
    }

    // Checks if stored energy has been released
    if( deltaV > deltaThreshold ) {
      Serial.println("over threshold");
      numSteps = numSteps + 1;
      Serial.println(numSteps);
      int deltaJ = (deltaV * deltaV) * 0.5 * (5.0/1023);
      if (deltaJ >= 0) {
        energy += deltaJ;
      }
      //energy += ( (deltaV * deltaV) * 0.5 * ( 0.000001 ) ) * (5.0/1023) * 1000000;
      Serial.print(energy );
      Serial.println(" mJ");
      lcd.setCursor( 8, 0 );
      lcd.print( energy );
      lcd.setCursor( 11, 0);
      lcd.print("mJ");
      lcd.setCursor( 8, 1 );
      lcd.print( numSteps );
     }
    
    previousVolt = sensorReading;
    
    delay(700);  // delay to avoid overloading the serial port buffer
  }
