Teensy4.1のEEPROMのサイズは4284bytes  
アドレスの変数名の先頭には`EEP_`が付く（例: EEP_Kp）

| 変数名 |  Kp  |  Ki  |  Kd  | HueGreenMin | HueGreenMax | SqturationGreenMin | ValueGreenMin | HueRedMin | HueRedMax | SqturationRedMin | ValueRedMin | startingTilt | FloorProtrusion | LoadCell_isPressed |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| アドレス | 0 | 4 | 8 | 12 | 14 | 16 | 18 | 20 | 22 | 24 | 26 | 28 | 32 | 34 |
| 領域 | 0~3 | 4~7 | 8~11 | 12~13 | 14~15 | 16~17 | 18~19 | 20~21 | 22~23 | 24~25 | 26~27 | 28~31 | 32~33 | 34~35 |
| 型 | flost |  float  |  float  | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t | uint16_t |  float  | uint16_t | int16_t |
| 概要 |  ライントレースのKp  |  ライントレースのKi  |  ライントレースのKd  |  |  |  |  |  |  |  |  | 傾き始めたときの加速度 | バンプないし坂を検知したとする床との距離 | 左右のロードセルにかかる力の合力で押されたと判断する基準 |