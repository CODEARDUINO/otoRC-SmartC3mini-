#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define AIN1 0
#define AIN2 1
#define BIN1 2
#define BIN2 3
#define IR_MID 6
#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// =========================================================
// BIEN TINH CHINH - BAN CHI CAN SUA O DAY
// =========================================================
int baseSpeed = 220;       // Toc do chay (0-255). 200 boc, 150 cham
int obstacleDistance = 6; // Khoang cach ne vat (cm). 12 xa, 8 vua, 5 sat
int leftTrim = 0;          // Can lech trai
int rightTrim = 5;         // Can lech phai +5 la phai manh hon
unsigned long straightUntil = 0;

// =========================================================
// HAM DONG CO
// =========================================================
void initMotors(){pinMode(AIN1,OUTPUT);pinMode(AIN2,OUTPUT);pinMode(BIN1,OUTPUT);pinMode(BIN2,OUTPUT);stopRobot();}
void stopRobot(){analogWrite(AIN1,0);analogWrite(AIN2,0);analogWrite(BIN1,0);analogWrite(BIN2,0);}

void forward(int s){
  int l = constrain(s + leftTrim, 0, 255);
  int r = constrain(s + rightTrim, 0, 255);
  analogWrite(AIN1,l); analogWrite(AIN2,0);
  analogWrite(BIN1,r); analogWrite(BIN2,0);
}
void backward(int s){analogWrite(AIN1,0);analogWrite(AIN2,s);analogWrite(BIN1,0);analogWrite(BIN2,s);}
void turnRight(int s){analogWrite(AIN1,s);analogWrite(AIN2,0);analogWrite(BIN1,0);analogWrite(BIN2,s);}
void turnLeft(int s){analogWrite(AIN1,0);analogWrite(AIN2,s);analogWrite(BIN1,s);analogWrite(BIN2,0);}
void curveLeft(int s){analogWrite(AIN1,s*0.65);analogWrite(AIN2,0);analogWrite(BIN1,s);analogWrite(BIN2,0);}
void curveRight(int s){analogWrite(AIN1,s);analogWrite(AIN2,0);analogWrite(BIN1,s*0.65);analogWrite(BIN2,0);}

// =========================================================
// LOC NHIEU CAM BIEN
// =========================================================
bool checkEdge(){
  int cnt=0;
  for(int i=0;i<3;i++){ if(digitalRead(IR_MID)==HIGH) cnt++; delay(5); }
  return cnt>=2; // 2/3 lan moi tinh la mep ban
}
bool checkObstacle(){
  int cnt=0;
  for(int i=0;i<3;i++){
    VL53L0X_RangingMeasurementData_t m;
    lox.rangingTest(&m,false);
    int d = (m.RangeStatus!=4) ? m.RangeMilliMeter/10 : 999;
    if(d < obstacleDistance && d>0) cnt++;
    delay(10);
  }
  return cnt>=2; // 2/3 lan moi tinh la vat can
}

void setup(){
  Serial.begin(115200);
  Wire.begin(SDA_PIN,SCL_PIN);
  if(!lox.begin()){Serial.println("VL53L0X fail!");while(1);}
  initMotors();
  pinMode(IR_MID,INPUT);
  randomSeed(analogRead(0));
  Serial.println("VOTRI GOC LON READY");
  straightUntil = millis() + 3000; // 3s dau chay thang
  delay(500);
}

void loop(){
  // 1. CHONG ROI - QUAY GOC LON 120-150 DO
  if(checkEdge()){
    Serial.println("EDGE - QUAY LON");
    stopRobot();delay(150);
    backward(200);delay(1300); // Lui xa hon cu (1200 -> 1300)
    if(random(0,2)==0) turnRight(240); else turnLeft(240); // Tang toc quay 220 -> 240
    delay(random(1200,1600)); // QUAN TRONG: Quay lau hon 1200-1600ms ~ 120-150 do
    straightUntil = millis() + random(2000,4000);
    return;
  }

  // 2. TRANH VAT - QUAY GOC LON 90-120 DO
  if(checkObstacle()){
    Serial.println("OBSTACLE - QUAY LON");
    stopRobot();delay(120);
    backward(200);delay(1100); // Lui xa hon cu (1000 -> 1100)
    if(random(0,2)==0) turnRight(240); else turnLeft(240); // Tang toc
    delay(random(1000,1400)); // Quay lau hon 1000-1400ms ~ 90-120 do
    straightUntil = millis() + random(2000,3500);
    return;
  }

  // 3. DI THANG MUOT
  if(millis() > straightUntil){
    straightUntil = millis() + random(5000,10000); // 5-10s moi doi huong
    int c = random(0,100);
    if(c < 70){
      forward(baseSpeed);
    } else if(c < 80){
      curveLeft(baseSpeed); delay(random(400,800));
    } else if(c < 90){
      curveRight(baseSpeed); delay(random(400,800));
    } else {
      stopRobot(); delay(random(300,600));
    }
  }

  forward(baseSpeed);
  delay(20);
}
