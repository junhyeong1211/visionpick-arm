#include <Wire.h>
#include "HUSKYLENS.h"

HUSKYLENS camera;

int targetID = 1;  // 빨강 선택

void setup() {
    Serial.begin(115200);
    Wire.begin();

    while (!camera.begin(Wire)) {
        Serial.println("Connection Failed!");
        delay(1000);
    }

    Serial.println("HUSKYLENS Connected!");
}

void loop() {
    if (!camera.request()) {
        Serial.println("Request Failed!");
        delay(500);
        return;
    }

    if (!camera.isLearned()) {
        Serial.println("No learned object!");
    }
    else if (camera.available() == 0) {
        Serial.println("No object detected!");
    }
    else {
        while (camera.available()) {
            HUSKYLENSResult result = camera.read();

            if (result.ID == targetID) {
                Serial.print("ID: ");
                Serial.println(result.ID);

                Serial.print("X: ");
                Serial.println(result.xCenter);

                Serial.print("Y: ");
                Serial.println(result.yCenter);

                Serial.println("----------");
            }
        }
    }

    delay(300);
}
