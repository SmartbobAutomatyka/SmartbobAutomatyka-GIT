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

SHA-256:
SMARTBOB-0202R-full-2026-06-20.bin  5daaf99957844550b8324b50bf2116e6531fb7cda8e11e4dd0927357868f686d
SMARTBOB-0808R-full-2026-06-20.bin  0d951c3492200a71d00558adac4bd4045c66b81a3175719a1ca77ffb71e3997d
SMARTBOB-1616R-full-2026-06-20.bin  7b1a659dede4d834d79cf0b19883bb6fd858c33b8274ab6ba90c0a4120c0ae51
