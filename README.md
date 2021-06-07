The littlest wireless macro pad! AWWWWW!!!
(c) 2021 Zack Freedman and Voidstar Lab. Licensed CC-BY-NC (credit me and don't sell it)
Built partially on stream (like comment and subscribe baby) so it might suck.

Print one of each STL. The faceplate and the middle piece with the stupid name need supports, the baseplate don't.

You'll also need:
* An Adafruit NRF52 Feather* - https://amzn.to/3inlreo
* 3x Cherry-sized keyswitches - https://amzn.to/3pqnakz
* 3x keycaps of your choice (NSFW) - https://amzn.to/3v0asKp 
* A 128x32 SSD1306 OLED module - https://amzn.to/3z47vvA
* A 750mAh Li-ion 14500 battery - https://amzn.to/3gfcfG7
* 2x AA battery clips - https://www.digikey.com/en/products/detail/mpd-memory-protection-devices/BK-92/2079904
* 4x 10mm and 2x 8mm M2.5 screws - https://amzn.to/3fZuJM1
* 4x 8mm rubber feet - https://amzn.to/2RrHw04
* Some wire
* Some hot glue

Assembly:
* Print everything ya dingus
* Stick the switches in the switch holes
* Jam the battery clips into their battery clip slots
* Use a soldering iron set to 210C or something to melt the plastic around the battery clips to fuse them in place
* Wire one pin of each switch together
* Wire those switches to one of the battery clips. This is now the GND clip.
* Solder a wire to the other clip, that's now the VBAT clip
* Solder a wire to the other pin of each switch
* Solder wires to the OLED and glue it into position in the faceplate
* Solder the OLED GND wire to the GND clip. It's kinda overloaded, I know
* Solder the GND wire to the GND pad on the Feather
* Solder the VBAT wire to the VBAT pad
* Solder the left, center, and right switch wires to pins 16, 15, and 7
* Solder the OLED's SDA, SCL, and 3V to the corresponding pads
* Screw it together. The 8mm screws are in the bottom front corners.
* Stick the feet on the bottom. Or the side, or the top. You're the boss.
* Charge that battery and load the code
* Go to patreon.com/zackfreedman and give me all your money

\*you should probably buy this directly from Adafruit