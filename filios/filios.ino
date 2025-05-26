#include <FS.h>
#include <SPIFFS.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Trying to read..");
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  File file = SPIFFS.open("/hell_o.txt", "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }

  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void loop() {}
