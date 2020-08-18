static void doSomeWork( const gps_fix & fix );
static void doSomeWork( const gps_fix & fix )
{
  if (fix.valid.location) {

    if (fix.dateTime.seconds < 10 )
  latitude = fix.latitude();
  longitude = fix.longitude();
  }

  DEBUG_PORT.println();

}


static void GPSloop()
{
  while (gps.available( gpsPort ))
    doSomeWork( gps.read() );

}
