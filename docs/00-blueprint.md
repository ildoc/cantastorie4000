# 🎵 Lettore Musicale NFC per Bambini
## Progetto ESP32 + DFPlayer Mini

---

## 📋 INDICE

1. [Panoramica del Progetto](#-panoramica-del-progetto)
2. [Lista Componenti](#-lista-componenti)
3. [Schema Elettrico](#-schema-elettrico)
4. [Preparazione Componenti](#-preparazione-componenti)
5. [Assemblaggio Hardware](#-assemblaggio-hardware)
6. [Configurazione Software](#-configurazione-software)
7. [USB-C Dual Mode](#-usb-c-dual-mode)
8. [Struttura File Audio](#-struttura-file-audio)
9. [Costruzione Scatola](#-costruzione-scatola)
10. [App Bluetooth](#-app-bluetooth)
11. [Testing e Troubleshooting](#-testing-e-troubleshooting)

---

## 🎯 PANORAMICA DEL PROGETTO

### Caratteristiche Principali
- **Controllo NFC**: Tag colorati per selezionare playlist
- **Controlli fisici**: 3 pulsanti arcade (Play/Pausa, Next, Prev) + Potenziometro volume
- **App Bluetooth**: Gestione remota, associazione tag NFC
- **MicroSD estraibile**: Slot esterno per facile aggiornamento contenuti
- **LED di stato**: LED bicolore per feedback visivo (boot, NFC, batteria, errori)
- **Autonomia**: 13-15 ore (volume alto, NFC sempre attivo) con 2 batterie 3000mAh in parallelo (6000mAh)
- **Avvio rapido**: 2-3 secondi dall'accensione
- **USB-C**: Ricarica batteria (+ opzionale trasferimento dati)

### Specifiche Tecniche
- **Microcontrollore**: ESP32 (WiFi + Bluetooth integrati)
- **Audio**: DFPlayer Mini (MP3, WAV, supporta microSD fino a 32GB)
- **NFC**: PN532 (compatibile ISO14443A, range ~5cm)
- **Amplificatore**: PAM8403 classe D (2x3W stereo)
- **Batteria**: 2×18650 Li-ion 3.7V 3000mAh in parallelo (6000mAh totali)
- **Formato audio**: MP3 128-320kbps

---

## 🛒 LISTA COMPONENTI

### Elettronica Principale

| Componente | Specifiche | Quantità | Prezzo | Note |
|------------|------------|----------|--------|------|
| **ESP32 DevKit V1** | 30 pin, WiFi+BT | 1 | €10 | Cerca "ESP32 WROOM-32 DevKit" |
| **DFPlayer Mini** | Modulo MP3 | 1 | €4 | Verifica che includa resistenza 1KΩ |
| **PN532 NFC Module** | I2C/SPI | 1 | €10 | Preferisci versione con antenna PCB |
| **PAM8403** | Amplificatore 2x3W | 1 | €3 | Modulo con potenziometro volume |
| **TP4056** | Ricarica Li-ion + protezione | 1 | €2 | Modello con chip DW01A (protezione) |
| **Step-up MT3608** | Boost 3.7V → 5V | 1 | €2 | Regolabile, max 2A |
| **MicroSD Card** | 16-32GB Classe 10 | 1 | €7 | SanDisk o Samsung |

### Alimentazione

| Componente | Specifiche | Quantità | Prezzo |
|------------|------------|----------|--------|
| **Batteria 18650** | 3.7V 3000mAh protetta | 2 | €20 | Marca affidabile (LG, Samsung, Panasonic). Collegate in parallelo per 6000mAh totali |
| **Porta-batteria 18650** | Con fili, doppio | 1 | €5 | Supporto per 2 batterie in parallelo |
| **Interruttore ON/OFF** | Rocker switch | 1 | €2 | |
| **Cavo USB-C** | Per ricarica | 1 | €3 | |

### Audio e Controlli

| Componente | Specifiche | Quantità | Prezzo |
|------------|------------|----------|--------|
| **Speaker** | 4Ω 3W, 50mm | 1 | €6 | Preferisci full-range |
| **Pulsanti Arcade** | 30mm, colorati | 3 | €6 | Colori: Rosso, Giallo, Bianco |
| **Potenziometro** | 10KΩ lineare con manopola | 1 | €3 | Per controllo volume |
| **LED Bicolore** | 5mm rosso/verde catodo comune | 1 | €1 | LED di stato generico |
| **MicroSD Extension** | Cavo adattatore 10-15cm | 1 | €4 | Slot esterno per SD |
| **Tag NFC** | NTAG215 o NTAG213 | 10 | €8 | Adesivi o portachiavi |
| **Resistori** | 100KΩ, 47KΩ, 220Ω (2x) | 4 | €1 | Per partitore tensione e LED |

### Materiali per Costruzione

| Componente | Specifiche | Quantità | Prezzo |
|------------|------------|----------|--------|
| **Compensato** | 10mm, 30x40cm | 1 | €8 | Per scatola |
| **Griglia speaker** | Metallica 60mm | 1 | €3 | Protezione speaker |
| **Viti M3** | 10-15mm | 20 | €3 | Con dadi |
| **Distanziali** | M3 10mm | 10 | €2 | In plastica o ottone |
| **Cavi Dupont** | F-F, M-F | 20 | €3 | Kit misto |
| **Stagno** | Filo 0.8mm | - | €5 | Per saldature |
| **Colla a caldo** | Stick | 10 | €5 | Per fissaggio componenti |
| **Vernice atossica** | Acrilica colorata | - | €10 | Opzionale |

**COSTO TOTALE STIMATO**: €105-125 (con 2 batterie)

---

## ⚡ SCHEMA ELETTRICO

### Schema a Blocchi Completo

```
┌─────────────────── ALIMENTAZIONE ───────────────────┐
│                                                      │
│  [Batteria 18650 #1] ──┐                            │
│  3.7V 3000mAh          ├──> [TP4056] <── USB-C     │
│  [Batteria 18650 #2] ──┤      ↓                    │
│  3.7V 3000mAh (parall.)│                            │
│                     │   [Switch]                     │
│                     │      ↓                         │
│                     └──> [MT3608] ──> 5V Rail        │
│                         (Step-up)                    │
└──────────────────────────┬───────────────────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
        ↓                  ↓                  ↓
   ┌────────┐        ┌─────────┐       ┌──────────┐
   │  ESP32 │        │ DFPlayer│       │  PN532   │
   │        │        │  Mini   │       │   NFC    │
   │  5V    │        │  5V     │       │   3.3V   │
   └────────┘        └─────────┘       └──────────┘
        │                  │                  │
        │ TX(17)──────────>│ RX              │
        │ RX(16)──────────<│ TX              │
        │                  │                  │
        │ SDA(21)─────────────────────────────> SDA
        │ SCL(22)─────────────────────────────> SCL
        │                  │                  │
        │ GPIO13 ─────> [Pulsante PLAY]      │
        │ GPIO27 ─────> [Pulsante NEXT]      │
        │ GPIO26 ─────> [Pulsante PREV]      │
        │ GPIO34 ─────> [Potenziometro VOL]  │
        │ GPIO35 ─────> [Partitore Batteria] │
        │ GPIO25 ─────> [LED Rosso]          │
        │ GPIO33 ─────> [LED Verde]          │
        │                  │                  │
        │                  │ SPK+ ──┐         │
        │                  │ SPK- ──┼───> [PAM8403]
        │                  │        │          │
        │                  │        │          ↓
        │                  │        │    [Speaker 4Ω]
        └──────────────────┴────────┴──────────┘
```

### Connessioni Dettagliate

#### ESP32 ↔ DFPlayer Mini
```
ESP32          DFPlayer
─────          ────────
GPIO17 (TX) -> RX (via resistenza 1KΩ)
GPIO16 (RX) <- TX
5V          -> VCC
GND         -> GND
```

**IMPORTANTE**: Resistenza 1KΩ tra TX ESP32 e RX DFPlayer per protezione.

#### ESP32 ↔ PN532 (I2C)
```
ESP32          PN532
─────          ─────
GPIO21 (SDA) -> SDA
GPIO22 (SCL) -> SCL
3.3V         -> VCC (USARE 3.3V, NON 5V!)
GND          -> GND
```

**CONFIGURAZIONE PN532**: Jumper per modalità I2C:
- CH1: OFF
- CH2: ON

#### ESP32 ↔ Pulsanti
```
ESP32          Pulsante
─────          ────────
GPIO13      -> PLAY (Rosso)
GPIO27      -> NEXT (Giallo)
GPIO26      -> PREV (Bianco)
GND         -> Altro terminale
```

Pull-up interno configurato via software.

#### ESP32 ↔ Potenziometro Volume
```
Potenziometro 10KΩ
─────────────────
Pin 1 (GND)    -> GND
Pin 2 (Wiper)  -> GPIO34 (ADC)
Pin 3 (VCC)    -> 3.3V
```

#### ESP32 ↔ LED di Stato + Monitoraggio Batteria
```
Partitore tensione (collegare al punto comune delle 2 batterie):
Punto comune + (2 batterie) ──[100KΩ]──┬──> GPIO35 (ADC)
                                        │
                                     [47KΩ]
                                        │
                                       GND

LED Bicolore (Stato):
GPIO25 ──[220Ω]──> LED Rosso ──┐
GPIO33 ──[220Ω]──> LED Verde ──┤
                                ├──> GND (catodo comune)

Funzioni LED:
- Verde fisso: Sistema pronto (boot completato)
- Verde lampeggiante rapido: Tag NFC letto con successo
- Rosso lampeggiante: Batteria bassa o tag non associato
- Rosso fisso: Batteria critica o errore
```

#### DFPlayer ↔ PAM8403 ↔ Speaker
```
DFPlayer       PAM8403        Speaker
────────       ───────        ───────
SPK_1       -> RIN
SPK_2       -> LIN          -> Speaker+
GND         -> GND          -> Speaker-

PAM8403 VCC -> 5V
```

---

## 🔧 PREPARAZIONE COMPONENTI

### 1. Test ESP32
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Test OK!");
}
void loop() { delay(1000); }
```

### 2. Test DFPlayer Mini
- Formattare microSD in FAT32
- Creare cartella `/mp3/`
- Copiare `0001.mp3`, `0002.mp3`
- Collegare a 5V + speaker
- Dovrebbe riprodurre automaticamente

### 3. Test PN532
- Configurare jumper I2C (CH1=OFF, CH2=ON)
- Collegare a ESP32 (3.3V!)
- Caricare esempio `Adafruit_PN532`
- Testare lettura tag

### 4. Configurazione MT3608
**PRIMA di collegare l'ESP32:**
1. Input 3.7V
2. Misurare output
3. Regolare a **5.0V precisi**
4. Segnare posizione

### 5. Test TP4056
- Collegare 2 batterie 18650 in parallelo
- Verificare che tensioni siano simili (±0.1V) prima di collegare
- Collegare USB-C
- LED rosso = in carica
- LED blu = carica completa
- Tempo carica: ~6-7 ore per 6000mAh @ 1A

---

## 🔨 ASSEMBLAGGIO HARDWARE

### FASE 1: Saldature

#### A. ESP32
Saldare header 30 pin se necessario

#### B. DFPlayer
```
4 cavi da 15cm:
- Rosso:   VCC
- Nero:    GND
- Giallo:  TX (con resistenza 1KΩ)
- Verde:   RX

2 cavi per speaker:
- SPK_1, SPK_2
```

#### C. PN532
```
4 cavi da 15cm:
- Rosso:   VCC
- Nero:    GND
- Blu:     SDA
- Verde:   SCL
```

#### D. Pulsanti
Un cavo a GPIO, uno a GND comune.

#### E. Potenziometro
```
3 cavi da 10cm:
- Nero:    GND
- Rosso:   3.3V
- Giallo:  Wiper → GPIO34
```

#### F. LED di Stato Bicolore
```
LED Bicolore (3 pin):
Anodo Rosso  → GPIO25 via 220Ω
Anodo Verde  → GPIO33 via 220Ω
Catodo       → GND

Funzione: LED di stato generico per feedback visivo
- Boot completato
- Lettura tag NFC
- Errori sistema
- Stato batteria
```

#### G. Partitore Tensione Batteria
```
Partitore: 100KΩ + 47KΩ → GPIO35
Collegare al punto comune delle 2 batterie in parallelo
(La tensione è la stessa, ma la capacità è doppia)
```

#### H. Collegamento 2 Batterie in Parallelo
```
Batteria 1+ ──┬──> TP4056 BAT+
Batteria 1- ──┤
              │
Batteria 2+ ──┤
Batteria 2- ──┴──> TP4056 BAT-

⚠️ IMPORTANTE:
- Batterie devono avere tensione simile (±0.1V) prima di collegarle
- Usare batterie identiche (stessa marca, stesso modello)
- Caricare insieme la prima volta
- Partitore tensione si collega al punto comune
```

### FASE 2: Test su Breadboard
Testare ogni modulo singolarmente prima di assemblare definitivamente.

### FASE 3: Layout nella Scatola

```
Vista dall'alto (15x10cm):

┌────────────────────────────────────┐
│  ●                      [Griglia]  │  ← LED + Speaker
│                                    │
│  [●]  [●]  [●]          [🎚️]      │  ← Pulsanti + Volume
│  PLAY PREV NEXT         VOLUME    │
│                                    │
│        ┌──────────┐                │
│        │   NFC    │                │  ← Zona PN532
│        └──────────┘                │
│                                    │
│  Componenti interni sotto          │
└────────────────────────────────────┘

Pannello posteriore:
│  [SD]═══  [USB-C]  [○]Switch      │
```

### FASE 4: Montaggio

1. **Forare pannello frontale**:
   - 3 fori 30mm (pulsanti)
   - 1 foro 8mm (potenziometro)
   - 1 foro 60mm (speaker)
   - 1 foro 5mm (LED)

2. **Forare pannello posteriore**:
   - Slot microSD (15x2mm)
   - USB-C (10x4mm)
   - Switch (12mm)

3. **Fissare componenti interni**:
   - ESP32/DFPlayer: viti M3 + distanziali
   - Altri moduli: colla a caldo
   - Batteria: supporto o velcro

4. **Test pre-chiusura** ✅

---

## 💻 CONFIGURAZIONE SOFTWARE

### Installazione Arduino IDE

1. Scaricare Arduino IDE 2.x
2. Aggiungere supporto ESP32:
   - URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Installare librerie:
   - `DFRobotDFPlayerMini`
   - `Adafruit PN532`
   - `ArduinoJson`

### Codice Completo

```cpp
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include <BluetoothSerial.h>
#include <Preferences.h>

// Pin Definitions
#define DFPLAYER_RX 16
#define DFPLAYER_TX 17
#define PN532_SDA 21
#define PN532_SCL 22
#define BTN_PLAY 13
#define BTN_NEXT 27
#define BTN_PREV 26
#define VOLUME_POT 34
#define BATTERY_PIN 35
#define LED_RED 25
#define LED_GREEN 33

// Objects
HardwareSerial DFSerial(1);
DFRobotDFPlayerMini dfPlayer;
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);
BluetoothSerial SerialBT;
Preferences preferences;

// Variables
int currentVolume = 15;
bool isPlaying = false;
String lastTagUID = "";
unsigned long lastTagTime = 0;
const int TAG_DEBOUNCE = 2000;
const int MAX_SAFE_VOLUME = 25;

// Battery monitoring constants
const float VOLTAGE_DIVIDER = 3.13;  // (100K+47K)/47K = 3.128 (approssimato)
const float ADC_REFERENCE = 3.3;
const float ADC_MAX = 4095.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Lettore Musicale NFC - Avvio...");
  
  // Init pins
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(VOLUME_POT, INPUT);
  pinMode(BATTERY_PIN, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  // LED test sequenza
  digitalWrite(LED_GREEN, HIGH);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  delay(300);
  digitalWrite(LED_RED, LOW);
  
  // Segnala boot completato: LED verde fisso
  digitalWrite(LED_GREEN, HIGH);
  
  // Init DFPlayer
  DFSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  delay(1000);
  
  if (!dfPlayer.begin(DFSerial)) {
    Serial.println("Errore DFPlayer!");
    while(true) delay(1000);
  }
  
  Serial.println("DFPlayer OK");
  dfPlayer.volume(currentVolume);
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
  // Init NFC
  nfc.begin();
  if (!nfc.getFirmwareVersion()) {
    Serial.println("Errore PN532!");
  } else {
    Serial.println("PN532 OK");
    nfc.SAMConfig();
  }
  
  // Init Bluetooth
  SerialBT.begin("LettoreMusicale");
  Serial.println("Bluetooth: LettoreMusicale");
  
  // Load preferences
  preferences.begin("music-player", false);
  currentVolume = preferences.getInt("volume", 15);
  dfPlayer.volume(currentVolume);
  
  Serial.println("Sistema pronto!");
}

void loop() {
  checkButtons();
  checkVolume();
  checkBattery();
  checkNFC();
  checkBluetooth();
  delay(50);
}

void checkButtons() {
  static bool lastPlay = HIGH, lastNext = HIGH, lastPrev = HIGH;
  
  bool play = digitalRead(BTN_PLAY);
  if (play == LOW && lastPlay == HIGH) {
    delay(50);
    if (isPlaying) {
      dfPlayer.pause();
      isPlaying = false;
    } else {
      dfPlayer.start();
      isPlaying = true;
    }
  }
  lastPlay = play;
  
  bool next = digitalRead(BTN_NEXT);
  if (next == LOW && lastNext == HIGH) {
    delay(50);
    dfPlayer.next();
  }
  lastNext = next;
  
  bool prev = digitalRead(BTN_PREV);
  if (prev == LOW && lastPrev == HIGH) {
    delay(50);
    dfPlayer.previous();
  }
  lastPrev = prev;
}

void checkVolume() {
  static unsigned long lastCheck = 0;
  static int lastVol = -1;
  
  if (millis() - lastCheck > 200) {
    int raw = analogRead(VOLUME_POT);
    int vol = map(raw, 0, 4095, 0, MAX_SAFE_VOLUME);
    
    if (abs(vol - lastVol) > 1) {
      currentVolume = vol;
      dfPlayer.volume(vol);
      preferences.putInt("volume", vol);
      Serial.print("Volume: ");
      Serial.println(vol);
      lastVol = vol;
    }
    lastCheck = millis();
  }
}

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  return (raw / ADC_MAX) * ADC_REFERENCE * VOLTAGE_DIVIDER;
}

int getBatteryPercentage(float voltage) {
  if (voltage >= 4.2) return 100;
  if (voltage <= 3.0) return 0;
  return (int)((voltage - 3.0) / 1.2 * 100);
}

void checkBattery() {
  static unsigned long lastCheck = 0;
  
  if (millis() - lastCheck > 30000) {
    float voltage = readBatteryVoltage();
    int percent = getBatteryPercentage(voltage);
    
    Serial.print("Batteria: ");
    Serial.print(voltage);
    Serial.print("V (");
    Serial.print(percent);
    Serial.println("%)");
    
    // Gestione LED di stato (vedi guida dettagliata per sistema completo)
    if (percent > 20) {
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
    } else if (percent > 10) {
      // Batteria bassa: rosso lampeggiante
      static bool ledState = false;
      ledState = !ledState;
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, ledState);
    } else {
      // Batteria critica: rosso fisso
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
    }
    
    lastCheck = millis();
  }
}

// NOTA: Per un sistema completo di gestione LED di stato (boot, NFC, errori),
// vedere la guida dettagliata in programmazione-microcontrollore.md

void checkNFC() {
  uint8_t uid[7];
  uint8_t uidLength;
  
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) {
    String tagUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) tagUID += "0";
      tagUID += String(uid[i], HEX);
    }
    tagUID.toUpperCase();
    
    if (tagUID != lastTagUID || (millis() - lastTagTime) > TAG_DEBOUNCE) {
      Serial.print("Tag NFC: ");
      Serial.println(tagUID);
      handleNFCTag(tagUID);
      lastTagUID = tagUID;
      lastTagTime = millis();
    }
  }
}

void handleNFCTag(String uid) {
  String key = "tag_" + uid;
  int folder = preferences.getInt(key.c_str(), 0);
  
  if (folder > 0) {
    Serial.print("Riproduzione cartella: ");
    Serial.println(folder);
    
    // Feedback LED: verde lampeggiante rapido (vedi guida dettagliata)
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_GREEN, HIGH);
      delay(100);
      digitalWrite(LED_GREEN, LOW);
      delay(100);
    }
    
    dfPlayer.loopFolder(folder);
    isPlaying = true;
  } else {
    Serial.println("Tag non associato");
    
    // Feedback LED: rosso lampeggiante rapido
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_RED, HIGH);
      delay(150);
      digitalWrite(LED_RED, LOW);
      delay(150);
    }
    
    dfPlayer.playMp3Folder(9999);
  }
}

