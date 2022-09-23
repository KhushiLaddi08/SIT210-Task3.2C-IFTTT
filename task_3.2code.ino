// BH1750FVI - Version: Latest 
#include <BH1750FVI.h>
#include <WiFiNINA.h>
BH1750FVI Sensor(BH1750FVI::k_DevModeContLowRes);

//please enter the Wifi Connection Details
char ssid[] = "";
char pass[] = "";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String queryString = "?value1=57&value2=25";

bool sunlight = false;

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  Sensor.begin();
  
  Serial.begin(9600);
  while (!Serial);
}

void sunlight_received() {
  String PATH_NAME   = "/trigger/sunlight_received/with/key/your_key"; // change your EVENT-NAME and YOUR-KEY
  
  // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header
   

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
}

void no_sunlight() {
  String PATH_NAME   = "/trigger/no_sunlight/with/key/your_key"; // change your EVENT-NAME and YOUR-KEY
  
  // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header


    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
}

void connect() {
  while (!Serial);

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
}

void disconnect() {
  client.stop();
  Serial.println();
  Serial.println("disconnected");
}

void loop() {
  int light_intensity = Sensor.GetLightIntensity();
  int counter = 0;
  
  Serial.println(light_intensity);

  if (light_intensity < 100 && sunlight == true )
  {
    count++;
    if(counter > 5000)
    {
    connect();
    no_sunlight();
    disconnect();
    sunlight = false;
    counter = 0;
    }
  }
  else if (light_intensity > 100 && sunlight == false)
  {
    count++;
    if (counter > 5000)
    {
    connect();
    sunlight_received();
    disconnect();
    sunlight = true;
    counter = 0;
    }
  }
}
