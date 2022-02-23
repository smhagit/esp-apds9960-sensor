/*******************************************************************************************
* Basic starter template as a good point to start a new script
* Features:
*   - WiFi connection/ reconnection
*   - Control APDS9960 sensor to detect gestures
* Created for a ESP8266/ NodeMCU project
* * 
* ******************************************************************************************
*/

 

/** Some libraries **/
/** -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- **/
#include <Adafruit_APDS9960.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_BusIO_Register.h>

/** WLAN **/
/** -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- **/
#include <ESP8266WiFi.h>
#include <Ticker.h>

#define WIFI_SSID "WLAN_SSID"
#define WIFI_PASSWORD "WLAN_PASSWORD"

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

static bool WIFI_STATUS = false;

/** APDS9960 Sensor **/
/** -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- **/
Adafruit_APDS9960 apds;



/**
 * SETUP
 */
void setup() {
  Serial.begin(9600);
  Serial.println("== [SETUP]: started ==");
  
  // Setup WiFi
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  connectToWifi();
  Serial.println("WiFi connected!");


  // Setup APDS9960 sensor
  // ---    ---    ---    ---    ---    ---    ---    ---    ---    ---    ---    
  if (!apds.begin()){
    Serial.println("Initialiserung des Sensors fehlgeschlagen.");
  } else {
    Serial.println("Der Sensor wurde erfolgreich initialisiert!");
  }

  // Set GGAIN_1
  apds.setGestureGain(APDS9960_GGAIN_1);
  
  // Enable Proximity and Gesture detection
  apds.enableProximity(true);
  apds.enableGesture(true);
  
  Serial.println("== [SETUP]: completed ==");
}




/** -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- **/

/**
 * LOOP
 */
void loop() {
  // Read sensor value
  uint8_t gesture = apds.readGesture();

  switch(gesture){
    case APDS9960_LEFT: Serial.println("LEFT"); break;
    case APDS9960_RIGHT: Serial.println("RIGHT"); break;
    case APDS9960_UP: Serial.println("UP"); break;
    case APDS9960_DOWN: Serial.println("DOWN"); break;  
  }
}

/** -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- **/


/**
 * WiFi connect
 */
void connectToWifi() {
  Serial.println("[WiFi]: Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

/**
 * WiFi is connected
 */
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("[WiFi]: Connected to Wi-Fi.");
  WIFI_STATUS = true;
//  connectToMqtt();
}

/**
 * WiFi is disconnected
 */
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("[WiFi]: Disconnected from Wi-Fi.");
  wifiReconnectTimer.once(2, connectToWifi);
}