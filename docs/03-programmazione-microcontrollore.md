# 💻 Guida Completa Programmazione Microcontrollore - ESP32

## 📋 INDICE

1. [Panoramica Software](#-panoramica-software)
2. [Setup Ambiente di Sviluppo](#-setup-ambiente-di-sviluppo)
3. [Architettura del Codice](#-architettura-del-codice)
4. [Inizializzazione e Setup](#-inizializzazione-e-setup)
5. [Gestione DFPlayer Mini](#-gestione-dfplayer-mini)
6. [Gestione NFC (PN532)](#-gestione-nfc-pn532)
7. [Gestione Pulsanti](#-gestione-pulsanti)
8. [Gestione Volume](#-gestione-volume)
9. [Monitoraggio Batteria](#-monitoraggio-batteria)
10. [Comunicazione Bluetooth](#-comunicazione-bluetooth)
11. [Persistenza Dati (Preferences)](#-persistenza-dati-preferences)
12. [Gestione Errori e Debug](#-gestione-errori-e-debug)
13. [Ottimizzazioni e Performance](#-ottimizzazioni-e-performance)
14. [Codice Completo Commentato](#-codice-completo-commentato)

---

## 🎯 PANORAMICA SOFTWARE

### Architettura del Sistema

Il firmware ESP32 gestisce:
- **Riproduzione audio**: Comunicazione seriale con DFPlayer Mini
- **Lettura NFC**: Comunicazione I2C con PN532
- **Interfaccia utente**: Pulsanti, potenziometro, LED
- **Comunicazione remota**: Bluetooth Serial per app
- **Persistenza**: Salvataggio associazioni tag NFC e preferenze

### Flusso Principale

```
setup() → Inizializza tutti i moduli
    ↓
loop() → Esegue continuamente:
    ├── checkButtons()      → Legge pulsanti
    ├── checkVolume()       → Legge potenziometro
    ├── checkBattery()      → Monitora batteria
    ├── checkNFC()          → Cerca tag NFC
    └── checkBluetooth()    → Gestisce comandi BT
```

### Dipendenze e Librerie

- `Arduino.h`: Core Arduino per ESP32
- `DFRobotDFPlayerMini.h`: Gestione DFPlayer
- `Wire.h`: Comunicazione I2C
- `Adafruit_PN532.h`: Gestione PN532
- `BluetoothSerial.h`: Bluetooth Serial ESP32
- `Preferences.h`: Memoria non volatile (EEPROM)

---

## 🔧 SETUP AMBIENTE DI SVILUPPO

### Installazione Arduino IDE

1. **Scaricare Arduino IDE 2.x**
   - https://www.arduino.cc/en/software
   - Versione consigliata: 2.2.1 o superiore

2. **Aggiungere Supporto ESP32**
   - File → Preferenze
   - URL Gestori schede aggiuntive:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Strumenti → Scheda → Gestori schede
   - Cercare "ESP32" e installare

3. **Configurare Scheda**
   - Strumenti → Scheda → ESP32 Arduino → ESP32 Dev Module
   - Upload Speed: 115200
   - CPU Frequency: 240MHz
   - Flash Frequency: 80MHz
   - Flash Size: 4MB
   - Partition Scheme: Default 4MB with spiffs
   - Core Debug Level: Info

### Installazione Librerie

**Metodo 1: Gestore Librerie Arduino**
- Sketch → Includi libreria → Gestione librerie
- Cercare e installare:
  - `DFRobotDFPlayerMini` (v1.0.6+)
  - `Adafruit PN532` (v1.4.0+)
  - `ArduinoJson` (v6.21.0+)

**Metodo 2: Manuale (GitHub)**
```bash
# Cartella librerie: Documents/Arduino/libraries/
git clone https://github.com/DFRobot/DFRobotDFPlayerMini.git
git clone https://github.com/adafruit/Adafruit-PN532.git
```

### Verifica Installazione

**Sketch Test**:
```cpp
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <BluetoothSerial.h>
#include <Preferences.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Tutte le librerie caricate correttamente!");
}

void loop() {}
```

Se compila senza errori, tutto è configurato correttamente.

---

## 🏗️ ARCHITETTURA DEL CODICE

### Struttura Modulare

```
main.ino
├── Definizioni Pin
├── Oggetti Globali
├── Variabili Globali
├── setup()
├── loop()
│
├── checkButtons()          → Gestione pulsanti
├── checkVolume()           → Gestione volume
├── checkBattery()          → Monitoraggio batteria
├── checkNFC()              → Lettura tag NFC
├── checkBluetooth()        → Comandi Bluetooth
│
├── handleNFCTag()          → Elabora tag NFC
├── handleBluetoothCommand() → Elabora comandi BT
├── readBatteryVoltage()    → Legge tensione batteria
└── getBatteryPercentage()  → Converte in percentuale
```

### Convenzioni di Codice

- **Nomi variabili**: camelCase (`currentVolume`, `isPlaying`)
- **Nomi costanti**: UPPER_SNAKE_CASE (`MAX_SAFE_VOLUME`, `TAG_DEBOUNCE`)
- **Nomi funzioni**: camelCase (`checkButtons()`, `handleNFCTag()`)
- **Commenti**: Italiano per spiegazioni, inglese per codice

---

## ⚙️ INIZIALIZZAZIONE E SETUP

### Definizioni Pin

```cpp
// Pin Definitions
#define DFPLAYER_RX 16      // HardwareSerial RX
#define DFPLAYER_TX 17      // HardwareSerial TX
#define PN532_SDA 21        // I2C Data
#define PN532_SCL 22        // I2C Clock
#define BTN_PLAY 13         // Pulsante Play/Pausa
#define BTN_NEXT 27         // Pulsante Next
#define BTN_PREV 26         // Pulsante Previous
#define VOLUME_POT 34       // Potenziometro Volume (ADC)
#define BATTERY_PIN 35      // Monitoraggio Batteria (ADC)
#define LED_RED 25          // LED Rosso (stato/errore)
#define LED_GREEN 33        // LED Verde (stato/OK)
```

### Oggetti Globali

```cpp
// Hardware Serial per DFPlayer (Serial1)
HardwareSerial DFSerial(1);

// Oggetti moduli
DFRobotDFPlayerMini dfPlayer;
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);
BluetoothSerial SerialBT;
Preferences preferences;
```

### Variabili Globali

```cpp
// Stato sistema
int currentVolume = 15;              // Volume corrente (0-25)
bool isPlaying = false;              // Stato riproduzione
String lastTagUID = "";              // Ultimo tag NFC letto
unsigned long lastTagTime = 0;       // Timestamp ultimo tag

// Costanti
const int TAG_DEBOUNCE = 2000;       // Debounce tag NFC (ms)
const int MAX_SAFE_VOLUME = 25;      // Volume massimo sicuro

// Costanti monitoraggio batteria
const float VOLTAGE_DIVIDER = 3.13;  // Rapporto partitore (100K+47K)/47K = 3.128 (approssimato)
const float ADC_REFERENCE = 3.3;     // Tensione riferimento ADC
const float ADC_MAX = 4095.0;        // Valore massimo ADC 12-bit

// Stati LED
enum LEDStatus {
  LED_STATUS_OFF,
  LED_STATUS_READY,
  LED_STATUS_WAITING,
  LED_STATUS_NFC_SUCCESS,
  LED_STATUS_NFC_ERROR,
  LED_STATUS_BATTERY_LOW,
  LED_STATUS_BATTERY_CRIT,
  LED_STATUS_ERROR
};

LEDStatus currentLEDStatus = LED_STATUS_OFF;
unsigned long lastLEDUpdate = 0;
bool ledBlinkState = false;
```

### Funzione setup()

```cpp
void setup() {
  // Inizializzazione Serial Monitor
  Serial.begin(115200);
  delay(1000);  // Attesa stabilizzazione
  Serial.println("\n\n=== Lettore Musicale NFC ===");
  Serial.println("Avvio sistema...");
  
  // Inizializzazione pin GPIO
  initGPIO();
  
  // Inizializzazione DFPlayer
  initDFPlayer();
  
  // Inizializzazione NFC
  initNFC();
  
  // Inizializzazione Bluetooth
  initBluetooth();
  
  // Caricamento preferenze
  loadPreferences();
  
  // Test LED
  testLED();
  
  // Segnala boot completato
  setLEDStatus(LED_STATUS_READY);
  
  Serial.println("Sistema pronto!");
  Serial.println("In attesa di comandi...");
}

void initGPIO() {
  // Pulsanti con pull-up interno
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  
  // ADC (solo input, no pull-up)
  pinMode(VOLUME_POT, INPUT);
  pinMode(BATTERY_PIN, INPUT);
  
  // LED output
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  // Stato iniziale LED
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void initDFPlayer() {
  Serial.print("Inizializzazione DFPlayer... ");
  
  // Inizializzazione Serial1 per DFPlayer
  DFSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  delay(1000);  // Attesa stabilizzazione DFPlayer
  
  // Inizializzazione DFPlayer
  if (!dfPlayer.begin(DFSerial)) {
    Serial.println("ERRORE!");
    Serial.println("Verificare:");
    Serial.println("  - Collegamento TX/RX");
    Serial.println("  - Resistenza 1KΩ su TX");
    Serial.println("  - Alimentazione 5V");
    Serial.println("  - MicroSD inserita");
    while(true) {
      delay(1000);  // Blocca esecuzione
    }
  }
  
  Serial.println("OK");
  
  // Configurazione iniziale
  dfPlayer.volume(currentVolume);
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  dfPlayer.outputDevice(DFPLAYER_DEVICE_SD);  // Riproduci da SD
  
  Serial.print("  Volume: ");
  Serial.println(currentVolume);
}

void initNFC() {
  Serial.print("Inizializzazione PN532... ");
  
  // Inizializzazione I2C
  Wire.begin();
  delay(100);
  
  // Inizializzazione PN532
  nfc.begin();
  
  // Verifica firmware
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("ERRORE!");
    Serial.println("Verificare:");
    Serial.println("  - Collegamento I2C (SDA/SCL)");
    Serial.println("  - Alimentazione 3.3V (NON 5V!)");
    Serial.println("  - Jumper I2C (CH1=OFF, CH2=ON)");
    return;  // Continua senza NFC
  }
  
  Serial.print("OK (Firmware: 0x");
  Serial.print(versiondata, HEX);
  Serial.println(")");
  
  // Configurazione SAM (Secure Access Module)
  nfc.SAMConfig();
  
  Serial.println("  Pronto per leggere tag NFC");
}

void initBluetooth() {
  Serial.print("Inizializzazione Bluetooth... ");
  
  // Inizializzazione Bluetooth Serial
  if (!SerialBT.begin("LettoreMusicale")) {
    Serial.println("ERRORE!");
    return;
  }
  
  Serial.println("OK");
  Serial.print("  Nome dispositivo: LettoreMusicale");
  Serial.print("  MAC: ");
  Serial.println(SerialBT.getBtAddressString());
}

void loadPreferences() {
  Serial.print("Caricamento preferenze... ");
  
  preferences.begin("music-player", false);  // false = lettura/scrittura
  
  // Carica volume salvato
  currentVolume = preferences.getInt("volume", 15);  // Default: 15
  dfPlayer.volume(currentVolume);
  
  Serial.println("OK");
  Serial.print("  Volume salvato: ");
  Serial.println(currentVolume);
  
  // Nota: Le associazioni tag NFC vengono caricate on-demand
}

void testLED() {
  Serial.println("Test LED...");
  
  // Sequenza test: Verde → Rosso → Spento
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  delay(300);
  
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  delay(300);
  
  // Test opzionale: Giallo (solo se il LED supporta entrambi accesi)
  // Nota: Non tutti i LED bicolore mostrano giallo quando entrambi sono accesi
  // digitalWrite(LED_GREEN, HIGH);
  // digitalWrite(LED_RED, HIGH);
  // delay(300);
  
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  
  Serial.println("  LED OK");
}
```

---

## 🎵 GESTIONE DFPLAYER MINI

### Comunicazione Serial

Il DFPlayer comunica via UART a 9600 baud:
- **TX ESP32 → RX DFPlayer**: Comandi (con resistenza 1KΩ)
- **TX DFPlayer → RX ESP32**: Risposte e stato

### Comandi Principali

```cpp
// Riproduzione
dfPlayer.play(1);              // Riproduce file 0001.mp3
dfPlayer.playFolder(1, 1);     // Riproduce cartella 01, file 001.mp3
dfPlayer.loopFolder(1);        // Loop continuo cartella 01
dfPlayer.start();              // Avvia/riprende
dfPlayer.pause();              // Pausa
dfPlayer.stop();               // Ferma

// Navigazione
dfPlayer.next();               // Traccia successiva
dfPlayer.previous();           // Traccia precedente

// Volume
dfPlayer.volume(15);           // Imposta volume (0-30, consigliato 0-25)
dfPlayer.volumeUp();           // Aumenta volume
dfPlayer.volumeDown();         // Diminuisce volume

// Equalizzatore
dfPlayer.EQ(DFPLAYER_EQ_NORMAL);    // Normale
dfPlayer.EQ(DFPLAYER_EQ_POP);       // Pop
dfPlayer.EQ(DFPLAYER_EQ_ROCK);      // Rock
dfPlayer.EQ(DFPLAYER_EQ_JAZZ);      // Jazz
dfPlayer.EQ(DFPLAYER_EQ_CLASSIC);   // Classica
dfPlayer.EQ(DFPLAYER_EQ_BASS);      // Basso

// Dispositivo output
dfPlayer.outputDevice(DFPLAYER_DEVICE_SD);      // SD card
dfPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);  // USB (se presente)
```

### Gestione Errori

```cpp
void playFolderSafely(int folder) {
  if (folder < 1 || folder > 99) {
    Serial.print("Cartella non valida: ");
    Serial.println(folder);
    return;
  }
  
  if (!dfPlayer.loopFolder(folder)) {
    Serial.print("Errore riproduzione cartella: ");
    Serial.println(folder);
    // Riprova dopo delay
    delay(100);
    dfPlayer.loopFolder(folder);
  } else {
    Serial.print("Riproduzione cartella: ");
    Serial.println(folder);
    isPlaying = true;
  }
}
```

### Callback Eventi (Opzionale)

```cpp
void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println("DFPlayer Time Out!");
      break;
    case WrongStack:
      Serial.println("Stack Wrong!");
      break;
    case DFPlayerCardInserted:
      Serial.println("Card Inserted!");
      break;
    case DFPlayerCardRemoved:
      Serial.println("Card Removed!");
      break;
    case DFPlayerCardOnline:
      Serial.println("Card Online!");
      break;
    case DFPlayerPlayFinished:
      Serial.print("Number:");
      Serial.print(value);
      Serial.println(" Play Finished!");
      break;
    case DFPlayerError:
      Serial.print("DFPlayerError: ");
      switch (value) {
        case Busy:
          Serial.println("Card not found");
          break;
        case Sleeping:
          Serial.println("Sleeping");
          break;
        case SerialWrongStack:
          Serial.println("Get Wrong Stack");
          break;
        case CheckSumNotMatch:
          Serial.println("Check Sum Not Match");
          break;
        case FileIndexOut:
          Serial.println("File Index Out of Bound");
          break;
        case FileMismatch:
          Serial.println("Cannot Find File");
          break;
        case Advertise:
          Serial.println("In Advertise");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

// Nel loop(), se disponibile:
if (dfPlayer.available()) {
  printDetail(dfPlayer.readType(), dfPlayer.read());
}
```

---

## 📱 GESTIONE NFC (PN532)

### Lettura Tag NFC

```cpp
void checkNFC() {
  uint8_t uid[7];        // Buffer UID (max 7 byte)
  uint8_t uidLength;     // Lunghezza UID effettiva
  
  // Legge tag passivo ISO14443A (timeout 100ms)
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) {
    // Converte UID in stringa esadecimale
    String tagUID = uidToString(uid, uidLength);
    
    // Debounce: ignora stesso tag per 2 secondi
    if (tagUID != lastTagUID || (millis() - lastTagTime) > TAG_DEBOUNCE) {
      Serial.print("Tag NFC rilevato: ");
      Serial.println(tagUID);
      
      // Elabora tag
      handleNFCTag(tagUID);
      
      // Aggiorna stato
      lastTagUID = tagUID;
      lastTagTime = millis();
    }
  }
}

String uidToString(uint8_t *uid, uint8_t length) {
  String result = "";
  for (uint8_t i = 0; i < length; i++) {
    if (uid[i] < 0x10) result += "0";  // Zero padding
    result += String(uid[i], HEX);
  }
  result.toUpperCase();
  return result;
}
```

### Gestione Tag NFC

```cpp
void handleNFCTag(String uid) {
  // Cerca associazione tag → cartella
  String key = "tag_" + uid;
  int folder = preferences.getInt(key.c_str(), 0);
  
  if (folder > 0 && folder <= 99) {
    // Tag associato: riproduce cartella
    Serial.print("Riproduzione cartella: ");
    Serial.println(folder);
    
    // Feedback LED: verde lampeggiante rapido
    setLEDStatus(LED_STATUS_NFC_SUCCESS);
    
    dfPlayer.loopFolder(folder);
    isPlaying = true;
    
    // Notifica via Bluetooth (se connesso)
    if (SerialBT.hasClient()) {
      SerialBT.print("TAG_DETECTED:");
      SerialBT.println(uid);
    }
  } else {
    // Tag non associato: suono errore
    Serial.println("Tag non associato");
    
    // Feedback LED: rosso lampeggiante rapido
    setLEDStatus(LED_STATUS_NFC_ERROR);
    
    dfPlayer.playMp3Folder(9999);  // File errore (se presente)
    
    // Notifica via Bluetooth
    if (SerialBT.hasClient()) {
      SerialBT.print("TAG_UNKNOWN:");
      SerialBT.println(uid);
    }
  }
}
```

### Scrittura Tag NFC (Opzionale)

```cpp
bool writeTagName(String uid, String name) {
  // Converte UID stringa in array
  uint8_t uidBytes[7];
  int uidLength = hexStringToBytes(uid, uidBytes);
  
  // Autentica tag (se necessario)
  if (!nfc.mifareclassic_AuthenticateBlock(uidBytes, uidLength, 4, 0, keyA)) {
    return false;
  }
  
  // Scrive nome nella memoria NDEF
  // (Implementazione complessa, richiede libreria NDEF)
  return true;
}
```

---

## 🎮 GESTIONE PULSANTI

### Lettura Pulsanti con Debounce

```cpp
void checkButtons() {
  static bool lastPlay = HIGH;   // Stato precedente
  static bool lastNext = HIGH;
  static bool lastPrev = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;  // 50ms debounce
  
  // Legge stato attuale
  bool play = digitalRead(BTN_PLAY);
  bool next = digitalRead(BTN_NEXT);
  bool prev = digitalRead(BTN_PREV);
  
  // Rileva fronte di discesa (HIGH → LOW = premuto)
  if (play == LOW && lastPlay == HIGH) {
    // Debounce
    if (millis() - lastDebounceTime > debounceDelay) {
      handlePlayButton();
      lastDebounceTime = millis();
    }
  }
  lastPlay = play;
  
  if (next == LOW && lastNext == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      handleNextButton();
      lastDebounceTime = millis();
    }
  }
  lastNext = next;
  
  if (prev == LOW && lastPrev == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      handlePrevButton();
      lastDebounceTime = millis();
    }
  }
  lastPrev = prev;
}

void handlePlayButton() {
  Serial.println("Pulsante PLAY premuto");
  
  if (isPlaying) {
    dfPlayer.pause();
    isPlaying = false;
    Serial.println("  Pausa");
  } else {
    dfPlayer.start();
    isPlaying = true;
    Serial.println("  Play");
  }
  
  // Notifica Bluetooth
  if (SerialBT.hasClient()) {
    SerialBT.print("STATUS:Playing:");
    SerialBT.println(isPlaying ? "1" : "0");
  }
}

void handleNextButton() {
  Serial.println("Pulsante NEXT premuto");
  dfPlayer.next();
  delay(100);  // Attesa cambio traccia
  isPlaying = true;
}

void handlePrevButton() {
  Serial.println("Pulsante PREV premuto");
  dfPlayer.previous();
  delay(100);  // Attesa cambio traccia
  isPlaying = true;
}
```

### Gestione Pressione Lunga (Opzionale)

```cpp
void checkButtonsLongPress() {
  static unsigned long pressStartTime = 0;
  const unsigned long longPressTime = 2000;  // 2 secondi
  
  if (digitalRead(BTN_PLAY) == LOW) {
    if (pressStartTime == 0) {
      pressStartTime = millis();
    } else if (millis() - pressStartTime > longPressTime) {
      // Pressione lunga rilevata
      handleLongPress();
      pressStartTime = 0;
    }
  } else {
    pressStartTime = 0;
  }
}

void handleLongPress() {
  Serial.println("Pressione lunga rilevata");
  // Esempio: Reset associazioni tag
  // preferences.clear();
}
```

---

## 🔊 GESTIONE VOLUME

### Lettura Potenziometro

```cpp
void checkVolume() {
  static unsigned long lastCheck = 0;
  static int lastVol = -1;
  const unsigned long checkInterval = 200;  // Controlla ogni 200ms
  
  if (millis() - lastCheck > checkInterval) {
    // Legge ADC (0-4095)
    int raw = analogRead(VOLUME_POT);
    
    // Elimina zone morte (valori estremi instabili)
    raw = constrain(raw, 200, 3895);
    
    // Mappa a volume DFPlayer (0-25)
    int vol = map(raw, 200, 3895, 0, MAX_SAFE_VOLUME);
    
    // Applica solo se cambiamento significativo (>1)
    if (abs(vol - lastVol) > 1) {
      currentVolume = vol;
      dfPlayer.volume(vol);
      
      // Salva preferenza
      preferences.putInt("volume", vol);
      
      Serial.print("Volume: ");
      Serial.print(vol);
      Serial.print("/");
      Serial.println(MAX_SAFE_VOLUME);
      
      // Notifica Bluetooth
      if (SerialBT.hasClient()) {
        SerialBT.print("VOLUME:");
        SerialBT.println(vol);
      }
      
      lastVol = vol;
    }
    
    lastCheck = millis();
  }
}
```

### Calibrazione Volume

```cpp
// Funzione per calibrare potenziometro
void calibrateVolume() {
  Serial.println("Calibrazione volume...");
  Serial.println("Ruotare potenziometro completamente a sinistra e premere un pulsante");
  
  while (digitalRead(BTN_PLAY) == HIGH) {
    delay(10);
  }
  
  int minRaw = analogRead(VOLUME_POT);
  Serial.print("Minimo: ");
  Serial.println(minRaw);
  delay(1000);
  
  Serial.println("Ruotare potenziometro completamente a destra e premere un pulsante");
  
  while (digitalRead(BTN_PLAY) == HIGH) {
    delay(10);
  }
  
  int maxRaw = analogRead(VOLUME_POT);
  Serial.print("Massimo: ");
  Serial.println(maxRaw);
  
  // Salva valori calibrazione
  preferences.putInt("vol_min", minRaw);
  preferences.putInt("vol_max", maxRaw);
  
  Serial.println("Calibrazione completata!");
}
```

---

## 💡 GESTIONE LED DI STATO

### Sistema Stati LED

Il LED bicolore viene usato come indicatore di stato generico per fornire feedback visivo all'utente.

**Stati Disponibili**:
```cpp
enum LEDStatus {
  LED_STATUS_OFF,          // Spento
  LED_STATUS_READY,        // Verde fisso - Sistema pronto
  LED_STATUS_WAITING,      // Verde lampeggiante lento - In attesa
  LED_STATUS_NFC_SUCCESS,  // Verde lampeggiante rapido - Tag NFC letto
  LED_STATUS_NFC_ERROR,    // Rosso lampeggiante - Tag non associato
  LED_STATUS_BATTERY_LOW,  // Rosso lampeggiante - Batteria 10-20%
  LED_STATUS_BATTERY_CRIT, // Rosso fisso - Batteria <10%
  LED_STATUS_ERROR         // Rosso fisso - Errore sistema
};
```

### Funzioni Gestione LED

```cpp
// Variabili globali per gestione LED
LEDStatus currentLEDStatus = LED_STATUS_OFF;
unsigned long lastLEDUpdate = 0;
bool ledBlinkState = false;

void setLEDStatus(LEDStatus status) {
  currentLEDStatus = status;
  updateLED();  // Aggiorna immediatamente
}

void updateLED() {
  unsigned long now = millis();
  
  switch (currentLEDStatus) {
    case LED_STATUS_OFF:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      break;
      
    case LED_STATUS_READY:
      // Verde fisso
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      break;
      
    case LED_STATUS_WAITING:
      // Verde lampeggiante lento (1 secondo)
      if (now - lastLEDUpdate > 1000) {
        ledBlinkState = !ledBlinkState;
        digitalWrite(LED_GREEN, ledBlinkState ? HIGH : LOW);
        digitalWrite(LED_RED, LOW);
        lastLEDUpdate = now;
      }
      break;
      
    case LED_STATUS_NFC_SUCCESS:
      // Verde lampeggiante rapido (200ms) per 2 secondi
      if (now - lastLEDUpdate > 200) {
        ledBlinkState = !ledBlinkState;
        digitalWrite(LED_GREEN, ledBlinkState ? HIGH : LOW);
        digitalWrite(LED_RED, LOW);
        lastLEDUpdate = now;
      }
      // Dopo 2 secondi, torna a READY
      static unsigned long nfcSuccessStart = 0;
      if (nfcSuccessStart == 0) {
        nfcSuccessStart = now;
      } else if (now - nfcSuccessStart > 2000) {
        nfcSuccessStart = 0;
        setLEDStatus(LED_STATUS_READY);
      }
      break;
      
    case LED_STATUS_NFC_ERROR:
      // Rosso lampeggiante rapido (150ms) per 1 secondo
      if (now - lastLEDUpdate > 150) {
        ledBlinkState = !ledBlinkState;
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, ledBlinkState ? HIGH : LOW);
        lastLEDUpdate = now;
      }
      // Dopo 1 secondo, torna a READY
      static unsigned long nfcErrorStart = 0;
      if (nfcErrorStart == 0) {
        nfcErrorStart = now;
      } else if (now - nfcErrorStart > 1000) {
        nfcErrorStart = 0;
        setLEDStatus(LED_STATUS_READY);
      }
      break;
      
    case LED_STATUS_BATTERY_LOW:
      // Rosso lampeggiante lento (500ms)
      if (now - lastLEDUpdate > 500) {
        ledBlinkState = !ledBlinkState;
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, ledBlinkState ? HIGH : LOW);
        lastLEDUpdate = now;
      }
      break;
      
    case LED_STATUS_BATTERY_CRIT:
      // Rosso fisso
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      break;
      
    case LED_STATUS_ERROR:
      // Rosso fisso
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      break;
  }
}

// Chiamare nel loop() per aggiornare LED
void updateLEDStatus() {
  updateLED();
}
```

### Uso negli Eventi

```cpp
// Nel setup(), dopo inizializzazione completa
setLEDStatus(LED_STATUS_READY);

// Quando tag NFC viene letto con successo
void handleNFCTag(String uid) {
  // ... codice esistente ...
  
  if (folder > 0) {
    setLEDStatus(LED_STATUS_NFC_SUCCESS);  // Feedback visivo
    // ... resto del codice ...
  } else {
    setLEDStatus(LED_STATUS_NFC_ERROR);  // Tag non associato
    // ... resto del codice ...
  }
}

// Nel loop(), aggiorna LED continuamente
void loop() {
  // ... altri controlli ...
  updateLEDStatus();  // Aggiorna stato LED
  delay(10);
}
```

---

## 🔋 MONITORAGGIO BATTERIA

### Lettura Tensione Batteria

```cpp
float readBatteryVoltage() {
  // Legge ADC (0-4095)
  int raw = analogRead(BATTERY_PIN);
  
  // Converte a tensione ADC (0-3.3V)
  float adcVoltage = (raw / ADC_MAX) * ADC_REFERENCE;
  
  // Applica partitore (100K + 47K) / 47K = 3.128 (approssimato a 3.13)
  float batteryVoltage = adcVoltage * VOLTAGE_DIVIDER;
  
  return batteryVoltage;
}

int getBatteryPercentage(float voltage) {
  // Batteria Li-ion: 3.0V (0%) - 4.2V (100%)
  if (voltage >= 4.2) return 100;
  if (voltage <= 3.0) return 0;
  
  // Interpolazione lineare
  return (int)((voltage - 3.0) / 1.2 * 100);
}
```

### Gestione LED in Base a Batteria

```cpp
void checkBattery() {
  static unsigned long lastCheck = 0;
  const unsigned long checkInterval = 30000;  // Controlla ogni 30 secondi
  
  if (millis() - lastCheck > checkInterval) {
    float voltage = readBatteryVoltage();
    int percent = getBatteryPercentage(voltage);
    
    Serial.print("Batteria: ");
    Serial.print(voltage, 2);
    Serial.print("V (");
    Serial.print(percent);
    Serial.println("%)");
    
    // Aggiorna LED solo se non ci sono altri stati attivi
    // (es. NFC_SUCCESS ha priorità temporanea)
    if (currentLEDStatus == LED_STATUS_READY || 
        currentLEDStatus == LED_STATUS_WAITING ||
        currentLEDStatus == LED_STATUS_BATTERY_LOW ||
        currentLEDStatus == LED_STATUS_BATTERY_CRIT) {
      
      if (percent > 20) {
        // Batteria OK: LED verde fisso (READY)
        setLEDStatus(LED_STATUS_READY);
      } else if (percent > 10) {
        // Batteria bassa: LED rosso lampeggiante
        setLEDStatus(LED_STATUS_BATTERY_LOW);
      } else {
        // Batteria critica: LED rosso fisso
        setLEDStatus(LED_STATUS_BATTERY_CRIT);
        Serial.println("ATTENZIONE: Batteria molto bassa!");
        // Opzionale: Spegnimento automatico
        // esp_deep_sleep_start();
      }
    }
    
    // Notifica Bluetooth
    if (SerialBT.hasClient()) {
      SerialBT.print("BATTERY:");
      SerialBT.print(voltage, 2);
      SerialBT.print("V,");
      SerialBT.print(percent);
      SerialBT.println("%");
    }
    
    lastCheck = millis();
  }
}
```

### Spegnimento Automatico (Opzionale)

```cpp
void checkBatteryShutdown() {
  float voltage = readBatteryVoltage();
  
  if (voltage < 3.0) {
    Serial.println("Batteria critica: spegnimento...");
    
    // Salva stato
    preferences.putInt("volume", currentVolume);
    preferences.end();
    
    // Spegni LED
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    
    // Deep sleep (consumo ~10µA)
    esp_deep_sleep_start();
  }
}
```

---

## 📡 COMUNICAZIONE BLUETOOTH

### Protocollo Comunicazione

**Comandi App → ESP32**:
- `PLAY` - Avvia riproduzione
- `PAUSE` - Mette in pausa
- `NEXT` - Traccia successiva
- `PREV` - Traccia precedente
- `VOL:15` - Imposta volume (0-25)
- `FOLDER:2` - Riproduce cartella 2
- `ASSOCIATE:04A1B2C3:1` - Associa tag a cartella
- `STATUS` - Richiede stato corrente
- `BATTERY` - Richiede stato batteria

**Risposte ESP32 → App**:
- `OK` - Comando eseguito
- `ERROR:messaggio` - Errore
- `STATUS:Volume:15,Playing:1,Battery:85` - Stato completo
- `TAG_DETECTED:04A1B2C3` - Tag NFC rilevato
- `BATTERY:3.8V,75%` - Stato batteria

### Gestione Comandi

```cpp
void checkBluetooth() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();  // Rimuove spazi e newline
    
    if (cmd.length() > 0) {
      handleBluetoothCommand(cmd);
    }
  }
}

void handleBluetoothCommand(String cmd) {
  Serial.print("BT CMD: ");
  Serial.println(cmd);
  
  // Play/Pause
  if (cmd == "PLAY") {
    dfPlayer.start();
    isPlaying = true;
    SerialBT.println("OK");
  }
  else if (cmd == "PAUSE") {
    dfPlayer.pause();
    isPlaying = false;
    SerialBT.println("OK");
  }
  
  // Navigazione
  else if (cmd == "NEXT") {
    dfPlayer.next();
    isPlaying = true;
    SerialBT.println("OK");
  }
  else if (cmd == "PREV") {
    dfPlayer.previous();
    isPlaying = true;
    SerialBT.println("OK");
  }
  
  // Volume
  else if (cmd.startsWith("VOL:")) {
    int vol = cmd.substring(4).toInt();
    if (vol >= 0 && vol <= MAX_SAFE_VOLUME) {
      currentVolume = vol;
      dfPlayer.volume(vol);
      preferences.putInt("volume", vol);
      SerialBT.println("OK");
    } else {
      SerialBT.println("ERROR:Volume out of range");
    }
  }
  
  // Cartella
  else if (cmd.startsWith("FOLDER:")) {
    int folder = cmd.substring(7).toInt();
    if (folder >= 1 && folder <= 99) {
      dfPlayer.loopFolder(folder);
      isPlaying = true;
      SerialBT.println("OK");
    } else {
      SerialBT.println("ERROR:Folder out of range");
    }
  }
  
  // Associazione tag
  else if (cmd.startsWith("ASSOCIATE:")) {
    // Formato: ASSOCIATE:UID:FOLDER
    int sep1 = cmd.indexOf(':', 10);
    int sep2 = cmd.indexOf(':', sep1 + 1);
    
    if (sep1 > 0 && sep2 > 0) {
      String uid = cmd.substring(10, sep1);
      int folder = cmd.substring(sep2 + 1).toInt();
      
      if (folder >= 1 && folder <= 99) {
        String key = "tag_" + uid;
        preferences.putInt(key.c_str(), folder);
        
        SerialBT.print("Associated ");
        SerialBT.print(uid);
        SerialBT.print(" to folder ");
        SerialBT.println(folder);
      } else {
        SerialBT.println("ERROR:Invalid folder");
      }
    } else {
      SerialBT.println("ERROR:Invalid format");
    }
  }
  
  // Stato
  else if (cmd == "STATUS") {
    float voltage = readBatteryVoltage();
    int battery = getBatteryPercentage(voltage);
    
    SerialBT.print("STATUS:Volume:");
    SerialBT.print(currentVolume);
    SerialBT.print(",Playing:");
    SerialBT.print(isPlaying ? "1" : "0");
    SerialBT.print(",Battery:");
    SerialBT.println(battery);
  }
  
  // Batteria
  else if (cmd == "BATTERY") {
    float voltage = readBatteryVoltage();
    int percent = getBatteryPercentage(voltage);
    
    SerialBT.print("BATTERY:");
    SerialBT.print(voltage, 2);
    SerialBT.print("V,");
    SerialBT.print(percent);
    SerialBT.println("%");
  }
  
  // Comando sconosciuto
  else {
    SerialBT.print("ERROR:Unknown command: ");
    SerialBT.println(cmd);
  }
}
```

---

## 💾 PERSISTENZA DATI (PREFERENCES)

### Uso Preferences

```cpp
// Inizializzazione
preferences.begin("music-player", false);  // false = lettura/scrittura

// Scrittura
preferences.putInt("volume", 15);
preferences.putString("last_tag", "04A1B2C3");

// Lettura
int volume = preferences.getInt("volume", 15);  // Default: 15
String tag = preferences.getString("last_tag", "");

// Rimozione
preferences.remove("volume");

// Cancellazione totale
preferences.clear();

// Chiusura
preferences.end();
```

### Gestione Associazioni Tag

```cpp
// Salva associazione
void saveTagAssociation(String uid, int folder) {
  String key = "tag_" + uid;
  preferences.putInt(key.c_str(), folder);
  Serial.print("Salvato: ");
  Serial.print(uid);
  Serial.print(" → cartella ");
  Serial.println(folder);
}

// Carica associazione
int loadTagAssociation(String uid) {
  String key = "tag_" + uid;
  return preferences.getInt(key.c_str(), 0);  // 0 = non associato
}

// Lista tutte le associazioni (per debug)
void listTagAssociations() {
  Serial.println("Associazioni tag:");
  // Nota: Preferences non supporta iterazione diretta
  // Serve mantenere lista separata o usare chiavi note
}
```

---

## 🐛 GESTIONE ERRORI E DEBUG

### Serial Debug

```cpp
// Macro per debug condizionale
#define DEBUG_MODE true

#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

// Uso
DEBUG_PRINT("Volume: ");
DEBUG_PRINTLN(currentVolume);
```

### Gestione Errori DFPlayer

```cpp
bool safeDFPlayerCommand(void (*command)(), const char* name) {
  int attempts = 3;
  
  for (int i = 0; i < attempts; i++) {
    command();
    delay(100);
    
    if (dfPlayer.available()) {
      uint8_t type = dfPlayer.readType();
      if (type != DFPlayerError) {
        return true;  // Successo
      }
    } else {
      return true;  // Nessun errore riportato
    }
    
    Serial.print("Tentativo ");
    Serial.print(i + 1);
    Serial.print(" fallito per: ");
    Serial.println(name);
    delay(500);
  }
  
  Serial.print("ERRORE: ");
  Serial.print(name);
  Serial.println(" fallito dopo 3 tentativi");
  return false;
}
```

### Watchdog Timer

```cpp
#include "esp_task_wdt.h"

void setup() {
  // Abilita watchdog (timeout 30 secondi)
  esp_task_wdt_init(30, true);
  esp_task_wdt_add(NULL);
  
  // ...
}

void loop() {
  // Reset watchdog
  esp_task_wdt_reset();
  
  // ...
}
```

---

## ⚡ OTTIMIZZAZIONI E PERFORMANCE

### Riduzione Consumo

```cpp
void optimizePowerConsumption() {
  // Riduce frequenza CPU (risparmio ~40%)
  setCpuFrequencyMhz(80);
  
  // Disabilita WiFi
  WiFi.mode(WIFI_OFF);
  
  // Disabilita Bluetooth quando non usato (opzionale)
  // btStop();
  
  // Riduce consumo ADC
  // (già ottimizzato leggendo solo quando necessario)
}
```

### Ottimizzazione Loop

```cpp
void loop() {
  // Esegui controlli con frequenze diverse
  static unsigned long lastButtonCheck = 0;
  static unsigned long lastVolumeCheck = 0;
  static unsigned long lastBatteryCheck = 0;
  static unsigned long lastNFCCheck = 0;
  
  unsigned long now = millis();
  
  // Pulsanti: ogni 10ms (responsività)
  if (now - lastButtonCheck > 10) {
    checkButtons();
    lastButtonCheck = now;
  }
  
  // Volume: ogni 200ms (sufficiente)
  if (now - lastVolumeCheck > 200) {
    checkVolume();
    lastVolumeCheck = now;
  }
  
  // Batteria: ogni 30 secondi
  if (now - lastBatteryCheck > 30000) {
    checkBattery();
    lastBatteryCheck = now;
  }
  
  // NFC: ogni 100ms
  if (now - lastNFCCheck > 100) {
    checkNFC();
    lastNFCCheck = now;
  }
  
  // Bluetooth: sempre (non blocca)
  checkBluetooth();
  
  // LED: sempre (per lampeggi e stati temporanei)
  updateLEDStatus();
  
  // Delay minimo per stabilità
  delay(5);
}
```

---

## 📝 CODICE COMPLETO COMMENTATO

Vedi file `main.ino` nel repository per il codice completo con tutti i commenti dettagliati.

**Struttura file**:
- `main.ino` - Codice principale
- `config.h` - Configurazioni e pin (opzionale)
- `functions.h` - Funzioni ausiliarie (opzionale)

---

## 📚 RISORSE

### Documentazione
- ESP32: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
- Arduino ESP32: https://github.com/espressif/arduino-esp32
- DFPlayer: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
- PN532: https://www.adafruit.com/product/364

### Esempi Codice
- DFPlayer: Esempi nella libreria DFRobotDFPlayerMini
- PN532: Esempi nella libreria Adafruit_PN532
- Bluetooth: Esempi ESP32 → BluetoothSerial

---

**Fine Guida Programmazione Microcontrollore** 💻

*Per domande o problemi, consultare la sezione Troubleshooting o il blueprint principale.*

