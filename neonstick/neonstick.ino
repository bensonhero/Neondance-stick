#include <Adafruit_NeoPixel.h>
#include <math.h>

#define  PIN  6
#define  NUMPIXELS  80
#define  NUMSTRIP  20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


float t = 0;
int type=49;
byte rgb[7]={0,100,100,100,100,100,100};
int r1=50;int g1=50;int b1=50;
int r2=50;int g2=50;int b2=50;
uint32_t rainbow[7]={pixels.Color(255,0,0),pixels.Color(255,150,0),pixels.Color(255,255,0),pixels.Color(0,255,0),pixels.Color(0,0,255),pixels.Color(100,0,255),pixels.Color(205,0,255)};

void setup() {
  Serial.begin(9600);
  pixels.begin();
}

void loop() {
  t += 1;
  if (t >= 256) t = 0;
  if (Serial.available()) {
    Serial.readBytes(rgb,7);
    while(Serial.available()){
    Serial.read();}
  }
  switch(rgb[0]){
    case 48://dark
      SolidColor(0,0,0,255);
      break;
    case 49://single solid color
      SolidColor(rgb[1],rgb[2],rgb[3],255);
      break;
    case 50://blinking color
      SolidColor(rgb[1],rgb[2],rgb[3],255*sq(sin(20*t*2*PI/255)));
      break;
    case 51://dual solid colors
      SolidColor2(rgb[1],rgb[2],rgb[3],255,rgb[4],rgb[5],rgb[6],255);
      break;
    case 52://dual blinking colors
      SolidColor2(rgb[1],rgb[2],rgb[3],255*sq(sin(20*t*2*PI/255)),rgb[4],rgb[5],rgb[6],255*sq(sin(20*t*2*PI/255)));
      break;
    case 53://spiral colors with phase 0
      Spiral2(pixels.Color(rgb[1],rgb[2],rgb[3]),20*t*2*PI/255,pixels.Color(rgb[4],rgb[5],rgb[6]),20*t*2*PI/255);
      break;
    case 54://spiral colors with phase pi
      Spiral2(pixels.Color(rgb[1],rgb[2],rgb[3]),20*t*2*PI/255+PI,pixels.Color(rgb[4],rgb[5],rgb[6]),20*t*2*PI/255);
      break;
    case 55:
      Pure_random(rgb[1],rgb[2],rgb[3]);
      break;
    case 56:
      Rainbow();
      break;
    case 57:
      DoubleRainbow();
      break;
  }
}


void SolidColor(int r, int g, int b, int brt) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r * brt / 255, g * brt / 255, b * brt / 255));
  }
  pixels.show();
  delay(25);
}

void SolidColor2(int r1, int g1, int b1, int brt1, int r2, int g2, int b2, int brt2) {
  for (int i = 0; i < NUMPIXELS / 4; i++) {
    pixels.setPixelColor(i, pixels.Color(r1 * brt1 / 255, g1 * brt1 / 255, b1 * brt1 / 255));
    pixels.setPixelColor(i + 20, pixels.Color(r1 * brt1 / 255, g1 * brt1 / 255, b1 * brt1 / 255));
    pixels.setPixelColor(i + 40, pixels.Color(r2 * brt2 / 255, g2 * brt2 / 255, b2 * brt2 / 255));
    pixels.setPixelColor(i + 60, pixels.Color(r2 * brt2 / 255, g2 * brt2 / 255, b2 * brt2 / 255));
  }
  pixels.show();
  delay(25);
}
/*the following not used*/
/*
void SolidColor4(int r1, int g1, int b1, int brt1, int r2, int g2, int b2, int brt2,int r3, int g3, int b3, int brt3, int r4, int g4, int b4, int brt4) {
  for (int i = 0; i < NUMPIXELS / 4; i++) {
    pixels.setPixelColor(i, pixels.Color(r1 * brt1 / 255, g1 * brt1 / 255, b1 * brt1 / 255));
    pixels.setPixelColor(i + 20, pixels.Color(r2 * brt2 / 255, g2 * brt2 / 255, b2 * brt2 / 255));
    pixels.setPixelColor(i + 40, pixels.Color(r3 * brt3 / 255, g3 * brt3 / 255, b3 * brt3 / 255));
    pixels.setPixelColor(i + 60, pixels.Color(r4 * brt4 / 255, g4 * brt4 / 255, b4 * brt4 / 255));
  }
  pixels.show();
  delay(25);
}
*/
void Spiral2(uint32_t color1, float rad1,uint32_t color2, float rad2) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  for (int j = -2; j < 3; j++) {
    pixels.setPixelColor(j + (int)(10 + 8 * 0.99 * sin(rad1)), color1/(uint32_t)2);
    pixels.setPixelColor(20 + j + (int)(10 + 8 * 0.99 * sin(rad2)), color2);
    pixels.setPixelColor(40 + j + (int)(10 + 8 * 0.99 * sin(rad1)), color1);
    pixels.setPixelColor(60 + j + (int)(10 + 8 * 0.99 * sin(rad2)), color2);
  }
  pixels.show();
  delay(25);
}

void Rainbow(){
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  for(int j=0;j<2;j++){
    for (int i = 0;i<7;i++){
      pixels.setPixelColor(3*i+40*j, rainbow[i]);
      pixels.setPixelColor(3*i+1+40*j, rainbow[i]);
      pixels.setPixelColor(3*i+2+40*j, rainbow[i]);
      pixels.setPixelColor(39-3*i+40*j, rainbow[i]);
      pixels.setPixelColor(-3*i+38+40*j, rainbow[i]);
      pixels.setPixelColor(-3*i+37+40*j, rainbow[i]);
    }
  }
  pixels.show();
  delay(25);
}

void DoubleRainbow(){
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  for(int j=0;j<4;j++){
    for (int i = 0;i<7;i++){
      pixels.setPixelColor(i+20*j, rainbow[i]);
      pixels.setPixelColor(19-i+20*j, rainbow[i]);
    }
  }
  pixels.show();
  delay(25);
}
void Pure_random(int r,int g,int b){
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r*random(0,255)/255, g*random(0,255)/255, b*random(0,255)/255));
  }
  pixels.show();
  delay(25);
}
