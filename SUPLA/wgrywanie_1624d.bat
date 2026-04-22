esptool.exe -p COM6 erase_flash
esptool.exe -p COM6 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m  0x0 SM-LITE-1624D-hw15-v26.04.21-M.bin
