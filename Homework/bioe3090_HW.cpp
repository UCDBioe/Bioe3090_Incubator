/* Bioe3090_HW.cpp
 *
 * Homework for Bioe3090 Junior Design Spring 2018
 *
 * Student Instructions - 
 *  Modify the C++ file here so that Temperature, Battery Voltage
 *  and Index are printed, formatted, to screen. 
 *
 *  Fix array_average, push_array, print_array functions
 *
 *  Fix battery_safe_chk and temperature_safe_chk
 *
 */

#include<iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// -----------------------------------------------------------------------------
// FUNCTIONS ----- FUNCTIONS ------  FUNCTIONS ------ FUNCTIONS ------ FUNCTIONS
// -----------------------------------------------------------------------------
//
void assignInitialBuffers(double _tempBuffer[], int bufferSize, double _tempVal)
{
  // Assign initial values to buffer
  for (int ind=0; ind < bufferSize; ind++)
  {
    _tempBuffer[ind] = _tempVal;
  }
}

void print_setpoints(double _tempAlarm, double _batteryAlarm)
{
  cout << "\n\n";
  cout << "-----------------------------------------------------\n";
  cout << "Temperature Setpoint: " << _tempAlarm    << "\n";
  cout << "Battery Low Setpoint: " << _batteryAlarm << "\n";
  cout << "-----------------------------------------------------\n";
}

void gen_array_data(double _tmpArr[], int _arrSize, double midpoint, double span)
{
// Generate randomized data for analysis
// _tmpArr[]: is the prallocated array. Since this is a pointer passed by
//    reference the values are changed in the main workspace and there is
//    no need to return the array.
// _arrSize: array size
// midpoint: mean value around which values are randomized
// span: midpoint +/- span = random number

  // seed the random number generator
  srand(time(NULL));

  for (int ind = 0; ind<_arrSize; ind++)
  {
    //float x = rand() % (int)(span*100.0);
    //x = x/(span*100.0) + (midpoint - span/2.0);
    float x = rand() % 100;
    x = (x/100.0)*span + (midpoint - span/2.0);
    //cout << "rand: " << x << "\n";
    _tmpArr[ind] = x;
  }
}

void merge_arrays(double _tmpArr1[], int _arrSize1, double _tmpArr2[], int _arrSize2, double _tmpArrMerge[])
// Concatenate two arrays (_tmpArr1 and _tmpArr2) into one large array
// _tmpArrMerge = [_tmpArr1, _tmpArr2]
{
  for (int ind=0; ind < _arrSize1; ind++)
  {
    _tmpArrMerge[ind] = _tmpArr1[ind];
  }

  for (int ind=0; ind < _arrSize2; ind++)
  {
    _tmpArrMerge[ind+_arrSize1] = _tmpArr2[ind];
  }
}

double array_average(double _tmpArr[], int _arrSize)
// Calculate the mean of the array _tmpArr
{
  // STUDENT COMPLETE THIS SECTION  >> -- HWS2----
  // The array_average function is defined here, you need to
  // complete the body of the function.
  // hint: maybe a sum and a division here...
  // STUDENT COMPLETE THIS SECTION  << -- HWS2----
}

void push_array(double _arr[], int _arrSize, double newVal)
// This function shifts the elements of a array (_arr[]) to the left
// one position and replaces the rightmost element with a new value
// (newVal)
//
// Incomming array _arr[] = [1,2,3,4]
// Returned array  _arr[] = [2,3,4,newVal]
//
// Shift array elements to the left one position, replace the last
// element with newVal. Arrays are passed by reference, so no need
// to return anything.
{
  // STUDENT COMPLETE THIS SECTION  >> -- HWS3----
  // You need to complete the body of the function.
  // hint: a couple of ways to do this, could use a loop here? 
  // STUDENT COMPLETE THIS SECTION  << -- HWS3----
}


void print_array(double _tmpArr[], double _battArr[], int _arrSize)
{
// Print contents of the array to the terminal
  int ind = 0;
  while (ind<_arrSize){
    // STUDENT COMPLETE THIS SECTION  >> -- HWS1----
    // hint: the code here produces ugly, un-aligned, output. How could you make it look better? printf? 
    // Replace the line below with something that produces a cleaner output.
    cout << "Temp Value: " << _tmpArr[ind] << ", Battery Voltage: " << _battArr[ind] << ", Index: " << ind << "\n"; 
    // STUDENT COMPLETE THIS SECTION  << -- HWS1----

    ++ind;
  }
  cout << "\n";
}


