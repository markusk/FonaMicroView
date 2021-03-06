//--------------------------------------------------------------
// Arduino Pins
//--------------------------------------------------------------
const int FONA_RX   =  2;
const int FONA_TX   =  3;
const int FONA_RST  =  4;
const int buttonPin = A0; // Pushbutton
//--------------------------------------------------------------
// other global stuff
//--------------------------------------------------------------
int returnValue = 0;



//--------------------------------------------------------------
// The Adafruit FONA library
// see https://github.com/adafruit/Adafruit_FONA_Library
//--------------------------------------------------------------
#include <SoftwareSerial.h>
#include <Adafruit_FONA.h>

// this is a large buffer for replies
char replybuffer[255];

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(&fonaSS, FONA_RST);

// store the state if FONA is okay or not (init okay etc.)
boolean FONAstate = false;


//--------------------------------------------------------------
// The MicroView library
// see https://github.com/geekammo/MicroView-Arduino-Library
//--------------------------------------------------------------
#include <MicroView.h>

// Create widget pointers
MicroViewWidget *widgetBattery;


void setup()
{
  //--------------------
  // setup Arduino PINs
  //--------------------
  pinMode(buttonPin, INPUT_PULLUP);     


  //-----------------
  // start MicroView
  //-----------------
  uView.begin();

  // clear page
  uView.clear(PAGE);

  // draw Slider widget at x=0, y=0, min=0, max=4700(mV)  
  widgetBattery = new MicroViewSlider(0, 0, 0, 4700);


  //--------------------------------
  // See if the FONA is responding
  //--------------------------------
  if (! fona.begin(4800))
  {  // make it slow so its easy to read!
    uView.print("Couldn't find FONA!");
    while (1);
  }


  //----------------------------------
  // FONA is Okay... We continue. :-)
  //----------------------------------

  //--------------------------------
  // get battery voltage
  //--------------------------------
  returnValue = readBattery();

  // display value
  if (returnValue != -1)
  {
    widgetBattery->setValue( returnValue );
  }
  else
  {
    widgetBattery->setValue(0);
    uView.println("Error Bat.");
  }

  // move to next line
  uView.println("");


  /* works but not needed
  //--------------------------------
  // get SIM card IMEI number
  //--------------------------------
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  
  if (imeiLen > 0)
  {
    uView.println("SIM IMEI: ");
    uView.println(imei);
  }
  */


  //--------------------------------
  // unlock phone with PIN
  //--------------------------------
  if (unlockSIM() == -1)
  {
    FONAstate = false;
    
    uView.println("Error PIN!");
  }
  else
  {
    //---------------------
    // PIN okay. go ahead.
    //---------------------

    FONAstate = true;
    
  } // PIN okay


  //------------------------------------------
  // display the content in the screen buffer
  //------------------------------------------
  uView.display();
}


void loop()
{
  if (FONAstate == true)
  {
    // read button
    if (digitalRead(buttonPin) == LOW)
    {
      //--------------------------------
      // get number of available SMS
      //--------------------------------
      returnValue = readNumSMS();
    
      // display value
      if (returnValue != -1)
      {
        uView.print(returnValue);
        uView.println(" SMS");
      }
      else
      {
        uView.println("Error SMS!");
      }
    }

    //------------------------------------------
    // display the content in the screen buffer
    //------------------------------------------
    uView.display();

    // sleep
    delay(100);
  } // FONA is okay

  
  // sleep
  delay(100);
}


// read the battery voltage
uint16_t readBattery()
{
  uint16_t vbat = -1;


  if (! fona.getBattVoltage(&vbat))
  {
    // uView.print("Failed to read Batt");
    return -1;
  } 

  return vbat;
}


// read the number of SMS's
int8_t readNumSMS()
{
  int8_t smsnum = fona.getNumSMS();


  if (smsnum < 0)
  {
    return -1;
  }

  return smsnum; 
}


int unlockSIM()
{
  // Unlock the SIM with a PIN code
  char PIN[5];


  // PIN
//  PIN[0] = '0';
//  PIN[1] = '0';
//  PIN[2] = '0';
//  PIN[3] = '0';
//  PIN[4] = NULL;

  // unlock
  if (! fona.unlockSIM(PIN))
  {
    // error
    return -1;
  } 
  else
  {
    // ok
    return 0;
  }        
}

