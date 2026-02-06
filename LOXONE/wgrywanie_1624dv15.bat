esptool.exe -p COM4 erase_flash
esptool.exe -p COM4 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 SM-LITE-1624DV15/SMARTBOBSOFT-SM-1624DV15.ino.bootloader.bin 0x8000 SM-LITE-1624DV15/SMARTBOBSOFT-SM-1624DV15.ino.partitions.bin 0x10000 SM-LITE-1624DV15/SMARTBOBSOFT-SM-1624DV15.ino.bin
