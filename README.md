# SMARTBOB — firmware sterowników i czujników

Repozytorium zawiera firmware dla sterowników SMARTBOB SM-LITE oraz czujnika obecności SMARTBOB PS01C3. Gotowe pliki binarne znajdują się w katalogu [`SMARTBOBSOFT`](SMARTBOBSOFT).

## Dostępne urządzenia

| Firmware | Platforma | Wejścia | Wyjścia | Czujniki i interfejsy |
|---|---|---:|---:|---|
| SM-LITE-0202R | ESP32, 4 MB | 2 bezpośrednie, aktywne stanem niskim | 2 przekaźniki | Ethernet, Wi-Fi, OLED, 1-Wire, 1 × TMP102 `0x49`, 2 × wejście analogowe 24 V, RS485, opcjonalne zewnętrzne MCP23017 |
| SM-LITE-0808R | ESP32, 4 MB | 8 przez MCP23017 | 8 przekaźników przez MCP23017 | Ethernet, Wi-Fi, OLED, 1-Wire, 1 × TMP102 `0x49`, 2 × wejście analogowe 24 V, SCT-013, RS485, opcjonalne zewnętrzne MCP23017 |
| SM-LITE-1616R | ESP32, 4 MB | 16 przez MCP23017 | 16 przekaźników przez MCP23017 | Ethernet, Wi-Fi, OLED, 1-Wire, 2 × TMP102 `0x48`/`0x49`, 2 × wejście analogowe 24 V, SCT-013, RS485, opcjonalne zewnętrzne MCP23017 |

## Pozostałe katalogi i platformy

### LOXONE-UDP-STARE

Katalog `LOXONE-UDP-STARE` zawiera starsze wersje firmware SMARTBOB przeznaczone do integracji z systemami Loxone, Ampio oraz MQTT. Są to wydania archiwalne, utrzymywane głównie ze względu na zgodność ze starszymi instalacjami i wcześniejszym sposobem komunikacji.

Do nowych instalacji zalecane są aktualne firmware z katalogów `SM-LITE-0202R`, `SM-LITE-0808R` i `SM-LITE-1616R`, chyba że dana instalacja wymaga konkretnej starszej wersji.

### SUPLA

Katalog `Supla` zawiera firmware SMARTBOB przeznaczone do pracy z platformą SUPLA. Należy wybierać plik zgodny z dokładnym modelem sterownika i jego wersją sprzętową.

### ESPHome i Home Assistant

