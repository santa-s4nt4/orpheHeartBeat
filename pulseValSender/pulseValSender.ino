#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

#include <Adafruit_NeoPixel.h>

char ssid[] = "ssid";          // your network SSID (name)
char pass[] = "pass";                    // your network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,0,5);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
//int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.

const int LEDPIN = 0;
Adafruit_NeoPixel neopixel(1, LEDPIN, NEO_GRB + NEO_KHZ800);


void setup() {
    Serial.begin(74880);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");６ｑ
    Serial.println(localPort);
#else
    Serial.println(Udp.localPort());
#endif
  
  neopixel.begin();
  neopixel.clear(); // Set all pixel colors to 'off'

    neopixel.setPixelColor(0, neopixel.Color(255, 0, 0));
}

void loop() {
    int pulse_val = analogRead(A0);
    Serial.println(pulse_val);
    
    OSCMessage msg("/pulse_val");
    msg.add(pulse_val);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    
    neopixel.clear(); // Set all pixel colors to 'off'

    int red = map(pulse_val, 400, 1000, 50, 255);
    neopixel.setPixelColor(0, neopixel.Color(red, 0, 50));

    neopixel.show();

    
    delay(20);
}
