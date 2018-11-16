#include <ESP8266WiFi.h>
#include <CapacitiveSensor.h>

#include <PubSubClient.h>

// variables

const char* ssid = "Sensors Network";
const char* password = "sensors4ever";
const char* mqtt_server = "192.168.2.227";
const char* room = "mdef/hello";

CapacitiveSensor capSensor = CapacitiveSensor(2, 4);


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
char msg2[50];

// setup
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

// loop
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    int rawMoisture = analogRead(A0);
    long rawCapacity = capSensor.capacitiveSensor(30);

    
    snprintf (msg, 50, "plant moisture: #%ld", rawMoisture);
    snprintf (msg2, 50, "plant capacity: #%ld", rawCapacity);
    Serial.print("Publish message: ");
    Serial.println(msg);
    Serial.print("Publish message 2: ");
    Serial.println(msg);
    client.publish(room, msg);
    client.publish(room, msg2);
  }
}

// wifi setup
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//reconnect to 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mdef/hello", "wet-my-leaf: hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
