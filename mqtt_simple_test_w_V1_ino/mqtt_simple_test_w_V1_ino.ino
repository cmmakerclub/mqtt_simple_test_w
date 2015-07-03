#define DEBUG_MODE

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttWrapper.h>
#include <PubSubClient.h>

const char* ssid     = "CMMC.47";
const char* pass = "guestnetwork";

// const char* ssid     = "OpenWrt_NAT_500GP.101";
// const char* pass = "activegateway";

// const char* ssid     = "MAKERCLUB-CM";
// const char* pass = "welcomegogogo";

//const char* ssid     = "Opendream Play";
//const char* pass = "5k,skrijv',7'sik";

// const char* ssid     = "Opendream";
// const char* pass = "gfkgvkgv'2015!!!!";

float W3S=0;
int swt=1;
MqttWrapper *mqtt;

void connect_wifi()
{
    WiFi.begin(ssid, pass);

    int retries = 0;
    while ((WiFi.status() != WL_CONNECTED))
    {
        Serial.print(".");
        retries++;
        delay(500);
    }

    Serial.println("WIFI CONNECTED ");
}

void reconnect_wifi_if_link_down() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("WIFI DISCONNECTED");
        connect_wifi();
    }
}


void callback(const MQTT::Publish& pub) {
    if (pub.payload_string() == "1") {
        Serial.print(" => ");
        Serial.println(pub.payload_string());
        digitalWrite(14, HIGH);
        swt=1;
    }
    else if(pub.payload_string() == "0") {
        Serial.print(" => ");
        Serial.println(pub.payload_string());
        digitalWrite(14, LOW);
        swt=0;
        W3S=0;
    }
    else {
        Serial.print(pub.topic());
        Serial.print(" => ");
        Serial.println(pub.payload_string());
    }
}

void hook_prepare_data(JsonObject** root) {
  JsonObject& data = (*(*root))["d"];
  Watts();
  data["myName"] = "TONG_Watts";
  data["W"] = W3S;;

}

void setup() {
    Serial.begin(115200);
    pinMode(0, INPUT_PULLUP);
    pinMode(14, OUTPUT);
    delay(10);
    Serial.println();
    Serial.println();

    connect_wifi();

    mqtt = new MqttWrapper("128.199.104.122");
    mqtt->connect(callback);
    mqtt->set_prepare_data_hook(hook_prepare_data);
}
void Watts (){
      double sum;
    for(int i=0;i<500;i++){
    int sensorValue = analogRead(A0)-555;
    if(sensorValue<0)
    sensorValue=-sensorValue;
     sum=sum+sensorValue;
     }
     sum=(sum/500)-4;
     Serial.println(sum);
     sum=sum*0.031*220;
     if(swt==1){
       sum=sum/(3600/3);
       W3S=W3S+sum;
     }
}

void loop() {
    reconnect_wifi_if_link_down();
    mqtt->loop();

    // ตรวจจับการกด Switch
//    if (digitalRead(0) == LOW) {
//        // วนลูปจนกว่าจะเอาปล่อย Switch
//        while(digitalRead(0) == LOW) { 
//            mqtt->loop();
//            yield(); 
//        }
//        String status = "0";
//        mqtt->sync_pub(status);
//    }
}

