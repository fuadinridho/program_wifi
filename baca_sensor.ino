float cbfr;
float humbfr;
  
float read_temp()
{
  float temp_c;
  temp_c = sht1x.readTemperatureC();
  if(temp_c<0 || temp_c>80){temp_c = cbfr;}
  cbfr = temp_c;
  return temp_c;
}

float read_hum()
{
  float humidity;
  humidity = sht1x.readHumidity();
  if(humidity<0 || humidity>100){humidity = humbfr;}
  humbfr = humidity;
  return humidity;
}

float read_bat(unsigned char adcpin)
{
  float voltage;
  voltage = analogRead(adcpin);
  return voltage;
}

double read_baro()
{
  double realTemperature2 = ms5611.readTemperature(true);
  long realPressure2 = ms5611.readPressure(true);
  double realAltitude2 = ms5611.getAltitude(realPressure2);
  return realPressure2/101325.0;
}

float read_lux()
{
  float lux = lightMeter.readLightLevel(true);
  if (lux > 40000.0)lightMeter.setMTreg(32);
  if (lux > 10.0 && lux <=40000)lightMeter.setMTreg(69);
  if (lux <= 10.0 && lux > 0)lightMeter.setMTreg(138);
  return lux;
}
