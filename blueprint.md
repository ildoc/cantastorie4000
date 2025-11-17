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
7. [Struttura File Audio](#-struttura-file-audio)
8. [Costruzione Scatola](#-costruzione-scatola)
9. [App Bluetooth](#-app-bluetooth)
10. [Testing e Troubleshooting](#-testing-e-troubleshooting)

---

## 🎯 PANORAMICA DEL PROGETTO

### Caratteristiche Principali
- **Controllo NFC**: Tag colorati per selezionare playlist
- **Controlli fisici**: 5 pulsanti arcade (Play/Pausa, Vol+, Vol-, Next, Prev)
- **App Bluetooth**: Gestione remota, associazione tag, upload file audio
- **Autonomia**: 6-8 ore di riproduzione continua
- **Avvio rapido**: 2-3 secondi dall'accensione

### Specifiche Tecniche
- **Microcontrollore**: ESP32 (WiFi + Bluetooth integrati)
- **Audio**: DFPlayer Mini (MP3, WAV, supporta microSD fino a 32GB)
- **NFC**: PN532 (compatibile ISO14443A, range ~5cm)
- **Amplificatore**: PAM8403 classe D (2x3W stereo)
- **Batteria**: 18650 Li-ion 3.7V 3000mAh
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
| **Batteria 18650** | 3.7V 3000mAh protetta | 1-2 | €10 | Marca affidabile (LG, Samsung, Panasonic) |
| **Porta-batteria 18650** | Con fili e interruttore | 1 | €3 | Preferibilmente con switch integrato |
| **Interruttore ON/OFF** | Con LED rosso | 1 | €2 | Tipo rocker switch |
| **Cavo USB-C** | Per ricarica | 1 | €3 | Qualsiasi da 50cm |

### Audio e Controlli

| Componente | Specifiche | Quantità | Prezzo |
|------------|------------|----------|--------|
| **Speaker** | 4Ω 3W, 50mm | 1 | €6 | Preferisci full-range |
| **Pulsanti Arcade** | 30mm, colorati | 5 | €10 | Colori: Rosso, Blu, Verde, Giallo, Bianco |
| **Tag NFC** | NTAG215 o NTAG213 | 10 | €8 | Adesivi o portachiavi |

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

### Strumenti Necessari

- Saldatore (30-40W)
- Multimetro
- Trapano con punte (5mm, 30mm)
- Seghetto per legno
- Carta vetrata (grana 120-220)
- Pinze, cacciaviti

**COSTO TOTALE STIMATO**: €90-120

---

## ⚡ SCHEMA ELETTRICO

### Schema a Blocchi Completo

```
┌─────────────────── ALIMENTAZIONE ───────────────────┐
│                                                      │
│  [Batteria 18650] ──┬──> [TP4056] <── USB-C         │
│      3.7V 3000mAh   │      ↓                        │
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
   │  5V    │        │  5V     │       │   5V     │
   └────────┘        └─────────┘       └──────────┘
        │                  │                  │
        │ TX(17)──────────>│ RX              │
        │ RX(16)──────────<│ TX              │
        │                  │                  │
        │ SDA(21)─────────────────────────────> SDA
        │ SCL(22)─────────────────────────────> SCL
        │                  │                  │
        │ GPIO13 ─────> [Pulsante PLAY]      │
        │ GPIO12 ─────> [Pulsante VOL+]      │
        │ GPIO14 ─────> [Pulsante VOL-]      │
        │ GPIO27 ─────> [Pulsante NEXT]      │
        │ GPIO26 ─────> [Pulsante PREV]      │
        │                  │                  │
        │                  │ SPK+ ──┐         │
        │                  │ SPK- ──┼───> [PAM8403]
        │                  │        │          │
        │                  │        │          ↓
        │                  │        │    [Speaker 4Ω]
        │                  │        │
        └──────────────────┴────────┴──────────┘
```

### Connessioni Dettagliate

#### ESP32 ↔ DFPlayer Mini
```
ESP32          DFPlayer
─────          ────────
GPIO17 (TX) -> RX
GPIO16 (RX) <- TX
5V          -> VCC
GND         -> GND
```

**IMPORTANTE**: Il DFPlayer necessita di una resistenza da 1KΩ tra RX del DFPlayer e TX dell'ESP32 per proteggere il modulo.

#### ESP32 ↔ PN532 (I2C)
```
ESP32          PN532
─────          ─────
GPIO21 (SDA) -> SDA
GPIO22 (SCL) -> SCL
3.3V         -> VCC (usare 3.3V, non 5V!)
GND          -> GND
```

**CONFIGURAZIONE PN532**: Posizionare i jumper per modalità I2C:
- CH1: OFF
- CH2: ON

#### ESP32 ↔ Pulsanti
```
ESP32          Pulsante
─────          ────────
GPIO13      -> PLAY/PAUSA (Rosso)
GPIO12      -> VOL+ (Verde)
GPIO14      -> VOL- (Blu)
GPIO27      -> NEXT (Giallo)
GPIO26      -> PREV (Bianco)
GND         -> Altro terminale pulsante
```

Ogni pulsante va collegato con **pull-up interno** (configurato via software).

#### DFPlayer ↔ PAM8403 ↔ Speaker
```
DFPlayer       PAM8403        Speaker
────────       ───────        ───────
SPK_1 (R+)  -> RIN           
DAC_R       -> (opzionale)
SPK_2 (L+)  -> LIN          -> Speaker+ (canale L)
DAC_L       -> (opzionale)
GND         -> GND          -> Speaker-

PAM8403 VCC -> 5V
```

**NOTA**: Il DFPlayer ha uscita amplificata (SPK_1/SPK_2) ma di bassa potenza. Per maggior volume, usa l'uscita DAC con PAM8403.

#### Alimentazione - Schema Completo
```
Batteria+ ──> [Switch] ──> TP4056 IN+ ──┬──> MT3608 IN+
                           TP4056 OUT+   │
                                         │
USB-C+ ────────────────> TP4056 USB+    │
                                         │
Batteria- ──> TP4056 IN- ────────────────┴──> MT3608 IN-
USB-C-    ──> TP4056 USB-                    
                                         
MT3608 OUT+ (5V regolati) ──> ESP32 5V, DFPlayer VCC, PAM8403 VCC
MT3608 OUT- (GND)         ──> GND comune
```

---

## 🔧 PREPARAZIONE COMPONENTI

### 1. Test Iniziale dei Moduli

#### Test ESP32
1. Collegare ESP32 via USB al PC
2. Aprire Arduino IDE
3. Caricare sketch di test:
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Test OK!");
}

void loop() {
  delay(1000);
}
```
4. Verificare output sul Serial Monitor

#### Test DFPlayer Mini
1. **Preparare microSD**:
   - Formattare in FAT32
   - Creare cartella `/mp3/`
   - Copiare file: `0001.mp3`, `0002.mp3` (rinominare con numerazione a 4 cifre)
   
2. **Collegamento test**:
   - VCC → 5V (power bank)
   - GND → GND
   - SPK_1 → Speaker+
   - SPK_2 → Speaker-
   
3. **Verificare**: dovrebbe riprodurre automaticamente la prima traccia

#### Test PN532
1. Configurare jumper per I2C (CH1=OFF, CH2=ON)
2. Collegare a ESP32 (SDA, SCL, 3.3V, GND)
3. Caricare esempio da libreria `Adafruit_PN532`
4. Avvicinare tag NFC, verificare lettura UID

### 2. Configurazione MT3608 (Step-up)

**PRIMA di collegare l'ESP32**:
1. Collegare input a 3.7V (batteria o alimentatore)
2. Misurare output con multimetro
3. Ruotare potenziometro con cacciavite fino a leggere **5.0V** precisi
4. Segnare posizione con pennarello indelebile

### 3. Test TP4056

1. Collegare batteria 18650 a OUT+ e OUT-
2. Collegare USB-C
3. Verificare:
   - LED rosso acceso = in carica
   - LED blu acceso = carica completa
4. Misurare tensione batteria: deve essere 3.7-4.2V

---

## 🔨 ASSEMBLAGGIO HARDWARE

### FASE 1: Saldature dei Moduli

#### A. Preparare l'ESP32
1. Saldare header maschio (30 pin) se non presenti
2. Verificare continuità con multimetro

#### B. Cablaggio DFPlayer
```
Preparare 4 cavi da 15cm:
- Rosso:   VCC (saldare a pin VCC)
- Nero:    GND (saldare a pin GND)
- Giallo:  TX (via resistenza 1KΩ → RX DFPlayer)
- Verde:   RX (diretto a TX DFPlayer)

Preparare 2 cavi da 10cm per speaker:
- Rosso:   SPK_1
- Nero:    SPK_2
```

**SCHEMA RESISTENZA**:
```
ESP32 TX (GPIO17) ───[ 1KΩ ]─── RX DFPlayer
```

#### C. Cablaggio PN532
```
Preparare 4 cavi da 15cm:
- Rosso:   VCC
- Nero:    GND
- Blu:     SDA
- Verde:   SCL
```

#### D. Pulsanti
Ogni pulsante arcade ha 2 terminali:
1. Saldare cavo a un terminale (GPIO)
2. Saldare cavo GND all'altro terminale
3. Usare cavi di colore corrispondente al pulsante

**SUGGERIMENTO**: Crea un cavo GND comune con 5 diramazioni usando saldature o morsetti Wago.

### FASE 2: Assemblaggio Breadboard di Test

Prima di saldare definitivamente, testare su breadboard:

1. **Inserire ESP32** al centro della breadboard
2. **Collegare alimentazione**:
   - Rail + → 5V (da MT3608 o USB)
   - Rail - → GND
3. **Collegare moduli uno alla volta**:
   - Prima DFPlayer → testare audio
   - Poi PN532 → testare lettura tag
   - Infine pulsanti → testare input
4. **Caricare software di test** (vedi sezione Software)

### FASE 3: Assemblaggio Definitivo

#### Schema di Montaggio nella Scatola

```
Vista dall'alto della scatola (15x10cm):

┌────────────────────────────────────┐
│  [●]                    [Griglia]  │  ← Speaker + griglia
│ LED                      Speaker   │
│                                    │
│  [○] [○] [○] [○] [○]              │  ← 5 pulsanti arcade
│ PLAY  +   -  NEXT PREV             │
│                                    │
│        ┌─────────────┐             │
│        │   Antenna   │             │  ← PN532 (zona NFC)
│        │     NFC     │             │
│        └─────────────┘             │
│                                    │
│  Componenti interni sotto:         │
│  - ESP32                           │
│  - DFPlayer                        │
│  - PAM8403                         │
│  - MT3608                          │
│  - TP4056                          │
│  - Batteria 18650                  │
│                                    │
│              [USB-C] [Switch]      │  ← Ricarica + accensione
└────────────────────────────────────┘
```

#### Ordine di Montaggio

1. **Preparare la scatola** (vedi sezione Costruzione Scatola)

2. **Montare componenti sul pannello frontale**:
   - Forare 5 fori da 30mm per pulsanti
   - Forare 1 foro da 60mm per speaker
   - Montare speaker con viti M3 o colla a caldo
   - Inserire pulsanti arcade (si fissano a scatto)
   - Fissare griglia protettiva

3. **Montare sul pannello laterale**:
   - Forare per interruttore ON/OFF
   - Forare per porta USB-C (TP4056)
   - Montare LED di stato (collegato a TP4056)

4. **Layout interno** (su base in compensato con distanziali):
```
Disposizione ottimale:

[Batteria 18650]    [MT3608]
                    [TP4056]

[ESP32]            [DFPlayer]
                   [MicroSD]

[PN532] (sotto pulsanti)
[PAM8403] (vicino speaker)
```

5. **Fissaggio componenti**:
   - ESP32: 4 viti M3 con distanziali
   - Altri moduli: colla a caldo (lasciare possibilità di rimozione)
   - Batteria: supporto dedicato o velcro forte
   - Cavi: organizzare con fascette

6. **Routing cavi**:
   - Tenere separati cavi di potenza da quelli segnale
   - Cavi speaker lontani da ESP32 (interferenze)
   - Lasciare cavi abbastanza lunghi per manutenzione

### FASE 4: Test Pre-Chiusura

**PRIMA di chiudere la scatola**:

1. ✅ Verificare tutte le connessioni con multimetro
2. ✅ Testare ogni pulsante (Serial Monitor)
3. ✅ Testare NFC con 3-4 tag diversi
4. ✅ Riprodurre audio a volume massimo per 5 minuti
5. ✅ Verificare temperatura ESP32 e PAM8403 (non >60°C)
6. ✅ Testare ricarica batteria (LED TP4056)
7. ✅ Misurare autonomia (almeno 2 ore test)

---

## 💻 CONFIGURAZIONE SOFTWARE

### Installazione Arduino IDE

1. Scaricare Arduino IDE 2.x da arduino.cc
2. **Installare supporto ESP32**:
   - File → Preferences
   - URL boards manager: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools → Board → Boards Manager
   - Cercare "ESP32" by Espressif
   - Installare versione 2.0.14 o superiore

3. **Installare librerie**:
   - Tools → Manage Libraries
   - Installare:
     - `DFRobotDFPlayerMini` (1.0.6+)
     - `Adafruit PN532` (1.4.0+)
     - `ArduinoJson` (6.21.0+)

### Codice Principale (Sketch)

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
#define BTN_VOL_UP 12
#define BTN_VOL_DOWN 14
#define BTN_NEXT 27
#define BTN_PREV 26

// Objects
HardwareSerial DFSerial(1);
DFRobotDFPlayerMini dfPlayer;
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);
BluetoothSerial SerialBT;
Preferences preferences;

// Variables
int currentVolume = 15; // 0-30
bool isPlaying = false;
String lastTagUID = "";
unsigned long lastTagTime = 0;
const int TAG_DEBOUNCE = 2000; // ms

// Button states
bool lastPlayState = HIGH;
bool lastVolUpState = HIGH;
bool lastVolDownState = HIGH;
bool lastNextState = HIGH;
bool lastPrevState = HIGH;

void setup() {
  Serial.begin(115200);
  Serial.println("Lettore Musicale NFC - Avvio...");
  
  // Init buttons
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_VOL_UP, INPUT_PULLUP);
  pinMode(BTN_VOL_DOWN, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  
  // Init DFPlayer
  DFSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX, DFPLAYER_TX);
  delay(1000);
  
  if (!dfPlayer.begin(DFSerial)) {
    Serial.println("Errore DFPlayer!");
    while(true) { delay(1000); }
  }
  
  Serial.println("DFPlayer OK");
  dfPlayer.volume(currentVolume);
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
  // Init NFC
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
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
  checkNFC();
  checkBluetooth();
  delay(50);
}

void checkButtons() {
  // Play/Pause
  bool playState = digitalRead(BTN_PLAY);
  if (playState == LOW && lastPlayState == HIGH) {
    delay(50); // debounce
    if (isPlaying) {
      dfPlayer.pause();
      isPlaying = false;
      Serial.println("Pausa");
    } else {
      dfPlayer.start();
      isPlaying = true;
      Serial.println("Play");
    }
  }
  lastPlayState = playState;
  
  // Volume Up
  bool volUpState = digitalRead(BTN_VOL_UP);
  if (volUpState == LOW && lastVolUpState == HIGH) {
    delay(50);
    if (currentVolume < 30) {
      currentVolume++;
      dfPlayer.volume(currentVolume);
      preferences.putInt("volume", currentVolume);
      Serial.print("Volume: ");
      Serial.println(currentVolume);
    }
  }
  lastVolUpState = volUpState;
  
  // Volume Down
  bool volDownState = digitalRead(BTN_VOL_DOWN);
  if (volDownState == LOW && lastVolDownState == HIGH) {
    delay(50);
    if (currentVolume > 0) {
      currentVolume--;
      dfPlayer.volume(currentVolume);
      preferences.putInt("volume", currentVolume);
      Serial.print("Volume: ");
      Serial.println(currentVolume);
    }
  }
  lastVolDownState = volDownState;
  
  // Next
  bool nextState = digitalRead(BTN_NEXT);
  if (nextState == LOW && lastNextState == HIGH) {
    delay(50);
    dfPlayer.next();
    Serial.println("Next track");
  }
  lastNextState = nextState;
  
  // Previous
  bool prevState = digitalRead(BTN_PREV);
  if (prevState == LOW && lastPrevState == HIGH) {
    delay(50);
    dfPlayer.previous();
    Serial.println("Previous track");
  }
  lastPrevState = prevState;
}

void checkNFC() {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) {
    String tagUID = "";
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) tagUID += "0";
      tagUID += String(uid[i], HEX);
    }
    tagUID.toUpperCase();
    
    // Debounce: ignore same tag within 2 seconds
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
  // Load folder associated with this tag
  String key = "tag_" + uid;
  int folder = preferences.getInt(key.c_str(), 0);
  
  if (folder > 0) {
    Serial.print("Riproduzione cartella: ");
    Serial.println(folder);
    dfPlayer.loopFolder(folder);
    isPlaying = true;
  } else {
    Serial.println("Tag non associato");
    // Play error sound or beep
    dfPlayer.playMp3Folder(9999); // file speciale per errore
  }
}

void checkBluetooth() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();
    
    handleBluetoothCommand(command);
  }
}

void handleBluetoothCommand(String cmd) {
  Serial.print("BT Command: ");
  Serial.println(cmd);
  
  // Parse JSON command (esempio: {"cmd":"play","folder":1})
  // Implementazione completa nella sezione App
  
  if (cmd.startsWith("PLAY")) {
    dfPlayer.start();
    isPlaying = true;
    SerialBT.println("OK");
  }
  else if (cmd.startsWith("PAUSE")) {
    dfPlayer.pause();
    isPlaying = false;
    SerialBT.println("OK");
  }
  else if (cmd.startsWith("VOL:")) {
    int vol = cmd.substring(4).toInt();
    if (vol >= 0 && vol <= 30) {
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
    // Formato: ASSOCIATE:TAG_UID:FOLDER_NUM
    // Es: ASSOCIATE:04A1B2C3:1
    int sep1 = cmd.indexOf(':', 11);
    String tagUID = cmd.substring(11, sep1);
    int folder = cmd.substring(sep1 + 1).toInt();
    
    String key = "tag_" + tagUID;
    preferences.putInt(key.c_str(), folder);
    
    SerialBT.print("Associated ");
    SerialBT.print(tagUID);
    SerialBT.print(" to folder ");
    SerialBT.println(folder);
  }
  else if (cmd == "STATUS") {
    SerialBT.print("Volume:");
    SerialBT.print(currentVolume);
    SerialBT.print(",Playing:");
    SerialBT.println(isPlaying ? "1" : "0");
  }
}
```

### Upload del Codice

1. Collegare ESP32 via USB
2. Tools → Board → ESP32 Dev Module
3. Tools → Port → (selezionare porta COM/ttyUSB)
4. Tools → Upload Speed → 115200
5. Sketch → Upload
6. Aprire Serial Monitor (115200 baud) per verificare output

---

## 📁 STRUTTURA FILE AUDIO

### Organizzazione MicroSD

Il DFPlayer richiede una struttura precisa:

```
📁 SD Card (FAT32)
│
├── 📁 01/                    ← Cartella 1: Fiabe della buonanotte
│   ├── 001.mp3
│   ├── 002.mp3
│   └── 003.mp3
│
├── 📁 02/                    ← Cartella 2: Canzoni allegre
│   ├── 001.mp3
│   ├── 002.mp3
│   └── 003.mp3
│
├── 📁 03/                    ← Cartella 3: Musica classica
│   ├── 001.mp3
│   └── 002.mp3
│
├── 📁 mp3/                   ← File singoli (opzionale)
│   ├── 0001.mp3              ← Suono di errore
│   └── 0002.mp3
│
└── 📁 advert/                ← Annunci (opzionale)
    ├── 0001.mp3
    └── 0002.mp3
```

### Regole di Nomenclatura

**IMPORTANTE**: Il DFPlayer è molto esigente con i nomi dei file:

1. **Cartelle numerate**: `01`, `02`, `03`, ... `99` (sempre 2 cifre)
2. **File numerati**: `001.mp3`, `002.mp3`, ... `999.mp3` (sempre 3 cifre)
3. **NO spazi** nei nomi
4. **NO caratteri speciali** (accenti, simboli)
5. **Formato audio**: MP3 (128-320kbps), WAV (16bit, 44.1kHz)

### Esempio di Associazione Tag NFC

```
Tag Blu (#04A1B2C3)    → Cartella 01 → Fiabe della buonanotte
Tag Rosso (#05C3D4E5)  → Cartella 02 → Canzoni allegre
Tag Verde (#06E5F6A7)  → Cartella 03 → Musica classica
Tag Giallo (#07A8B9C0) → Cartella 04 → Ninna nanne
Tag Viola (#08D1E2F3)  → Cartella 05 → Suoni della natura
```

### Preparazione File Audio

**Tool consigliati**:
- **Audacity** (gratuito): conversione, taglio, normalizzazione
- **fre:ac** (gratuito): conversione batch

**Procedura**:
1. Convertire tutti i file in MP3 128-192kbps
2. Normalizzare volume a -3dB (evita clipping)
3. Rinominare con numerazione corretta
4. Copiare su microSD formattata FAT32
5. **Espellere correttamente** la scheda

---

## 📦 COSTRUZIONE SCATOLA

### Materiali e Dimensioni

**Dimensioni consigliate**: 15cm x 10cm x 5cm (L x P x H)

**Componenti legno**:
- 2 pannelli 15x10cm (fronte/retro)
- 2 pannelli 15x5cm (lati lunghi)
- 2 pannelli 10x5cm (lati corti)
- 1 base interna 13x8cm (per fissare componenti)

### Schema di Taglio

```
Vista esplosa della scatola:

        ┌─────────────────┐
        │   PANNELLO      │  15x10cm (fronte)
        │   FRONTALE      │  
        │  ●●●●●  ◉      │  ← Pulsanti + speaker
        └─────────────────┘

┌───────┐                 ┌───────┐
│ LATO  │                 │ LATO  │  15x5cm
│ SX    │                 │ DX    │
└───────┘                 └───────┘

        ┌─────────────────┐
        │  BASE INTERNA   │  13x8cm
        │  (componenti)   │  
        └─────────────────┘

┌───────┐                 ┌───────┐
│ LATO  │                 │ LATO  │  10x5cm
│ TOP   │                 │ BOTTOM│
└───────┘                 └───────┘

        ┌─────────────────┐
        │   PANNELLO      │  15x10cm (retro)
        │   POSTERIORE    │  ← USB-C + Switch
        └─────────────────┘
```

### Procedura di Costruzione

#### FASE 1: Foratura Pannello Frontale

**Pulsanti** (5 fori da 30mm):
```
Disposizione (vista dall'alto):

    25mm     25mm    25mm    25mm    25mm
  ┌──●───────●───────●───────●───────●──┐
  │ PLAY   VOL+   VOL-   NEXT   PREV    │
  │                                      │
  │         3cm dal bordo superiore      │
  └──────────────────────────────────────┘
  
  Spaziatura orizzontale: 25mm tra centri
```

**Speaker** (1 foro da 60mm):
- Posizione: angolo alto a destra
- 20mm dal bordo destro
- 15mm dal bordo superiore

**Strumenti**:
- Fresa a tazza da 30mm per pulsanti
- Fresa a tazza da 60mm per speaker
- Carta vetrata per rifinire

#### FASE 2: Foratura Pannello Posteriore

**USB-C** (foro rettangolare 10x4mm):
- 30mm dal bordo sinistro
- Al centro in altezza

**Switch ON/OFF** (foro da 12mm):
- 60mm dal bordo sinistro
- Al centro in altezza

**LED di stato** (foro da 5mm):
- 50mm dal bordo sinistro
- 10mm sopra lo switch

#### FASE 3: Assemblaggio Scatola

1. **Levigare tutti i bordi** con carta vetrata grana 120
2. **Pre-assemblaggio a secco** per verificare combaciamento
3. **Incollaggio**:
   - Colla vinilica per legno sui bordi
   - Chiodini sottili (15mm) per rinforzo
   - Morsetti per 2-4 ore
4. **Finitura**:
   - Carta vetrata grana 220 su tutta la superficie
   - Arrotondare angoli e spigoli
5. **Verniciatura** (opzionale):
   - 2 mani di vernice acrilica atossica
   - Asciugatura 24h tra le mani
   - Finitura con cera d'api

#### FASE 4: Montaggio Base Interna

```
Vista dall'alto base interna (13x8cm):

┌─────────────────────────────────────┐
│  [ESP32]          [DFPlayer+SD]     │
│   con distanziali   con distanziali │
│                                     │
│  [TP4056]          [MT3608]         │
│   colla a caldo    colla a caldo    │
│                                     │
│        [Batteria 18650]             │
│        con supporto/velcro          │
│                                     │
│  [PN532]           [PAM8403]        │
│  sotto pulsanti    vicino speaker   │
└─────────────────────────────────────┘
```

**Fissaggio**:
1. ESP32 e DFPlayer: 4 viti M3 con distanziali 10mm
2. Altri moduli: colla a caldo (rimovibile con calore)
3. Batteria: supporto in plastica o velcro industriale
4. Base interna fissata alla scatola con 4 viti dal basso

---

## 📱 APP BLUETOOTH

### Architettura dell'App

L'app comunica con ESP32 via Bluetooth Serial (SPP) usando comandi testuali o JSON.

### Protocollo Comunicazione

#### Comandi dall'App all'ESP32

| Comando | Formato | Descrizione |
|---------|---------|-------------|
| Play | `PLAY` | Avvia riproduzione |
| Pausa | `PAUSE` | Mette in pausa |
| Volume | `VOL:15` | Imposta volume (0-30) |
| Cartella | `FOLDER:2` | Riproduce cartella 2 |
| Traccia | `TRACK:5` | Riproduce traccia 5 |
| Associa Tag | `ASSOCIATE:04A1B2:3` | Associa tag a cartella 3 |
| Stato | `STATUS` | Richiede stato attuale |
| Scansione NFC | `SCAN_NFC` | Attiva modalità lettura tag |
| Lista Tag | `LIST_TAGS` | Elenca tag memorizzati |

#### Risposte dall'ESP32 all'App

```
OK                           ← Comando eseguito
ERROR:messaggio              ← Errore
STATUS:vol=15,playing=1      ← Stato corrente
TAG_DETECTED:04A1B2C3        ← Tag NFC letto
TAG_LIST:04A1B2:1,05C3D4:2   ← Lista associazioni
```

### Codice ESP32 Aggiornato (Gestione Upload File)

**NOTA**: L'ESP32 non può scrivere direttamente sulla microSD del DFPlayer, ma può:
1. Ricevere file via Bluetooth
2. Salvarli in SPIFFS/LittleFS (4MB flash)
3. Istruire l'utente a copiarli su SD

```cpp
// Aggiungi all'inizio del codice principale:
#include <SPIFFS.h>

// In setup():
void setup() {
  // ... codice esistente ...
  
  if (!SPIFFS.begin(true)) {
    Serial.println("Errore SPIFFS");
  } else {
    Serial.println("SPIFFS OK");
  }
}

// Nuova funzione per ricevere file:
void receiveAudioFile() {
  if (SerialBT.available() < 4) return;
  
  // Protocollo: FILE:filename.mp3:SIZE:12345\n[dati binari]
  String header = SerialBT.readStringUntil('\n');
  
  if (header.startsWith("FILE:")) {
    int sep1 = header.indexOf(':', 5);
    int sep2 = header.indexOf(':', sep1 + 1);
    
    String filename = header.substring(5, sep1);
    int fileSize = header.substring(sep2 + 1).toInt();
    
    Serial.print("Ricevo file: ");
    Serial.print(filename);
    Serial.print(" - ");
    Serial.print(fileSize);
    Serial.println(" bytes");
    
    File file = SPIFFS.open("/" + filename, FILE_WRITE);
    
    int received = 0;
    uint8_t buffer[512];
    
    while (received < fileSize) {
      int toRead = min(512, fileSize - received);
      int actualRead = SerialBT.readBytes(buffer, toRead);
      file.write(buffer, actualRead);
      received += actualRead;
      
      // Invia progresso
      if (received % 5000 == 0) {
        SerialBT.print("PROGRESS:");
        SerialBT.println((received * 100) / fileSize);
      }
    }
    
    file.close();
    SerialBT.println("FILE_COMPLETE");
    Serial.println("File ricevuto!");
    
    // Istruzioni per l'utente
    SerialBT.println("INFO:Copia il file su microSD nella cartella corretta");
  }
}

// Aggiungi in loop():
void loop() {
  checkButtons();
  checkNFC();
  checkBluetooth();
  receiveAudioFile(); // ← Nuova funzione
  delay(50);
}
```

### Struttura App Mobile (React Native / Flutter)

**Schermata Principale**:
```
┌────────────────────────────────────┐
│  🔊 Lettore Musicale                │
│                                    │
│  ┌──────────────┐                 │
│  │ Connesso ●   │  [Disconnetti] │
│  └──────────────┘                 │
│                                    │
│  ━━━━━━━━━━━━━  Volume: 15/30     │
│  [−]         [━━━━━━━━━━]      [+]│
│                                    │
│  ▶ PLAY    ⏸ PAUSE    ⏹ STOP     │
│  ⏮ PREV    ⏭ NEXT                 │
│                                    │
│  📂 CARTELLE:                      │
│  ┌────────────────────────────────┐│
│  │ 📁 01 - Fiabe      [▶]        ││
│  │ 📁 02 - Canzoni    [▶]        ││
│  │ 📁 03 - Classica   [▶]        ││
│  │ 📁 04 - Ninna nanne [▶]       ││
│  └────────────────────────────────┘│
│                                    │
│  [📋 Gestione Tag NFC]             │
│  [⬆ Carica File Audio]            │
└────────────────────────────────────┘
```

**Schermata Gestione Tag NFC**:
```
┌────────────────────────────────────┐
│  ← Gestione Tag NFC                │
│                                    │
│  🔍 SCANSIONE TAG:                 │
│  ┌────────────────────────────────┐│
│  │                                ││
│  │    Avvicina un tag NFC...     ││
│  │                                ││
│  │         📡                     ││
│  └────────────────────────────────┘│
│                                    │
│  TAG MEMORIZZATI:                  │
│  ┌────────────────────────────────┐│
│  │ 🔵 Tag Blu                     ││
│  │    ID: 04A1B2C3                ││
│  │    → Cartella 01 (Fiabe)       ││
│  │    [✏ Modifica] [🗑 Elimina]   ││
│  │                                ││
│  │ 🔴 Tag Rosso                   ││
│  │    ID: 05C3D4E5                ││
│  │    → Cartella 02 (Canzoni)     ││
│  │    [✏ Modifica] [🗑 Elimina]   ││
│  └────────────────────────────────┘│
│                                    │
│  [+ Associa Nuovo Tag]             │
└────────────────────────────────────┘
```

### Codice App di Esempio (Flutter)

```dart
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class MusicPlayerApp extends StatefulWidget {
  @override
  _MusicPlayerAppState createState() => _MusicPlayerAppState();
}

class _MusicPlayerAppState extends State<MusicPlayerApp> {
  BluetoothConnection? connection;
  int volume = 15;
  bool isPlaying = false;
  String lastTag = "";

  void connectBluetooth() async {
    try {
      connection = await BluetoothConnection.toAddress("XX:XX:XX:XX:XX:XX");
      connection!.input!.listen((data) {
        String response = String.fromCharCodes(data);
        handleResponse(response);
      });
    } catch (e) {
      print("Errore connessione: $e");
    }
  }

  void sendCommand(String cmd) {
    if (connection != null && connection!.isConnected) {
      connection!.output.add(ascii.encode(cmd + "\n"));
    }
  }

  void handleResponse(String response) {
    if (response.startsWith("TAG_DETECTED:")) {
      setState(() {
        lastTag = response.substring(13);
      });
      showDialog(
        context: context,
        builder: (context) => AlertDialog(
          title: Text("Tag NFC Rilevato"),
          content: Text("UID: $lastTag\n\nAssocia a quale cartella?"),
          actions: [
            TextButton(
              child: Text("Cartella 1"),
              onPressed: () {
                sendCommand("ASSOCIATE:$lastTag:1");
                Navigator.pop(context);
              },
            ),
            // Altri pulsanti...
          ],
        ),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Lettore Musicale")),
      body: Column(
        children: [
          // Volume slider
          Slider(
            value: volume.toDouble(),
            min: 0,
            max: 30,
            divisions: 30,
            onChanged: (val) {
              setState(() => volume = val.toInt());
              sendCommand("VOL:$volume");
            },
          ),
          
          // Pulsanti controllo
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              IconButton(
                icon: Icon(isPlaying ? Icons.pause : Icons.play_arrow),
                iconSize: 64,
                onPressed: () {
                  sendCommand(isPlaying ? "PAUSE" : "PLAY");
                  setState(() => isPlaying = !isPlaying);
                },
              ),
              IconButton(
                icon: Icon(Icons.skip_previous),
                iconSize: 48,
                onPressed: () => sendCommand("PREV"),
              ),
              IconButton(
                icon: Icon(Icons.skip_next),
                iconSize: 48,
                onPressed: () => sendCommand("NEXT"),
              ),
            ],
          ),
          
          // Lista cartelle
          Expanded(
            child: ListView.builder(
              itemCount: 5,
              itemBuilder: (context, index) {
                return ListTile(
                  leading: Icon(Icons.folder),
                  title: Text("Cartella ${index + 1}"),
                  trailing: IconButton(
                    icon: Icon(Icons.play_circle),
                    onPressed: () => sendCommand("FOLDER:${index + 1}"),
                  ),
                );
              },
            ),
          ),
          
          // Pulsanti gestione
          ElevatedButton(
            child: Text("Gestione Tag NFC"),
            onPressed: () {
              sendCommand("SCAN_NFC");
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => NFCManagementScreen()),
              );
            },
          ),
        ],
      ),
    );
  }
}
```

### Alternative Semplificate

Se non vuoi sviluppare un'app custom, puoi usare:

1. **Serial Bluetooth Terminal** (Android/iOS gratuita)
   - Invia comandi testuali direttamente
   - Ricevi risposte in tempo reale

2. **MIT App Inventor** (online, drag & drop)
   - Crea app senza codice
   - Componenti Bluetooth già pronti

3. **Web Bluetooth API** (Chrome mobile)
   - App web che si connette via Bluetooth
   - Nessuna installazione richiesta

---

## 🔍 TESTING E TROUBLESHOOTING

### Checklist Test Completa

#### ✅ Test 1: Alimentazione

**Procedura**:
1. Staccare ESP32 e tutti i moduli
2. Collegare solo batteria → TP4056 → MT3608
3. Misurare output MT3608: deve essere 5.0V ± 0.1V
4. Collegare carico (LED + resistenza 220Ω)
5. Misurare corrente: batteria deve erogare >500mA

**Problemi comuni**:
- Output MT3608 instabile → verificare saldature, regolare potenziometro
- Batteria si scarica velocemente → verificare consumo moduli, controllare cortocircuiti

#### ✅ Test 2: ESP32

**Procedura**:
1. Collegare solo ESP32 a 5V
2. Caricare sketch di test con Serial.println()
3. Verificare output su Serial Monitor
4. Testare ogni GPIO con LED

**Problemi comuni**:
- ESP32 non si connette → premere BOOT durante upload
- Reset continui → alimentazione insufficiente, aggiungere condensatore 100µF

#### ✅ Test 3: DFPlayer

**Procedura**:
1. Inserire microSD con file test (001.mp3, 002.mp3)
2. Collegare speaker direttamente a SPK_1/SPK_2
3. Alimentare a 5V
4. Dovrebbe riprodurre automaticamente prima traccia

**Problemi comuni**:
- Nessun audio → verificare formato file (MP3 128kbps), nome file (001.mp3)
- Audio distorto → verificare impedenza speaker (4Ω), ridurre volume
- Non legge SD → formattare FAT32, max 32GB, classe 10

#### ✅ Test 4: NFC

**Procedura**:
1. Configurare PN532 in modalità I2C (jumper)
2. Collegare a ESP32 (SDA, SCL, 3.3V!, GND)
3. Caricare esempio Adafruit_PN532
4. Avvicinare tag NTAG215

**Problemi comuni**:
- Non rileva tag → verificare jumper I2C, controllare alimentazione 3.3V (NO 5V!)
- Lettura intermittente → avvicinare tag <3cm, aggiungere condensatore 10µF su VCC

#### ✅ Test 5: Bluetooth

**Procedura**:
1. Caricare sketch con SerialBT.begin("LettoreMusicale")
2. Cercare dispositivo su smartphone
3. Connettere e inviare comando "STATUS"
4. Verificare risposta

**Problemi comuni**:
- Non visibile → verificare nome dispositivo, riavviare ESP32
- Disconnessioni → ridurre distanza, verificare alimentazione stabile

#### ✅ Test 6: Sistema Completo

**Procedura**:
1. Assembrare tutto nella scatola (non chiudere ancora)
2. Accendere con batteria
3. Testare sequenza:
   - Avvio automatico (LED acceso)
   - Lettura tag NFC (suono avvio)
   - Pulsanti (play, volume, next/prev)
   - App Bluetooth (connessione, comandi)
4. Misurare autonomia: almeno 5 ore

### Problemi Comuni e Soluzioni

| Problema | Causa Probabile | Soluzione |
|----------|-----------------|-----------|
| ESP32 si riavvia | Alimentazione insufficiente | Aggiungere condensatore 470µF su 5V |
| Audio distorto | Impedenza speaker sbagliata | Usare speaker 4Ω, non 8Ω |
| NFC non legge | Alimentazione 5V invece di 3.3V | Collegare PN532 a pin 3.3V ESP32 |
| Batteria dura poco | Consumo eccessivo PAM8403 | Ridurre volume massimo via software |
| Tag non riconosciuti | Memoria piena | Usare Preferences con namespace |
| MicroSD non letta | Formato sbagliato | Formattare FAT32, non exFAT |
| Bluetooth disconnette | Interferenze WiFi | Disabilitare WiFi: `WiFi.mode(WIFI_OFF)` |

### Ottimizzazioni Finali

**Ridurre consumo energetico**:
```cpp
// Aggiungi in setup():
setCpuFrequencyMhz(80); // Riduce da 240MHz a 80MHz
WiFi.mode(WIFI_OFF);     // Disabilita WiFi

// Aggiungi in loop() quando non riproduce:
if (!isPlaying) {
  delay(500); // Rallenta loop
}
```

**Migliorare qualità audio**:
```cpp
// In setup():
dfPlayer.EQ(DFPLAYER_EQ_BASS);     // Più bassi
dfPlayer.outputDevice(DFPLAYER_DEVICE_SD); // Forza SD
```

**Protezione volume per bambini**:
```cpp
const int MAX_SAFE_VOLUME = 20; // Limita a 20/30

void setVolume(int vol) {
  vol = constrain(vol, 0, MAX_SAFE_VOLUME);
  dfPlayer.volume(vol);
}
```

---

## 🚀 MIGLIORAMENTI FUTURI

### Funzionalità Avanzate

1. **Sleep Mode**: Spegnimento automatico dopo 10 minuti di inattività
2. **Timer**: "Tra 20 minuti spegni musica" (per addormentamento)
3. **Sequenze**: Tag speciale che riproduce playlist mista
4. **Registrazione**: Microfono per registrare messaggi vocali
5. **WiFi Sync**: Sincronizza playlist con cloud via WiFi
6. **Display OLED**: Mostra titolo traccia e volume
7. **Sensore di movimento**: Pausa automatica quando viene capovolto
8. **Multi-lingua**: Supporto per audio in diverse lingue

### Hardware Opzionale

| Componente | Funzione | Costo |
|------------|----------|-------|
| **Display OLED 0.96"** | Info traccia/volume | €5 |
| **Microfono MAX4466** | Registrazione vocale | €4 |
| **LED RGB WS2812** | Effetti luminosi | €3 |
| **Sensore MPU6050** | Rilevamento movimento | €4 |
| **Modulo RTC DS3231** | Timer e spegnimento programmato | €3 |

---

## 📚 RISORSE E RIFERIMENTI

### Documentazione Tecnica

- **ESP32**: https://docs.espressif.com/projects/esp-idf/
- **DFPlayer Mini**: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
- **PN532**: https://www.nxp.com/docs/en/user-guide/141520.pdf
- **Arduino IDE**: https://docs.arduino.cc/

### Librerie

```cpp
// In Arduino IDE → Manage Libraries:
DFRobotDFPlayerMini v1.0.6+
Adafruit_PN532 v1.4.0+
ArduinoJson v6.21.0+
```

### Community e Supporto

- **Forum Arduino**: https://forum.arduino.cc/
- **ESP32 Reddit**: https://www.reddit.com/r/esp32/
- **Stack Overflow**: Tag `esp32`, `dfplayer`, `nfc`

### Video Tutorial Consigliati

1. "ESP32 DFPlayer Mini Tutorial" - YouTube
2. "PN532 NFC Module with Arduino" - YouTube
3. "Building a battery powered ESP32" - YouTube

---

## 🎉 CONCLUSIONI

### Cosa Hai Ottenuto

✅ Un lettore musicale funzionale e robusto  
✅ Controllo intuitivo tramite tag NFC colorati  
✅ App Bluetooth per gestione avanzata  
✅ Autonomia di 6-8 ore  
✅ Sistema espandibile e personalizzabile  
✅ Costo contenuto (~€100)  

### Prossimi Passi

1. **Ordina i componenti** (vedi lista sopra)
2. **Testa ogni modulo singolarmente** prima dell'assemblaggio finale
3. **Assembla il prototipo** su breadboard
4. **Carica il software** e verifica tutte le funzioni
5. **Costruisci la scatola** e assembla definitivamente
6. **Testa l'autonomia** con sessioni di 3-4 ore
7. **Sviluppa l'app** (o usa Serial Terminal)
8. **Personalizza** con adesivi, colori, nome del bambino

### Supporto

Per domande o problemi durante la realizzazione:
- Condividi foto/video del setup
- Copia l'output del Serial Monitor
- Descrivi il comportamento anomalo

**Buona costruzione! 🎵🔧**

---

*Documento creato per progetto Lettore Musicale NFC*  
*Versione 1.0 - Novembre 2025*
