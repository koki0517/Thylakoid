Teensy4.1のEEPROMのサイズは4284bytes

| 変数名 |  Kp  |  Ki  |  Kd  | HueGreenMin | HueGreenMax | SqturationGreenMin | ValueGreenMin | HueRedMin | HueRedMax | SqturationRedMin | ValueRedMin | startingTilt |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| アドレス | 0 | 4 | 8 | 12 | 14 | 16 | 18 | 20 | 22 | 24 | 26 | 28 |
| 領域 | 0~3 | 4~7 | 8~11 | 12~13 | 14~15 | 16~17 | 18~19 | 20~21 | 22~23 | 24~25 | 26~27 | 28~31 |  
| 型 | flost |  float  |  float  | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t |  float  |
| 概要 |  ライントレースのKp  |  ライントレースのKi  |  ライントレースのKd  |  |  |  |  |  |  |  |  | 傾き始めたときの加速度 |