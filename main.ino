#include "Arduino.h"
#include "heltec.h"
#include "images.h"

const int numReadings = 100;


/* Termistor settings */
const int rezistor = 22000;
const int termNom = 22000;
const int refTemp = 25;
const int beta = 3950;


float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

float tempDiffArr[60];
int tempDiffIndex = 0;

const int inputPin = 34; // THERMISTOR PIN
const int buttonPin = 0;
const int ledPin = 25;

unsigned long interval = 1000;

void setup() {
	pinMode(ledPin, OUTPUT);
	pinMode(buttonPin, INPUT);

	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
	Heltec.display->flipScreenVertically();
	Heltec.display->clear();
	Heltec.display->setFont(ArialMT_Plain_10);

	Heltec.display->drawXbm(0, 0, nerudny_logo_width, nerudny_logo_height, nerudny_logo_bits);
	delay(2000);

	Serial.begin(11520);
	for (int thisReading = 0; thisReading < numReadings; thisReading++) {
	readings[thisReading] = 0; // set the array to 0 on all positions
  	}
}

void loop() {
	unsigned long currMillis = Millis();

	// subtract the last reading:
	total = total - readings[readIndex];
	// read from the sensor:
	readings[readIndex] = analogRead(inputPin);
	// add the reading to the total:
	total = total + readings[readIndex];
	// advance to the next position in the array:
	readIndex = readIndex + 1;

	// if we're at the end of the array...
	if (readIndex >= numReadings) {
	// ...wrap around to the beginning:
	readIndex = 0;
	}

	// calculate the average:
	average = total / numReadings; // THIS!!!
	// send it to the computer as ASCII digits
	
	average = 4095 - average - 1;
	average = rezistor / average;

	float temp;
	temp = average / termNom;
	temp = log(temp);
	temp /= beta;
	temp += 1.0 / (refTemp+273.15);
	temp = 1.0 / temp;
	temp -= 273.15; // NOW there is the FINAL TEMP

	Serial.println(temp);
	
	if(digitalRead(buttonPin)==HIGH){
		Heltec.display->drawString(0, 0, "Teplota: ");
		Heltec.display->drawString(0, 10, String(temp));
	}
	else{Heltec.display->clear();}


	delay(1);        // delay in between reads for stability
}

double ReadVoltage(byte pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if(reading < 1 || reading >= 4095)
    //return 0;
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required