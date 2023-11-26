#include "Display.h"

Display::Display(){}

bool Display::init(){
  // EEPROMの読み出し
  // updateEEPROM();

  // ILI9341の初期化
  tft.begin();
  tft.setClock(clockDisplay);
  tft.fillScreen(ILI9341_BLUE);
  tft.setRotation( displayDirection );
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Waiting for Arduino Serial Monitor...");
  while (!Serial) {
    if (millis() > 1000) break;
  }

  // SDカードの初期化
  tft.println("Init SD card...");
  while (!sd.begin(SD_CONFIG)) {
    Serial.println("failed to access SD card!");
    tft.println("failed to access SD card!");
    delay(1000);
  }
  return true;
}

void Display::clear(uint16_t color){
  /* キョムリな画面を創るんだ */
  tft.fillScreen(color);
}

uint8_t Display::drawPhoto(char *filename, int xStart, int yStart){
  /* 写真を表示しようそうしよう */
  int bmpWidth, bmpHeight;   // W+H in pixels

  String filePath = String(filename) + ".txt";
  const char* _filePath = filePath.c_str();
  FsFile bmpFile = sd.open(_filePath, FILE_READ);
  if (!bmpFile) {
    Serial.print("File not found");
    return FILE_NOT_FOUND;
  }

  bmpHeight = read32(bmpFile);
  bmpWidth  = read32(bmpFile);
  if (bmpHeight > 240 || bmpWidth > 320) return FILE_SIZE_ERROR;

  uint32_t startTime = millis();

  // 実際に描画する部分
  tft.beginSPITransaction(tft._clock);
  tft.setAddr(xStart, yStart, 319, 239); //書く範囲指定
  tft.writecommand_cont(ILI9341_RAMWR);
  for(int y=0; y<240*320; y++){
    if(y==240*320-1){//最後はwritedata16_last
        tft.writedata16_last(read16(bmpFile)); 
        tft.endSPITransaction(); //描画終了
      }
    else tft.writedata16_cont(read16(bmpFile)); //ピクセル(x,y)描画
  }

  // while(millis()-startTime <= 49); //fps固定
  // かかった時間の表示
  Serial.print("Loaded in ");
  Serial.print(millis() - startTime);
  Serial.println(" ms");

  bmpFile.close();
  return FILE_OK;
}

uint8_t Display::playMovie(char *filename){
  /* 動画を流そうぞ 
   * ファイルサイズは240*320しか受け入れません。断じて、
  */
  int bmpWidth, bmpHeight;   // W+H in pixels

  String filePath = String(filename) + ".txt";
  const char* _filePath = filePath.c_str();
  FsFile bmpFile = sd.open(_filePath, FILE_READ);
  if (!bmpFile) {
    Serial.print("File not found");
    return FILE_NOT_FOUND;
  }

  bmpHeight = read32(bmpFile);
  bmpWidth  = read32(bmpFile);
  if (bmpHeight != 240 || bmpWidth != 320) return FILE_SIZE_ERROR;

  for(int i = 0; i <= 6043; i++) {
    uint32_t startTime = millis();

    // 実際に描画する部分
    tft.beginSPITransaction(tft._clock);
    tft.setAddr(0, 0, 319, 239); //書く範囲指定
    tft.writecommand_cont(ILI9341_RAMWR);
    for(int y=0; y<240*320; y++){
      if(y==240*320-1){//最後はwritedata16_last
          tft.writedata16_last(read16(bmpFile)); 
          tft.endSPITransaction(); //描画終了
        }
      else tft.writedata16_cont(read16(bmpFile)); //ピクセル(x,y)描画
    }

    // while(millis()-startTime <= 49); //fps固定
    // かかった時間の表示
    Serial.print("Loaded in ");
    Serial.print(millis() - startTime);
    Serial.println(" ms");
  }
  bmpFile.close();
  return FILE_OK;
}

void Display::updateEEPROM(){
  // EEPROMの読み出し 4284バイトもあるんだってすごいね
  EEPROM.get(EEP_diaplayDirection, displayDirection);
  EEPROM.get(EEP_clockDisplay, clockDisplay);
}

uint8_t Display::convertPhotoBMPtoRGB565(char *readFileName, char *writeFileName, bool ifDisplay, bool writeSize){
  /* BMPの写真をRGB565に変換しよう */
  FsFile     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  // uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint16_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  // Open requested file on SD card
  String fname_withBMP = String(readFileName) + ".bmp";
  const char* fname = fname_withBMP.c_str();
  bmpFile = sd.open(fname, FILE_READ);
  if (!bmpFile) {
    Serial.print("File not found");
    Serial.println(fname);
    return FILE_NOT_FOUND;
  }

  // Parse BMP header
  (void)read16(bmpFile); // 0x4D42  BMP signature
  
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

  // BMP rows are padded (if needed) to 4-byte boundary
  rowSize = (bmpWidth * 3 + 3) & ~3;

  if(bmpHeight < 0) {
    bmpHeight = -bmpHeight;
    flip      = false;
  }

  // Crop area to be loaded
  w = bmpWidth; // 320
  h = bmpHeight; // 240
  if (w > 320) return FILE_SIZE_ERROR;
  if (h > 240) return FILE_SIZE_ERROR;

  // 画像サイズ分のメモリを確保
  uint16_t **awColors = new uint16_t *[h];
  for (int i = 0; i < h; ++i) {
    awColors[i] = new uint16_t[w];
  }

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

  // 実際に描画する部分
  tft.beginSPITransaction(tft._clock);
  tft.setAddr(0, 0, w-1, h-1); //書く範囲指定
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

  // ファイルに書き込む
  if (ifDisplay){
    String fname_withTXT = String(writeFileName) + ".txt";
    const char* fname_txt = fname_withTXT.c_str();
    FsFile dataFile = sd.open(fname_txt, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.write((const uint8_t *)&bmpHeight, sizeof(bmpHeight));
      dataFile.write((const uint8_t *)&bmpWidth, sizeof(bmpWidth));
      for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
          dataFile.write((const uint8_t *)&awColors[i][j], sizeof(awColors[i][j]));
        }
      }
      dataFile.close();
    } else return FILE_NOT_FOUND;
  }

  // メモリ解放
	for( int i=0; i<h; i++ ){
		delete[] awColors[i];
		awColors[i] = 0;
	}
	delete[] awColors;
	awColors = 0;

  // かかった時間の表示
  Serial.print("Loaded in ");
  Serial.print(millis() - startTime);
  Serial.println(" ms");

  bmpFile.close();
  return FILE_OK;
}

uint8_t Display::convertMovieBMPtoRGB565(char *writeFileName /* 書き込み先のファイル名 元のBMPは0.bmp~n.bmpになっているものとする*/, int numPhotos /* 写真の枚数 */, bool ifdisplay ){
  /* BMPから構成される動画をRGB565に変換しよう 
   * もっと効率的な方法はもちろん頭にはあるけど、面倒だからつくらない
  */
  char fnum[4];
  itoa(0, fnum, 10);
  uint8_t result = convertPhotoBMPtoRGB565(fnum, writeFileName, ifdisplay, true);
  if (result != FILE_OK) return result;
  for (int i=1; i<numPhotos; i++){
    char fnum[4];
    itoa(i, fnum, 10);
    result = convertPhotoBMPtoRGB565(fnum, writeFileName, ifdisplay, false);
    if (result != FILE_OK) return result;
  }
  return FILE_OK;
}

uint16_t Display::read16(FsFile &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t Display::read32(FsFile &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

/* ==================== Private ==================== */