bool safe_val_chk(double _tempArr[], double _arrSize, double _setpoint, bool _trigger) 
// Check if buffer <_trigger> alarm setpoint. Return true if alarm condition is
//  met, false otherwise.
//  _trigger: 
//            0 - low value trigger  (buffer < setpoint)
//            1 - high value trigger (buffer > setpoint)
{
  bool shutdownCondition = false;
  // STUDENT COMPLETE THIS SECTION  >> -- HWS4----
  // You need to complete the body of the function.
  // hint: this function should do what the function docstring details. It should return a 1 if the alarm
  // condition is met. High or Low alarm is set using the _trigger.
  // STUDENT COMPLETE THIS SECTION  << -- HWS4----
  return shutdownCondition;
}


void heater_shutdown(int ind)
{
  cout << "!!!!!!!!!!!!!!! ALARM !!!!! ALARM !!!!!!!!!!!!!!!\n";
  cout << "Shutdown Heater index: "<< ind <<"\n";
}

void battery_shutdown(int ind)
{
  cout << "!!!!!!!!!!!!!!! ALARM !!!!! ALARM !!!!!!!!!!!!!!!\n";
  cout << "Shutdown System, Battery Discharged index: " << ind << "\n";
}


int main()
{
  // Setup and allocate data arrays. These hold fake data to test alarm
  // functions. Fake data is generated in a random manner. 
  // >> Allocate Data Arrays >> ---------------------------------------
  const double tempAlarm    = 45.0;
  const double batteryAlarm = 11.6;

  const int tempArraySizeLow  = 12;
  const int tempArraySizeHigh = 18;
  const int tempArraySize = tempArraySizeLow + tempArraySizeHigh;

  double tempValsHigh[tempArraySizeHigh];
  double tempValsLow[tempArraySizeLow];
  double tempVals[tempArraySizeLow+tempArraySizeHigh];

  const int battArraySizeLow = 22;
  const int battArraySize = tempArraySize;
  const int battArraySizeHigh = battArraySize + battArraySizeLow;

  double battValsHigh[battArraySizeHigh];
  double battValsLow[battArraySizeLow];
  double battVals[battArraySizeLow+battArraySizeHigh];

  
  gen_array_data(tempValsLow, tempArraySizeLow, 37.0, 1.0);
  gen_array_data(tempValsHigh, tempArraySizeHigh, 39.0, 1.0);
  merge_arrays(tempValsLow, tempArraySizeLow, tempValsHigh, tempArraySizeHigh, tempVals);

  gen_array_data(battValsLow, battArraySizeLow, 12.0, 0.02);
  gen_array_data(battValsHigh, battArraySizeHigh, 11.3, 0.02);
  merge_arrays(battValsLow, battArraySizeLow, battValsHigh, battArraySizeHigh, battVals);
  // << Allocate Data Arrays << ---------------------------------------


  // Setup and pre-allocate circular buffers for averaging data
  // >> Allocate Buffers >> -------------------------------------------
  const int tempBufferSize = 5;
  const int battBufferSize = 5;

  double tempBuffer[tempBufferSize];
  double battBuffer[battBufferSize];
  double avgTemp;
  double avgBatt;

  assignInitialBuffers(tempBuffer, tempBufferSize, 37.0);
  assignInitialBuffers(battBuffer, battBufferSize, 12.0);
  print_setpoints(tempAlarm, batteryAlarm);

  print_array(tempVals, battVals, tempArraySize);
  double tempAverage = array_average(tempVals, tempArraySize);

  cout << "Average Temperature: " << tempAverage << "\n\n";
  // << Allocate Buffers << -------------------------------------------


  // Check functionality of heater and battery shutdown functions 
  // >> Check Shutdown Conditions >> ----------------------------------
  //
  // Heater Shutdown index is the index of the temperature value that 
  // causes the heater to enter shutdown state.
  bool heaterShutdownCondition = 0;
  int heaterShutdownIndex = 0;
  // Battery Shutdown index is the index of the voltage value that 
  // causes the system to to enter shutdown state.
  bool batteryShutdownCondition = 0;
  int batteryShutdownIndex = 0;
  int ind = 0;
  while (ind < tempArraySize)
  {
    push_array(tempBuffer, tempBufferSize, tempVals[ind]);
    heaterShutdownCondition  = safe_val_chk(tempBuffer, tempBufferSize, tempAlarm, 1);


    push_array(battBuffer, battBufferSize, battVals[ind]);
    batteryShutdownCondition = safe_val_chk(battBuffer, battBufferSize, batteryAlarm, 0);

    if (heaterShutdownCondition){
      heaterShutdownIndex = ind;
      heater_shutdown(heaterShutdownIndex);
      break;
    }

    if (batteryShutdownCondition){
      batteryShutdownIndex = ind;
      battery_shutdown(batteryShutdownIndex);
      break;
    }

    ind++;
  } // end while
  // << Check Shutdown Conditions << ----------------------------------



  cout << "-----------------------------------------------------\n";
  cout << "-----------------------------------------------------\n";


}
