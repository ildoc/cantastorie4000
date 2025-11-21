// Cosa fare:

// Unire tutte le funzionalità
// Sistema completamente funzionale via comandi
// Pronto per integrare hardware reale

// Sistema Completo Simulato - Tutto funzionante senza hardware
// Comandi: P=Play, N=Next, V=Prev, T1-T5=Tag, +=Vol Up, -=Vol Down
// Bluetooth: Tutti i comandi del protocollo

#include <BluetoothSerial.h>
#include <Preferences.h>

BluetoothSerial SerialBT;
Preferences preferences;

// GPIO Pins (già configurati per hardware futuro)
#define BTN_PLAY 13
#define BTN_NEXT 27
#define BTN_PREV 26
#define LED_RED 25
#define LED_GREEN 33

// Stato sistema
bool isPlaying = false;
int currentTrack = 1;
int currentVolume = 15;
int currentFolder = 0;
String lastTagUID = "";
unsigned long lastTagTime = 0;
const int TAG_DEBOUNCE = 2000;

// Tag simulati
const String tagUIDs[5] = {
  "04A1B2C3", "05C3D4E5", "06E5F6A7", "07A8B9C0", "08D1E2F3"
};

// LED di stato
unsigned long lastLedBlink = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   LETTORE MUSICALE NFC - SIMULATORE   ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  // Init GPIO (pronti per hardware)
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  // Test LED
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  delay(300);
  digitalWrite(LED_RED, LOW);
  
  // Init Bluetooth
  if (!SerialBT.begin("LettoreMusicale")) {
    Serial.println("❌ ERRORE: Bluetooth non disponibile!");
    while(1) delay(1000);
  }
  Serial.println("✅ Bluetooth: LettoreMusicale");
  
  // Init Preferences
  preferences.begin("music-player", false);
  currentVolume = preferences.getInt("volume", 15);
  
  // Sistema pronto
  digitalWrite(LED_GREEN, HIGH); // LED verde fisso = pronto
  Serial.println("✅ Sistema pronto!");
  printHelp();
}

void loop() {
  // Pulsanti fisici (se collegati)
  checkPhysicalButtons();
  
  // Comandi Serial
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    handleSerialCommand(cmd);
  }
  
  // Comandi Bluetooth
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();
    handleBluetoothCommand(cmd);
  }
  
  // LED status blinking (se in riproduzione)
  if (isPlaying) {
    if (millis() - lastLedBlink > 1000) {
      ledState = !ledState;
      digitalWrite(LED_GREEN, ledState);
      lastLedBlink = millis();
    }
  }
  
  delay(10);
}

void checkPhysicalButtons() {
  static bool lastPlay = HIGH, lastNext = HIGH, lastPrev = HIGH;
  
  bool play = digitalRead(BTN_PLAY);
  if (play == LOW && lastPlay == HIGH) {
    delay(50); // Debounce
    handlePlayPause();
  }
  lastPlay = play;
  
  bool next = digitalRead(BTN_NEXT);
  if (next == LOW && lastNext == HIGH) {
    delay(50);
    handleNext();
  }
  lastNext = next;
  
  bool prev = digitalRead(BTN_PREV);
  if (prev == LOW && lastPrev == HIGH) {
    delay(50);
    handlePrev();
  }
  lastPrev = prev;
}

void handleSerialCommand(String cmd) {
  Serial.print(">>> ");
  Serial.println(cmd);
  
  if (cmd == "P" || cmd == "p") {
    handlePlayPause();
  }
  else if (cmd == "N" || cmd == "n") {
    handleNext();
  }
  else if (cmd == "V" || cmd == "v") {
    handlePrev();
  }
  else if (cmd == "+") {
    handleVolumeUp();
  }
  else if (cmd == "-") {
    handleVolumeDown();
  }
  else if (cmd.startsWith("T") && cmd.length() == 2) {
    int tagNum = cmd[1] - '0';
    if (tagNum >= 1 && tagNum <= 5) {
      simulateTagRead(tagNum - 1);
    }
  }
  else if (cmd.startsWith("A")) {
    handleAssociationSerial(cmd);
  }
  else if (cmd == "S" || cmd == "s") {
    printStatus();
  }
  else if (cmd == "L" || cmd == "l") {
    listAssociations();
  }
  else if (cmd == "H" || cmd == "h" || cmd == "?") {
    printHelp();
  }
  else {
    Serial.println("❌ Comando non riconosciuto. Usa H per help.");
  }
}

