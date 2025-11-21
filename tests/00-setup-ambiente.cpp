// Cosa fare:

// ✅ Installare Arduino IDE 2.x
// ✅ Aggiungere supporto ESP32
// ✅ Installare tutte le librerie
// ✅ Testare ESP32 base

// Test ESP32 Base - Verifica GPIO e Serial
// Caricare questo sketch per verificare che ESP32 funzioni

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Info sistema
  Serial.println("\n=== ESP32 Test ===");
  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMhz());
  Serial.println(" MHz");
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");
  
  // Test LED onboard (GPIO2)
  pinMode(2, OUTPUT);
  Serial.println("LED onboard test...");
  
  // Test GPIO che useremo
  pinMode(13, INPUT_PULLUP); // Futuro PLAY button
  pinMode(27, INPUT_PULLUP); // Futuro NEXT button
  pinMode(26, INPUT_PULLUP); // Futuro PREV button
  pinMode(25, OUTPUT);       // Futuro LED Rosso
  pinMode(33, OUTPUT);       // Futuro LED Verde
  
  Serial.println("GPIO configurati!");
  Serial.println("Sistema pronto!\n");
}

void loop() {
  // Blink LED onboard
  digitalWrite(2, HIGH);
  digitalWrite(25, HIGH); // LED Rosso (se collegato)
  Serial.println("LED ON");
  delay(500);
  
  digitalWrite(2, LOW);
  digitalWrite(25, LOW);
  digitalWrite(33, HIGH); // LED Verde (se collegato)
  Serial.println("LED OFF");
  delay(500);
  digitalWrite(33, LOW);
  
  // Test lettura pulsanti (se collegati)
  Serial.print("BTN_PLAY: ");
  Serial.print(digitalRead(13));
  Serial.print(" | BTN_NEXT: ");
  Serial.print(digitalRead(27));
  Serial.print(" | BTN_PREV: ");
  Serial.println(digitalRead(26));
  
  delay(1000);
}
