#include <SPI.h>
#include <WiFi101.h>
//#include <ArduinoJson.h>
//#include <HttpClient.h>

WiFiClient client;

char ssid[] = "XXXX";     //  your network SSID (name)
char pass[] = "XXXX";  // your network password

int status = WL_IDLE_STATUS;     // the Wifi radio's status

int BLUE=0;
int RED=1;
int YELLOW=2;
int SPEED=500;

void setup() {
  // put your setup code here, to run once:
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  while ( status != WL_CONNECTED) {
    // attempt to connect to WiFi network:
    
    
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println (ssid);
    
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    
    delay(10000);
    
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    
    
  }


  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(5,INPUT);

}

void loop() {

  Serial.println("Starting loop");
  
//client.get("http://slack.com/api/search.messages?token=XXXXXXXXXquery=in%3Atech-arduino%20obrigado&pretty=1");
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connectSSL("www.slack.com", 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /api/search.messages?token=XXXXXXXXXXX&query=in%3Atech-arduino%20obrigado&pretty=1 HTTP/1.1");
    //client.println("GET /search?q=arduino HTTP/1.1");
    
    //client.println("Host: www.google.com");
    client.println("Host: www.slack.com");
    
    client.println("Connection: close");
    client.println();
  }

  delay(10000);
  

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }


  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    
  }
  // put your main code here, to run repeatedly:

  int btnValue = digitalRead(5);

  if(btnValue == HIGH){
    lightState(HIGH,HIGH,HIGH);
    delay(SPEED);

    lightState(HIGH,LOW,LOW);
    delay(SPEED);

    lightState(LOW,HIGH,LOW);
    delay(SPEED);
       
    lightState(LOW,LOW,HIGH);
    delay(SPEED);
    
    lightState(HIGH,LOW,LOW);
    delay(SPEED);

    lightState(LOW,HIGH,LOW);
    delay(SPEED);
       
    lightState(LOW,LOW,HIGH);
    delay(SPEED);

    lightState(HIGH,HIGH,HIGH);
    delay(SPEED);

    lightState(LOW,LOW,LOW);
    delay(SPEED);

    lightState(HIGH,HIGH,HIGH);
    delay(2*SPEED);

  }
  
  lightState(LOW,LOW,LOW);

  delay(10000);
}

void lightState(int blue, int red, int yellow){
    digitalWrite(BLUE,blue);
    digitalWrite(RED,red);
    digitalWrite(YELLOW,yellow);
}
