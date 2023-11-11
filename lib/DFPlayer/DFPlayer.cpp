#include "DFPlayer.h"

void printDetail(uint8_t type, int value);

bool Voice::init(){
  mySoftwareSerial.begin(9600);
  // DFPlayerを初期化します。USBピンを使ってなければ、デバイスはSD(TF)カードが選択されます。
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    // 2秒以内に初期化できなかった場合はエラーメッセージを表示
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }

  myDFPlayer.volume(20);  // ボリュームをセット。(ボリュームは0から30の値で指定可能)
}

void Voice::play(uint8_t num){
  myDFPlayer.play(num);  // 先頭のmp3ファイルを再生
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void Voice::volume(uint8_t volume){
  myDFPlayer.volume(volume);  // ボリュームをセット。(ボリュームは0から30の値で指定可能)
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println("Time Out!");
      break;
    case WrongStack:
      Serial.println("Stack Wrong!");
      break;
    case DFPlayerCardInserted:
      Serial.println("Card Inserted!");
      break;
    case DFPlayerCardRemoved:
      Serial.println("Card Removed!");
      break;
    case DFPlayerCardOnline:
      Serial.println("Card Online!");
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print("Number:");
      Serial.print(value);
      Serial.println(" Play Finished!");
      break;
    case DFPlayerError:
      Serial.print("DFPlayerError:");
      switch (value) {
        case Busy:
          Serial.println("Card not found");
          break;
        case Sleeping:
          Serial.println("Sleeping");
          break;
        case SerialWrongStack:
          Serial.println("Get Wrong Stack");
          break;
        case CheckSumNotMatch:
          Serial.println("Check Sum Not Match");
          break;
        case FileIndexOut:
          Serial.println("File Index Out of Bound");
          break;
        case FileMismatch:
          Serial.println("Cannot Find File");
          break;
        case Advertise:
          Serial.println("In Advertise");
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}