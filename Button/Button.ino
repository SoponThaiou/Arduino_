#define D1 4
#define button D1 // switch input Active Low
#define pressed LOW

void setup()
{
    Serial.begin(9600);
    pinMode(button, INPUT_PULLUP);
}

void loop()
{
    bool ReadSwitch = digitalRead(button);
    if (ReadSwitch == pressed)
    {
        Serial.println("Pressed Switch.");
        delay(500);
    }
}