#include <MicroView.h>   // include MicroView library

MicroViewWidget *widget;  // create widget pointer
MicroViewWidget *widget2;  // create widget pointer
int sensorPin = A1;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup()
{
  digitalWrite(sensorPin, HIGH);  // Internal Pull-up
  pinMode(sensorPin, INPUT);   // make pin as INPUT
  uView.begin();      // start MicroView
  uView.clear(PAGE);     // clear page
  widget = new MicroViewGauge(15, 25, 0, 1024); // make widget as Gauge
  widget2 = new MicroViewGauge(48, 25, 0, 1024); // make widget as Gauge
  uView.display();     // display the content in the screen buffer
}

void loop()
{
  sensorValue = analogRead(sensorPin); // read sensorPin
  widget->setValue(sensorValue);   // set value of sensorPin to widget
  widget2->setValue(sensorValue);   // set value of sensorPin to widget

  uView.display();      // display the content in the screen buffer
}

