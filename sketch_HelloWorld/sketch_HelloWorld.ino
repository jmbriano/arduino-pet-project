#include <SPI.h> 
#include <WiFi101.h> 
#include <ArduinoJson.h>

  // IMPORTANT: in order for this sketch to work, the SSL certifiates must be installed in the board. To do so:
  //  1) Load into the board the Examples > WiFi101 > FirmwareUpdater example
  //  2) Go to Tools > WiFi101 Firmware Updater. And load the certificate for slack.com (make sure the Serial Console is not open otherwise it would not connect)

WiFiClient client;

char ssid[] = "XXXX"; //  your network SSID (name)
char pass[] = "XXXX"; // your network password

int status = WL_IDLE_STATUS; // the Wifi radio's status

int BLUE = 2;
int GREEN = 1;
int YELLOW = 0;
int NO_BUTTON = 10;
int SPEED = 5000;

void setup() {
  // put your setup code here, to run once:

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  //; // wait for serial port to connect. Needed for native USB port only
  //}

  while (status != WL_CONNECTED) {
    // attempt to connect to WiFi network:

    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    delay(10000);

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

  }

  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(5, INPUT);
  pinMode(NO_BUTTON, OUTPUT);

}

void loop() {

  //Serial.println("Starting loop");

  // put your main code here, to run repeatedly:

  int btnValue = digitalRead(5);

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connectSSL("www.slack.com", 443)) {
    Serial.println("connected to server");

    // Make a HTTP request:
    String lastMessage = searchSlack("XXXX", "in%3Atech-arduino");
    Serial.println(lastMessage);

    lastMessage.toLowerCase();

    if (lastMessage.equals("blue")) {
      lightState(HIGH, LOW, LOW);
    } else if (lastMessage.equals("green")) {
      lightState(LOW, HIGH, LOW);
    } else if (lastMessage.equals("red")) {
      lightState(LOW, LOW, HIGH);
    } else if (lastMessage.indexOf("obrigado") >= 0) {
      lightState(HIGH, HIGH, HIGH);
    } else if (lastMessage.indexOf("nooo") >= 0) {
      lightState(HIGH, HIGH, HIGH);
      shoutAll();
    } else {
      lightState(LOW, LOW, LOW);
    }

  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

  }

  delay(SPEED);
}

String searchSlack(String token, String query) {

  client.println("GET /api/search.messages?token=" + token + "&query=" + query + "&pretty=1&count=1 HTTP/1.1");
  //client.println("GET /search?q=arduino HTTP/1.1");

  //client.println("Host: www.google.com");
  client.println("Host: www.slack.com");

  client.println("Connection: close");
  client.println();

  delay(1000);

  // Check HTTP status
  char status[32] = {
    0
  };
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return "";
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return "";
  }

  // skip characters before json
  client.find("\n");

  // Parse JSON object

  DynamicJsonBuffer jsonBuffer(2000);
  JsonObject & root = jsonBuffer.parseObject(client);
  if (!root.success()) {
    Serial.println(F("Parsing failed!"));

    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

  }

  Serial.println(F("Response:"));
  Serial.println(root["ok"].as < char * > ());

  String lastMessage = root["messages"]["matches"][0]["text"].as < char * > ();

  return lastMessage;
}

void lightState(int blue, int green, int yellow) {
  digitalWrite(BLUE, blue);
  digitalWrite(GREEN, green);
  digitalWrite(YELLOW, yellow);
}

void shout() {
  digitalWrite(NO_BUTTON, HIGH);
  delay(1000);
  digitalWrite(NO_BUTTON, LOW);
}

void shoutAll() {
  for (int i = 0; i < 5; i++) {
    shout();
    delay(1500);
  }
}
