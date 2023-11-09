/*
This is the code from this tutorial: https://docs.arduino.cc/tutorials/nano-33-ble-sense-rev2/get-started-with-machine-learning
I've commented the shit out of it ok, so I can understand what's going on. 
I am going to use Coolterm https://freeware.the-meiers.org/ to copy paste the values on the serial monitor because the Arduino IDE won't let you copy paste more than 10 lines 
for some reason. 
*/

/*
  IMU Capture
  This example uses the on-board IMU to start reading acceleration and gyroscope
  data from on-board IMU and prints it to the Serial Monitor for one second
  when the significant motion is detected.
  You can also use the Serial Plotter to graph the data.
  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.
  Created by Don Coleman, Sandeep Mistry
  Modified by Dominic Pajak, Sandeep Mistry
  This example code is in the public domain.
*/

//#include <Arduino_LSM9DS1.h> //COMMENT OUT - this is the library for IMU on the old version
#include "Arduino_BMI270_BMM150.h" // libreary for BMI270 - accelerometer and gyroscope, BMMi50 - geomagnetic sensor (compass)

const float accelerationThreshold = 2.5; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;


/////////////////////////////////////////////////////////////////////////////////////
//////////////////SET UP ONLY HAPPENS ONCE AT THE START OF THE PROGRAM //////////////
/////////////////////////////////////////////////////////////////////////////////////
	
	
void setup() {   //setup only happens once at the start of the program
  Serial.begin(9600);  //begin serial communications at this baud rate
  
  while (!Serial);    //if you aren't getting any serial data? 
  if (!IMU.begin()) { //and if IMU doesn't begin
    Serial.println("Failed to initialize IMU!"); //then it must be this
    while (1); //this check is repeated as long as it is not intialized
  }

  // print the header, in serial monitor this will show the beginning of the capture session
  Serial.println("aX,aY,aZ,gX,gY,gZ"); 
}

/////////////////////////////////////////////////////////////////////////////////////
///////////////////LOOP, HAPPNENS REPEATEDLY UNTIL PROGRAM ENDS//////////////////////
/////////////////////////////////////////////////////////////////////////////////////
	
void loop() {  
  float aX, aY, aZ, gX, gY, gZ;  //in the loop this is where the data from the sensors will be put  

//--------------------------THIS IS HOW THE CAPTURE SESSION GETS STARTED -------------------------------
  // wait for significant motion,
  while (samplesRead == numSamples) { 
    if (IMU.accelerationAvailable()) {
      // read the acceleration data
      IMU.readAcceleration(aX, aY, aZ);

      // sum up the absolutes of the accelerometer reading
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

      // check if it's above the threshold, that means there is enough motion to start the capturing session
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0; //resets the count so that it can start a capture session
        break; //exits the "while" loop immediately so it starts capturing
      }
    }
  }
  
//---------------------------SENSORS ARE READ HERE AND PRINTED----------------------------------

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) { //while there are still samples to be read before it hits its limit of number of samples
    // check if both new acceleration and gyroscope data is available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++; // each time this happens, it increasing samples read by one

      // print the data in CSV format in serial monitor
      Serial.print(aX, 3);  //the "3" here means print up to 3 digits past decimal point
      Serial.print(',');  	//then print comma
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();		//ends sample in capture

      if (samplesRead == numSamples) {  //when it's at the limit of number of samples
        // add an empty line if it's the last sample, this signifies the end of the capture, 
        //this whole session then gets copy and pasted into a text file. 
        Serial.println();
      }
    }
  }
}