esptool.exe -p COM4 erase_flash
esptool.exe -p COM4 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 SM-LITE-1624D/SMARTBOBSOFT-SM-1624D.ino.bootloader.bin 0x8000 SM-LITE-1624D/SMARTBOBSOFT-SM-1624D.ino.partitions.bin 0x10000 SM-LITE-1624D/SMARTBOBSOFT-SM-1624D.ino.bin
