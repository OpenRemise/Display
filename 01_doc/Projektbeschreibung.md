# Projektbeschreibung: Openremise_Display

Dieses Dokument bietet eine Übersicht über die Module des Projekts *Openremise_Display* und listet die wichtigsten `#define`-Makros. Die Beschreibungen sind in deutscher Sprache verfasst.

---

## 📁 Module

### Display
Das Modul `Display` übernimmt die Steuerung des OLED-Displays (128x128, SH1107). Es ist in den Dateien `Display.h` und `Display.cpp` organisiert.

**Kernfunktionen:**
- Initialisierung des Displays (`begin()`)
- Aktualisierung der Anzeige (`update()`, `force_update()`)
- Verwaltung von Bildschirmen (Willkommens-, Fehler-, Normalmodus, Lernmodus usw.)
- Speicherung und Anzeige von statischen und dynamischen Textinhalten
- Implementierung von Blinkeffekten und Frame-Steuerung

Der `OpenRemiseDisplay`-Konstruktor konfiguriert die u8g2-Bibliothek für Hardware-I²C.
Verschiedene Helper-Funktionen ermitteln den aktuellen Bildschirm und toggeln Blinkstatus.

### Main
Die Hauptlogik befindet sich in `main.cpp` (bei Bedarf auch in `main.h` für globale Strukturen).

**Aufgaben:**
- Initialisierung des Systems (Zustand, Fehlercode, Schnittstellen, Anzeige, Tasten)
- Einrichtung des Protokollmoduls und Warten auf erste Daten vom Transport
- Hauptloop: Aktualisierung von Protokoll, Tasten und Anzeige, Reaktion auf Benutzerinteraktionen und Zeitüberschreitungen
- Bildschirmsteuerung und Inhaltsbereitstellung basierend auf aktuellem Bildschirm
- LED-Management und Schlafmodus

Globale Strukturen (`SYS_`) und Funktionen (`Set_next_Screen`, `Set_LEDS`, `Set_Content`) definieren den Ablauf der Anwendung.

### Buttons
Das Modul `Buttons` (Dateien `Buttons.h`/`Buttons.cpp`) verwaltet zwei Tasten (A/B) plus zugeordnete LEDs.

**Eigenschaften:**
- Entprellung und Erkennung von Drücken, Loslassen, Halten und Reset-Anforderung
- LED-Steuerung für jede Taste
- Timerbasierte Logik für Halte- und Resetdauer
- Rückgabe eines Bitmasken-Werts, der den aktuellen Zustand der beiden Tasten codiert

### Protokoll
`protokoll.h`/`Protokoll_2.cpp` implementieren einen einfachen JSON-basierten Parser für eingehende Datenströme über einen `Stream` (z. B. Hardware-Serial).

**Details:**
- Definierte Kommando- und JSON-Feldcodes
- Zustandsmaschine zum Abarbeiten von Schlüssel-Wert-Paaren im JSON-Format
- Speicher für empfangene Zeichen und Abbildung in interne `data`-Strings
- Methoden zur Fehler- und Timeout-Prüfung sowie zum Abruf empfangener Inhalte

Der Parser ist bewusst schlank gehalten, um auf Microcontrollern effizient zu laufen.

---

## 📌 Übersicht der wichtigsten `#define`-Makros

### Allgemeine Definitionen (z. B. in `main.h`)
```
#define SW_Version "Rev 1.0.0"
#define ON 1
#define OFF 0
#define INIT 0
#define RUN 1

//#define SERIAL_DEBUG // Enable Serial debugging
#define Feedback_LED_PIN 13
#define SLEEP_TIMEOUT 30000
#define DISPLAY_UPDATE_INTERVAL 150
#define WAIT_TIME_STARTTIME 15000

// Button/LED Pin-Belegungen
#define BUTTON_A_PIN 4
#define LED_A_PIN 5
#define BUTTON_B_PIN 6
#define LED_B_PIN 7

// SoftSerial Pins für Protokoll
#define PROTOKOLL_RX_PIN 10
#define PROTOKOLL_TX_PIN 11
```

### Tasten-Makros (`Buttons.h`)
```
#define BUTTON_A_PRESSED  0x10
#define BUTTON_B_PRESSED  0x01
#define BUTTON_A_HOLD     0x20
#define BUTTON_B_HOLD     0x02
#define NO_BUTTONS        0x00
#define mask_Button_A    0xF0
#define mask_Button_B    0x0F
#define mask_out_Button_A      0x0F
#define mask_out_Button_B      0xF0

#define BUTTON_A          0x00
#define BUTTON_B          0x01
#define RESET_REQUEST     0xEE
#define BUTTON_FAIL       0xFF

// LED-Logik
#define LED_ON           LOW
#define LED_OFF          HIGH
#define Button_DEBOUNCE_TIME 50
#define BUTTON_HOLD_TIME    160
#define BUTTON_RESET_TIME   5000
```

### Display-Makros (`Display.h`)
```
#define SCREEN_Title_x 128-32
#define SCREEN_Title_y 20
#define LOGO_x 8
#define LOGO_y 8
#define SCREEN_FIXED_X 5
#define SCREEN_FIXED_Y 75
#define SCREEN_LINE_DIS 15
#define SCREEN_DATA_X 50

#define BLINKTIME 750
```
(Zusätzlich sind zahlreiche Bildschirm-IDs und Strukturen im Header definiert.)

### Protokoll-Makros (`protokoll.h`)
```
#define PROTOKOLL_MANUAL

// Kommando-Codes
#define ALIVE 0x07
#define STATE1 0x11
#define STATE2 0x12
#define ERROR  0x13
#define WRITE_DIRECT 0x02
#define WRITE_END 0x03
#define CLEAR_DISPLAY 0x04
#define JSON_DISPLAY_CLEAR "display_clear"

// JSON-Feldernamen
#define JSON_ID         "version"
#define JSON_IP         "ip"
#define JSON_STATUS     "state"
#define JSON_WIFI_SSID   "ssid"
#define JSON_MDNS         "mdns"
#define JSON_WIFI_PWR     "rssi"
#define JSON_TRK_VOLTAGE  "voltage"
#define JSON_TRK_CURRENT  "current"
#define JSON_SUP_VOLTAGE  "sup_voltage"
#define JSON_Service      "service"
// (weitere, teils auskommentiert)

#define PROTOKOLL_DATA_TIMEOUT 2000
#define RX_BUFFER_SIZE  25
#define ERROR_TEXT_LEN  10
#define ERROR_INFO_LEN  10
#define MAX_STRING  16
```

---

> **Hinweis:** Diese Dokumentation stellt eine Zusammenfassung der aktuellen Implementierung dar. Für detaillierte Informationen sollte der Quellcode direkt konsultiert werden.

---

*Ende der Projektbeschreibung.*
