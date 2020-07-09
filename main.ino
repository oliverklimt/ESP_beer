const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;



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
	average = total / numReadings;
	// send it to the computer as ASCII digits
	
	delay(1);        // delay in between reads for stability
}
