# ⚡ Guida Completa Elettronica - Lettore Musicale NFC

## 📋 INDICE

1. [Panoramica Sistema](#-panoramica-sistema)
2. [Componenti Dettagliati](#-componenti-dettagliati)
3. [Schema Elettrico Completo](#-schema-elettrico-completo)
4. [Alimentazione e Gestione Batteria](#-alimentazione-e-gestione-batteria)
5. [Connessioni Dettagliate](#-connessioni-dettagliate)
6. [Preparazione e Test Componenti](#-preparazione-e-test-componenti)
7. [Saldature e Assemblaggio](#-saldature-e-assemblaggio)
8. [Layout PCB e Scatola](#-layout-pcb-e-scatola)
9. [Troubleshooting Hardware](#-troubleshooting-hardware)
10. [Ottimizzazioni Energetiche](#-ottimizzazioni-energetiche)

---

## 🎯 PANORAMICA SISTEMA

### Architettura Hardware

Il sistema è composto da 5 blocchi principali:

1. **Sistema di Alimentazione**: Batteria 18650 → TP4056 → MT3608 → 5V Rail
2. **Microcontrollore**: ESP32 (gestione logica, Bluetooth, I/O)
3. **Sistema Audio**: DFPlayer Mini → PAM8403 → Speaker
4. **Sistema NFC**: PN532 (lettura tag)
5. **Interfaccia Utente**: Pulsanti, potenziometro, LED

### Flusso Segnale

```
Tag NFC → PN532 → ESP32 → DFPlayer → PAM8403 → Speaker
Pulsanti → ESP32 → DFPlayer
Potenziometro → ESP32 (ADC) → DFPlayer (volume)
Batteria → Partitore → ESP32 (ADC) → LED indicatore
```

---

## 🔌 COMPONENTI DETTAGLIATI

### ESP32 DevKit V1

**Specifiche**:
- **Chip**: ESP32-WROOM-32
- **CPU**: Dual-core 240MHz
- **Memoria**: 520KB SRAM, 4MB Flash
- **WiFi**: 802.11 b/g/n
- **Bluetooth**: 4.2 BR/EDR + BLE
- **GPIO**: 30 pin disponibili
- **ADC**: 12-bit (0-3.3V)
- **Alimentazione**: 5V via USB o VIN, 3.3V regolato interno

**Pin Utilizzati**:
- GPIO16: RX DFPlayer (HardwareSerial)
- GPIO17: TX DFPlayer (HardwareSerial)
- GPIO21: SDA I2C (PN532)
- GPIO22: SCL I2C (PN532)
- GPIO13: Pulsante PLAY
- GPIO27: Pulsante NEXT
- GPIO26: Pulsante PREV
- GPIO34: Potenziometro Volume (ADC - solo input)
- GPIO35: Monitoraggio Batteria (ADC - solo input)
- GPIO25: LED Rosso
- GPIO33: LED Verde

**Note Importanti**:
- GPIO34 e GPIO35 sono **solo input** (non hanno pull-up interni)
- Usare pull-up esterni se necessario
- GPIO16/17 sono dedicati a HardwareSerial(1)

### DFPlayer Mini

**Specifiche**:
- **Chip**: YX5200-24SS
- **Formati supportati**: MP3, WAV
- **MicroSD**: fino a 32GB FAT32
- **Uscita audio**: DAC stereo, 3W per canale
- **Comunicazione**: Serial UART 9600 baud
- **Alimentazione**: 3.2V - 5V (consigliato 5V)
- **Consumo**: 20-50mA idle, 80-120mA riproduzione

**Pin Principali**:
- VCC: 5V
- GND: Massa
- RX: Riceve comandi (collegare a TX ESP32 via 1KΩ)
- TX: Invia stato (collegare a RX ESP32)
- SPK_1: Uscita audio canale destro
- SPK_2: Uscita audio canale sinistro
- BUSY: Segnale occupato (opzionale)

**⚠️ ATTENZIONE**:
- **SEMPRE** usare resistenza 1KΩ tra TX ESP32 e RX DFPlayer
- Senza resistenza, il DFPlayer può danneggiarsi
- Il DFPlayer richiede 5V stabili (non 3.3V)

**Struttura File SD**:
```
/mp3/          → File singoli (0001.mp3, 0002.mp3...)
/01/           → Cartella 1 (001.mp3, 002.mp3...)
/02/           → Cartella 2
...
/99/           → Cartella 99
```

### PN532 NFC Module

**Specifiche**:
- **Chip**: PN532
- **Protocolli**: ISO14443A/B, FeliCa, ISO18092
- **Range**: ~5cm
- **Interfacce**: I2C, SPI, UART (configurabile via jumper)
- **Alimentazione**: 3.3V (⚠️ NON 5V!)
- **Consumo**: 80-150mA durante lettura

**Configurazione Jumper**:
- **Modalità I2C** (consigliata):
  - CH1: OFF
  - CH2: ON
- **Modalità SPI**:
  - CH1: ON
  - CH2: OFF
- **Modalità UART**:
  - CH1: ON
  - CH2: ON

**Pin I2C**:
- VCC: 3.3V (collegare a ESP32 3.3V)
- GND: Massa
- SDA: GPIO21
- SCL: GPIO22

**⚠️ ERRORE COMUNE**:
- Collegare PN532 a 5V lo danneggia irreparabilmente
- Verificare sempre con multimetro prima di alimentare

**Tag Supportati**:
- NTAG213 (180 byte)
- NTAG215 (540 byte) ⭐ Consigliato
- NTAG216 (924 byte)
- MIFARE Classic 1K/4K

### PAM8403 Amplificatore

**Specifiche**:
- **Tipo**: Classe D stereo
- **Potenza**: 2x3W @ 4Ω, 1.5W @ 8Ω
- **Alimentazione**: 2.5V - 5.5V (consigliato 5V)
- **Efficienza**: >90%
- **THD**: <0.5% @ 1W
- **Consumo**: 5-10mA idle, 50-200mA @ volume medio

**Pin**:
- VDD: 5V
- GND: Massa
- LIN: Input canale sinistro
- RIN: Input canale destro
- LOUT+: Uscita sinistra +
- LOUT-: Uscita sinistra -
- ROUT+: Uscita destra +
- ROUT-: Uscita destra -

**Collegamento Speaker**:
- Speaker 4Ω: Collegare tra LOUT+ e LOUT- (o ROUT+ e ROUT-)
- Speaker 8Ω: Funziona ma potenza ridotta
- **NON** collegare massa speaker a GND (amplificatore differenziale)

**Note**:
- Il PAM8403 ha già un potenziometro integrato (se presente sul modulo)
- Se usiamo potenziometro esterno, impostare quello integrato al massimo

### TP4056 Caricabatterie

**Specifiche**:
- **Chip**: TP4056
- **Corrente carica**: 1A (regolabile)
- **Tensione batteria**: 3.7V Li-ion
- **Protezione**: DW01A (sovracarica, scarica, corto)
- **Indicatori LED**:
  - Rosso: In carica
  - Blu: Carica completa

**Pin**:
- BAT+: Collegare a batteria +
- BAT-: Collegare a batteria -
- OUT+: Uscita positiva (verso sistema)
- OUT-: Uscita negativa (massa)
- USB: Input USB-C (5V)

**Protezione DW01A**:
- Sovracarica: >4.25V → disconnette
- Sottoscarica: <2.4V → disconnette
- Sovracorrente: >3A → disconnette
- Cortocircuito: Protezione automatica

**⚠️ IMPORTANTE**:
- Usare **sempre** batterie con protezione integrata o modulo con DW01A
- Batterie senza protezione possono esplodere!

### MT3608 Step-Up Converter

**Specifiche**:
- **Tipo**: Boost converter
- **Input**: 2V - 24V
- **Output**: Regolabile 5V - 28V
- **Corrente max**: 2A
- **Efficienza**: >85%
- **Frequenza**: 1.2MHz

**Regolazione Output**:
1. Collegare input 3.7V (batteria)
2. Misurare output con multimetro
3. Ruotare trimmer fino a 5.0V
4. **PRIMA** di collegare ESP32!

**Formula regolazione**:
```
Vout = 0.6V × (1 + R1/R2)
```

**Pin**:
- IN+: Input positivo (batteria +)
- IN-: Input negativo (massa)
- OUT+: Output positivo (5V)
- OUT-: Output negativo (massa)

**Condensatori Consigliati**:
- Input: 100µF (opzionale, per ridurre ripple)
- Output: 470µF (consigliato, per stabilità)

### Batteria 18650

**Specifiche**:
- **Tensione nominale**: 3.7V
- **Tensione max**: 4.2V (carica completa)
- **Tensione min**: 3.0V (scarica)
- **Capacità**: 3000mAh (consigliato)
- **Tipo**: Li-ion

**Marche Affidabili**:
- LG (LG HG2, LG MJ1)
- Samsung (30Q, 35E)
- Panasonic (NCR18650B)
- Sony (VTC6)

**⚠️ SICUREZZA**:
- **NON** cortocircuitare
- **NON** sovraccaricare (>4.2V)
- **NON** scaricare sotto 2.5V
- Usare sempre protezione (TP4056 + DW01A)
- Conservare in luogo fresco e asciutto

**Calcolo Autonomia**:
```
Consumo medio sistema: ~200mA
Capacità batteria: 3000mAh
Autonomia teorica: 3000mAh / 200mA = 15 ore
Autonomia reale (80%): ~12 ore
Autonomia con volume alto: ~7-9 ore
```

### Speaker

**Specifiche**:
- **Impedenza**: 4Ω (⚠️ NON 8Ω)
- **Potenza**: 3W RMS
- **Diametro**: 50mm
- **Tipo**: Full-range (consigliato)

**Perché 4Ω?**:
- PAM8403 eroga 3W @ 4Ω
- Con 8Ω eroga solo 1.5W (audio più debole)
- Speaker 4Ω sono più comuni e economici

**Collegamento**:
- Collegare direttamente tra LOUT+ e LOUT- (o ROUT+ e ROUT-)
- **NON** collegare massa speaker a GND sistema
- PAM8403 è amplificatore differenziale

---

## ⚡ SCHEMA ELETTRICO COMPLETO

### Schema a Blocchi Dettagliato

```
┌─────────────────────────────────────────────────────────┐
│                    ALIMENTAZIONE                         │
│                                                          │
│  [Batteria 18650]                                        │
│  3.7V 3000mAh                                            │
│     │                                                     │
│     ├──> [TP4056] <── USB-C (5V)                        │
│     │     │                                              │
│     │     └──> OUT+ ──┐                                 │
│     │                  │                                 │
│     └──> [Switch ON/OFF]                                │
│            │                                             │
│            └──> [MT3608] ──> 5V Rail ──┬───────────────┤
│                  (Step-up)              │               │
└─────────────────────────────────────────┼───────────────┘
                                          │
        ┌─────────────────────────────────┼─────────────────┐
        │                                 │                 │
        ↓                                 ↓                 ↓
   ┌────────┐                      ┌─────────┐      ┌──────────┐
   │  ESP32 │                      │ DFPlayer│      │  PN532   │
   │        │                      │  Mini   │      │   NFC    │
   │  5V    │                      │  5V     │      │   3.3V   │
   └────────┘                      └─────────┘      └──────────┘
        │                                 │                 │
        │ TX(17)──[1KΩ]──────────────> RX│                 │
        │ RX(16)<──────────────────────── TX               │
        │                                 │                 │
        │ SDA(21)──────────────────────────────────────> SDA
        │ SCL(22)──────────────────────────────────────> SCL
        │                                 │                 │
        │ GPIO13 ───> [Pulsante PLAY]    │                 │
        │ GPIO27 ───> [Pulsante NEXT]    │                 │
        │ GPIO26 ───> [Pulsante PREV]    │                 │
        │ GPIO34 ───> [Potenziometro]    │                 │
        │ GPIO35 ───> [Partitore Batt]   │                 │
        │ GPIO25 ───> [LED Rosso]        │                 │
        │ GPIO33 ───> [LED Verde]        │                 │
        │                                 │                 │
        │                                 │ SPK_1 ──┐       │
        │                                 │ SPK_2 ──┼──> [PAM8403]
        │                                 │         │         │
        │                                 │         │         ↓
        │                                 │         │    [Speaker 4Ω]
        └─────────────────────────────────┴─────────┴─────────┘
```

### Schema Partitore Tensione Batteria

```
Batteria+ (3.7-4.2V)
    │
    ├──[100KΩ]──┬──> GPIO35 (ADC)
    │           │
    │        [47KΩ]
    │           │
    └───────────┴──> GND

Calcolo tensione:
V_adc = V_batt × (47K / (100K + 47K))
V_adc = V_batt × (47 / 147)
V_adc = V_batt × 0.3197
V_batt = V_adc / 0.3197
V_batt = V_adc × 3.128

**Nota**: Il valore 3.13 è sufficientemente preciso per la calibrazione.
```

**Perché questo partitore?**:
- ESP32 ADC max: 3.3V
- Batteria max: 4.2V
- Rapporto necessario: 4.2V / 3.3V = 1.27
- Partitore: (100K + 47K) / 47K = 147K / 47K = 3.128
- V_adc = V_batt / 3.128 ≈ 1.34V max ✅

### Schema LED di Stato Bicolore

```
LED Bicolore (Catodo Comune):
    Anodo Rosso ──[220Ω]── GPIO25
    Anodo Verde ──[220Ω]── GPIO33
    Catodo Comune ──────────── GND

Stati Logici:
- Verde acceso: GPIO25=LOW, GPIO33=HIGH
- Rosso acceso: GPIO25=HIGH, GPIO33=LOW
- Spento: GPIO25=LOW, GPIO33=LOW
- Giallo/Arancione (entrambi): GPIO25=HIGH, GPIO33=HIGH ⚠️ Opzionale

**Nota sul colore Giallo**:
- Accendendo entrambi gli anodi contemporaneamente (GPIO25=HIGH, GPIO33=HIGH), alcuni LED bicolore possono mostrare un colore giallo/arancione
- Questo funziona solo se il LED ha due LED separati (rosso e verde) nello stesso package
- **Non è garantito** - dipende dal modello specifico di LED
- Se il tuo LED non supporta il giallo, accendere entrambi mostrerà solo una miscela di rosso e verde (non un giallo puro)
- Per questo progetto, il giallo **non è utilizzato** nelle funzioni principali

**Funzioni LED di Stato**:
- **Verde fisso**: Sistema pronto (boot completato)
- **Verde lampeggiante rapido**: Tag NFC letto con successo
- **Verde lampeggiante lento**: Sistema in attesa
- **Rosso fisso**: Errore critico o batteria molto bassa (<10%)
- **Rosso lampeggiante**: Batteria bassa (10-20%) o avviso
- **Spento**: Sleep mode o sistema spento

**Calcolo Resistenza**:
- Tensione ESP32: 3.3V
- LED forward voltage: ~2.0V
- Corrente LED: 20mA
- R = (3.3V - 2.0V) / 0.020A = 65Ω
- Usiamo 220Ω per sicurezza (corrente ~6mA, LED ancora visibile)

---

## 🔋 ALIMENTAZIONE E GESTIONE BATTERIA

### Catena Alimentazione Completa

```
USB-C (5V) ──> TP4056 ──> Batteria 18650 (3.7V)
                              │
                              ├──> Switch ON/OFF
                              │       │
                              │       └──> MT3608 ──> 5V Rail
                              │              │
                              │              ├──> ESP32 (5V → 3.3V interno)
                              │              ├──> DFPlayer (5V)
                              │              └──> PAM8403 (5V)
                              │
                              └──> Partitore ──> ESP32 GPIO35 (monitoraggio)
```

### Consumo Energetico

| Componente | Idle | Attivo | Note |
|------------|------|--------|------|
| ESP32 | 80mA | 120mA | CPU 240MHz, WiFi/BT spenti |
| DFPlayer | 20mA | 100mA | Riproduzione audio |
| PN532 | 80mA | 150mA | Durante lettura tag |
| PAM8403 | 5mA | 150mA | Volume medio |
| **TOTALE** | **185mA** | **520mA** | Volume alto, NFC attivo |

**Autonomia Stimata**:
- Volume basso, idle: 3000mAh / 185mA = **16 ore**
- Volume medio, riproduzione: 3000mAh / 350mA = **8.5 ore**
- Volume alto, NFC attivo: 3000mAh / 520mA = **5.7 ore**

### Protezione Batteria

**TP4056 + DW01A**:
- **Sovracarica**: >4.25V → disconnette carica
- **Sottoscarica**: <2.4V → disconnette uscita
- **Sovracorrente**: >3A → disconnette
- **Cortocircuito**: Protezione istantanea

**Monitoraggio Software**:
- ESP32 legge tensione via ADC ogni 30 secondi
- LED verde fisso: Sistema pronto (batteria >20%)
- LED rosso lampeggiante: Batteria bassa (10-20%)
- LED rosso fisso: Batteria critica (<10%)
- Spegnimento automatico: <10% (opzionale, da implementare)

### Calibrazione Monitoraggio

```cpp
// Valori da calibrare con multimetro
const float VOLTAGE_DIVIDER = 3.14;  // (100K + 47K) / 47K
const float ADC_REFERENCE = 3.3;     // Tensione riferimento ESP32
const float ADC_MAX = 4095.0;        // Valore max ADC 12-bit

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  float adc_voltage = (raw / ADC_MAX) * ADC_REFERENCE;
  return adc_voltage * VOLTAGE_DIVIDER;
}
```

**Procedura Calibrazione**:
1. Misurare tensione batteria con multimetro (es. 4.0V)
2. Leggere valore ADC (es. 1300)
3. Calcolare: V_batt = (1300 / 4095) × 3.3 × 3.14 = 3.29V
4. Correggere VOLTAGE_DIVIDER: 4.0 / 3.29 = 1.22 → moltiplicatore = 3.14 × 1.22 = 3.83

---

## 🔌 CONNESSIONI DETTAGLIATE

### ESP32 ↔ DFPlayer Mini

```
ESP32 Pin          DFPlayer Pin        Note
─────────────────────────────────────────────────
GPIO17 (TX)    →   RX                 Via resistenza 1KΩ
GPIO16 (RX)    ←   TX                 Diretto
5V             →   VCC                5V rail
GND            →   GND                Massa comune
```

**⚠️ CRITICO**: Resistenza 1KΩ obbligatoria su TX!

**Perché?**:
- ESP32 TX: 3.3V logica
- DFPlayer RX: accetta 3.3V ma può essere sensibile
- Resistenza limita corrente e protegge entrambi

**Alternativa (se problemi)**:
- Usare convertitore di livello 3.3V ↔ 5V
- O usare divisore resistivo (non consigliato)

### ESP32 ↔ PN532 (I2C)

```
ESP32 Pin          PN532 Pin           Note
─────────────────────────────────────────────────
GPIO21 (SDA)   →   SDA                I2C Data
GPIO22 (SCL)   →   SCL                I2C Clock
3.3V           →   VCC                ⚠️ NON 5V!
GND            →   GND                Massa comune
```

**Configurazione I2C**:
- Velocità: 100kHz (default) o 400kHz
- Pull-up: Interni ESP32 (opzionali, ~45KΩ)
- Pull-up esterni: 4.7KΩ (consigliati per stabilità)

**Test I2C**:
```cpp
Wire.begin();
Wire.beginTransmission(0x24);  // PN532 I2C address
if (Wire.endTransmission() == 0) {
  Serial.println("PN532 trovato!");
}
```

### ESP32 ↔ Pulsanti

```
ESP32 Pin          Pulsante            Note
─────────────────────────────────────────────────
GPIO13         →   Terminale 1        PLAY (Rosso)
GPIO27         →   Terminale 1        NEXT (Giallo)
GPIO26         →   Terminale 1        PREV (Bianco)
GND            →   Terminale 2        Comune a tutti
```

**Configurazione**:
- Pull-up interno: `pinMode(pin, INPUT_PULLUP)`
- Logica invertita: LOW = premuto, HIGH = rilasciato
- Debounce software: 50ms delay dopo rilevamento

**Schema Pulsante**:
```
GPIO13 ──┬──> [Pulsante] ──> GND
         │
         └──> [Pull-up interno ~45KΩ] ──> 3.3V
```

### ESP32 ↔ Potenziometro Volume

```
Potenziometro 10KΩ Lineare
───────────────────────────
Pin 1 (GND)    →   GND
Pin 2 (Wiper)  →   GPIO34 (ADC)
Pin 3 (VCC)    →   3.3V
```

**Caratteristiche ADC ESP32**:
- Risoluzione: 12-bit (0-4095)
- Tensione riferimento: 3.3V
- Precisione: ~0.8mV per step
- GPIO34: Solo input, no pull-up

**Mappatura Volume**:
```cpp
int raw = analogRead(VOLUME_POT);  // 0-4095
int volume = map(raw, 0, 4095, 0, 25);  // 0-25 (DFPlayer max)
```

**Calibrazione Zone Morte**:
```cpp
// Eliminare valori estremi instabili
raw = constrain(raw, 200, 3895);
volume = map(raw, 200, 3895, 0, MAX_SAFE_VOLUME);
```

### DFPlayer ↔ PAM8403 ↔ Speaker

```
DFPlayer Pin       PAM8403 Pin        Speaker
─────────────────────────────────────────────────
SPK_1          →   RIN               
SPK_2          →   LIN               
GND            →   GND               
                    │
                    ├──> LOUT+ ──┐
                    │            │
                    └──> LOUT- ──┼──> Speaker 4Ω
                                 │
                    (ROUT+ e ROUT- non usati per mono)
```

**Nota**: DFPlayer ha uscita DAC, PAM8403 amplifica segnale analogico.

**Collegamento Speaker Mono**:
- Usare solo canale sinistro (LIN → LOUT+/-)
- Canale destro può restare scollegato
- Speaker collegato tra LOUT+ e LOUT- (differenziale)

---

## 🔧 PREPARAZIONE E TEST COMPONENTI

### Test 1: ESP32 Base

**Sketch Test**:
```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Test OK!");
  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMhz());
  Serial.println(" MHz");
}

void loop() {
  Serial.println("Loop running...");
  delay(1000);
}
```

**Verifiche**:
- ✅ Serial Monitor mostra messaggi
- ✅ LED onboard lampeggia (se presente)
- ✅ CPU frequency corretta (240MHz)

### Test 2: DFPlayer Mini

**Preparazione SD**:
1. Formattare microSD in **FAT32** (non exFAT!)
2. Creare cartella `/mp3/`
3. Copiare file: `0001.mp3`, `0002.mp3`
4. Inserire SD nel DFPlayer

**Collegamento Test**:
```
DFPlayer VCC  →  5V
DFPlayer GND  →  GND
DFPlayer SPK1 →  Speaker +
DFPlayer SPK2 →  Speaker -
```

**Comportamento Atteso**:
- DFPlayer riproduce automaticamente `0001.mp3`
- LED sul modulo lampeggia durante riproduzione

**Se non funziona**:
- Verificare formato SD (FAT32)
- Verificare nome file (4 cifre: 0001.mp3)
- Verificare alimentazione 5V
- Verificare speaker (4Ω o 8Ω)

### Test 3: PN532 NFC

**Configurazione Jumper**:
- CH1: OFF
- CH2: ON (modalità I2C)

**Collegamento Test**:
```
PN532 VCC  →  3.3V ESP32  ⚠️ NON 5V!
PN532 GND  →  GND
PN532 SDA  →  GPIO21
PN532 SCL  →  GPIO22
```

**Sketch Test**:
```cpp
#include <Wire.h>
#include <Adafruit_PN532.h>

Adafruit_PN532 nfc(21, 22);

void setup() {
  Serial.begin(115200);
  nfc.begin();
  
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 non trovato!");
    while(1);
  }
  
  Serial.print("PN532 trovato! Firmware: ");
  Serial.println(versiondata, HEX);
  
  nfc.SAMConfig();
  Serial.println("Pronto per leggere tag...");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;
  
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 500)) {
    Serial.print("Tag UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
    }
    Serial.println();
  }
  delay(1000);
}
```

**Verifiche**:
- ✅ Serial mostra firmware version
- ✅ Avvicinando tag, mostra UID
- ✅ Range ~5cm

### Test 4: MT3608 Step-Up

**⚠️ IMPORTANTE**: Regolare **PRIMA** di collegare carichi!

**Procedura**:
1. Collegare input: Batteria 3.7V o alimentatore
2. **NON** collegare output a nulla
3. Misurare output con multimetro
4. Ruotare trimmer fino a **5.0V precisi**
5. Segnare posizione trimmer
6. Solo ora collegare carichi

**Test Carico**:
- Collegare resistenza 10Ω @ 5W su output
- Misurare tensione: deve restare ~5.0V
- Se scende sotto 4.8V: MT3608 insufficiente o batteria scarica

### Test 5: TP4056 Caricabatterie

**Collegamento Test**:
```
TP4056 USB  →  USB-C (5V)
TP4056 BAT+ →  Batteria +
TP4056 BAT- →  Batteria -
```

**Comportamento Atteso**:
- LED rosso acceso: Batteria in carica
- LED blu acceso: Batteria carica (4.2V)
- LED lampeggiante: Errore (batteria difettosa o corto)

**Verifiche**:
- Misurare tensione batteria durante carica: aumenta gradualmente
- Corrente carica: ~1A (misurabile con multimetro in serie)

### Test 6: PAM8403 Amplificatore

**Collegamento Test**:
```
PAM8403 VDD  →  5V
PAM8403 GND  →  GND
PAM8403 LIN  →  Segnale audio (es. smartphone)
PAM8403 LOUT+ → Speaker +
PAM8403 LOUT- → Speaker -
```

**Test**:
- Collegare smartphone a LIN
- Riprodurre musica
- Speaker deve emettere audio
- Volume regolabile via potenziometro integrato (se presente)

---

## 🔨 SALDATURE E ASSEMBLAGGIO

### FASE 1: Preparazione Cavi

**Cavi Necessari**:
- 4 cavi 15cm (DFPlayer: VCC, GND, TX, RX)
- 4 cavi 15cm (PN532: VCC, GND, SDA, SCL)
- 3 cavi 20cm (Pulsanti: PLAY, NEXT, PREV)
- 3 cavi 10cm (Potenziometro: GND, 3.3V, Wiper)
- 2 cavi 15cm (LED: Rosso, Verde)
- 2 cavi 20cm (Partitore batteria: 100K, 47K)
- 2 cavi 10cm (Speaker: +, -)
- 1 cavo 30cm (GND comune)

**Codifica Colori** (consigliata):
- Rosso: VCC/5V
- Nero: GND
- Giallo: Segnali TX/Data
- Verde: Segnali RX/Clock
- Blu: I2C SDA
- Bianco: I2C SCL
- Arancione: Segnali GPIO

### FASE 2: Saldature Moduli

#### A. ESP32
- Saldare header 30 pin (se non presente)
- Verificare che tutti i pin siano saldati correttamente

#### B. DFPlayer Mini
```
Cavi da saldare:
- Rosso (VCC) → Pin VCC
- Nero (GND) → Pin GND
- Giallo (TX) → Pin TX
- Verde (RX) → Pin RX (con resistenza 1KΩ in serie)

Resistenza 1KΩ:
ESP32 GPIO17 ──[1KΩ]── DFPlayer RX
```

**Saldatura Resistenza**:
- Opzione 1: Saldare resistenza direttamente su cavo
- Opzione 2: Usare breadboard per test, poi saldare definitivamente

#### C. PN532
```
Cavi da saldare:
- Rosso (VCC) → Pin VCC (⚠️ 3.3V, non 5V!)
- Nero (GND) → Pin GND
- Blu (SDA) → Pin SDA
- Verde (SCL) → Pin SCL
```

#### D. Pulsanti Arcade
- Ogni pulsante ha 2 terminali
- Un terminale → GPIO ESP32
- Altro terminale → GND comune

**Saldatura**:
- Saldare cavo su terminale pulsante
- Saldare altro terminale su GND comune

#### E. Potenziometro
```
Potenziometro 10KΩ (3 pin):
Pin 1 (sinistra)  → GND (nero)
Pin 2 (centro)    → GPIO34 (giallo)
Pin 3 (destra)    → 3.3V (rosso)
```

#### F. LED di Stato Bicolore
```
LED Bicolore (3 pin):
Anodo Rosso  → GPIO25 via 220Ω
Anodo Verde  → GPIO33 via 220Ω
Catodo       → GND
```

**Funzione**: LED di stato generico per feedback visivo:
- Boot completato
- Lettura tag NFC
- Errori sistema
- Stato batteria

**Saldatura Resistori LED**:
- Saldare resistore 220Ω direttamente su anodo LED
- Poi saldare cavo da resistore a GPIO

#### G. Partitore Tensione Batteria
```
Batteria+ ──[100KΩ]──┬──> GPIO35
                      │
                   [47KΩ]
                      │
                     GND
```

**Saldatura**:
- Saldare resistenza 100KΩ tra batteria+ e punto centrale
- Saldare resistenza 47KΩ tra punto centrale e GND
- Saldare cavo da punto centrale a GPIO35

### FASE 3: Test su Breadboard

**Prima di saldare definitivamente**:
1. Montare tutto su breadboard
2. Testare ogni modulo singolarmente
3. Testare sistema completo
4. Solo dopo saldare definitivamente

**Layout Breadboard**:
```
[ESP32] ── cavi ── [DFPlayer]
    │
    ├── [PN532]
    ├── [Pulsanti]
    ├── [Potenziometro]
    └── [LED]
```

### FASE 4: Assemblaggio Definitivo

**Opzioni**:
1. **Perfboard/Stripboard**: Saldare tutto su scheda forata
2. **PCB Custom**: Progettare PCB (consigliato per produzione)
3. **Wire Wrapping**: Per prototipi (non consigliato)

**Consigli**:
- Usare distanziali per moduli (evita cortocircuiti)
- Fissare batteria con velcro o supporto
- Organizzare cavi con fascette
- Lasciare spazio per manutenzione

---

## 📐 LAYOUT PCB E SCATOLA

### Layout Scatola (15x10x5cm)

**Pannello Frontale**:
```
┌────────────────────────────────────┐
│  ●                      [Griglia]  │  ← LED + Speaker
│                                    │
│  [●]  [●]  [●]          [🎚️]      │  ← Pulsanti + Volume
│  PLAY PREV NEXT         VOLUME    │
│                                    │
│        ┌──────────┐                │  ← Zona NFC
│        │   NFC    │                │
│        └──────────┘                │
│                                    │
│  Componenti interni sotto          │
└────────────────────────────────────┘
```

**Pannello Posteriore**:
```
┌────────────────────────────────────┐
│  [SD]═══  [USB-C]  [○]Switch      │
│                                    │
│  Componenti interni                │
└────────────────────────────────────┘
```

### Foratura Pannello Frontale

**Fori Pulsanti (3x, diametro 30mm)**:
- Spaziatura: 35mm tra centri
- Posizione: 3cm dal bordo superiore
- Allineamento: Centrati orizzontalmente

**Foro Potenziometro (diametro 8mm)**:
- Posizione: Lato destro, 25mm dal bordo
- Altezza: Centrato verticalmente

**Foro Speaker (diametro 60mm)**:
- Posizione: Angolo alto destra
- Offset: 20mm da bordo destro, 15mm da bordo superiore

**Foro LED (diametro 5mm)**:
- Posizione: Angolo alto sinistra
- Offset: 10mm da entrambi i bordi

**Zona NFC (area 40x40mm)**:
- Posizione: Centro pannello frontale
- **NON** forare: PN532 interno, tag esterni

### Foratura Pannello Posteriore

**Slot MicroSD (15x2mm)**:
- Posizione: 30mm da bordo sinistro
- Altezza: Centrato verticalmente

**USB-C (10x4mm)**:
- Posizione: Centro pannello
- Altezza: Centrato verticalmente

**Switch ON/OFF (12mm)**:
- Posizione: 60mm da bordo sinistro
- Altezza: Centrato verticalmente

### Layout Interno Componenti

**Disposizione Consigliata**:
```
┌────────────────────────────────────┐
│  [ESP32]      [DFPlayer]           │  ← Scheda superiore
│                                    │
│  [PN532]      [PAM8403]            │
│                                    │
│  [TP4056]     [MT3608]             │  ← Scheda inferiore
│                                    │
│  [Batteria 18650]                  │  ← Lato
└────────────────────────────────────┘
```

**Distanziamenti**:
- Altezza minima: 5cm (per batteria 18650: 6.5cm)
- Spazio tra moduli: 5mm minimo
- Ventilazione: Fori laterali opzionali

---

## 🔍 TROUBLESHOOTING HARDWARE

### Problema: ESP32 si riavvia continuamente

**Cause Possibili**:
1. Alimentazione insufficiente (<4.5V)
2. Picchi di corrente (condensatori mancanti)
3. Cortocircuito su GPIO
4. Overheating

**Soluzioni**:
- ✅ Aggiungere condensatore 470µF su 5V rail
- ✅ Verificare output MT3608 (deve essere 5.0V)
- ✅ Verificare corrente batteria (deve essere >500mA)
- ✅ Controllare cortocircuiti con multimetro
- ✅ Aggiungere dissipatore su ESP32 (opzionale)

### Problema: Audio distorto o assente

**Cause Possibili**:
1. Speaker sbagliato (8Ω invece di 4Ω)
2. Alimentazione PAM8403 insufficiente
3. Collegamento speaker errato
4. DFPlayer non inizializzato

**Soluzioni**:
- ✅ Verificare impedenza speaker (deve essere 4Ω)
- ✅ Verificare alimentazione PAM8403 (5V)
- ✅ Verificare collegamento speaker (LOUT+ e LOUT-)
- ✅ Verificare inizializzazione DFPlayer (Serial Monitor)

### Problema: NFC non legge tag

**Cause Possibili**:
1. PN532 alimentato a 5V invece di 3.3V ⚠️
2. Jumper I2C configurati male
3. Tag non compatibile
4. Range troppo grande

**Soluzioni**:
- ✅ **VERIFICARE SUBITO**: PN532 deve essere a 3.3V!
- ✅ Verificare jumper (CH1=OFF, CH2=ON per I2C)
- ✅ Usare tag NTAG215 (compatibili)
- ✅ Avvicinare tag a <5cm
- ✅ Testare con sketch esempio Adafruit_PN532

### Problema: Batteria si scarica velocemente

**Cause Possibili**:
1. Consumo eccessivo (volume alto, WiFi attivo)
2. Batteria difettosa o vecchia
3. Cortocircuito parziale
4. MT3608 inefficiente

**Soluzioni**:
- ✅ Ridurre volume massimo (MAX_SAFE_VOLUME = 20)
- ✅ Disabilitare WiFi: `WiFi.mode(WIFI_OFF)`
- ✅ Ridurre frequenza CPU: `setCpuFrequencyMhz(80)`
- ✅ Verificare corrente idle con multimetro (<200mA)
- ✅ Sostituire batteria se vecchia

### Problema: Pulsanti non funzionano

**Cause Possibili**:
1. Pull-up non configurati
2. Collegamento errato (GND mancante)
3. Pulsante difettoso
4. Debounce insufficiente

**Soluzioni**:
- ✅ Verificare `pinMode(pin, INPUT_PULLUP)`
- ✅ Verificare collegamento GND
- ✅ Testare pulsante con multimetro (continuità)
- ✅ Aumentare debounce delay (50ms → 100ms)

### Problema: Volume non regola

**Cause Possibili**:
1. Potenziometro collegato male
2. ADC non configurato
3. Mappatura errata
4. Potenziometro difettoso

**Soluzioni**:
- ✅ Verificare collegamento (GND, 3.3V, Wiper)
- ✅ Verificare GPIO34 (solo input, no pull-up)
- ✅ Testare ADC: `Serial.println(analogRead(VOLUME_POT))`
- ✅ Verificare potenziometro con multimetro (resistenza variabile)

---

## ⚡ OTTIMIZZAZIONI ENERGETICHE

### Riduzione Consumo ESP32

```cpp
// In setup()
setCpuFrequencyMhz(80);        // Riduce consumo del 40%
WiFi.mode(WIFI_OFF);           // Disabilita WiFi
btStop();                      // Disabilita Bluetooth quando non usato
```

**Risparmio**:
- CPU 240MHz → 80MHz: -40% consumo
- WiFi spento: -20mA
- **Totale**: ~100mA invece di 120mA

### Sleep Mode (Opzionale)

```cpp
// Deep sleep dopo 10 minuti di inattività
if (millis() - lastActivity > 600000) {
  esp_deep_sleep_start();  // Consumo: ~10µA
}
```

**Risveglio**:
- Pulsante → GPIO interrupt
- Tag NFC → Non possibile (richiede ESP32 attivo)

### Gestione PN532

```cpp
// Accendere PN532 solo quando necessario
void enableNFC() {
  // Se PN532 ha pin enable, usarlo
  // Altrimenti, alimentare via MOSFET
}

void disableNFC() {
  // Spegnere quando non in uso
  // Risparmio: ~80mA
}
```

### Ottimizzazione Audio

```cpp
// Ridurre volume massimo
const int MAX_SAFE_VOLUME = 20;  // Invece di 25

// Equalizzatore efficiente
dfPlayer.EQ(DFPLAYER_EQ_NORMAL);  // Meno elaborazione = meno consumo
```

**Risparmio Totale Stimato**:
- CPU 80MHz: -40mA
- WiFi spento: -20mA
- Volume ridotto: -30mA
- **Totale**: ~150mA invece di 250mA
- **Autonomia**: 20 ore invece di 12 ore

---

## 📚 RISORSE E APPROFONDIMENTI

### Datasheet Componenti

- **ESP32**: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
- **DFPlayer Mini**: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
- **PN532**: https://www.nxp.com/docs/en/user-guide/141520.pdf
- **PAM8403**: https://www.diodes.com/assets/Datasheets/PAM8403.pdf
- **TP4056**: https://dlnmh9ip6v2uc.cloudfront.net/datasheets/Prototyping/TP4056.pdf
- **MT3608**: https://www.olimex.com/Products/Breadboarding/BB-PWR-3608/resources/MT3608.pdf

### Strumenti Utili

- **Multimetro**: Essenziale per debugging
- **Oscilloscopio**: Opzionale, per segnali audio
- **Alimentatore Variabile**: Per test senza batteria
- **Breadboard**: Per prototipazione

### Calcoli Utili

**Autonomia Batteria**:
```
Ore = (Capacità mAh) / (Consumo mA)
Esempio: 3000mAh / 200mA = 15 ore
```

**Resistenza LED**:
```
R = (V_supply - V_LED) / I_LED
Esempio: (3.3V - 2.0V) / 0.020A = 65Ω → Usare 220Ω
```

**Partitore Tensione**:
```
V_out = V_in × (R2 / (R1 + R2))
Esempio: 4.2V × (47K / (100K + 47K)) = 1.34V
```

---

**Fine Guida Elettronica** ⚡

*Per domande o problemi, consultare la sezione Troubleshooting o il blueprint principale.*

