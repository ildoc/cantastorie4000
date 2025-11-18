# 🔋 Analisi Autonomia Batteria - Lettore Musicale NFC

## 📊 RIEPILOGO

**Configurazione Standard**: 2 batterie 3000mAh in parallelo (6000mAh totali)

**Scenario d'Uso**: NFC sempre attivo + Volume alto frequente

**Risposta**: 
- ✅ **2 batterie 6000mAh**: Autonomia 13-15 ore a volume alto con NFC sempre attivo
- ✅ **Configurazione standard**: Progetto progettato per 2 batterie
- ✅ **Sufficiente**: Autonomia adeguata per uso intensivo

---

## 📈 CONSUMI DETTAGLIATI

### Consumo per Componente

**Scenario d'Uso**: NFC sempre attivo + Volume alto frequente

| Componente | Idle | Attivo | Note |
|------------|------|--------|------|
| ESP32 | 80mA | 120mA | CPU 240MHz, WiFi/BT spenti |
| DFPlayer | 20mA | 100mA | Riproduzione audio |
| PN532 | 80mA | 80mA | **Sempre attivo** (non solo durante lettura) |
| PAM8403 | 5mA | 150mA | Volume alto |
| **TOTALE** | **185mA** | **450mA** | Volume alto, NFC sempre attivo |

### Scenari d'Uso

#### Scenario 1: Volume Basso / Idle
- **Consumo**: 185mA
- **Autonomia**: 3000mAh / 185mA = **16.2 ore** ✅
- **Uso**: Sistema acceso ma poco utilizzato

#### Scenario 2: Volume Medio
- **Consumo**: ~350mA
  - ESP32: 120mA
  - DFPlayer: 100mA
  - PN532: 80mA (sempre attivo)
  - PAM8403: 50mA (volume medio)
- **Autonomia (2 batterie)**: 6000mAh / 350mA = **17.1 ore** ✅
- **Uso**: Riproduzione normale, volume confortevole

#### Scenario 3: Volume Alto + NFC Sempre Attivo (Scenario Reale)
- **Consumo**: ~450mA
  - ESP32: 120mA
  - DFPlayer: 100mA
  - PN532: 80mA (sempre attivo)
  - PAM8403: 150mA (volume alto)
