const int numReadings = 60;

const int rezistor = 22000;
const int termNom = 22000;
const int refTemp = 25;
const int beta = 3950;


float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

const int inputPin = A0;



void setup() {
	for (int thisReading = 0; thisReading < numReadings; thisReading++) {
	readings[thisReading] = 0; // set the array to 0 on all positions
  	}
}

void loop() {
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




	delay(1);        // delay in between reads for stability
}
