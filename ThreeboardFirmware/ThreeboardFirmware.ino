//
// OneBitDisplay library simple demo
//
// Demonstrates how to initialize and use a few functions of the library
// If your MCU has enough RAM, enable the backbuffer to see a demonstration
// of the speed difference between drawing directly on the display versus
// deferred rendering, followed by a "dump" of the memory to the display
//
#include <OneBitDisplay.h>
#include <bluefruit.h>
#include <Bounce2.h>

BLEDis bledis;
BLEHidAdafruit blehid;

// if your system doesn't have enough RAM for a back buffer, comment out
// this line (e.g. ATtiny85)
#define USE_BACKBUFFER

#ifdef USE_BACKBUFFER
static uint8_t ucBackBuffer[1024];
#else
static uint8_t *ucBackBuffer = NULL;
#endif

int leftButtonPin = 16;
int midButtonPin = 15;
int rightButtonPin = 7;

Bounce leftButton = Bounce();
Bounce midButton = Bounce();
Bounce rightButton = Bounce();

bool hasKeyPressed;

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are the pin numbers for the M5Stack Atom Grove port I2C (reversed SDA/SCL for straight through wiring)
#define SDA_PIN 2
#define SCL_PIN 3
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let OneBitDisplay figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 1

// Change these if you're using a different OLED display
#define MY_OLED OLED_128x32
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
//#define MY_OLED OLED_64x32
//#define OLED_WIDTH 64
//#define OLED_HEIGHT 32

OBDISP obd;

uint8_t keycode7[6] = { 0x5F };
uint8_t keycode8[6] = { 0x60 };
uint8_t keycode9[6] = { 0x61 };

