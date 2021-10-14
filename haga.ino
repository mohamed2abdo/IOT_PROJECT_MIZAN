/*************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings

#define BLYNK_TEMPLATE_ID "TMPL2SjsVpqO"
#define BLYNK_DEVICE_NAME "MIZAN"
#define BLYNK_AUTH_TOKEN "dgo1ARO_kHaZNeW6aTyud5X438L5dJK3";
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include "HX711.h"
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

HX711 scale;

const int LOADCELL_DOUT_PIN = D5;
const int LOADCELL_SCK_PIN = D6;

char auth[] = BLYNK_AUTH_TOKEN ;
char ssid[] = "agribot";
char pass[] = "agribot1";

//double wazn = scale.get_units();// for notify

BlynkTimer timer;//variable to sending data
float weight;
float calibration_factor = 75028.00; // for me this vlaue works just perfect 419640

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  double wazn = scale.get_units();
  Blynk.virtualWrite(V3, wazn);
}

//sending notifications
void notifyUptime()
{
  long uptime = millis() / 60000L;

  // Actually send the message.
  // Note:
  //   We allow 1 notification per 5 seconds for now.
  Blynk.notify(String("Mizan ") + uptime + " minutes.");

  // You can also use {DEVICE_NAME} placeholder for device name,
  // that will be replaced by your device name on the server side.
  // Blynk.notify(String("{DEVICE_NAME} running for ") + uptime + " minutes.");
}

void setup() 
{
  Serial.begin(115200);
 
  // hx711 load cell start
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  
  //blynk start
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

   // Notify immediately on startup
  Blynk.notify("Device started");

  // Setup a function to be called every minute
  timer.setInterval(60000L, notifyUptime);
}

void loop()
{
  Blynk.run();
  timer.run(); 
 /* double mazn = scale.get_units();
  if(mazn <10)
  {
      Blynk.notify("ahmed alhafez is requiring sugar!");
  }
  */
}
