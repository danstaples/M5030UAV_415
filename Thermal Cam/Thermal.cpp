/***************************************************************************
  This is a library for the AMG88xx GridEYE 8x8 IR camera

  This sketch tries to read the pixels from the sensor

  Designed specifically to work with the Adafruit AMG88 breakout
  ----> http://www.adafruit.com/products/3538

  These sensors use I2C to communicate. The device's I2C address is 0x69

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/


#include <Adafruit_AMG88xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
int M_pixels[8][8];
float LowerThresh = 25.0; //
float LowerMidThresh = 27.5;//
float MidThresh = 30.0; // Change these value to alter when LED lights up, value is in celsius
float UpperMidThresh = 32.5
float UpperThresh = 35.0; //
void setup() {
	printf("Thermal sensor\n");
    Thermal_ini();
}


void loop() { 
    //read all the pixels
    amg.readPixels(pixels);

    printf("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      printf(pixels[i-1]);
      printf(", ");
      if( i%8 == 0 ) printf(" /n");
    }
    printf("]/n");
    printf(" /n");

    check_pixels();
   // refreshScreen();
    
    //delay half second
    sleep(0.5);
}

void Thermal_ini(){

bool status;
    
    // default settings
    status = amg.begin();
    if (!status) {
        printf("Could not find a valid AMG88xx sensor, check wiring!/n");
        while (1);
    }
    
    printf("-- Pixels Test --/n");

    printf("/n");

    sleep(0.1); // let sensor boot up

}


void check_pixels(){
  int row;
  int col;
  int val;
// clear all previous pixels for next refresh
  for (int j = 0; j<8; j++){
    for  (int h = 0; h<8; h++){
        M_pixels[j][h] = 0;
    }
  }
  // if a pixel is above the temp threshold set to high
  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
    
      row = floor(i/8);
     // printf(row);
     // printf(',');
     if (i%8 == 0){
     col = 8;
     }
     else
      col = i%8;
     // printf(col);
     if (pixels(i-1) >= UpperThresh){
      val = 6;
      else if (pixels(i-1) >= UpperMidThresh) val = 5;
      else if (pixels(i-1) >= MidThresh) val = 4;
      else if (pixels(i-1) >= LowerMidThresh) val = 3;
      else if (pixels(i-1) >= MidThresh) val = 2;
      else if (pixels(i-1) >= LowerThresh) val = 1;
      else val = 0;
     }
  M_pixels[row][col] = val;
}

//This will print out all the pixels that should be turned on to 
pixels_debug();
}


void pixels_debug(){

  for (int i = 0; i<8; i++){
    for  (int j = 0; j<8; j++){
         printf(M_pixels[i][j]);
         printf(',');
    }
    printf("/n");
  }
}