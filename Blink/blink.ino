//#define D0 16

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    //pinMode(D0, OUTPUT);
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1500);
}   