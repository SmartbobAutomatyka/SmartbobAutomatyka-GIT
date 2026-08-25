SMARTBOB - pelne obrazy firmware 2026-06-20

Pliki *-full-2026-06-20.bin zawieraja:
- bootloader,
- tablice partycji,
- firmware,
- LittleFS z panelem WWW.

Wgrywanie od adresu 0x0:

python3 -m esptool --chip esp32 --port /dev/cu.usbserial-XXXX --baud 460800 write_flash 0x0 SMARTBOB-0808R-full-2026-06-20.bin

Zmien nazwe portu i pliku odpowiednio do sterownika.

UWAGA: pelny obraz nadpisuje LittleFS, a wiec rowniez zapisana konfiguracje sterownika.
Nie wgrywaj obrazu przeznaczonego dla innego modelu.