void checkBluetooth() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();
    handleBluetoothCommand(cmd);
  }
}

void handleBluetoothCommand(String cmd) {
  Serial.print("BT: ");
  Serial.println(cmd);
  
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
  else if (cmd.startsWith("VOL:")) {
    int vol = cmd.substring(4).toInt();
    if (vol >= 0 && vol <= MAX_SAFE_VOLUME) {
      currentVolume = vol;
      dfPlayer.volume(vol);
      preferences.putInt("volume", vol);
      SerialBT.println("OK");
    }
  }
  else if (cmd.startsWith("FOLDER:")) {
    int folder = cmd.substring(7).toInt();
    dfPlayer.loopFolder(folder);
    SerialBT.println("OK");
  }
  else if (cmd.startsWith("ASSOCIATE:")) {
    int sep = cmd.indexOf(':', 11);
    String uid = cmd.substring(11, sep);
    int folder = cmd.substring(sep + 1).toInt();
    
    String key = "tag_" + uid;
    preferences.putInt(key.c_str(), folder);
    
    SerialBT.print("Associated ");
    SerialBT.print(uid);
    SerialBT.print(" to folder ");
    SerialBT.println(folder);
  }
  else if (cmd == "STATUS") {
    float voltage = readBatteryVoltage();
    int battery = getBatteryPercentage(voltage);
    
    SerialBT.print("Volume:");
    SerialBT.print(currentVolume);
    SerialBT.print(",Playing:");
    SerialBT.print(isPlaying ? "1" : "0");
    SerialBT.print(",Battery:");
    SerialBT.println(battery);
  }
  else if (cmd == "BATTERY") {
    float voltage = readBatteryVoltage();
    int percent = getBatteryPercentage(voltage);
    SerialBT.print("BATTERY:");
    SerialBT.print(voltage);
    SerialBT.print("V,");
    SerialBT.print(percent);
    SerialBT.println("%");
  }
}
```

---

## 🔌 USB-C DUAL MODE

### La Domanda
**Posso usare la stessa porta USB-C per ricarica E trasferimento dati?**

### La Risposta: SÌ (con limitazioni)

#### Opzione 1: USB Switch Board con CH340G (Avanzata)

**Componente**: Modulo con CH340G + Auto-Switch  
**Costo**: €6

**Come funziona**:
```
USB-C ──> [CH340G + Switch] ──┬──> ESP32 UART (dati)
                               └──> TP4056 (ricarica)