void setup() {
  delay(5000);

  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(midButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  leftButton.attach(leftButtonPin);
  leftButton.interval(5); // interval in ms

  midButton.attach(midButtonPin);
  midButton.interval(5); // interval in ms

  rightButton.attach(rightButtonPin);
  rightButton.interval(5); // interval in ms

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("MicroMacro");

  // Configure and Start Device Information Service
  bledis.setManufacturer("Voidstar Lab");
  bledis.setModel("Micro Macro Pad");
  bledis.begin();

  blehid.begin();

  blehid.setKeyboardLedCallback(set_keyboard_led);

  int rc;
  // The I2C SDA/SCL pins set to -1 means to use the default Wire library
  // If pins were specified, they would be bit-banged in software
  // This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
  // The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
  //    that can be configured as either SPI or I2C
  //
  // obdI2CInit(OBDISP *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)

  rc = obdI2CInit(&obd, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 800000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND)
  {
    char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D", (char *)"SH1106 @ 0x3C", (char *)"SH1106 @ 0x3D"};
    obdFill(&obd, 0, 1);
    obdWriteString(&obd, 0, 0, 0, msgs[rc], FONT_8x8, 0, 1);
    obdSetBackBuffer(&obd, ucBackBuffer);
    delay(2000);
  }

  startAdv();
} /* setup() */

void loop() {
  // put your main code here, to run repeatedly:
  leftButton.update();
  midButton.update();
  rightButton.update();

  bool leftButtonPressed = leftButton.fell();
  bool midButtonPressed = midButton.fell();
  bool rightButtonPressed = rightButton.fell();

  int i, x, y;
  char szTemp[32];
  unsigned long ms;

  if ( hasKeyPressed )
  {
    hasKeyPressed = false;
    blehid.keyRelease();

    // Delay a bit after a report
    delay(5);
  }

  if (leftButtonPressed) {
    blehid.keyboardReport(0, keycode7);
    hasKeyPressed = true;
    delay(5);
  }

  if (midButtonPressed) {
    blehid.keyboardReport(0, keycode8);
    hasKeyPressed = true;
    delay(5);
  }

  if (rightButtonPressed) {
    blehid.keyboardReport(0, keycode9);
    hasKeyPressed = true;
    delay(5);
  }

  /*
    obdFill(&obd, 0x0, 1);
    obdWriteString(&obd, 0, 28, 0, (char *)"OLED Demo", FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"Written by Larry Bank", FONT_6x8, 1, 1);
    obdWriteString(&obd, 0, 0, 3, (char *)"**Demo**", FONT_16x32, 0, 1);
    delay(2000);

    // Pixel and line functions won't work without a back buffer
    #ifdef USE_BACKBUFFER
    obdFill(&obd, 0, 1);
    obdWriteString(&obd, 0, 0, 0, (char *)"Backbuffer Test", FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"3000 Random dots", FONT_8x8, 0, 1);
    delay(2000);
    obdFill(&obd, 0, 1);
    ms = millis();
    for (i = 0; i < 3000; i++)
    {
      x = random(OLED_WIDTH);
      y = random(OLED_HEIGHT);
      obdSetPixel(&obd, x, y, 1, 1);
    }
    ms = millis() - ms;
    sprintf(szTemp, "%dms", (int)ms);
    obdWriteString(&obd, 0, 0, 0, szTemp, FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"Without backbuffer", FONT_6x8, 0, 1);
    delay(2000);
    obdFill(&obd, 0, 1);
    ms = millis();
    for (i = 0; i < 3000; i++)
    {
      x = random(OLED_WIDTH);
      y = random(OLED_HEIGHT);
      obdSetPixel(&obd, x, y, 1, 0);
    }
    obdDumpBuffer(&obd, NULL);
    ms = millis() - ms;
    sprintf(szTemp, "%dms", (int)ms);
    obdWriteString(&obd, 0, 0, 0, szTemp, FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"With backbuffer", FONT_6x8, 0, 1);
    delay(2000);
    obdFill(&obd, 0, 1);
    obdWriteString(&obd, 0, 0, 0, (char *)"Backbuffer Test", FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"96 lines", FONT_8x8, 0, 1);
    delay(2000);
    ms = millis();
    for (x = 0; x < OLED_WIDTH - 1; x += 2)
    {
      obdDrawLine(&obd, x, 0, OLED_WIDTH - x, OLED_HEIGHT - 1, 1, 1);
    }
    for (y = 0; y < OLED_HEIGHT - 1; y += 2)
    {
      obdDrawLine(&obd, OLED_WIDTH - 1, y, 0, OLED_HEIGHT - 1 - y, 1, 1);
    }
    ms = millis() - ms;
    sprintf(szTemp, "%dms", (int)ms);
    obdWriteString(&obd, 0, 0, 0, szTemp, FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"Without backbuffer", FONT_6x8, 0, 1);
    delay(2000);
    obdFill(&obd, 0, 1);
    ms = millis();
    for (x = 0; x < OLED_WIDTH - 1; x += 2)
    {
      obdDrawLine(&obd, x, 0, OLED_WIDTH - 1 - x, OLED_HEIGHT - 1, 1, 0);
    }
    for (y = 0; y < OLED_HEIGHT - 1; y += 2)
    {
      obdDrawLine(&obd, OLED_WIDTH - 1, y, 0, OLED_HEIGHT - 1 - y, 1, 0);
    }
    obdDumpBuffer(&obd, ucBackBuffer);
    ms = millis() - ms;
    sprintf(szTemp, "%dms", (int)ms);
    obdWriteString(&obd, 0, 0, 0, szTemp, FONT_8x8, 0, 1);
    obdWriteString(&obd, 0, 0, 1, (char *)"With backbuffer", FONT_6x8, 0, 1);
    delay(2000);
    #endif
  */
} /* loop() */

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);

  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();

  /* Start Advertising
     - Enable auto advertising if disconnected
     - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     - Timeout for fast mode is 30 seconds
     - Start(timeout) with timeout = 0 will advertise forever (until connected)

     For recommended advertising interval
     https://developer.apple.com/library/content/qa/qa1931/_index.html
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

/**
   Callback invoked when received Set LED from central.
   Must be set previously with setKeyboardLedCallback()

   The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
      Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
*/
void set_keyboard_led(uint16_t conn_handle, uint8_t led_bitmap)
{
  (void) conn_handle;

  // light up Red Led if any bits is set
  if ( led_bitmap )
  {
    ledOn( LED_RED );
  }
  else
  {
    ledOff( LED_RED );
  }
}
