# 🛒 Lista della Spesa Completa - Lettore Musicale NFC

## 📋 ORGANIZZAZIONE ACQUISTI PER FASE

Gli acquisti sono organizzati secondo il piano di implementazione per permetterti di procedere gradualmente. Ogni componente include descrizione e link Amazon Italia.

---

## 🎯 FASE 0: PREPARAZIONE BASE

### Strumenti Essenziali (da avere subito)

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 1 | **Multimetro Digitale** | Strumento essenziale per misurare tensioni, correnti e verificare collegamenti. Indispensabile per debugging e configurazione MT3608. | €15-20 | [Amazon - Multimetro](https://www.amazon.it/s?k=multimetro+digitale) |
| 2 | **Breadboard 830 punti** | Piastra per prototipazione senza saldature. Permette di testare tutti i circuiti prima dell'assemblaggio finale. | €8-10 | [Amazon - Breadboard](https://www.amazon.it/s?k=breadboard+830) |
| 3 | **Kit Cavi Dupont (120pz)** | Cavi maschio-maschio, maschio-femmina, femmina-femmina per collegamenti. Essenziali per connettere componenti su breadboard. | €8-10 | [Amazon - Cavi Dupont](https://www.amazon.it/s?k=dupont+jumper+wires) |
| 4 | **Saldatore 60W con supporto** | Per saldature finali dei componenti. Include punta, supporto e spugna per pulizia. | €20-25 | [Amazon - Saldatore](https://www.amazon.it/s?k=saldatore+elettronico) |
| 5 | **Stagno 0.8mm (50g)** | Filo per saldature (preferire eutettico 63/37 con flux). Necessario per connessioni permanenti. | €8-10 | [Amazon - Stagno](https://www.amazon.it/s?k=stagno+saldatura+elettronica) |

**Subtotale Fase 0 (Strumenti)**: €59-75

---

## ⚡ FASE 1: ALIMENTAZIONE E ESP32 BASE

### Componenti per Sistema di Alimentazione

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 6 | **ESP32 DevKit V1 (30 pin)** | Microcontrollore principale con WiFi e Bluetooth integrati. Gestisce tutta la logica del sistema, NFC, audio e controlli. | €10-12 | [Amazon - ESP32](https://www.amazon.it/s?k=esp32+devkit+v1) |
| 7 | **Batteria 18650 3000mAh (x2)** | Due batterie Li-ion ricaricabili da collegare in parallelo (6000mAh totali). Forniscono 13-15h autonomia a volume alto. Scegliere marche affidabili: LG, Samsung, Panasonic. | €20-25 | [Amazon - 18650](https://www.amazon.it/s?k=18650+3000mah+samsung) |
| 8 | **Porta-batteria 18650 doppio** | Supporto per 2 batterie con fili per collegamento in parallelo. Mantiene le batterie sicure e facilita la sostituzione. | €5-6 | [Amazon - Portabatteria doppio](https://www.amazon.it/s?k=battery+holder+2x18650) |
| 9 | **TP4056 con protezione DW01A** | Modulo carica-batterie con protezione da sovraccarica, sottoscarica e cortocircuito. Gestisce la ricarica sicura via USB-C. | €3-4 | [Amazon - TP4056](https://www.amazon.it/s?k=tp4056+usb+c+protection) |
| 10 | **MT3608 Step-Up Converter** | Converte 3.7V batteria in 5V stabili per alimentare ESP32 e moduli. Regolabile con trimmer per output preciso. | €3-4 | [Amazon - MT3608](https://www.amazon.it/s?k=mt3608+step+up) |
| 11 | **Interruttore Rocker ON/OFF** | Interruttore per accendere/spegnere il dispositivo. Dimensioni 12mm, da montare sul pannello posteriore. | €3-4 | [Amazon - Interruttore](https://www.amazon.it/s?k=rocker+switch+12mm) |
| 12 | **Cavo USB-C 1m** | Per ricarica batterie. Collegare a TP4056 per caricare le 2 batterie in parallelo (6-7h ricarica completa). | €5-6 | [Amazon - USB-C](https://www.amazon.it/s?k=cavo+usb+c) |

**Subtotale Fase 1**: €52-65

**⚠️ Note Fase 1**:
- Le 2 batterie devono avere tensione simile (±0.1V) prima di collegarle in parallelo
- Configurare MT3608 a 5.0V precisi PRIMA di collegare ESP32
- Verificare che TP4056 abbia chip DW01A per protezione

---

## 🎵 FASE 2: SISTEMA AUDIO

### Componenti per Audio e Riproduzione

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 13 | **DFPlayer Mini** | Modulo MP3 player con slot microSD. Riproduce file audio MP3/WAV, gestisce cartelle e volume. Cuore del sistema audio. | €4-5 | [Amazon - DFPlayer](https://www.amazon.it/s?k=dfplayer+mini) |
| 14 | **MicroSD 32GB Classe 10** | Scheda per memorizzare file audio MP3. Formattare in FAT32, organizzare in cartelle per diverse playlist/storie. | €7-8 | [Amazon - MicroSD 32GB](https://www.amazon.it/s?k=microsd+32gb+sandisk) |
| 15 | **Adattatore MicroSD esterno** | Prolunga per rendere slot SD accessibile dall'esterno. Permette cambio contenuti senza aprire scatola (cavo 10-15cm). | €4-5 | [Amazon - SD Extension](https://www.amazon.it/s?k=microsd+extension+cable) |
| 16 | **PAM8403 Amplificatore** | Amplificatore audio Classe D 2×3W. Amplifica segnale DFPlayer per pilotare speaker. Efficienza >90%, basso consumo. | €3-4 | [Amazon - PAM8403](https://www.amazon.it/s?k=pam8403+amplifier) |
| 17 | **Speaker 4Ω 3W (50mm)** | Altoparlante per audio. Impedenza 4Ω necessaria per PAM8403 (con 8Ω potenza si dimezza). Diametro 50mm. | €6-7 | [Amazon - Speaker 4ohm](https://www.amazon.it/s?k=speaker+4+ohm+3w+50mm) |
| 18 | **Griglia Speaker 60mm** | Griglia metallica protettiva per speaker. Protegge da urti e migliora estetica. Montaggio su pannello frontale. | €3-4 | [Amazon - Griglia Speaker](https://www.amazon.it/s?k=speaker+grill+60mm) |
| 19 | **Resistenza 1KΩ (5 pz)** | Resistenza da 1/4W per proteggere comunicazione ESP32→DFPlayer. **OBBLIGATORIA** tra TX ESP32 e RX DFPlayer. | €2-3 | [Amazon - Resistenze](https://www.amazon.it/s?k=resistenze+1k+ohm) |

**Subtotale Fase 2**: €32-40

**⚠️ Note Fase 2**:
- Resistenza 1KΩ è CRITICA: senza di essa il DFPlayer può danneggiarsi
- Speaker deve essere 4Ω, non 8Ω (altrimenti audio debole)
- MicroSD deve essere FAT32 (non exFAT)

---

## 📱 FASE 3: SISTEMA NFC

### Componenti per Lettura Tag NFC

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 20 | **PN532 NFC Module** | Lettore NFC per tag NTAG215/213. Legge UID tag (range ~5cm) per selezionare playlist. Comunicazione I2C con ESP32. | €10-12 | [Amazon - PN532](https://www.amazon.it/s?k=pn532+nfc+module) |
| 21 | **Tag NFC NTAG215 (10pz)** | Tag NFC adesivi o portachiavi colorati. Ogni tag rappresenta una playlist/storia diversa. Compatibili ISO14443A. | €8-10 | [Amazon - NTAG215](https://www.amazon.it/s?k=ntag215+stickers) |

**Subtotale Fase 3**: €18-22

**⚠️ Note Fase 3**:
- PN532 va alimentato a **3.3V**, NON 5V (altrimenti si danneggia)
- Configurare jumper I2C: CH1=OFF, CH2=ON
- Range lettura ~5cm è normale

---

## 🎮 FASE 4: CONTROLLI FISICI

### Componenti per Interfaccia Utente

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 22 | **Pulsanti Arcade 30mm (3pz)** | Pulsanti colorati per Play/Pausa, Next, Prev. Robusti e facili da premere per bambini. Colori: Rosso, Giallo, Bianco. | €6-8 | [Amazon - Pulsanti Arcade](https://www.amazon.it/s?k=arcade+button+30mm) |
| 23 | **Potenziometro 10KΩ lineare** | Manopola per regolare volume (0-25). Collegato a ADC ESP32, mappa rotazione in livello volume DFPlayer. | €3-4 | [Amazon - Potenziometro](https://www.amazon.it/s?k=potenziometro+10k+lineare) |
| 24 | **Manopola per Potenziometro** | Manopola da avvitare su potenziometro per facilitare rotazione. Diametro ~20mm, montaggio 6mm. | €2-3 | [Amazon - Manopola](https://www.amazon.it/s?k=knob+potenziometro) |

**Subtotale Fase 4**: €11-15

---

## 🔋 FASE 5: MONITORAGGIO E FEEDBACK

### Componenti per Monitoraggio Batteria e LED Stato

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 25 | **LED Bicolore 5mm (rosso/verde)** | LED a 3 pin (catodo comune) per feedback visivo. Verde=sistema pronto/NFC letto, Rosso=batteria bassa/errore. | €2-3 | [Amazon - LED Bicolore](https://www.amazon.it/s?k=led+bicolor+red+green+5mm) |
| 26 | **Kit Resistenze (100KΩ, 47KΩ, 220Ω)** | Resistenze per partitore tensione batteria (100K+47K) e limitazione corrente LED (220Ω×2). Kit assortito conveniente. | €5-6 | [Amazon - Kit Resistenze](https://www.amazon.it/s?k=resistor+kit+assorted) |

**Subtotale Fase 5**: €7-9

**⚠️ Note Fase 5**:
- Partitore tensione: collega al punto comune delle 2 batterie in parallelo
- LED bicolore: verificare che sia catodo comune
- Resistenze 220Ω per limitare corrente LED (~6mA)

---

## 📦 FASE 7: ASSEMBLAGGIO FINALE

### Materiali per Costruzione Scatola

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 27 | **Compensato 10mm (A4 30×40cm)** | Pannello per costruire scatola 15×10×5cm. Compensato resistente e facile da lavorare. Sufficiente per tutti i pannelli. | €8-10 | [Amazon - Compensato](https://www.amazon.it/s?k=compensato+10mm) |
| 28 | **Viti M3 × 10-15mm (20pz)** | Viti per fissare moduli elettronici su distanziali. Lunghezza 10-15mm, con dadi inclusi. | €4-5 | [Amazon - Viti M3](https://www.amazon.it/s?k=viti+m3+10mm) |
| 29 | **Distanziali M3 10mm (10pz)** | Distanziali per sollevare moduli dal fondo. Evitano cortocircuiti e migliorano ventilazione. Plastica o ottone. | €3-4 | [Amazon - Distanziali](https://www.amazon.it/s?k=distanziali+m3+10mm) |
| 30 | **Pistola Colla a Caldo + Stick** | Per fissare componenti interni (batteria, moduli piccoli). Include pistola 20W e 20 stick colla. | €10-12 | [Amazon - Colla a caldo](https://www.amazon.it/s?k=pistola+colla+a+caldo) |
| 31 | **Fascette per Cavi (100pz)** | Per organizzare cavi interni. Mantiene ordine e previene cortocircuiti. Diverse misure assortite. | €5-6 | [Amazon - Fascette](https://www.amazon.it/s?k=fascette+cavi) |
| 32 | **Vernice Acrilica Atossica** | Per decorare scatola (opzionale). Scegliere colori vivaci adatti ai bambini. Set 6-12 colori. | €12-15 | [Amazon - Vernice Acrilica](https://www.amazon.it/s?k=vernice+acrilica+bambini) |
| 33 | **Velcro Adesivo (1m)** | Per fissare batteria e moduli removibili. Permette manutenzione senza saldature. Larghezza 20mm. | €5-6 | [Amazon - Velcro](https://www.amazon.it/s?k=velcro+adesivo) |

**Subtotale Fase 7**: €47-58

---

## 🔧 STRUMENTI AGGIUNTIVI (Opzionali ma Consigliati)

| # | Componente | Descrizione | Prezzo | Link Amazon |
|---|------------|-------------|---------|-------------|
| 34 | **Trapano con Punte** | Per forare pannelli (fori 5mm, 8mm, 12mm, 30mm, 60mm). Set con trapano e punte assortite. | €40-50 | [Amazon - Trapano](https://www.amazon.it/s?k=trapano+avvitatore) |
| 35 | **Frese a Tazza (30mm, 60mm)** | Per fori grandi (pulsanti 30mm, speaker 60mm). Set 2 pezzi per legno. | €12-15 | [Amazon - Frese](https://www.amazon.it/s?k=hole+saw+30mm+60mm) |
| 36 | **Lima per Legno Set** | Per rifinire fori e bordi. Set 5 pezzi forme diverse (piatta, tonda, mezza tonda). | €10-12 | [Amazon - Lime](https://www.amazon.it/s?k=lime+per+legno) |
| 37 | **Carta Vetrata (P80-P220)** | Per levigare pannelli e bordi. Set assortito grane diverse per finitura progressiva. | €6-8 | [Amazon - Carta Vetrata](https://www.amazon.it/s?k=carta+vetrata+assortita) |
| 38 | **Morsa da Banco Piccola** | Per tenere fermi pezzi durante lavorazione. Apertura 75mm, fissaggio a tavolo. | €15-20 | [Amazon - Morsa](https://www.amazon.it/s?k=morsa+da+banco+75mm) |

**Subtotale Strumenti Opzionali**: €83-105

---

## 📊 RIEPILOGO COSTI

### Costi per Fase (senza strumenti opzionali)

| Fase | Descrizione | Costo |
|------|-------------|-------|
| **Fase 0** | Strumenti Base | €59-75 |
| **Fase 1** | Alimentazione e ESP32 | €52-65 |
| **Fase 2** | Sistema Audio | €32-40 |
| **Fase 3** | Sistema NFC | €18-22 |
| **Fase 4** | Controlli Fisici | €11-15 |
| **Fase 5** | Monitoraggio e LED | €7-9 |
| **Fase 7** | Assemblaggio Scatola | €47-58 |
| **TOTALE ESSENZIALE** | | **€226-284** |

### Con Strumenti Opzionali

| Voce | Costo |
|------|-------|
| Totale Essenziale | €226-284 |
| Strumenti Opzionali | €83-105 |
| **TOTALE COMPLETO** | **€309-389** |

---

## 🎯 STRATEGIA D'ACQUISTO CONSIGLIATA

### Ordine 1: Strumenti + Fase 1 (Subito)
**Costo**: €111-140

Componenti: #1-12 (Strumenti + Alimentazione + ESP32)

**Perché**: Puoi iniziare subito a testare ESP32 e sistema di alimentazione mentre aspetti altri componenti.

---

### Ordine 2: Fasi 2-3 (Dopo 1 settimana)
**Costo**: €50-62

Componenti: #13-21 (Audio + NFC)

**Perché**: Ordina quando hai completato Fase 1 e verificato che alimentazione funzioni.

---

### Ordine 3: Fasi 4-5 + Assemblaggio (Dopo 2-3 settimane)
**Costo**: €65-82

Componenti: #22-33 (Controlli + Monitoraggio + Materiali Scatola)

**Perché**: Ordina quando hai testato audio e NFC su breadboard e sei pronto per assemblaggio finale.

---

### Ordine 4: Strumenti Opzionali (Se Necessario)
**Costo**: €83-105

Componenti: #34-38

**Perché**: Acquista solo se non hai già trapano/attrezzi per lavorare il legno.

---

## 💡 CONSIGLI PER GLI ACQUISTI

### ✅ Da Fare

1. **Verifica specifiche**: Leggi attentamente descrizioni prodotto
2. **Controlla recensioni**: Preferisci prodotti con 4+ stelle e molte recensioni
3. **Batterie autentiche**: Per 18650, preferisci venditori affidabili (rischio contraffazione)
4. **Kit vs singoli**: Per resistenze e cavi, kit assortiti sono più convenienti
5. **Amazon Prime**: Se hai Prime, sfrutta spedizione rapida per componenti urgenti

### ❌ Da Evitare

1. **Non comprare batterie senza marca**: Rischio sicurezza
2. **Non prendere microSD troppo economiche**: Possono essere lente o difettose
3. **Non comprare "Arduino compatibili" troppo economici**: Qualità scarsa
4. **Evita ESP32 senza header saldati** (a meno che tu non voglia saldarli)

---

## 🔍 ALTERNATIVE E SOSTITUZIONI

### Se Non Trovi Componente Esatto

| Componente Originale | Alternativa Valida |
|---------------------|-------------------|
| ESP32 DevKit V1 | ESP32 WROOM-32, ESP32-WROVER |
| NTAG215 | NTAG213 (meno memoria ma compatibile) |
| Speaker 4Ω 3W 50mm | Speaker 4Ω 5W 50-57mm |
| PAM8403 | MAX98357A (I2S, migliore qualità) |
| Batteria 18650 3000mAh | 2600-3500mAh (autonomia varia) |

---

## 📝 CHECKLIST ACQUISTI

Prima di ordinare, verifica:

- [ ] Ho letto specifiche di ogni componente
- [ ] Ho verificato compatibilità (tensioni, dimensioni)
- [ ] Ho controllato recensioni prodotti
- [ ] Ho calcolato costo totale e budget disponibile
- [ ] Ho deciso strategia ordini (tutto insieme o graduale)
- [ ] Ho verificato tempi consegna
- [ ] Ho account Amazon e metodo pagamento pronto

---

## 🎉 PROSSIMI PASSI

Dopo aver ordinato:

1. **Organizza workspace**: Prepara tavolo, lampada, strumenti
2. **Studia documentazione**: Leggi blueprint mentre aspetti componenti
3. **Prepara Arduino IDE**: Installa software e librerie
4. **Pianifica tempo**: Dedica 2-3 ore per sessione di lavoro
5. **Inizia Fase 1**: Non appena arrivano primi componenti

---

**Buoni acquisti e buona costruzione! 🛒🔧**

*Lista aggiornata - Novembre 2025*
*Prezzi indicativi Amazon Italia*