```

**Logica**: 
- PC con driver → modalità dati
- Caricatore → modalità ricarica

#### Opzione 2: Due Porte USB-C (CONSIGLIATA per semplicità)

- **USB-C principale**: TP4056 ricarica (sempre accessibile)
- **USB-C secondaria**: ESP32 dati (nascosta/sportellino interno)

#### Opzione 3: Slot MicroSD Estraibile (LA PIÙ SEMPLICE)

**✅ RACCOMANDATA per prima versione:**
- USB-C → solo ricarica
- MicroSD → slot esterno estraibile
- Programmazione ESP32 → via micro-USB temporaneo del DevKit

**Gestione file audio**:
1. Spegnere dispositivo
2. Estrarre microSD
3. Inserire in lettore PC
4. Copiare file MP3
5. Reinserire microSD

---

## 📁 STRUTTURA FILE AUDIO

### Organizzazione MicroSD

```
📁 SD Card (FAT32)
│
├── 📁 01/              ← Fiabe
│   ├── 001.mp3
│   ├── 002.mp3
│   └── 003.mp3
│
├── 📁 02/              ← Canzoni
│   ├── 001.mp3
│   └── 002.mp3
│
├── 📁 03/              ← Classica
│   └── 001.mp3
│
└── 📁 mp3/             ← File singoli
    └── 9999.mp3        ← Suono errore