Katalog [ESPHOME](https://github.com/SmartbobAutomatyka/SmartbobAutomatyka-GIT/tree/main/ESPHOME) zawiera konfiguracje firmware oparte na ESPHome, przeznaczone do integracji urządzeń SMARTBOB z platformą Home Assistant.

Firmware ESPHome należy kompilować i instalować zgodnie z dokumentacją ESPHome oraz Home Assistant. Przed instalacją trzeba sprawdzić zgodność konfiguracji z modelem urządzenia i mapowaniem jego GPIO.

## SM-LITE-0202R

Firmware dla dwuwejściowego i dwuprzekaźnikowego sterownika SM-LITE-0202R.

Najważniejsze cechy sprzętowe:

- wejścia `IN1` i `IN2`: GPIO39 i GPIO36;
- wejścia są aktywne stanem `LOW`;
- przekaźniki `OUT1` i `OUT2`: GPIO2 i GPIO4;
- sterownik nie ma wlutowanych ekspanderów MCP23017 dla lokalnych wejść i wyjść;
- opcjonalne zewnętrzne ekspandery wejść działają na I²C1 pod adresami `0x20`–`0x23`;
- jeden czujnik temperatury TMP102 pod adresem `0x49`;
- OLED SSD1306 pod adresem `0x3C`;
- 1-Wire na GPIO32;
- wejścia analogowe na GPIO34 i GPIO35;
- RS485: TX GPIO14, RX GPIO13;
- Ethernet: MDC GPIO23, MDIO GPIO18, zegar GPIO17.

W wydaniu z 2026-08-28 poprawiono polaryzację wejść bezpośrednich, konfigurację jednego TMP102 oraz obsługę płytki bez lokalnych ekspanderów MCP. Przed odczytem TMP102 sprawdzana jest jego obecność, dzięki czemu brak odpowiedzi urządzenia nie powoduje ciągłego komunikatu I²C `Error 263`.

Projekt źródłowy: [`SM-LITE-0202R`](SM-LITE-0202R)

Zalecany aktualny plik:

- [`SMARTBOB-0202R-full-2026-08-28.bin`](SMARTBOBSOFT/SMARTBOB-0202R-full-2026-08-28.bin)

## SM-LITE-0808R

Firmware dla ośmiowejściowego i ośmioprzekaźnikowego sterownika SM-LITE-0808R.

Najważniejsze cechy sprzętowe:

- 8 wejść i 8 wyjść obsługiwanych przez MCP23017 na I²C0;
- MCP23017 pod adresem `0x20`;
- jeden czujnik TMP102 pod adresem `0x49`;
- OLED SSD1306 pod adresem `0x3C`;
- 1-Wire na GPIO14;
- wejścia analogowe na GPIO34 i GPIO35;
- SCT-013 na GPIO36;
- RS485: TX GPIO33, RX GPIO13;
- Ethernet oraz Wi-Fi;
- dodatkowa magistrala I²C1: SDA GPIO16, SCL GPIO32.

Projekt źródłowy: [`SM-LITE-0808R`](SM-LITE-0808R)

Zalecany aktualny plik:

- [`SMARTBOB-0808R-full-2026-08-25.bin`](SMARTBOBSOFT/SMARTBOB-0808R-full-2026-08-25.bin)

## SM-LITE-1616R

Firmware dla szesnastowejściowego i szesnastoprzekaźnikowego sterownika SM-LITE-1616R.

Najważniejsze cechy sprzętowe:

- 16 wejść przez MCP23017 pod adresem `0x20`;
- 16 przekaźników przez MCP23017 pod adresem `0x21`;
- dwa czujniki TMP102 pod adresami `0x48` i `0x49`;
- OLED SSD1306 pod adresem `0x3C`;
- 1-Wire na GPIO32;
- wejścia analogowe na GPIO34 i GPIO35;
- SCT-013 na GPIO39;
- RS485: TX GPIO33, RX GPIO13;
- Ethernet oraz Wi-Fi;
- dodatkowa magistrala I²C1: SDA GPIO16, SCL GPIO14.

Projekt źródłowy: [`SM-LITE-1616R`](SM-LITE-1616R)

Zalecany aktualny plik:

- [`SMARTBOB-1616R-full-2026-08-25.bin`](SMARTBOBSOFT/SMARTBOB-1616R-full-2026-08-25.bin)

## Wspólne funkcje sterowników SM-LITE

Firmware 0202R, 0808R i 1616R korzysta ze wspólnego panelu i udostępnia między innymi:

- panel WWW przechowywany na partycji LittleFS;
- Ethernet i opcjonalne Wi-Fi;
- awaryjny punkt dostępowy z nazwą zależną od modelu i końcówki adresu MAC;
- konfigurację DHCP lub statycznego adresu IP;
- obsługę wejść, przekaźników, przycisków i rolet;
- regulowany debounce wejść i przycisków;
- integrację Loxone przez UDP lub bezpieczny WebSocket;
- eksport konfiguracji wejść i wyjść do Loxone Config;
- integrację MQTT;
- integrację Homey przez HTTP;
- czujniki DS18B20 na magistrali 1-Wire;
- odczyt temperatury PCB z TMP102 zależnie od modelu;
- dwa wejścia analogowe 24 V;
- licznik energii Eastron przez RS485/Modbus;
- opcjonalny pomiar prądu SCT-013 w obsługiwanych modelach;
- opcjonalne zewnętrzne ekspandery wejść MCP23017;
- podgląd stanu komunikacji i dziennik błędów Loxone.

Jeżeli po uruchomieniu nie jest dostępna skonfigurowana sieć, sterownik uruchamia własny punkt dostępowy. Domyślne hasło AP to `12345678`.

## Znaczenie nazw plików

Nazwy plików mają postać:

```text
SMARTBOB-<MODEL>-full-<ROK-MIESIĄC-DZIEŃ>.bin
SMARTBOB-<MODEL>-v<WERSJA>-<ROK-MIESIĄC-DZIEŃ>.bin
```

- `full` dla sterowników SM-LITE oznacza kompletny obraz pamięci zawierający bootloader, tablicę partycji, aplikację oraz panel WWW LittleFS. Wgrywa się go od adresu `0x0`;

- do nowych instalacji należy wybierać najnowszy plik przeznaczony dokładnie dla danego modelu.

Nie wolno wgrywać firmware przeznaczonego dla innego modelu sterownika. Poszczególne modele mają inne mapowanie GPIO, liczbę ekspanderów i adresy urządzeń I²C.
