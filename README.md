
# Amlogic_s905W2_S4
Info about Amlogic S905W2 ANDL recovery procedure

## Recover from erase bootloader

When you flash wrong bootloader for Your device, there is a way to boot it from USB/RAM using new adnl protocol.

When bootloader is damaged or erased you will see on UART something like this:
`S4:BL:25dcde;ID:00D9A30840B12824;FEAT:30F:1FFF0000:B002F:19;POC:FF;RCY:0;OVD:0;DFU:0S4:BL:25dcde;ID:00D9A30840B12824;FEAT:30F:1FFF0000:B002F:19;POC:FF;RCY:0;OVD:0;DFU:0;eMMC:0;RD-0:0;CHK:0;`
It means that device is in recovery mode using Amlogic new ADNL protocol. To fix this you need:

1. USB Male cable (both sides)
2. Original firmware (or slimbox firmware) to have 2 files: `DDR.USB` and `bootloader.PARTITION`
3. ADNL Tools (you can get from this repo for Windows)
4. Amlogic USB drivers
5. FAT32 formatted USB drive

### How to
1. Connect USB cable to USB port and PC. Connect UART pins to have console access.
2. Place DDR.USB and bootloader.PARITION in same directory as `adnl.exe` 
3. Place `bootloader.PARTITION` on USB drive.
4. Check connection between Amlogic and PC:
`adnl.exe devices`
should show somethiing like
`00d9a30840b1282400000000        Aml_DNL`
5. Next check if device is in BL1 mode:
`adnl getvar identify`
should return
`DNL mode [BL1]  06-00-00-00-00-00-00-0F`
5. Upload to memory BL2 stage from DDR.USB
`adnl  bl1_boot -f DDR.USB`
should return:
`MSG[DNL]Amlogic USB DNL tool: V[2.6.3] at Aug 20 2021`
`bl1_boot[DDR.USB]...`
`MSG[DNL]device mode:06-00-00-00`
`MSG[DNL]soc[37], feat 0`
`MSG[DNL]download sz is 0x00029800`
`MSG[DNL]okay download bl2`
`response:`
`OKAY [  0.045s]`
`finished. total time: 0.046s`

7. On UART console you should see:
`INFO: bl2_load_bl2ex start!`
`INFO: load ...`
`Load: BL2E From: USB - 1.0 src: 00002000, dst: 00fffff0, size: 00011000`

8. Load BL3 (U-Boot) from `bootloader.PARITION`:
`adnl  bl2_boot -f bootloader.PARTITION`
and result should be booting U-Boot from RAM.

9. Interrupt it on command line befere autoboot. Place USB stick into second USB port in BOX and flash it to MMC:
`usb start`
`usb_update bootloader bootloader.PARTITION`
and you should see:
`BURN bootloader.PARTITION to part bootloader OK!
`=====>Burn part bootloader in fmt normal OK<======
10. Disconnect USB cable and issue `reset` in U-Boot command line. Box should work again.




