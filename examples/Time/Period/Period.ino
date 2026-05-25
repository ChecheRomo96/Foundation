#include <Foundation.h>

static Foundation::Time::Clock::TimePoint ArduinoNow() {
    return millis();
}

Foundation::Time::Clock Clock(ArduinoNow, 1000);

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println(Clock.Now());
    delay(1000);
}
