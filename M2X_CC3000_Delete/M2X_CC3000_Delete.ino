/****************************************************************
M2X_CC3000_Delete.ino
Delete entries from an AT&T M2X stream
Shawn Hymel @ SparkFun Electronics
August 19, 2014

Manually connects to a WiFi network and an M2X stream. Deletes
values between the date/time ranges provided.

Change AP_SSID, AP_PASSWORD, AP_SECURITY, FEED_ID, STREAM_NAME,
and M2X_KEY to match your WiFi and M2X parameters. Update
FROM_TIME and END_TIME.

The security mode is defined by one of the following:
WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, WLAN_SEC_WPA2

Resources:
Include SPI.h, SFE_CC3000.h, SFE_CC3000_Client.h, jsonlite.h,
M2XStreamClient.h, Wire.h, HTU21D.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with Arduino UNO R3

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
****************************************************************/

#include <jsonlite.h>
#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>
#include <M2XStreamClient.h>

// Pins
#define CC3000_INT      2   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno

// Connection info data lengths
#define IP_ADDR_LEN     4   // Length of IP address in bytes

// WiFi Constants
#define AP_SSID         "<SSID>"      // SSID of network
#define AP_PASSWORD     "<PASSWORD>"  // Password of network
#define AP_SECURITY     WLAN_SEC_WPA2 // Security of network
#define TIMEOUT         30000         // Milliseconds

// M2X Constants
#define FEED_ID         "<FFED ID>"
#define STREAM_NAME     "<STREAM NAME>"
#define M2X_KEY         "<M2X MASTER KEY>"
#define FROM_TIME       "1970-01-01T00:00:00.000Z"
#define END_TIME        "<END TIME>"

// Global Variables
SFE_CC3000 wifi = SFE_CC3000(CC3000_INT, CC3000_EN, CC3000_CS);
SFE_CC3000_Client client = SFE_CC3000_Client(wifi);
M2XStreamClient m2x_client(&client, M2X_KEY);
int g_response;

// Setup. Configure WiFi and M2X connections. Delete entries.
void setup() {
  
  g_response = 0;
  
  // Initialize UART for debugging
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("SparkFun CC3000 - M2X Delete"));
  
  // Initialize CC3000 (configure SPI communications)
  if ( wifi.init() ) {
    Serial.println(F("CC3000 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during CC3000 init!"));
  }
  
  // Connect using DHCP
  Serial.print(F("Connecting to SSID: "));
  Serial.println(AP_SSID);
  if(wifi.connect(AP_SSID, AP_SECURITY, AP_PASSWORD, TIMEOUT)) {
    Serial.println(F("Connected!"));
  } else {
    Serial.println(F("Error: Could not connect to AP"));
  }
  
  // Delete values
  Serial.println(F("Deleting values..."));
  g_response = m2x_client.deleteValues(FEED_ID, \
                                       STREAM_NAME, \
                                       FROM_TIME, \
                                       END_TIME);
  Serial.print(F("Delete response: "));
  Serial.println(g_response);                    
}

// Main loop. Do nothing.
void loop() {
  delay(5000);
}
