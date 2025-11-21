// Cosa fare:

// Implementare comunicazione Bluetooth
// Testare con app Serial Bluetooth Terminal
// Sviluppare protocollo comandi

// Simulatore NFC - Testa logica associazioni prima di avere PN532
// Comandi: T1-T5 = simula tag 1-5, A<tag>:<folder> = associa tag a cartella

#include <Preferences.h>

Preferences preferences;

// UID simulati di 5 tag
const String tagUIDs[5] = {
  "04A1B2C3",
  "05C3D4E5", 
  "06E5F6A7",
  "07A8B9C0",
  "08D1E2F3"
};

int currentFolder = 0;
String lastTagUID = "";
unsigned long lastTagTime = 0;
const int TAG_DEBOUNCE = 2000; // 2 secondi

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Init Preferences
  preferences.begin("music-player", false);
  
  Serial.println("\n=== Simulatore NFC ===");
  Serial.println("Comandi disponibili:");
  Serial.println("T1-T5 = Simula lettura tag 1-5");
  Serial.println("A<tag>:<folder> = Associa tag a cartella");
  Serial.println("  Esempio: A1:2 = Associa tag 1 alla cartella 2");
  Serial.println("L = Lista associazioni");
  Serial.println("C = Cancella tutte associazioni");
  Serial.println("S = Status\n");
  
  // Mostra tag disponibili
  Serial.println("Tag disponibili:");
  for (int i = 0; i < 5; i++) {
    Serial.print("Tag ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(tagUIDs[i]);
    
    // Controlla se associato
    String key = "tag_" + tagUIDs[i];
    int folder = preferences.getInt(key.c_str(), 0);
    if (folder > 0) {
      Serial.print(" → Cartella ");
      Serial.println(folder);
    } else {
      Serial.println(" (non associato)");
    }
  }
  Serial.println();
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    // Comandi tag T1-T5
    if (cmd.length() == 2 && cmd[0] == 'T') {
      int tagNum = cmd[1] - '0';
      if (tagNum >= 1 && tagNum <= 5) {
        simulateTagRead(tagNum - 1);
      }
    }
    // Comando associazione A<tag>:<folder>
    else if (cmd[0] == 'A' || cmd[0] == 'a') {
      handleAssociation(cmd);
    }
    // Lista associazioni
    else if (cmd == "L" || cmd == "l") {
      listAssociations();
    }
    // Cancella tutto
    else if (cmd == "C" || cmd == "c") {
      clearAllAssociations();
    }
    // Status
    else if (cmd == "S" || cmd == "s") {
      printStatus();
    }
  }
}

void simulateTagRead(int tagIndex) {
  String uid = tagUIDs[tagIndex];
  
  Serial.print("\n[NFC] 📱 Tag ");
  Serial.print(tagIndex + 1);
  Serial.print(" letto: ");
  Serial.println(uid);
  
  // Debounce
  if (uid == lastTagUID && (millis() - lastTagTime) < TAG_DEBOUNCE) {
    Serial.println("[NFC] ⏸️ Tag già letto recentemente (debounce)");
    return;
  }
  
  lastTagUID = uid;
  lastTagTime = millis();
  
  // Cerca associazione
  String key = "tag_" + uid;
  int folder = preferences.getInt(key.c_str(), 0);
  
  if (folder > 0) {
    Serial.print("[NFC] ✅ Cartella associata: ");
    Serial.println(folder);
    currentFolder = folder;
    
    // Qui chiameresti: dfPlayer.loopFolder(folder);
    Serial.print("      ▶️ Riproduzione cartella ");
    Serial.println(folder);
  } else {
    Serial.println("[NFC] ❌ Tag non associato!");
    Serial.println("      Usa comando A per associarlo.");
    Serial.print("      Esempio: A");
    Serial.print(tagIndex + 1);
    Serial.println(":1");
    
    // Qui riprodurresti suono errore
    // dfPlayer.playMp3Folder(9999);
  }
}

void handleAssociation(String cmd) {
  // Formato: A<tag>:<folder>
  // Esempio: A1:2 = tag 1 → cartella 2
  
  int colonPos = cmd.indexOf(':');
  if (colonPos == -1) {
    Serial.println("[ERRORE] Formato: A<tag>:<folder>");
    Serial.println("         Esempio: A1:2");
    return;
  }
  
  String tagStr = cmd.substring(1, colonPos);
  String folderStr = cmd.substring(colonPos + 1);
  
  int tagNum = tagStr.toInt();
  int folder = folderStr.toInt();
  
  if (tagNum < 1 || tagNum > 5) {
    Serial.println("[ERRORE] Tag deve essere 1-5");
    return;
  }
  
  if (folder < 1 || folder > 99) {
    Serial.println("[ERRORE] Cartella deve essere 1-99");
    return;
  }
  
  String uid = tagUIDs[tagNum - 1];
  String key = "tag_" + uid;
  preferences.putInt(key.c_str(), folder);
  
  Serial.print("[ASSOCIAZIONE] ✅ Tag ");
  Serial.print(tagNum);
  Serial.print(" (");
  Serial.print(uid);
  Serial.print(") → Cartella ");
  Serial.println(folder);
}

void listAssociations() {
  Serial.println("\n=== ASSOCIAZIONI SALVATE ===");
  
  bool hasAssociations = false;
  for (int i = 0; i < 5; i++) {
    String key = "tag_" + tagUIDs[i];
    int folder = preferences.getInt(key.c_str(), 0);
    
    if (folder > 0) {
      Serial.print("Tag ");
      Serial.print(i + 1);
      Serial.print(" (");
      Serial.print(tagUIDs[i]);
      Serial.print(") → Cartella ");
      Serial.println(folder);
      hasAssociations = true;
    }
  }
  
  if (!hasAssociations) {
    Serial.println("Nessuna associazione salvata.");
  }
  Serial.println();
}

void clearAllAssociations() {
  Serial.print("[CANCELLAZIONE] ⚠️ Cancellare tutte le associazioni? (Y/N): ");
  
  // Aspetta conferma
  while (!Serial.available()) delay(10);
  char confirm = Serial.read();
  Serial.println(confirm);
  
  if (confirm == 'Y' || confirm == 'y') {
    preferences.clear();
    Serial.println("[CANCELLAZIONE] ✅ Tutte le associazioni cancellate!");
  } else {
    Serial.println("[CANCELLAZIONE] ❌ Operazione annullata.");
  }
}

void printStatus() {
  Serial.println("\n--- STATUS ---");
  Serial.print("Cartella corrente: ");
  if (currentFolder > 0) {
    Serial.println(currentFolder);
  } else {
    Serial.println("Nessuna");
  }
  Serial.print("Ultimo tag: ");
  Serial.println(lastTagUID.length() > 0 ? lastTagUID : "Nessuno");
  Serial.println("--------------\n");
}
