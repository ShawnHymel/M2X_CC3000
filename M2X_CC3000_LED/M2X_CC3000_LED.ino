/****************************************************************
M2X_CC3000_LED.ino
Control an LED with AT&T's M2X
Shawn Hymel @ SparkFun Electronics
August 19, 2014

Manually connects to a WiFi network and performs a GET to the
provided M2X stream. It reads the last data point available
and deletes all data points. This acts as a "producer-consumer"
model.

IMPORTANT: This WILL cause an Uno to run out of RAM. You need to
change SERIAL_BUFFER_SIZE from 64 to 32 in HardwareSerial.cpp,
which can be found in:
C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino

Change AP_SSID, AP_PASSWORD, AP_SECURITY, FEED_ID, STREAM_NAME,
and M2X_KEY to match your WiFi and M2X parameters.

The security mode is defined by one of the following:
WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, WLAN_SEC_WPA2

Resources:
Include SPI.h, SFE_CC3000.h, SFE_CC3000_Client.h, jsonlite.h,
M2XStreamClient.h

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

// Parameters
#define POLL_DELAY_MS   1000 // Poll M2X stream every 1 second

// Pins
#define CC3000_INT      2    // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7    // Can be any digital pin
#define CC3000_CS       10   // Preferred is pin 10 on Uno
#define LED_PIN         9    // LED (we can't use pin 13!)

// Connection info data lengths
#define IP_ADDR_LEN     4    // Length of IP address in bytes
#define TIME_LEN        25   // Length of timestamp string

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

// Global Variables
SFE_CC3000 wifi = SFE_CC3000(CC3000_INT, CC3000_EN, CC3000_CS);
SFE_CC3000_Client client = SFE_CC3000_Client(wifi);
M2XStreamClient m2x_client(&client, M2X_KEY);
uint8_t g_new;
int g_response;
int g_index;
char g_time[TIME_LEN];
uint8_t led_state;

// M2X fetch callback. This is called for each entry found.
void on_data_point_found( const char* at, \
                          const char* value, \
                          int index, \
                          void* context) {
  
  // Index 0 is the most recent. Save the timestamp so we can
  // delete all the entries up to that point. If the value is
  // exactly 0, turn off the LED. Otherwise, turn it on.
  if ( index == 0 ) {
    g_new = 1;
    strncpy(g_time, at, TIME_LEN);
    if ( strcmp(value, "0") == 0 ) {
      led_state = 0;
    } else {
      led_state = 1;
    }
  }
}

// Setup. Configure WiFi and M2X connections.
void setup() {
  
  g_response = 0;
  g_new = 0;
  
  // Initialize LED
  led_state = 0;
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Initialize UART for debugging
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("SparkFun CC3000 - M2X LED"));

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
}

// Main loop. Poll M2X stream, update LED, and delete entries.
void loop() {
  
  // Fetch values from M2X stream
  g_response = m2x_client.fetchValues(FEED_ID, \
                                      STREAM_NAME, \
                                      on_data_point_found, \
                                      NULL);
  
  // If we fail to receive a reponse, stop running
  Serial.print(F("Fetch response: "));
  Serial.println(g_response);
  if ( g_response == -1 ) {
    while(1);
  }
  
  // Update LED and delete entries if there was a new value
  if ( g_new ) {
    
    // Update LED state
    digitalWrite(LED_PIN, led_state);
    Serial.print(F("LED: "));
    Serial.println(led_state);
    
    // Fetch drops last 4 chars. Add some so we can delete entries.
    strncpy(g_time + 20, "999Z", 5);
  
    // Delete all entries
    g_response = m2x_client.deleteValues( FEED_ID, \
                                          STREAM_NAME, \
                                          FROM_TIME, \
                                          g_time);
    
    // If we fail to receive a reponse, stop running
    Serial.print(F("Delete response: "));
    Serial.println(g_response);
    if ( g_response == -1 ) {
      while(1);
    }
    
    // Reset new flag
    g_new = 0;
  }
                                          
  // Wait to poll stream again
  delay(POLL_DELAY_MS);
}
