esptool.exe -p COM4 erase_flash
esptool.exe -p COM4 -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 SM-MINI-0808R/SMARTBOBSOFT-SM-0808R.ino.bootloader.bin 0x8000 SM-MINI-0808R/SMARTBOBSOFT-SM-0808R.ino.partitions.bin 0x10000 SM-MINI-0808R/SMARTBOBSOFT-SM-0808R.ino.bin