```

### Regole di Nomenclatura

1. **Cartelle**: `01`, `02`, `03`... (2 cifre)
2. **File**: `001.mp3`, `002.mp3`... (3 cifre)
3. **NO spazi o caratteri speciali**
4. **Formato**: MP3 128-320kbps

### Esempio Associazione Tag

```
Tag Blu    (#04A1B2C3) → Cartella 01 (Fiabe)
Tag Rosso  (#05C3D4E5) → Cartella 02 (Canzoni)
Tag Verde  (#06E5F6A7) → Cartella 03 (Classica)
```

---

## 📦 COSTRUZIONE SCATOLA

### Dimensioni: 15cm x 10cm x 5cm

### Foratura Pannello Frontale

**3 fori 30mm** (pulsanti):
- Spaziatura 35mm tra centri
- 3cm dal bordo superiore

**1 foro 8mm** (potenziometro):
- Lato destro, 25mm dal bordo

**1 foro 60mm** (speaker):
- Angolo alto destra
- 20mm dal bordo, 15mm dall'alto

**1 foro 5mm** (LED):
- Angolo alto sinistra
- 10mm dai bordi

### Foratura Pannello Posteriore

- **Slot microSD**: 15x2mm, 30mm da sinistra
- **USB-C**: 10x4mm, centro
- **Switch**: 12mm, 60mm da sinistra

### Assemblaggio

1. Levigare bordi
2. Pre-assemblaggio a secco
3. Incollaggio con colla vinilica
4. Chiodini rinforzo
5. Finitura carta vetrata
6. Verniciatura opzionale

---

## 📱 APP BLUETOOTH

### Protocollo Comunicazione

#### Comandi dall'App all'ESP32

| Comando | Formato | Descrizione |
|---------|---------|-------------|
| Play | `PLAY` | Avvia riproduzione |
| Pausa | `PAUSE` | Mette in pausa |
| Volume | `VOL:15` | Imposta volume (0-25) |
| Cartella | `FOLDER:2` | Riproduce cartella 2 |
| Associa Tag | `ASSOCIATE:04A1B2C3:1` | Associa tag a cartella |
| Stato | `STATUS` | Richiede stato corrente |
| Batteria | `BATTERY` | Richiede stato batteria |

#### Risposte dall'ESP32

```
OK
ERROR:messaggio
STATUS:Volume:15,Playing:1,Battery:85
TAG_DETECTED:04A1B2C3
BATTERY:3.8V,75%
```

### App Flutter Completa

**File: pubspec.yaml**

```yaml
name: music_player_app
description: Lettore Musicale NFC Controller
version: 1.0.0

dependencies:
  flutter:
    sdk: flutter
  flutter_bluetooth_serial: ^0.4.0

flutter:
  uses-material-design: true
```

**File: android/app/src/main/AndroidManifest.xml**

Aggiungi questi permessi dentro `<manifest>`:

```xml
<uses-permission android:name="android.permission.BLUETOOTH" />
<uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
<uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />
<uses-permission android:name="android.permission.BLUETOOTH_SCAN" />
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
```

**File: lib/main.dart**

```dart
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'dart:convert';
import 'dart:typed_data';

void main() => runApp(MusicPlayerApp());

class MusicPlayerApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Lettore Musicale',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        brightness: Brightness.light,
      ),
      home: MusicPlayerHome(),
    );
  }
}

