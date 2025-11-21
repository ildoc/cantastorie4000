// Cosa fare:

// Simulare pulsanti con comandi Serial
// Sviluppare logica di controllo
// Testare debouncing


// Simulatore Pulsanti - Testa logica controlli prima di avere hardware
// Invia 'P' per Play, 'N' per Next, 'V' per Prev via Serial Monitor

// Variabili stato
bool isPlaying = false;
int currentTrack = 1;
int currentVolume = 15;

// Debounce simulato
unsigned long lastButtonTime = 0;
const int DEBOUNCE_DELAY = 50;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== Simulatore Controlli ===");
  Serial.println("Comandi disponibili:");
  Serial.println("P = Play/Pausa");
  Serial.println("N = Next");
  Serial.println("V = Prev (preVious)");
  Serial.println("+ = Volume Up");
  Serial.println("- = Volume Down");
  Serial.println("S = Status");
  Serial.println("\nPronto!\n");
  
  printStatus();
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    // Debounce simulato
    if (millis() - lastButtonTime < DEBOUNCE_DELAY) {
      return;
    }
    lastButtonTime = millis();
    
    // Gestione comandi
    switch(cmd) {
      case 'P':
      case 'p':
        handlePlayPause();
        break;
        
      case 'N':
      case 'n':
        handleNext();
        break;
        
      case 'V':
      case 'v':
        handlePrev();
        break;
        
      case '+':
        handleVolumeUp();
        break;
        
      case '-':
        handleVolumeDown();
        break;
        
      case 'S':
      case 's':
        printStatus();
        break;
    }
  }
}

void handlePlayPause() {
  isPlaying = !isPlaying;
  Serial.print("[PLAY/PAUSA] ");
  if (isPlaying) {
    Serial.print("▶️ Riproducendo traccia ");
    Serial.println(currentTrack);
    // Qui chiameresti: dfPlayer.start();
  } else {
    Serial.println("⏸️ In pausa");
    // Qui chiameresti: dfPlayer.pause();
  }
}

void handleNext() {
  currentTrack++;
  if (currentTrack > 99) currentTrack = 1;
  
  Serial.print("[NEXT] ⏭️ Traccia ");
  Serial.println(currentTrack);
  // Qui chiameresti: dfPlayer.next();
}

void handlePrev() {
  currentTrack--;
  if (currentTrack < 1) currentTrack = 99;
  
  Serial.print("[PREV] ⏮️ Traccia ");
  Serial.println(currentTrack);
  // Qui chiameresti: dfPlayer.previous();
}

void handleVolumeUp() {
  if (currentVolume < 25) {
    currentVolume++;
    Serial.print("[VOLUME] 🔊 Volume: ");
    Serial.println(currentVolume);
    // Qui chiameresti: dfPlayer.volume(currentVolume);
  } else {
    Serial.println("[VOLUME] 🔊 Volume già al massimo!");
  }
}

void handleVolumeDown() {
  if (currentVolume > 0) {
    currentVolume--;
    Serial.print("[VOLUME] 🔉 Volume: ");
    Serial.println(currentVolume);
    // Qui chiameresti: dfPlayer.volume(currentVolume);
  } else {
    Serial.println("[VOLUME] 🔇 Volume già al minimo!");
  }
}

void printStatus() {
  Serial.println("\n--- STATUS ---");
  Serial.print("Stato: ");
  Serial.println(isPlaying ? "▶️ Riproducendo" : "⏸️ In pausa");
  Serial.print("Traccia: ");
  Serial.println(currentTrack);
  Serial.print("Volume: ");
  Serial.print(currentVolume);
  Serial.println("/25");
  Serial.println("--------------\n");
}
