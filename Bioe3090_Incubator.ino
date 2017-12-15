/* Bioe3090_Incubator.ino - Example controls for Bioe 3090 Incubator Project
	Created by: Steve Lammers, 12/11/2017
	Released to the public domain.
*/

// PID library for controls
#include <PID_v1.h>
// DS18B20 temperature sensor libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Values
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 7
// Set heater pin to analog pin 3
#define HEATER_PIN 6


// Define global variables
// heaterSetpoint is set in check_serial function
// heaterInput is set by print_temperature return in main loop
// heaterOutput is used in main loop to effect heater contol
double heaterSetpoint, heaterInput, heaterOutput;


String inString = "";
// Create a heaterPID object of type PID - class loaded by thge PID_v1 library
//
// variables passed by reference as outlined in the class.
PID heaterPID(&heaterInput, &heaterOutput, &heaterSetpoint,20,5,1, DIRECT);

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;


class TimeCheck{
	unsigned long timeNow, timePrevious, timeTrigger;

	public:
	TimeCheck(unsigned long _timeTrigger){
		timePrevious = millis();
		timeNow = millis();
		timeTrigger = _timeTrigger;
	}
	
	set_trigger(unsigned long _timeTrigger){
		timeTrigger = _timeTrigger;
	} 

	bool check_trigger(){
		bool triggered = false;
		timeNow = millis();
		unsigned long deltaTime = timeNow - timePrevious;
		if (deltaTime > timeTrigger ){
			timePrevious = timeNow;
			triggered = true;
		}
		else{ triggered = false; }
		
		return triggered;
	}
}; // class TimeCheck

// Create a clock to print the current temperature every second
TimeCheck printTempClock(1000);




void check_serial(){
  // Ask user to input the setpoint for the heater.
  //Serial.print("Enter temperature setpoint in degrees Celsius: ");
  // send data only when you receive data:
  while (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();
    // if the data is a digit, add it to inString
    if (isDigit(incomingByte)){
      inString += (char)incomingByte;
    }
    else{
      // ignore if the incomming byte is a carrige return or newline char
      if (incomingByte == '\n' || incomingByte == '\r'){
          //Serial.println("newline or CR");
        }
      else{
        Serial.println("Incompatible character, use only numbers");
      }
    }

    // Check if the incoming byte is a newline character
    if (incomingByte == '\n'){
      // say what you got:
      Serial.print("I received: ");
      // Set temperature setpoint to the inString number
      heaterSetpoint = inString.toFloat();
      Serial.println(heaterSetpoint);
      // Clear the input string 
      inString = "";
    }

  } 
} // << check_serial


void	assign_sensors(){
  // Assign address manually. The addresses below will beed to be changed
  // to valid device addresses on your bus. Device address can be retrieved
  // by using either oneWire.search(deviceAddress) or individually via
  // sensors.getAddress(deviceAddress, index)
  // Note that you will need to use your specific address here
  //insideThermometer = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };

  // Method 1:
  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then 
  // use those addresses and manually assign them (see above) once you know 
  // the devices on your bus (and assuming they don't change).
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 

  // method 2: search()
  // search() looks for the next device. Returns 1 if a new address has been
  // returned. A zero might mean that the bus is shorted, there are no devices, 
  // or you have already retrieved all of them. It might be a good idea to 
  // check the CRC to make sure you didn't get garbage. The order is 
  // deterministic. You will always get the same devices in the same order
  //
  // Must be called before search()
  //oneWire.reset_search();
  // assigns the first address found to insideThermometer
  //if (!oneWire.search(insideThermometer)) Serial.println("Unable to find address for insideThermometer");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  print_address(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
} // << assign_sensors


// function to print the temperature for a device
double print_temperature(DeviceAddress deviceAddress)
{
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  double tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
	
	return tempC;
}

// function to print a device address
void print_address(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


void setup(){
  // Start serial comms
	Serial.begin(9600);

	while (!Serial) {
		; // Wait for serial to connect
	}
	
  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  assign_sensors();	


	Serial.println("\n\n Incubator Control");
	Serial.println("Type in heater setpoint in degrees Celsius, numbers only");

	heaterPID.SetMode(AUTOMATIC);
}


void loop(){
  // Check the serial comms to see if the heater setpoint changed
  check_serial();
	if (printTempClock.check_trigger()){
		// Get new temperature reading, compute PID and control heater 
		heaterInput = print_temperature(insideThermometer); 
	}
	heaterPID.Compute(); 
	analogWrite(HEATER_PIN, heaterOutput);
  delay(1000);
}

