esptool.exe -p COM4 erase_flash
esptool.exe -p COM4 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m  0x0 SM-LITE-1616R-hw17-v26.04.15-M.bin
