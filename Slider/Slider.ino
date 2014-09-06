
//--------------------------------------------------------------
// The Adafruit FONA library
// see https://github.com/adafruit/Adafruit_FONA_Library
//--------------------------------------------------------------
#include <SoftwareSerial.h>
#include <Adafruit_FONA.h>

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(&fonaSS, FONA_RST);


//--------------------------------------------------------------
// The MicroView library
// see https://github.com/geekammo/MicroView-Arduino-Library
//--------------------------------------------------------------
#include <MicroView.h>


MicroViewWidget *widget;		// create widget pointer
MicroViewWidget *widget2;		// create widget pointer


void setup()
{
  //-----------------
  // start MicroView
  //-----------------
  uView.begin();

  // clear page
  uView.clear(PAGE);

  /*
  // make widget as Slider
   widget = new MicroViewSlider(0, 0, 0, 1024);
   // make widget as Silder STYLE1
   widget2 = new MicroViewSlider(0, 20, 0, 1024, WIDGETSTYLE1);
   
   // display the content in the screen buffer
   uView.display();
   */

  //--------------------------------
  // See if the FONA is responding
  //--------------------------------
  if (! fona.begin(4800))
  {  // make it slow so its easy to read!
    uView.print("Couldn't find FONA!");
    while (1);
  }

  uView.println("FONA is OK");

  // get FONAs battery voltage
  readBattery();

  // display the content in the screen buffer
  uView.display();
}


void loop()
{
  // sleep
  delay(100);
}


// read the battery voltage
void readBattery()
{
  uint16_t vbat;
  
  
  if (! fona.getBattVoltage(&vbat))
  {
    uView.print("Failed to read Batt");
  } 
  else
  {
    uView.print("Bat:"); 
    uView.print(vbat); 
    uView.print("mV");
  }
}

