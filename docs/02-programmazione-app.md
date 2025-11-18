# 📱 Guida Completa Programmazione App - Flutter

## 📋 INDICE

1. [Panoramica App](#-panoramica-app)
2. [Setup Ambiente Flutter](#-setup-ambiente-flutter)
3. [Architettura App](#-architettura-app)
4. [Configurazione Bluetooth](#-configurazione-bluetooth)
5. [Comunicazione con ESP32](#-comunicazione-con-esp32)
6. [Interfaccia Utente](#-interfaccia-utente)
7. [Gestione Stato](#-gestione-stato)
8. [Gestione Tag NFC](#-gestione-tag-nfc)
9. [Testing e Debug](#-testing-e-debug)
10. [Build e Distribuzione](#-build-e-distribuzione)
11. [Codice Completo](#-codice-completo)

---

## 🎯 PANORAMICA APP

### Funzionalità Principali

L'app Flutter permette di:
- **Connessione Bluetooth**: Collegarsi al lettore musicale
- **Controllo riproduzione**: Play, Pausa, Next, Previous
- **Regolazione volume**: Slider e pulsanti +/-
- **Gestione cartelle**: Selezione e riproduzione cartelle
- **Associazione tag NFC**: Associa tag colorati a cartelle
- **Monitoraggio batteria**: Visualizza stato batteria
- **Stato sistema**: Volume, riproduzione, connessione

### Protocollo Comunicazione

**Comandi App → ESP32**:
```
PLAY              → Avvia riproduzione
PAUSE             → Mette in pausa
NEXT              → Traccia successiva
PREV              → Traccia precedente
VOL:15            → Imposta volume (0-25)
FOLDER:2          → Riproduce cartella 2
ASSOCIATE:UID:1   → Associa tag a cartella
STATUS            → Richiede stato
BATTERY           → Richiede batteria
```

**Risposte ESP32 → App**:
```
OK                          → Comando eseguito
ERROR:messaggio            → Errore
STATUS:Volume:15,Playing:1,Battery:85
TAG_DETECTED:04A1B2C3      → Tag rilevato
BATTERY:3.8V,75%           → Stato batteria
```

---

## 🔧 SETUP AMBIENTE FLUTTER

### Installazione Flutter

1. **Scaricare Flutter SDK**
   - https://flutter.dev/docs/get-started/install
   - Estraggere in cartella (es. `C:\flutter`)

2. **Aggiungere al PATH**
   ```bash
   # Windows
   setx PATH "%PATH%;C:\flutter\bin"
   ```

3. **Verificare Installazione**
   ```bash
   flutter doctor
   ```

4. **Installare Dipendenze**
   - Android Studio (per Android SDK)
   - VS Code o Android Studio (editor)
   - Git

### Creazione Progetto

```bash
# Crea nuovo progetto
flutter create music_player_app
cd music_player_app

# Aggiungi dipendenza Bluetooth
flutter pub add flutter_bluetooth_serial
```

### Configurazione Android

**File: `android/app/src/main/AndroidManifest.xml`**

Aggiungi permessi dentro `<manifest>`:
```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    
    <!-- Permessi Bluetooth -->
    <uses-permission android:name="android.permission.BLUETOOTH" />
    <uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
    <uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />
    <uses-permission android:name="android.permission.BLUETOOTH_SCAN" />
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    
    <!-- Per Android 12+ -->
    <uses-permission android:name="android.permission.BLUETOOTH_SCAN" 
                     android:usesPermissionFlags="neverForLocation" />
    
    <application
        android:label="Lettore Musicale"
        android:name="${applicationName}"
        android:icon="@mipmap/ic_launcher">
        <!-- ... -->
    </application>
</manifest>
```

**File: `android/app/build.gradle`**

Verifica versione minima:
```gradle
android {
    defaultConfig {
        minSdkVersion 21  // Android 5.0+
        targetSdkVersion 33
    }
}
```

### Configurazione iOS (Opzionale)

**File: `ios/Runner/Info.plist`**

Aggiungi:
```xml
<key>NSBluetoothAlwaysUsageDescription</key>
<string>L'app necessita Bluetooth per connettersi al lettore musicale</string>
<key>NSBluetoothPeripheralUsageDescription</key>
<string>L'app necessita Bluetooth per connettersi al lettore musicale</string>
```

---

## 🏗️ ARCHITETTURA APP

### Struttura Progetto

```
lib/
├── main.dart                 → Entry point
├── models/
│   ├── device_state.dart    → Modello stato dispositivo
│   └── nfc_tag.dart         → Modello tag NFC
├── services/
│   ├── bluetooth_service.dart → Servizio Bluetooth
│   └── protocol_service.dart  → Gestione protocollo
├── screens/
│   ├── home_screen.dart     → Schermata principale
│   ├── device_list_screen.dart → Lista dispositivi
│   └── tag_association_screen.dart → Associazione tag
└── widgets/
    ├── player_controls.dart → Controlli riproduzione
    ├── volume_slider.dart   → Slider volume
    └── battery_indicator.dart → Indicatore batteria
```

### State Management

**Opzione 1: setState (Semplice)**
- Per app piccole
- Gestione stato locale

**Opzione 2: Provider (Consigliato)**
```yaml
# pubspec.yaml
dependencies:
  provider: ^6.0.0
```

**Opzione 3: Riverpod (Avanzato)**
```yaml
dependencies:
  flutter_riverpod: ^2.0.0
```

Per questa guida, usiamo **setState** per semplicità.

---

## 📡 CONFIGURAZIONE BLUETOOTH

### Servizio Bluetooth

**File: `lib/services/bluetooth_service.dart`**

```dart
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'dart:async';
import 'dart:typed_data';
import 'dart:convert';

class BluetoothService {
  BluetoothConnection? _connection;
  StreamSubscription<Uint8List>? _subscription;
  Function(String)? onDataReceived;
  bool _isConnected = false;
  
  bool get isConnected => _isConnected;
  
  // Cerca dispositivi accoppiati
  Future<List<BluetoothDevice>> getBondedDevices() async {
    List<BluetoothDevice> devices = 
        await FlutterBluetoothSerial.instance.getBondedDevices();
    return devices;
  }
  
  // Cerca dispositivi disponibili (richiede permessi)
  Future<List<BluetoothDevice>> discoverDevices() async {
    FlutterBluetoothSerial.instance.startDiscovery().listen((result) {
      // Gestisci risultati
    });
    
    // Attendi 10 secondi
    await Future.delayed(Duration(seconds: 10));
    return await getBondedDevices();
  }
  
  // Connetti a dispositivo
  Future<bool> connectToDevice(BluetoothDevice device) async {
    try {
      _connection = await BluetoothConnection.toAddress(device.address);
      
      if (_connection!.isConnected) {
        _isConnected = true;
        
        // Ascolta dati in arrivo
        _subscription = _connection!.input!.listen(
          (Uint8List data) {
            String message = ascii.decode(data);
            onDataReceived?.call(message);
          },
          onDone: () {
            _isConnected = false;
            onDataReceived?.call("DISCONNECTED");
          },
          onError: (error) {
            _isConnected = false;
            onDataReceived?.call("ERROR:$error");
          },
        );
        
        return true;
      }
      return false;
    } catch (e) {
      print("Errore connessione: $e");
      return false;
    }
  }
  
  // Invia comando
  Future<bool> sendCommand(String command) async {
    if (_connection != null && _connection!.isConnected) {
      try {
        _connection!.output.add(ascii.encode(command + "\n"));
        await _connection!.output.allSent;
        return true;
      } catch (e) {
        print("Errore invio: $e");
        return false;
      }
    }
    return false;
  }
  
  // Disconnetti
  Future<void> disconnect() async {
    await _subscription?.cancel();
    await _connection?.close();
    _isConnected = false;
  }
  
  // Verifica stato Bluetooth
  Future<bool> isBluetoothEnabled() async {
    return await FlutterBluetoothSerial.instance.isEnabled ?? false;
  }
  
  // Richiedi abilitazione Bluetooth
  Future<void> requestEnable() async {
    await FlutterBluetoothSerial.instance.requestEnable();
  }
}
```

---

## 💬 COMUNICAZIONE CON ESP32

### Servizio Protocollo

**File: `lib/services/protocol_service.dart`**

```dart
class ProtocolService {
  final BluetoothService bluetoothService;
  
  ProtocolService(this.bluetoothService);
  
  // Invia comando e attende risposta
  Future<String?> sendCommand(String command, {Duration timeout = const Duration(seconds: 2)}) async {
    if (!bluetoothService.isConnected) {
      return "ERROR:Not connected";
    }
    
    Completer<String?> completer = Completer();
    String? response;
    
    // Listener temporaneo per risposta
    Function(String)? originalListener = bluetoothService.onDataReceived;
    bluetoothService.onDataReceived = (data) {
      response = data;
      completer.complete(data);
    };
    
    // Invia comando
    bool sent = await bluetoothService.sendCommand(command);
    if (!sent) {
      bluetoothService.onDataReceived = originalListener;
      return "ERROR:Send failed";
    }
    
    // Attendi risposta con timeout
    try {
      response = await completer.future.timeout(timeout);
    } catch (e) {
      response = "ERROR:Timeout";
    }
    
    // Ripristina listener originale
    bluetoothService.onDataReceived = originalListener;
    
    return response;
  }
  
  // Parsing risposta STATUS
  Map<String, dynamic>? parseStatus(String response) {
    if (!response.startsWith("STATUS:")) return null;
    
    Map<String, dynamic> status = {};
    String data = response.substring(7); // Rimuove "STATUS:"
    
    List<String> parts = data.split(',');
    for (String part in parts) {
      List<String> keyValue = part.split(':');
      if (keyValue.length == 2) {
        String key = keyValue[0];
        String value = keyValue[1];
        
        if (key == "Volume") {
          status['volume'] = int.tryParse(value) ?? 0;
        } else if (key == "Playing") {
          status['playing'] = value == "1";
        } else if (key == "Battery") {
          status['battery'] = int.tryParse(value) ?? 0;
        }
      }
    }
    
    return status;
  }
  
  // Parsing risposta BATTERY
  Map<String, dynamic>? parseBattery(String response) {
    if (!response.startsWith("BATTERY:")) return null;
    
    String data = response.substring(8); // Rimuove "BATTERY:"
    List<String> parts = data.split(',');
    
    if (parts.length == 2) {
      String voltageStr = parts[0].replaceAll('V', '');
      String percentStr = parts[1].replaceAll('%', '');
      
      return {
        'voltage': double.tryParse(voltageStr) ?? 0.0,
        'percent': int.tryParse(percentStr) ?? 0,
      };
    }
    
    return null;
  }
  
  // Parsing TAG_DETECTED
  String? parseTagDetected(String response) {
    if (response.startsWith("TAG_DETECTED:")) {
      return response.substring(13).trim();
    }
    return null;
  }
}
```

---

## 🎨 INTERFACCIA UTENTE

### Schermata Principale

**File: `lib/screens/home_screen.dart`**

```dart
import 'package:flutter/material.dart';
import '../services/bluetooth_service.dart';
import '../services/protocol_service.dart';
import '../widgets/player_controls.dart';
import '../widgets/volume_slider.dart';
import '../widgets/battery_indicator.dart';

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  final BluetoothService bluetoothService = BluetoothService();
  final ProtocolService protocolService = ProtocolService(BluetoothService());
  
  bool isConnected = false;
  int volume = 15;
  int batteryPercent = 100;
  bool isPlaying = false;
  String? connectedDeviceName;
  
  @override
  void initState() {
    super.initState();
    _setupBluetoothListener();
  }
  
  void _setupBluetoothListener() {
    bluetoothService.onDataReceived = (data) {
      _handleBluetoothData(data);
    };
  }
  
  void _handleBluetoothData(String data) {
    setState(() {
      // Parsing risposte
      if (data.startsWith("STATUS:")) {
        Map<String, dynamic>? status = protocolService.parseStatus(data);
        if (status != null) {
          volume = status['volume'] ?? volume;
          isPlaying = status['playing'] ?? isPlaying;
          batteryPercent = status['battery'] ?? batteryPercent;
        }
      } else if (data.startsWith("BATTERY:")) {
        Map<String, dynamic>? battery = protocolService.parseBattery(data);
        if (battery != null) {
          batteryPercent = battery['percent'] ?? batteryPercent;
        }
      } else if (data.startsWith("TAG_DETECTED:")) {
        String? tagUID = protocolService.parseTagDetected(data);
        if (tagUID != null) {
          _showTagAssociationDialog(tagUID);
        }
      } else if (data == "DISCONNECTED") {
        isConnected = false;
        _showSnackBar("Disconnesso dal dispositivo");
      }
    });
  }
  
  Future<void> _connectToDevice() async {
    // Verifica Bluetooth abilitato
    if (!(await bluetoothService.isBluetoothEnabled() ?? false)) {
      await bluetoothService.requestEnable();
      return;
    }
    
    // Mostra lista dispositivi
    List<BluetoothDevice> devices = await bluetoothService.getBondedDevices();
    
    if (devices.isEmpty) {
      _showSnackBar("Nessun dispositivo trovato. Accoppia il lettore musicale nelle impostazioni Bluetooth.");
      return;
    }
    
    // Dialog selezione dispositivo
    BluetoothDevice? selectedDevice = await showDialog<BluetoothDevice>(
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
                onTap: () => Navigator.pop(context, devices[index]),
              );
            },
          ),
        ),
      ),
    );
    
    if (selectedDevice != null) {
      _showSnackBar("Connessione in corso...");
      bool connected = await bluetoothService.connectToDevice(selectedDevice);
      
      if (connected) {
        setState(() {
          isConnected = true;
          connectedDeviceName = selectedDevice.name;
        });
        _showSnackBar("Connesso a ${selectedDevice.name}");
        
        // Richiedi stato iniziale
        await protocolService.sendCommand("STATUS");
      } else {
        _showSnackBar("Errore di connessione");
      }
    }
  }
  
  Future<void> _disconnect() async {
    await bluetoothService.disconnect();
    setState(() {
      isConnected = false;
      connectedDeviceName = null;
    });
    _showSnackBar("Disconnesso");
  }
  
  Future<void> _sendCommand(String command) async {
    if (!isConnected) {
      _showSnackBar("Non connesso");
      return;
    }
    
    await bluetoothService.sendCommand(command);
  }
  
  void _showTagAssociationDialog(String tagUID) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text("Tag NFC Rilevato"),
        content: Text("UID: $tagUID\n\nAssocia a quale cartella?"),
        actions: [
          for (int i = 1; i <= 5; i++)
            TextButton(
              child: Text("Cartella $i"),
              onPressed: () {
                _sendCommand("ASSOCIATE:$tagUID:$i");
                Navigator.pop(context);
                _showSnackBar("Tag associato a cartella $i");
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
  
  void _showSnackBar(String message) {
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(content: Text(message)),
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
              onPressed: _connectToDevice,
            )
          else
            IconButton(
              icon: Icon(Icons.bluetooth_connected),
              onPressed: _disconnect,
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
                subtitle: Text(connectedDeviceName ?? ""),
                trailing: isConnected
                    ? TextButton(
                        child: Text("Disconnetti"),
                        onPressed: _disconnect,
                      )
                    : null,
              ),
            ),
            
            SizedBox(height: 16),
            
            // Indicatore batteria
            BatteryIndicator(
              percent: batteryPercent,
              onRefresh: () => _sendCommand("BATTERY"),
            ),
            
            SizedBox(height: 16),
            
            // Controllo volume
            VolumeSlider(
              volume: volume,
              maxVolume: 25,
              onChanged: (val) {
                setState(() => volume = val);
              },
              onChangeEnd: (val) {
                _sendCommand("VOL:$val");
              },
            ),
            
            SizedBox(height: 24),
            
            // Controlli riproduzione
            PlayerControls(
              isPlaying: isPlaying,
              onPlay: () {
                setState(() => isPlaying = true);
                _sendCommand("PLAY");
              },
              onPause: () {
                setState(() => isPlaying = false);
                _sendCommand("PAUSE");
              },
              onNext: () => _sendCommand("NEXT"),
              onPrev: () => _sendCommand("PREV"),
            ),
            
            SizedBox(height: 24),
            
            // Lista cartelle
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    "Cartelle",
                    style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  ),
                  SizedBox(height: 8),
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
                ],
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
                _showSnackBar("Avvicina un tag NFC al dispositivo");
              },
            ),
          ],
        ),
      ),
    );
  }
  
  @override
  void dispose() {
    bluetoothService.disconnect();
    super.dispose();
  }
}
```

### Widget Controlli Riproduzione

**File: `lib/widgets/player_controls.dart`**

```dart
import 'package:flutter/material.dart';

class PlayerControls extends StatelessWidget {
  final bool isPlaying;
  final VoidCallback onPlay;
  final VoidCallback onPause;
  final VoidCallback onNext;
  final VoidCallback onPrev;
  
  const PlayerControls({
    required this.isPlaying,
    required this.onPlay,
    required this.onPause,
    required this.onNext,
    required this.onPrev,
  });
  
  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        IconButton(
          icon: Icon(Icons.skip_previous),
          iconSize: 48,
          onPressed: onPrev,
        ),
        IconButton(
          icon: Icon(isPlaying ? Icons.pause_circle_filled : Icons.play_circle_filled),
          iconSize: 64,
          color: Colors.blue,
          onPressed: isPlaying ? onPause : onPlay,
        ),
        IconButton(
          icon: Icon(Icons.skip_next),
          iconSize: 48,
          onPressed: onNext,
        ),
      ],
    );
  }
}
```

### Widget Slider Volume

**File: `lib/widgets/volume_slider.dart`**

```dart
import 'package:flutter/material.dart';

class VolumeSlider extends StatelessWidget {
  final int volume;
  final int maxVolume;
  final ValueChanged<int> onChanged;
  final ValueChanged<int> onChangeEnd;
  
  const VolumeSlider({
    required this.volume,
    required this.maxVolume,
    required this.onChanged,
    required this.onChangeEnd,
  });
  
  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: EdgeInsets.all(16.0),
        child: Column(
          children: [
            Text(
              "Volume: $volume/$maxVolume",
              style: TextStyle(fontSize: 18),
            ),
            Row(
              children: [
                IconButton(
                  icon: Icon(Icons.remove),
                  onPressed: volume > 0
                      ? () => onChanged(volume - 1)
                      : null,
                ),
                Expanded(
                  child: Slider(
                    value: volume.toDouble(),
                    min: 0,
                    max: maxVolume.toDouble(),
                    divisions: maxVolume,
                    onChanged: (val) => onChanged(val.toInt()),
                    onChangeEnd: (val) => onChangeEnd(val.toInt()),
                  ),
                ),
                IconButton(
                  icon: Icon(Icons.add),
                  onPressed: volume < maxVolume
                      ? () => onChanged(volume + 1)
                      : null,
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
```

### Widget Indicatore Batteria

**File: `lib/widgets/battery_indicator.dart`**

```dart
import 'package:flutter/material.dart';

class BatteryIndicator extends StatelessWidget {
  final int percent;
  final VoidCallback onRefresh;
  
  const BatteryIndicator({
    required this.percent,
    required this.onRefresh,
  });
  
  IconData _getBatteryIcon() {
    if (percent > 75) return Icons.battery_full;
    if (percent > 50) return Icons.battery_5_bar;
    if (percent > 25) return Icons.battery_4_bar;
    if (percent > 10) return Icons.battery_2_bar;
    return Icons.battery_1_bar;
  }
  
  Color _getBatteryColor() {
    if (percent > 20) return Colors.green;
    return Colors.red;
  }
  
  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: EdgeInsets.all(16.0),
        child: Row(
          children: [
            Icon(
              _getBatteryIcon(),
              color: _getBatteryColor(),
              size: 32,
            ),
            SizedBox(width: 8),
            Text(
              "Batteria: $percent%",
              style: TextStyle(fontSize: 16),
            ),
            Spacer(),
            IconButton(
              icon: Icon(Icons.refresh),
              onPressed: onRefresh,
            ),
          ],
        ),
      ),
    );
  }
}
```

---

## 🧪 TESTING E DEBUG

### Test Bluetooth

```dart
void testBluetooth() async {
  BluetoothService service = BluetoothService();
  
  // Verifica Bluetooth abilitato
  bool enabled = await service.isBluetoothEnabled() ?? false;
  print("Bluetooth enabled: $enabled");
  
  // Lista dispositivi
  List<BluetoothDevice> devices = await service.getBondedDevices();
  print("Dispositivi trovati: ${devices.length}");
  
  // Connetti
  if (devices.isNotEmpty) {
    bool connected = await service.connectToDevice(devices[0]);
    print("Connesso: $connected");
    
    // Invia comando
    await service.sendCommand("STATUS");
  }
}
```

### Debug Logging

```dart
class DebugLogger {
  static void log(String message) {
    print("[${DateTime.now()}] $message");
  }
  
  static void logBluetooth(String message) {
    log("[BT] $message");
  }
  
  static void logProtocol(String message) {
    log("[PROTO] $message");
  }
}
```

---

## 📦 BUILD E DISTRIBUZIONE

### Build Android APK

```bash
# Debug APK
flutter build apk --debug

# Release APK
flutter build apk --release

# App Bundle (per Google Play)
flutter build appbundle --release
```

### Build iOS (Mac richiesto)

```bash
# Debug
flutter build ios --debug

# Release
flutter build ios --release
```

### Firma APK (Release)

**File: `android/key.properties`**
```
storePassword=password
keyPassword=password
keyAlias=key
storeFile=../keystore.jks
```

**File: `android/app/build.gradle`**
```gradle
def keystoreProperties = new Properties()
def keystorePropertiesFile = rootProject.file('key.properties')
if (keystorePropertiesFile.exists()) {
    keystoreProperties.load(new FileInputStream(keystorePropertiesFile))
}

android {
    signingConfigs {
        release {
            keyAlias keystoreProperties['keyAlias']
            keyPassword keystoreProperties['keyPassword']
            storeFile keystoreProperties['storeFile'] ? file(keystoreProperties['storeFile']) : null
            storePassword keystoreProperties['storePassword']
        }
    }
    buildTypes {
        release {
            signingConfig signingConfigs.release
        }
    }
}
```

---

## 📝 CODICE COMPLETO

### File: `pubspec.yaml`

```yaml
name: music_player_app
description: Lettore Musicale NFC Controller
version: 1.0.0+1

environment:
  sdk: '>=2.17.0 <4.0.0'

dependencies:
  flutter:
    sdk: flutter
  flutter_bluetooth_serial: ^0.4.0
  cupertino_icons: ^1.0.2

dev_dependencies:
  flutter_test:
    sdk: flutter
  flutter_lints: ^2.0.0

flutter:
  uses-material-design: true
```

### File: `lib/main.dart`

```dart
import 'package:flutter/material.dart';
import 'screens/home_screen.dart';

void main() {
  runApp(MusicPlayerApp());
}

class MusicPlayerApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Lettore Musicale',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        brightness: Brightness.light,
        useMaterial3: true,
      ),
      home: HomeScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}
```

---

## 📚 RISORSE

### Documentazione
- Flutter: https://flutter.dev/docs
- flutter_bluetooth_serial: https://pub.dev/packages/flutter_bluetooth_serial
- Material Design: https://material.io/design

### Esempi
- Bluetooth Serial Example: Esempi nella libreria
- Flutter Samples: https://github.com/flutter/samples

---

**Fine Guida Programmazione App** 📱

*Per domande o problemi, consultare la documentazione Flutter o il blueprint principale.*

