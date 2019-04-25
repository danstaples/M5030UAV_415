#include <Wire.h>
#include <VL53L1X.h>
#include <Adafruit_AMG88xx.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
//#include <std_msgs/Int32.h>

// ToF & Thermal sensor code for Project 415 Firefighting UAV's
// Code adapted for use in MECH5030 Team project from adafruits AMG8833 library and example code, and ST's VL53L1X librarie and example code

#define SensorNum 8 // Change number if you have less sensors, just be aware that the digital pins count down, so you may need to move the starting pin number up from pin 6

VL53L1X sensors[SensorNum];
Adafruit_AMG88xx amg;

ros::NodeHandle nh;
std_msgs::Float32 sensor_msg;
std_msgs::String str_msg;



float pixels[AMG88xx_PIXEL_ARRAY_SIZE]; // Pixel array size is 64, as it is an 8x8 thermopile array


/* Change these values below to alter what the bands the temperature is classified as
 *  these values are output into a 8x8 array with numbers of 0 to 5, with temperatures below the lower threshold being 0
 *  this is done because its a lot easier to read at a glance a small array of numbers and it's easy to visualise where the heat is compared to the 8x8 of floating numbers
 *  the values are in degrees celsius
 */ 

ros::Publisher chatter("chatter", &sensor_msg);
char hello[13] = "hello world!";


void setup() {

 
  
  nh.initNode();
  nh.advertise(chatter);


  Wire.begin();
  delay(100);

  tof_setup();
  thermal_setup();

 
}

void loop() {
  // put your main code here, to run repeatedly:


  sensor_msg.data = 0.123456789;
  chatter.publish( &sensor_msg );
  nh.spinOnce();


    
    thermal_read();
    tof_read();
    
   delay(1000);
  
}

void tof_setup(){

  int address = 0x28; // first address that the first sensor will be set to
  
  for (int i = (SensorNum+1); i > 1; i--){ // sets up pins 9 to 2, for the XSHUT pin on VL53L1X to allow for address change, as long as SensorNum is 8, otherwise it will count down from SensorNum+1 to 2
  
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
        delay(100);
    
  }
  
  
  
  for (int j = 0; j < SensorNum; j++){
        
        pinMode((SensorNum+1) - j, INPUT); 
        delay(250);
        sensors[j].init(true);
        delay(150);
        sensors[j].setAddress(address);
  
  
        address += 2;
        delay(150);
      
        sensors[j].setDistanceMode(VL53L1X::Long);
        sensors[j].setMeasurementTimingBudget(50000);
        sensors[j].startContinuous(50);
        sensors[j].setTimeout(100);
      }
      
    delay(150);
  
}

void thermal_setup(){

  
  bool status;
    
    // default settings
    status = amg.begin();

    delay(100); // let sensor boot up
  
}

void tof_read(){

  float buf = 0.1234;
 
  publish_data(buf);

  for (int i = 0; i < (SensorNum); i++){

    if (sensors[i].timeoutOccurred()) buf = 8000.0; 
    else buf = sensors[i].read()/1000.0; // converts mm reading to meter, 4 signicant figures

    publish_data(buf);
    
  }

//  buf = 0.1234;
 
 // publish_data(buf);
 
}

void thermal_read(){

  amg.readPixels(pixels);
  float buf = 0.5678; 

  publish_data(buf);

  for (int i = 1; i <= AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    
    buf = (pixels[i - 1]);
    publish_data(buf);
    
  }
  
}

void publish_data(float buf){

    sensor_msg.data = buf;
    chatter.publish( &sensor_msg );
    nh.spinOnce();

}