void handleBluetoothCommand(String cmd) {
  Serial.print("[BT] ");
  Serial.println(cmd);
  
  if (cmd == "PLAY") {
    isPlaying = true;
    SerialBT.println("OK:PLAYING");
    Serial.println("▶️ Play");
  }
  else if (cmd == "PAUSE") {
    isPlaying = false;
    digitalWrite(LED_GREEN, HIGH);
    SerialBT.println("OK:PAUSED");
    Serial.println("⏸️ Pausa");
  }
  else if (cmd == "NEXT") {
    handleNext();
    SerialBT.println("OK:NEXT");
  }
  else if (cmd == "PREV") {
    handlePrev();
    SerialBT.println("OK:PREV");
  }
  else if (cmd.startsWith("VOL:")) {
    int vol = cmd.substring(4).toInt();
    if (vol >= 0 && vol <= 25) {
      currentVolume = vol;
      preferences.putInt("volume", vol);
      SerialBT.print("OK:VOLUME:");
      SerialBT.println(vol);
      Serial.print("🔊 Volume: ");
      Serial.println(vol);
    } else {
      SerialBT.println("ERROR:VOLUME_OUT_OF_RANGE");
    }
  }
  else if (cmd.startsWith("FOLDER:")) {
    int folder = cmd.substring(7).toInt();
    if (folder >= 1 && folder <= 99) {
      currentFolder = folder;
      SerialBT.print("OK:FOLDER:");
      SerialBT.println(folder);
      Serial.print("📁 Cartella: ");
      Serial.println(folder);
    } else {
      SerialBT.println("ERROR:FOLDER_OUT_OF_RANGE");
    }
  }
  else if (cmd.startsWith("ASSOCIATE:")) {
    int sep = cmd.indexOf(':', 11);
    if (sep != -1) {
      String uid = cmd.substring(11, sep);
      int folder = cmd.substring(sep + 1).toInt();
      
      String key = "tag_" + uid;
      preferences.putInt(key.c_str(), folder);
      
      SerialBT.print("OK:ASSOCIATED:");
      SerialBT.print(uid);
      SerialBT.print(":");
      SerialBT.println(folder);
      
      Serial.print("🏷️ Associato ");
      Serial.print(uid);
      Serial.print(" → ");
      Serial.println(folder);
    } else {
      SerialBT.println("ERROR:INVALID_FORMAT");
    }
  }
  else if (cmd == "STATUS") {
    SerialBT.print("STATUS:Volume:");
    SerialBT.print(currentVolume);
    SerialBT.print(",Playing:");
    SerialBT.print(isPlaying ? "1" : "0");
    SerialBT.print(",Folder:");
    SerialBT.print(currentFolder);
    SerialBT.print(",Battery:");
    SerialBT.println(85); // Simulato
  }
  else if (cmd == "BATTERY") {
    SerialBT.println("BATTERY:3.8V,85%");
  }
  else {
    SerialBT.println("ERROR:UNKNOWN_COMMAND");
  }
}

void handlePlayPause() {
  isPlaying = !isPlaying;
  if (isPlaying) {
    Serial.println("▶️ Play");
  } else {
    Serial.println("⏸️ Pausa");
    digitalWrite(LED_GREEN, HIGH);
  }
}

void handleNext() {
  currentTrack++;
  Serial.print("⏭️ Next → Traccia ");
  Serial.println(currentTrack);
}

void handlePrev() {
  currentTrack--;
  if (currentTrack < 1) currentTrack = 1;
  Serial.print("⏮️ Prev → Traccia ");
  Serial.println(currentTrack);
}

void handleVolumeUp() {
  if (currentVolume < 25) {
    currentVolume++;
    preferences.putInt("volume", currentVolume);
    Serial.print("🔊 Volume: ");
    Serial.println(currentVolume);
  }
}

void handleVolumeDown() {
  if (currentVolume > 0) {
    currentVolume--;
    preferences.putInt("volume", currentVolume);
    Serial.print("🔉 Volume: ");
    Serial.println(currentVolume);
  }
}

