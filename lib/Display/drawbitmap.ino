/***************************************************
  This is our Bitmap drawing example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <ILI9341_t3.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.

#define TFT_DC  9
#define TFT_CS 37
#define RESET 10
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, RESET);

#define SD_CS BUILTIN_SDCARD

void setup(void) {
  tft.begin();
  tft.setClock(60000000);
  tft.fillScreen(ILI9341_BLUE);
  tft.setRotation( 3 );

  Serial.begin(115200);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(F("Waiting for Arduino Serial Monitor..."));
  while (!Serial) {
    if (millis() > 8000) break;
  }

  Serial.print(F("Initializing SD card..."));
  tft.println(F("Init SD card..."));
  while (!SD.begin(SD_CS)) {
    Serial.println(F("failed to access SD card!"));
    tft.println(F("failed to access SD card!"));
    delay(100);
  }
  Serial.println("OK!");
}

void loop() {
  // bmpDraw("home2.bmp", 0, 0);
  // delay(1000);
  bmpDraw("home.bmp", 0, 0);
  delay(1000);
  bmpDraw("cat.bmp", 0, 0);
  delay(1000);
  bmpDraw("task_bar.bmp", 0, 210);
  delay(1000);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance for tiny AVR chips.

// Larger buffers are slightly more efficient, but if
// the buffer is too large, extra data is read unnecessarily.
// For example, if the image is 240 pixels wide, a 100
// pixel buffer will read 3 groups of 100 pixels.  The
// last 60 pixels from the 3rd read may not be used.

#define BUFFPIXEL 80

//===========================================================
// Try Draw using writeRect
void bmpDraw(const char *filename, uint8_t xStart, uint16_t yStart) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  // uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  uint16_t awColors[240][320];  // hold colors for one row at a time...

  // Open requested file on SD card
  if (!(bmpFile = SD.open(filename))) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
  
    (void)read32(bmpFile); // file size
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    // Read DIB header
    (void)read32(bmpFile); // header size
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    (void)read16(bmpFile); // # planes -- must be '1'
    (void)read16(bmpFile); // bits per pixel must be 24
    (void)read32(bmpFile); // must be '0' // 0 = uncompressed

    goodBmp = true; // Supported BMP format -- proceed!

    // BMP rows are padded (if needed) to 4-byte boundary
    rowSize = (bmpWidth * 3 + 3) & ~3;

    // If bmpHeight is negative, image is in top-down order.
    // This is not canon but has been observed in the wild.
    if(bmpHeight < 0) {
      bmpHeight = -bmpHeight;
      flip      = false;
      // no
    }

    // Crop area to be loaded
    w = bmpWidth; // 240
    h = bmpHeight; // 320

    for (row=0; row<h; row++) { // For each scanline...
      if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
      else     // Bitmap is stored top-to-bottom
        pos = bmpImageoffset + row * rowSize;
      if(bmpFile.position() != pos) { // Need seek?
        bmpFile.seek(pos);
        buffidx = sizeof(sdbuffer); // Force buffer reload
      }
      
      for (col=0; col<w; col++) { // For each pixel...
        // Time to read more pixel data?
        if (buffidx >= sizeof(sdbuffer)) { // Indeed
          bmpFile.read(sdbuffer, sizeof(sdbuffer));
          buffidx = 0; // Set index to beginning
        }

        // Convert pixel from BMP to TFT format, push to display
        b = sdbuffer[buffidx++];
        g = sdbuffer[buffidx++];
        r = sdbuffer[buffidx++];
        awColors[row][col] = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
      }
    }

    // 範囲の端
    uint16_t xEnd = xStart+w-1;
    uint16_t yEnd = yStart+h-1;
    // 範囲外を淘汰する
    if (xEnd > 319) xEnd = 319;
    if (yEnd > 239) yEnd = 239;

    // 実際に描画する部分
    tft.beginSPITransaction(tft._clock);
    tft.setAddr(xStart, yStart, xEnd, yEnd); //書く範囲指定
    tft.writecommand_cont(ILI9341_RAMWR);
    for(int y=0; y<h; y++){
      for(int x=0; x<w; x++){
          if(x==319 && y==239){//最後はwritedata16_last
                tft.writedata16_last(awColors[y][x]); 
                tft.endSPITransaction(); //描画終了
            }
          else tft.writedata16_cont(awColors[y][x]); //ピクセル(x,y)描画
      }
    }

    Serial.print(F("Loaded in "));
    Serial.print(millis() - startTime);
    Serial.println(" ms");
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}