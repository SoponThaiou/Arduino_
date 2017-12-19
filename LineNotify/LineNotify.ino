void Line_Notify(String message);

#include <ESP8266WiFi.h>

// Config connect WiFi
#define WIFI_SSID "IOT"
#define WIFI_PASSWORD "00000000"

// Line config
#define LINE_TOKEN "j4LX7BjjnQdKVotkmzvw3Se10wrkrjSeszJUJ9iokLk"

// #define SW D2
#define D2 4 
#define button D2     // switch input Active Low
#define pressed LOW

String message = "%E0%B9%83%E0%B8%84%E0%B8%A3%E0%B8%81%E0%B8%94%E0%B8%9B%E0%B8%B8%E0%B9%88%E0%B8%A1%E0%B8%81%E0%B9%87%E0%B9%84%E0%B8%A1%E0%B9%88%E0%B8%A3%E0%B8%B9%E0%B9%89";

void setup()
{
    pinMode(button, INPUT_PULLUP);

    Serial.begin(9600);

    WiFi.mode(WIFI_STA);
    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (digitalRead(button) == HIGH)
    {
        while (digitalRead(button) == HIGH)
            delay(10);

        Serial.println("Enter !");
        Line_Notify(message);
        // Serial.println();
    }
    delay(10);
}

void Line_Notify(String message)
{
    WiFiClientSecure client;

    if (!client.connect("notify-api.line.me", 443))
    {
        Serial.println("connection failed");
        return;
    }

    String req = "";
    req += "POST /api/notify HTTP/1.1\r\n";
    req += "Host: notify-api.line.me\r\n";
    req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
    req += "Cache-Control: no-cache\r\n";
    req += "User-Agent: ESP8266\r\n";
    req += "Content-Type: application/x-www-form-urlencoded\r\n";
    req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
    req += "\r\n";
    req += "message=" + message;
    // Serial.println(req);
    client.print(req);

    delay(20);

    // Serial.println("-------------");
    while (client.connected())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
            break;
        }
        //Serial.println(line);
    }
    // Serial.println("-------------");
}