class MusicPlayerHome extends StatefulWidget {
  @override
  _MusicPlayerHomeState createState() => _MusicPlayerHomeState();
}

class _MusicPlayerHomeState extends State<MusicPlayerHome> {
  BluetoothConnection? connection;
  bool isConnected = false;
  int volume = 15;
  int batteryPercent = 100;
  bool isPlaying = false;
  String lastTag = "";
  List<BluetoothDevice> devices = [];

  @override
  void initState() {
    super.initState();
    _getDevices();
  }

  void _getDevices() async {
    List<BluetoothDevice> bondedDevices = 
        await FlutterBluetoothSerial.instance.getBondedDevices();
    setState(() {
      devices = bondedDevices;
    });
  }

  void connectToDevice(BluetoothDevice device) async {
    try {
      connection = await BluetoothConnection.toAddress(device.address);
      setState(() {
        isConnected = true;
      });
      
      connection!.input!.listen((Uint8List data) {
        String response = ascii.decode(data);
        _handleResponse(response);
      }).onDone(() {
        setState(() {
          isConnected = false;
        });
      });
      
      _sendCommand("STATUS");
      
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Connesso a ${device.name}')),
      );
    } catch (e) {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Errore di connessione')),
      );
    }
  }

  void _sendCommand(String cmd) {
    if (connection != null && connection!.isConnected) {
      connection!.output.add(ascii.encode(cmd + "\n"));
    }
  }

  void _handleResponse(String response) {
    if (response.startsWith("TAG_DETECTED:")) {
      setState(() {
        lastTag = response.substring(13).trim();
      });
      _showTagDialog();
    }
    else if (response.startsWith("STATUS:")) {
      var parts = response.substring(7).split(',');
      for (var part in parts) {
        if (part.startsWith("Volume:")) {
          setState(() {
            volume = int.tryParse(part.split(':')[1]) ?? 15;
          });
        }
        else if (part.startsWith("Playing:")) {
          setState(() {
            isPlaying = part.split(':')[1] == "1";
          });
        }
        else if (part.startsWith("Battery:")) {
          setState(() {
            batteryPercent = int.tryParse(part.split(':')[1]) ?? 100;
          });
        }
      }
    }
  }

  void _showTagDialog() {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text("Tag NFC Rilevato"),
        content: Text("UID: $lastTag\n\nAssocia a quale cartella?"),
        actions: [
          for (int i = 1; i <= 5; i++)
            TextButton(
              child: Text("Cartella $i"),
              onPressed: () {
                _sendCommand("ASSOCIATE:$lastTag:$i");
                Navigator.pop(context);
                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(content: Text('Tag associato a cartella $i')),
                );
              },
            ),
          TextButton(
            child: Text("Annulla"),
            onPressed: () => Navigator.pop(context),
          ),
        ],
      ),
    );
  }

  void _showDeviceSelector() {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text("Seleziona Dispositivo"),
        content: Container(
          width: double.maxFinite,
          child: ListView.builder(
            shrinkWrap: true,
            itemCount: devices.length,
            itemBuilder: (context, index) {
              return ListTile(
                title: Text(devices[index].name ?? "Sconosciuto"),
                subtitle: Text(devices[index].address),
                onTap: () {
                  Navigator.pop(context);
                  connectToDevice(devices[index]);
                },
              );
            },
          ),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Lettore Musicale"),
        actions: [
          if (!isConnected)
            IconButton(
              icon: Icon(Icons.bluetooth),
              onPressed: _showDeviceSelector,
            ),
        ],
      ),
      body: Padding(
        padding: EdgeInsets.all(16.0),
        child: Column(
          children: [
            // Stato connessione
            Card(
              child: ListTile(
                leading: Icon(
                  isConnected ? Icons.bluetooth_connected : Icons.bluetooth_disabled,
                  color: isConnected ? Colors.blue : Colors.grey,
                ),
                title: Text(isConnected ? "Connesso" : "Non connesso"),
                trailing: isConnected
                    ? TextButton(
                        child: Text("Disconnetti"),
                        onPressed: () {
                          connection?.dispose();
                          setState(() {
                            isConnected = false;
                          });
                        },
                      )
                    : null,
              ),
            ),
            
            SizedBox(height: 16),
            
            // Batteria
            Card(
              child: Padding(
                padding: EdgeInsets.all(16.0),
                child: Row(
                  children: [
                    Icon(Icons.battery_full, 
                         color: batteryPercent > 20 ? Colors.green : Colors.red),
                    SizedBox(width: 8),
                    Text("Batteria: $batteryPercent%", 
                         style: TextStyle(fontSize: 16)),
                    Spacer(),
                    if (isConnected)
                      IconButton(
                        icon: Icon(Icons.refresh),
                        onPressed: () => _sendCommand("BATTERY"),
                      ),
                  ],
                ),
              ),
            ),
            
            SizedBox(height: 16),
            
            // Volume
            Card(
              child: Padding(
                padding: EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    Text("Volume: $volume/25", style: TextStyle(fontSize: 18)),
                    Row(
                      children: [
                        IconButton(
                          icon: Icon(Icons.remove),
                          onPressed: () {
                            if (volume > 0) {
                              setState(() => volume--);
                              _sendCommand("VOL:$volume");
                            }
                          },
                        ),
                        Expanded(
                          child: Slider(
                            value: volume.toDouble(),
                            min: 0,
                            max: 25,
                            divisions: 25,
                            onChanged: (val) {
                              setState(() => volume = val.toInt());
                            },
                            onChangeEnd: (val) {
                              _sendCommand("VOL:${val.toInt()}");
                            },
                          ),
                        ),
                        IconButton(
                          icon: Icon(Icons.add),
                          onPressed: () {
                            if (volume < 25) {
                              setState(() => volume++);
                              _sendCommand("VOL:$volume");
                            }
                          },
                        ),
                      ],
                    ),
                  ],
                ),
              ),
            ),
            
            SizedBox(height: 16),
            
            // Controlli playback
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                IconButton(
                  icon: Icon(Icons.skip_previous),
                  iconSize: 48,
                  onPressed: () => _sendCommand("PREV"),
                ),
                IconButton(
                  icon: Icon(isPlaying ? Icons.pause_circle_filled 
                                        : Icons.play_circle_filled),
                  iconSize: 64,
                  color: Colors.blue,
                  onPressed: () {
                    _sendCommand(isPlaying ? "PAUSE" : "PLAY");
                    setState(() => isPlaying = !isPlaying);
                  },
                ),
                IconButton(
                  icon: Icon(Icons.skip_next),
                  iconSize: 48,
                  onPressed: () => _sendCommand("NEXT"),
                ),
              ],
            ),
            
            SizedBox(height: 16),
            
            // Lista cartelle
            Text("Cartelle:", 
                 style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
            Expanded(
              child: ListView.builder(
                itemCount: 5,
                itemBuilder: (context, index) {
                  return Card(
                    child: ListTile(
                      leading: Icon(Icons.folder, color: Colors.amber),
                      title: Text("Cartella ${index + 1}"),
                      trailing: IconButton(
                        icon: Icon(Icons.play_circle_outline),
                        onPressed: () => _sendCommand("FOLDER:${index + 1}"),
                      ),
                    ),
                  );
                },
              ),
            ),
            
            SizedBox(height: 16),
            
            // Pulsante gestione tag
            ElevatedButton.icon(
              icon: Icon(Icons.nfc),
              label: Text("Gestione Tag NFC"),
              style: ElevatedButton.styleFrom(
                minimumSize: Size(double.infinity, 48),
              ),
              onPressed: () {
                _sendCommand("SCAN_NFC");
                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(content: Text('Avvicina un tag NFC al dispositivo')),
                );
              },
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    connection?.dispose();
    super.dispose();
  }
}
```

### Compilazione App

```bash
# Installare dipendenze
flutter pub get