void simulateTagRead(int tagIndex) {
  String uid = tagUIDs[tagIndex];
  
  // Debounce
  if (uid == lastTagUID && (millis() - lastTagTime) < TAG_DEBOUNCE) {
    Serial.println("⏸️ Tag già letto (debounce)");
    return;
  }
  
  lastTagUID = uid;
  lastTagTime = millis();
  
  // LED verde lampeggiante rapido
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_GREEN, HIGH);
    delay(100);
    digitalWrite(LED_GREEN, LOW);
    delay(100);
  }
  
  Serial.print("\n📱 Tag ");
  Serial.print(tagIndex + 1);
  Serial.print(": ");
  Serial.println(uid);
  
  // Cerca associazione
  String key = "tag_" + uid;
  int folder = preferences.getInt(key.c_str(), 0);
  
  if (folder > 0) {
    currentFolder = folder;
    Serial.print("✅ Cartella ");
    Serial.println(folder);
    Serial.println("▶️ Riproduzione avviata");
    isPlaying = true;
    
    // Notifica BT
    SerialBT.print("TAG_DETECTED:");
    SerialBT.println(uid);
  } else {
    Serial.println("❌ Tag non associato!");
    
    // LED rosso lampeggiante
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_RED, HIGH);
      delay(150);
      digitalWrite(LED_RED, LOW);
      delay(150);
    }
    
    SerialBT.print("TAG_NOT_ASSOCIATED:");
    SerialBT.println(uid);
  }
  
  digitalWrite(LED_GREEN, HIGH);
}

void handleAssociationSerial(String cmd) {
  int colonPos = cmd.indexOf(':');
  if (colonPos == -1) {
    Serial.println("❌ Formato: A<tag>:<folder> (es: A1:2)");
    return;
  }
  
  int tagNum = cmd.substring(1, colonPos).toInt();
  int folder = cmd.substring(colonPos + 1).toInt();
  
  if (tagNum < 1 || tagNum > 5) {
    Serial.println("❌ Tag deve essere 1-5");
    return;
  }
  
  String uid = tagUIDs[tagNum - 1];
  String key = "tag_" + uid;
  preferences.putInt(key.c_str(), folder);
  
  Serial.print("✅ Tag ");
  Serial.print(tagNum);
  Serial.print(" → Cartella ");
  Serial.println(folder);
}

void listAssociations() {
  Serial.println("\n═══ ASSOCIAZIONI ═══");
  for (int i = 0; i < 5; i++) {
    String key = "tag_" + tagUIDs[i];
    int folder = preferences.getInt(key.c_str(), 0);
    
    Serial.print("Tag ");
    Serial.print(i + 1);
    Serial.print(": ");
    if (folder > 0) {
      Serial.print("→ Cartella ");
      Serial.println(folder);
    } else {
      Serial.println("(non associato)");
    }
  }
  Serial.println("═══════════════════\n");
}

void printStatus() {
  Serial.println("\n╔═════ STATUS ══════╗");
  Serial.print("║ Stato: ");
  Serial.println(isPlaying ? "▶️ Play    ║" : "⏸️ Pausa   ║");
  Serial.print("║ Traccia: ");
  Serial.print(currentTrack);
  Serial.println("       ║");
  Serial.print("║ Volume: ");
  Serial.print(currentVolume);
  Serial.println("/25      ║");
  Serial.print("║ Cartella: ");
  if (currentFolder > 0) {
    Serial.print(currentFolder);
    Serial.println("      ║");
  } else {
    Serial.println("--      ║");
  }
  Serial.println("╚═══════════════════╝\n");
}

void printHelp() {
  Serial.println("\n╔═══════ COMANDI SERIAL ════════╗");
  Serial.println("║ P = Play/Pausa                ║");
  Serial.println("║ N = Next                      ║");
  Serial.println("║ V = Prev                      ║");
  Serial.println("║ + = Volume Up                 ║");
  Serial.println("║ - = Volume Down               ║");
  Serial.println("║ T1-T5 = Simula tag            ║");
  Serial.println("║ A<tag>:<folder> = Associa     ║");
  Serial.println("║ L = Lista associazioni        ║");
  Serial.println("║ S = Status                    ║");
  Serial.println("║ H = Help                      ║");
  Serial.println("╚═══════════════════════════════╝\n");
}
