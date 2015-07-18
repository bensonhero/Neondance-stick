void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}
byte color[7]={0,100,100,100,100,100,100};
void loop() {
  if(Serial.available()){
    Serial.print("get");
    Serial.readBytes(color,7);
    Serial1.write(color,7);
  }
  while(Serial.available()){
    Serial.read();
  }
  delay(20);
}

