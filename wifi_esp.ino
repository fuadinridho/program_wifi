void init_wifi()
{
  Serial1.println("AT+RST");
  //while(Serial1.available()>0){Serial.write(Serial1.read());}
  delay(7000);
  
  Serial1.println("AT+CIPMUX=0");
  while (Serial1.available()) {char inByte = Serial1.read();Serial.print(inByte);}
  delay(7000);
  cmdconnect = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  Serial1.println(cmdconnect);
  while (Serial1.available()) {char inByte = Serial1.read();Serial.print(inByte);}
  delay(15000);
  
  Serial1.println("AT+CWMODE=1");
  while (Serial1.available()) {char inByte = Serial1.read();Serial.print(inByte);}
  delay(7000);
}

bool httpPost(char data[]) {
  const __FlashStringHelper *command = F("AT+CIPSTART=\"%s\",\"%s\",80\r\n");
  char buffer[50];
  sprintf_P(buffer, (const char *)command, "TCP", SERVER_URL);

  Serial.print(buffer);
  Serial1.println(buffer);
  delay(100);
  // while (!ESP.available()) {};
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
    delay(10);
  }
//   if (Serial1.find((char *)F("OK"))) {
//     Serial.println("Connected");
//   } else return false;
   delay(500);

  command = F("AT+CIPSEND=%d\r\n");
  sprintf_P(buffer, (const char *)command, strlen(data));
  Serial.println(buffer);
  Serial1.print(buffer);
  delay(100);
  while (Serial1.available() > 0) {
    Serial.write(Serial1.read());
    delay(10);
  }
  // // delay(500);
  //
  // if(Serial1.find((char *)F(">"))) {
    Serial.println("Sending..");
    for (size_t i = 0; i < strlen(data); i++) {
      Serial1.write(data[i]);
      Serial.print(data[i]);
    }
  // };
  if( Serial1.find((char *)F("SEND OK"))) {
    Serial.println("Packet sent");
  }else return false;

  Serial.println("Hello");
  String response;
  int x = 0;
  while (x < 2) {
    if (Serial1.available()) {
      char c = Serial1.read();
      if (c == '}') x++;
      response += c;
    }
  }
  Serial.println(response);

  Serial1.println("AT+CIPCLOSE");
  delay(100);

  return true;
}

//void write_wifi()
//{
//  //POST /measurements HTTP/1.1
////Host: myapi.com
////Accept: application/json
////Content-Type: application/json
////Content-Length: 40
//
// StaticJsonDocument<200> jsonBuffer;
//  
//  // Build your own object tree in memory to store the data you want to send in the request
//
//  jsonBuffer["device_id"] = 1;
//  jsonBuffer["temperature"] = suhu;
//  jsonBuffer["humidity"] = kelembaban;
//  jsonBuffer["luminous_flux"] = lux;
//  jsonBuffer["air_presure"] = pressure;
//  jsonBuffer["longitude"] = longitude;
//  jsonBuffer["latitude"] = latitude;
// 
//  char json[200];
//  Serial1.println("AT+CIPSTART=\"TCP\",\"http://amicagama.id\",8000");
//  while (Serial1.available()) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(2000);
//  serializeJson(jsonBuffer, json);
//  
//  
//  int len = strlen(json);
//
//  header1 = "POST /api/devices/1/measurements HTTP/1.1";
//  header2 = "Host: http://amicagama.id";
//  header3 = "Content-Type: application/json";
//  header4 = "Content-Length:" + String(len);
//  postdata = json;    
//  String send1,send2,send3,send4,senddata;
//  send1 = "AT+CIPSEND=" + String(header1.length());
//  Serial1.println(send1);
//  Serial1.println(header1);
//  Serial.println(header1);
// while(Serial1.available()>0) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(2000);
//  send2 = "AT+CIPSEND=" + String(header2.length());
//  Serial1.println(send2);
//  Serial1.println(header2);
//  Serial.println(header2);
//   while(Serial1.available()>0) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(2000);
//  send3 = "AT+CIPSEND=" + String(header3.length());
//  Serial1.println(send3);
//  Serial1.println(header3);
//  Serial.println(header3);
//   while(Serial1.available()>0) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(2000);
//  send4 = "AT+CIPSEND=" + String(header4.length());
//  Serial1.println(send4);
//  Serial1.println(header4);
//  Serial.println(header4);
//   while(Serial1.available()>0) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(2000);
//  senddata = "AT+CIPSEND=" + String(postdata.length());
//  Serial1.println(senddata);
//  Serial1.println(postdata);
//  Serial.println(postdata); 
//  while (Serial1.available()>0) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(6000);
//  
//  
//  Serial1.println("AT+CIPCLOSE");
//  while (Serial1.available()>0) {char inByte = Serial1.read();Serial.print(inByte);}
//  delay(5000);
//
//
//}
