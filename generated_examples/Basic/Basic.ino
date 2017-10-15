
/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <DHT.h>

const char* ssid     = "OUY_2";
const char* password = "4794459390";

#define APPID       "Metheepittaya"
#define KEY         "J9Kf8JeV6ikpYU5"
#define SECRET      "TAreTCkEdswAtxJgnDrJjjlmn"
#define ALIAS       "esp8266-2"
#define MYTOPIC     "/Topictest"
#define LED_WIFI    2

#define D4 2   // TXD1
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

int timer = 0;
int photo_diode = A0;   
int inputVal = 0;
float Humidity;
float Temp;

WiFiClient client;
MicroGear microgear(client);
DHT dht(DHTPIN, DHTTYPE);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    digitalWrite(LED_WIFI,LOW);
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    // Serial.println((char *)msg);
    
    char strState[msglen];
    for (int i = 0; i < msglen; i++) 
    {
        strState[i] = (char)msg[i];
        Serial.print((char)msg[i]);
    }
    
    Serial.println();
    
    String stateStr = String(strState).substring(0, msglen);
    digitalWrite(LED_WIFI,HIGH);

    if (stateStr == "RELAY_01_ON") 
    {
        digitalWrite(D7,HIGH);
        microgear.publish("/Topictest", "STAT_RELAY_01_ON");
    } 
    else if (stateStr == "RELAY_01_OFF") 
    {
        digitalWrite(D7,LOW);
        microgear.publish("/Topictest", "STAT_RELAY_01_OFF");
    }
   
  //=========== ช่วงประมวลผลคำสั่ง  =============
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}


void setup() {
    /* Setup some thing*/
    pinMode(LED_WIFI,OUTPUT);
    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(D7, OUTPUT);

    digitalWrite(LED_WIFI,HIGH);
    digitalWrite(LED_BUILTIN,HIGH);
    
    dht.begin();  // เริ่มต้นเรียกใช้งาน Library DHT11

    microgear.on(MESSAGE,onMsghandler); //Call onMsghandler() when new message arraives
    microgear.on(PRESENT,onFoundgear); //Call onFoundgear() when new gear appear
    microgear.on(ABSENT,onLostgear); //Call onLostgear() when some gear goes offline
    microgear.on(CONNECTED,onConnected); //Call onConnected() when NETPIE connection is established

    Serial.begin(115200);
    Serial.println("Starting...");

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS); //Initial with KEY, SECRET and also set the ALIAS here
    microgear.connect(APPID); //connect to NETPIE to a specific APPID

    microgear.subscribe("/Topictest"); //Subcribe some topic
}

void loop() {
    // inputVal = analogRead(photo_diode); // get infared value to variable
    inputVal = digitalRead(D1);

    Humidity = dht.readHumidity(); // Read Humidity
    Temp = dht.readTemperature(); // Read temperature as Celsius (the default)

    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        microgear.loop();

        if(inputVal == 0) {
            microgear.publish("/Topictest/INFARED_01","ACTIVE");
            delay(100);
        }
        else {
            microgear.publish("/Topictest/INFARED_01","DEACTIVE");
            delay(100);
        }

        if (timer >= 2000) {
            digitalWrite(LED_WIFI,LOW);
            //microgear.chat(ALIAS,"Hello");
            timer = 0;

             // Check if any reads failed and exit early (to try again).
            if (isnan(Humidity) || isnan(Temp)) {
                Serial.println("Failed to read from DHT sensor!");  // บอกสถานะหากเกิดการผิดพลาดในการอ่านข้อมูล สาเหตุอาจเกิดได้จากสัญญาณรบกวน หรือการเชื่อมต่อที่ไม่สมบรูณ์
                return;
            }
            else {
                microgear.publish("/Topictest/dthTemp",Temp);
                delay(100);
                microgear.publish("/Topictest/dthHum",Humidity);
                delay(100);
            }
            digitalWrite(LED_WIFI,HIGH);
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
