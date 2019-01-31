#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid= "iPhone";
const char* password= "tolgasivgin";

const char* mqtt_server= "m20.cloudmqtt.com";
const char *mqtt_user ="btknvabe";
const char *mqtt_pass="w8ASFc1mfisB";

String MakerIFTTT_Key ;
String MakerIFTTT_Event;
char *append_str(char *here, String s) {  int i=0; while (*here++ = s[i]){i++;};return here-1;}
char *append_ul(char *here, unsigned long u) { char buf[20]; return append_str(here, ultoa(u, buf, 10));}
char post_rqst[256];char *p;char *content_length_here;char *json_start;int compi;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg=0;
char msg[50];
int value =0;

void setup_wifi(){
  delay(100);

  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  randomSeed(micros());

    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
}

void reconnect(){

  while(!client.connected()){
    
    Serial.println("Attempting MQTT connection...");
    String clientId="ESP8266Client-";
    clientId+= String(random(0xffff), HEX);

    if(client.connect(clientId.c_str(), mqtt_user, mqtt_pass)){
      Serial.println("Connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
      
    }
    else{
      Serial.println("failed, rc=");
      Serial.println(client.state());
      Serial.println("Trying again in 5 seconds");

      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
Serial.println("Message arrived [");
Serial.println(topic);
Serial.println("] ");

for(int i=0; i< length; i++){
  Serial.println((char)payload[i]);
  
}
  Serial.println();
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 19973);
  client.setCallback(callback);
  reconnect();

}

void loop() {
  if (!client.connected()){
    reconnect();
  }

  client.loop();

  int sensorValue = digitalRead(D8);
  Serial.println("Current motion value "+String(sensorValue));

  delay(1000);
  String motion= String(sensorValue);

  Serial.println("Publish message: ");
  Serial.println(motion);

  int numgas= sensorValue;
  char csmotion[16];
  itoa(numgas, csmotion, 10);

  delay(1500);
  client.publish("Motion", csmotion);

  if(sensorValue > 0){

 if (espClient.connect("maker.ifttt.com",80)) {
    MakerIFTTT_Key ="n1KCimZadXu4vQcIUGnSv33N9oPFzZIbeqRMAOiGxjG";
    MakerIFTTT_Event ="email";
    p = post_rqst;
    p = append_str(p, "POST /trigger/");
    p = append_str(p, MakerIFTTT_Event);
    p = append_str(p, "/with/key/");
    p = append_str(p, MakerIFTTT_Key);
    p = append_str(p, " HTTP/1.1\r\n");
    p = append_str(p, "Host: maker.ifttt.com\r\n");
    p = append_str(p, "Content-Type: application/json\r\n");
    p = append_str(p, "Content-Length: ");
    content_length_here = p;
    p = append_str(p, "NN\r\n");
    p = append_str(p, "\r\n");
    json_start = p;
    p = append_str(p, "{\"value1\":\"");
    p = append_str(p, "NodeMCU");
    p = append_str(p, "\",\"value2\":\"");
    p = append_str(p, "Emergency! ");
    p = append_str(p, "\",\"value3\":\"");
    p = append_str(p, "Motion  is detected.");
    p = append_str(p, "\"}");

    compi= strlen(json_start);
    content_length_here[0] = '0' + (compi/10);
    content_length_here[1] = '0' + (compi%10);
    client.print(post_rqst);    
  }

  Serial.println("Motion is detected");
  Serial.println("Your message has been sent.");
  delay(5000);
  }
  else{
    Serial.println("No motion");
    delay(2000);
  }

}