# Eseguire in debug
flutter run

# Build release per Android
flutter build apk --release
```

### Alternative Semplificate

1. **Serial Bluetooth Terminal** (app gratuita)
   - Invia comandi testuali direttamente
   - Perfetta per testing

2. **MIT App Inventor**
   - Crea app senza codice
   - Drag & drop

---

## 🔍 TESTING E TROUBLESHOOTING

### Checklist Test

#### ✅ Test 1: Alimentazione
1. Solo batteria → TP4056 → MT3608
2. Output deve essere 5.0V ± 0.1V
3. Corrente >500mA sotto carico

#### ✅ Test 2: ESP32
1. Collegare via USB
2. Caricare sketch test
3. Verificare Serial Monitor
4. Testare GPIO con LED

#### ✅ Test 3: DFPlayer
1. MicroSD con file test
2. Collegare speaker a SPK_1/SPK_2
3. Alimentare a 5V
4. Deve riprodurre automaticamente

#### ✅ Test 4: NFC
1. Jumper I2C (CH1=OFF, CH2=ON)
2. Collegare a 3.3V (NON 5V!)
3. Testare con tag NTAG215

#### ✅ Test 5: Bluetooth
1. Cercare "LettoreMusicale"
2. Connettere
3. Inviare "STATUS"
4. Verificare risposta

#### ✅ Test 6: Sistema Completo
Prima di chiudere la scatola:
- Testare ogni pulsante
- Testare NFC con 3-4 tag
- Riprodurre audio 5 minuti a volume max
- Verificare temperatura (<60°C)
- Testare ricarica
- Misurare autonomia (almeno 2 ore)

### Problemi Comuni

| Problema | Causa | Soluzione |
|----------|-------|-----------|
| ESP32 si riavvia | Alimentazione insufficiente | Condensatore 470µF su 5V |
| Audio distorto | Speaker sbagliato | Usare 4Ω, non 8Ω |
| NFC non legge | 5V invece di 3.3V | Collegare a 3.3V! |
| Batteria dura poco | Consumo eccessivo | Ridurre volume max |
| MicroSD non letta | Formato sbagliato | FAT32, non exFAT |
| Bluetooth disconnette | Interferenze WiFi | `WiFi.mode(WIFI_OFF)` |

### Ottimizzazioni

**Ridurre consumo**:
```cpp
setCpuFrequencyMhz(80);  // -40% consumo
WiFi.mode(WIFI_OFF);
```

**Migliorare audio**:
```cpp
dfPlayer.EQ(DFPLAYER_EQ_BASS);
```

**Calibrazione potenziometro**:
```cpp
// Eliminare zone morte
rawValue = constrain(rawValue, 200, 3895);
volume = map(rawValue, 200, 3895, 0, MAX_SAFE_VOLUME);
```

---

## 🚀 MIGLIORAMENTI FUTURI

### Funzionalità Software
- Sleep mode automatico (10 min inattività)
- Timer spegnimento (per addormentamento)
- Playlist sequenziali
- Equalizzatore via app

### Hardware Opzionale

| Componente | Funzione | Costo |
|------------|----------|-------|
| Microfono MAX4466 | Registrazione | €4 |
| LED RGB WS2812 | Effetti luce | €3 |
| MPU6050 | Sensore movimento | €4 |
| RTC DS3231 | Timer programmato | €3 |

---

## 📚 RISORSE

### Documentazione
- ESP32: https://docs.espressif.com/
- DFPlayer: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
- PN532: https://www.nxp.com/docs/en/user-guide/141520.pdf

### Librerie Arduino
```
DFRobotDFPlayerMini v1.0.6+
Adafruit_PN532 v1.4.0+
ArduinoJson v6.21.0+
```

### Community
- Forum Arduino: https://forum.arduino.cc/
- ESP32 Reddit: https://www.reddit.com/r/esp32/
- Stack Overflow: tag `esp32`, `dfplayer`, `nfc`

---

## 🎉 CONCLUSIONI

### Cosa Hai Ottenuto

✅ Lettore musicale funzionale e robusto  
✅ Controllo NFC con tag colorati  
✅ Potenziometro analogico per volume  
✅ LED bicolore di stato (boot, NFC, batteria, errori)  
✅ MicroSD estraibile  
✅ App Flutter per gestione  
✅ Autonomia 7-9 ore  
✅ Costo ~€100  

### Prossimi Passi

1. **Ordinare componenti** (vedi lista)
2. **Testare ogni modulo** singolarmente
3. **Assemblare su breadboard**
4. **Caricare software** e verificare
5. **Costruire scatola**
6. **Assemblare definitivamente**
7. **Testare autonomia**
8. **Sviluppare app**
9. **Personalizzare**

### Supporto

Per problemi durante la realizzazione:
- Condividi foto/video setup
- Copia output Serial Monitor
- Descrivi comportamento anomalo

---

**Buona costruzione! 🎵🔧**

*Documento v1.0 - Novembre 2025*
*Progetto Lettore Musicale NFC per Bambini*
