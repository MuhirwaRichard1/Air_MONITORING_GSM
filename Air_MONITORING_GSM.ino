//-----------------------------------------------------------------------
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
//-----------------------------------------------------------------------
/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL2uyp9fV7l"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring FINAL"
#define BLYNK_AUTH_TOKEN "G2WkQhgcvepfI13IE0l0d6XKd2xxIyuk"
//-----------------------------------------------------------------------
// Select your modem:
//#define TINY_GSM_MODEM_SIM800
 #define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_M590
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_A7
//#define TINY_GSM_MODEM_BG96
//#define TINY_GSM_MODEM_XBEE
//-----------------------------------------------------------------------
// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30
//-----------------------------------------------------------------------
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
//-----------------------------------------------------------------------
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;
//-----------------------------------------------------------------------
// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "internet";
char user[] = "";
char pass[] = "";
//-----------------------------------------------------------------------
// Hardware Serial on Mega, Leonardo, Micro
//#define SIM900 Serial1
//-----------------------------------------------------------------------
// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // RX, TX

//TinyGsm modem(SerialAT);
TinyGsm modem(SIM900);
//-----------------------------------------------------------------------
#include <DHT.h>
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//-----------------------------------------------------------------------
BlynkTimer timer;
#define INTERVAL 100L

#include <Wire.h>

void SendDhtData()
{
  //-----------------------------------------------------------------------
  //Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  //-----------------------------------------------------------------------
  //Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //-----------------------------------------------------------------------
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // put your main code here, to run repeatedly:
delay(2000);

int sensor_value_1 = analogRead(A1);
int sensor_value_2 = analogRead(A2);
int sensor_value_3 = analogRead(A3);

Serial.print("Humidity: ");
Serial.print(h);
Serial.print("%");
Serial.print("  Temperature");
Serial.print(t);
Serial.println("Cel");
Serial.print("Air_quality_1: ");
Serial.print(sensor_value_1, DEC);
Serial.println(" PPM");

Serial.print("Air_quality_2: ");
Serial.print(sensor_value_2, DEC);
Serial.println(" CO");

Serial.print("Air_quality_3: ");
Serial.print(sensor_value_3, DEC);
Serial.println(" CO2");

  //-----------------------------------------------------------------------
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
   Blynk.virtualWrite(V0, t);
   Blynk.virtualWrite(V1, h);
   Blynk.virtualWrite(V2, sensor_value_1);
   Blynk.virtualWrite(V3, sensor_value_2);
   Blynk.virtualWrite(V4, sensor_value_3);
 
}

void setup() {

  // Debug console
  Serial.begin(115200);
  delay(10);
  //-----------------------------------------------------------
  // Set GSM module baud rate
   SIM900.begin(9600);
  //Sim800L.begin(9600,SWSERIAL_8N1,D3,D4);
  delay(3000);
  //-----------------------------------------------------------
  dht.begin();
  //-----------------------------------------------------------
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
   
  modem.init();
  // Unlock your SIM card with a PIN
   modem.simUnlock("1234");
  //-----------------------------------------------------------
  Blynk.begin(auth, modem, apn, user, pass);
  //-----------------------------------------------------------
  // Setup a function to be called every second
    timer.setInterval(INTERVAL, SendDhtData);
  //-----------------------------------------------------------  
}

void loop() {


    Blynk.run();
    timer.run();
}