- **Autonomia (2 batterie)**: 6000mAh / 450mA = **13.3 ore** ✅
- **Uso**: Volume alto, NFC sempre attivo (scenario d'uso reale)

---

## 🎯 AUTONOMIA CON 1 BATTERIA (3000mAh)

### Autonomia Reale

Considerando:
- Efficienza batteria: ~90% (non si scarica completamente)
- Efficienza MT3608: ~85%
- Margine di sicurezza: ~10%

**Autonomia Efficace**:
- Volume basso: 16h × 0.9 × 0.85 × 0.9 = **~11 ore**
- Volume medio: 8.6h × 0.9 × 0.85 × 0.9 = **~6 ore** (ma uso reale è misto)
- Volume alto: 5.8h × 0.9 × 0.85 × 0.9 = **~4 ore**

**Uso Reale Tipico** (mix di scenari):
- Volume medio per la maggior parte del tempo
- Picchi a volume alto occasionali
- NFC usato solo quando necessario
- **Autonomia media**: **7-9 ore** ✅

### Conclusione per 1 Batteria

✅ **1 batteria 3000mAh è SUFFICIENTE** per:
- Uso normale a volume medio
- Autonomia 7-9 ore (come dichiarato)
- Uso tipico di un bambino (non continuo a volume massimo)

⚠️ **Limitazioni**:
- A volume alto continuo: solo 5-6 ore
- Se usato intensivamente tutto il giorno: potrebbe non bastare

---

## 🔋 CONFIGURAZIONE STANDARD: 2 BATTERIE IN PARALLELO (6000mAh)

### ✅ Configurazione Standard del Progetto

Il progetto è progettato per **2 batterie 3000mAh in parallelo (6000mAh totali)** come configurazione standard.

### Vantaggi

- **Capacità doppia**: 6000mAh
- **Autonomia adeguata**: 13-15 ore a volume alto con NFC sempre attivo ✅
- **Maggiore sicurezza**: Se una batteria si scarica, l'altra continua
- **Maggiore durata**: Meno cicli di carica/scarica per batteria
- **Corrente distribuita**: Meno stress per singola batteria

### Autonomia con 2 Batterie (6000mAh)

- Volume basso: **~32 ore**
- Volume medio: **~17 ore** ✅
- **Volume alto + NFC sempre attivo**: **~13-15 ore** ✅ (scenario d'uso reale)

### Considerazioni

- **Costo**: +€10 (rispetto a 1 batteria)
- **Peso**: +50g
- **Spazio**: Richiede spazio per 2 batterie (affiancate o sovrapposte)
- **Complessità**: Gestione carica/scarica bilanciata (batterie identiche richieste)

---

## 💡 OTTIMIZZAZIONI PER AUMENTARE AUTONOMIA

### Ottimizzazioni Software

1. **Ridurre frequenza CPU**:
   ```cpp
   setCpuFrequencyMhz(80);  // Invece di 240MHz
   ```
   - Risparmio: ~40mA
   - Autonomia: +2-3 ore

2. **Disabilitare WiFi**:
   ```cpp
   WiFi.mode(WIFI_OFF);
   ```
   - Risparmio: ~20mA
   - Autonomia: +1 ora

3. **Spegnere PN532 quando non usato**:
   - Risparmio: ~80mA
   - Autonomia: +4-5 ore
   - **Nota**: Richiede MOSFET o pin enable

4. **Limitare volume massimo**:
   ```cpp
   const int MAX_SAFE_VOLUME = 20;  // Invece di 25
   ```
   - Risparmio: ~30mA
   - Autonomia: +1-2 ore

### Autonomia con Ottimizzazioni (1 batteria)

- **Consumo ottimizzato**: ~250mA (invece di 350mA)
- **Autonomia**: 3000mAh / 250mA = **12 ore** ✅
- **Con 2 batterie**: **24 ore** ✅

---

## 📋 RACCOMANDAZIONI

### Per Uso Normale (Bambino)

✅ **1 batteria 3000mAh è SUFFICIENTE** se:
- Volume medio per la maggior parte del tempo
- Uso non continuo tutto il giorno
- Autonomia 7-9 ore è accettabile

### Per Uso Intensivo

💡 **Considerare 2 batterie 3000mAh in parallelo** se:
- Si usa spesso a volume alto
- Si vuole autonomia >10 ore
- Si vuole maggiore sicurezza

### Per Massima Autonomia

💡 **1 batteria + Ottimizzazioni**:
- CPU 80MHz
- WiFi spento
- Volume max 20
- **Autonomia**: ~10-12 ore ✅

💡 **2 batterie + Ottimizzazioni**:
- **Autonomia**: ~20-24 ore ✅

---

## 🔧 IMPLEMENTAZIONE 2 BATTERIE

### Collegamento in Parallelo

```
Batteria 1+ ──┬──> TP4056 BAT+
Batteria 1- ──┤
              │
Batteria 2+ ──┤
Batteria 2- ──┴──> TP4056 BAT-
```

**⚠️ IMPORTANTE**:
- Batterie devono avere tensione simile (±0.1V)
- Caricare insieme la prima volta
- Usare batterie identiche (stessa marca, stesso modello)

### Modifiche Scatola

- Spazio aggiuntivo: ~65mm × 18mm × 18mm per seconda batteria
- Peso aggiuntivo: ~50g
- Supporto batteria: Usare 2 porta-batteria o supporto doppio

---

## 📊 TABELLA RIASSUNTIVA

| Configurazione | Volume Medio | Volume Alto + NFC Sempre Attivo | Note |
|----------------|--------------|----------------------------------|------|
| **2 batterie 6000mAh** (Standard) | 17 ore ✅ | **13-15 ore** ✅ | **Configurazione standard del progetto** |
| **2 batterie + ottimizzazioni** | 20-22 ore ✅ | 18-20 ore ✅ | Massima autonomia |

---

## ✅ CONCLUSIONE

**Risposta alla domanda**: 

✅ **SÌ, 1 batteria 3000mAh è sufficiente** per l'uso normale dichiarato (7-9 ore a volume medio).

⚠️ **MA**: Se si prevede uso intensivo a volume alto, considerare:
- 2 batterie in parallelo (6000mAh)
- Oppure ottimizzazioni software

**Raccomandazione finale**:
- **Per progetto base**: 1 batteria 3000mAh ✅
- **Per massima autonomia**: 2 batterie 6000mAh 💡
- **Per compromesso**: 1 batteria + ottimizzazioni 💡

---

*Documento v1.0 - Analisi Autonomia Batteria*
*Lettore Musicale NFC per Bambini*

