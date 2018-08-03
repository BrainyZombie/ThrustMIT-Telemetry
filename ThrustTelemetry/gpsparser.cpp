
#include "gpsparser.h"

GPSParser::GPSParser(GPSDataStorage *dataStore, QString location)
{
    this->dataStore = dataStore;
    fileLocation = location.toUtf8()+"/GPS.kml";
    firstPoint = true;
}

void GPSParser::gpsParse()
{
    while(dataStore->read(&data));
    for (QString i: data){
        i=i.simplified();
        QByteArray ba = i.toLatin1();
        char *nmea = ba.data();
        if(parseNMEA(nmea)){
            qDebug()<<nmea;
            editKML();
        }
    }
}

bool GPSParser::parseNMEA(char* nmea)
{
      // do checksum check

      // first look if we even have one
      if (nmea[strlen(nmea)-4] == '*') {
        uint16_t sum = parseHex(nmea[strlen(nmea)-3]) * 16;
        sum += parseHex(nmea[strlen(nmea)-2]);

        // check checksum
        for (uint8_t i=2; i < (strlen(nmea)-4); i++) {
          sum ^= nmea[i];
        }
        if (sum != 0) {
          // bad checksum :(
          return false;
        }
      }
      int32_t degree;
      long minutes;
      char degreebuff[10];
      // look for a few common sentences
      if (strstr(nmea, "$GPGGA")) {
          return false;
        // found GGA
        char *p = nmea;
        // get time
        p = strchr(p, ',')+1;
        float timef = atof(p);
        uint32_t time = timef;
        hour = time / 10000;
        minute = (time % 10000) / 100;
        seconds = (time % 100);

        milliseconds = fmod(timef, 1.0) * 1000;

        // parse out latitude
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          strncpy(degreebuff, p, 2);
          p += 2;
          degreebuff[2] = '\0';
          degree = atol(degreebuff) * 10000000;
          strncpy(degreebuff, p, 2); // minutes
          p += 3; // skip decimal point
          strncpy(degreebuff + 2, p, 6);
          degreebuff[8] = '\0';
          minutes = 0.5 * atol(degreebuff) / 3;
          latitude_fixed = degree + minutes;
          latitude = degree / 100000 + minutes * 0.000006F;
          latitudeDegrees = (latitude-100*int(latitude/100))/60.0;
          latitudeDegrees += int(latitude/100);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          if (p[0] == 'S') latitudeDegrees *= -1.0;
          if (p[0] == 'N') lat = 'N';
          else if (p[0] == 'S') lat = 'S';
          else if (p[0] == ',') lat = 0;
          else
              return false;
        }

        // parse out longitude
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          strncpy(degreebuff, p, 3);
          p += 3;
          degreebuff[3] = '\0';
          degree = atol(degreebuff) * 10000000;
          strncpy(degreebuff, p, 2); // minutes
          p += 3; // skip decimal point
          strncpy(degreebuff + 2, p, 6);
          degreebuff[8] = '\0';
          minutes = 0.5 * atol(degreebuff) / 3;
          longitude_fixed = degree + minutes;
          longitude = degree / 100000 + minutes * 0.000006F;
          longitudeDegrees = (longitude-100*int(longitude/100))/60.0;
          longitudeDegrees += int(longitude/100);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          if (p[0] == 'W') longitudeDegrees *= -1.0;
          if (p[0] == 'W') lon = 'W';
          else if (p[0] == 'E') lon = 'E';
          else if (p[0] == ',') lon = 0;
          else
              return false;
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          fixquality = atoi(p);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          satellites = atoi(p);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          HDOP = atof(p);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          altitude = atof(p);
        }

        p = strchr(p, ',')+1;
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          geoidheight = atof(p);
        }
        return true;
      }
      if (strstr(nmea, "$GPRMC")) {
       // found RMC
        char *p = nmea;

        // get time
        p = strchr(p, ',')+1;
        float timef = atof(p);
        uint32_t time = timef;
        hour = time / 10000;
        minute = (time % 10000) / 100;
        seconds = (time % 100);

        milliseconds = fmod(timef, 1.0) * 1000;

        p = strchr(p, ',')+1;
        // Serial.println(p);
        if (p[0] == 'A')
          fix = true;
        else if (p[0] == 'V')
          fix = false;
        else
          return false;

        // parse out latitude
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          strncpy(degreebuff, p, 2);
          p += 2;
          degreebuff[2] = '\0';
          long degree = atol(degreebuff) * 10000000;
          strncpy(degreebuff, p, 2); // minutes
          p += 3; // skip decimal point
          strncpy(degreebuff + 2, p, 6);
          degreebuff[8] = '\0';
          long minutes = 0.5 * atol(degreebuff) / 3;
          latitude_fixed = degree + minutes;
          latitude = degree / 100000 + minutes * 0.000006F;
          latitudeDegrees = (latitude-100*int(latitude/100))/60.0;
          latitudeDegrees += int(latitude/100);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          if (p[0] == 'S') latitudeDegrees *= -1.0;
          if (p[0] == 'N') lat = 'N';
          else if (p[0] == 'S') lat = 'S';
          else if (p[0] == ',') lat = 0;
          else return false;
        }

        // parse out longitude
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          strncpy(degreebuff, p, 3);
          p += 3;
          degreebuff[3] = '\0';
          degree = atol(degreebuff) * 10000000;
          strncpy(degreebuff, p, 2); // minutes
          p += 3; // skip decimal point
          strncpy(degreebuff + 2, p, 6);
          degreebuff[8] = '\0';
          minutes = 0.5 * atol(degreebuff) / 3;
          longitude_fixed = degree + minutes;
          longitude = degree / 100000 + minutes * 0.000006F;
          longitudeDegrees = (longitude-100*int(longitude/100))/60.0;
          longitudeDegrees += int(longitude/100);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          if (p[0] == 'W') longitudeDegrees *= -1.0;
          if (p[0] == 'W') lon = 'W';
          else if (p[0] == 'E') lon = 'E';
          else if (p[0] == ',') lon = 0;
          else return false;
        }
        // speed
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          speed = atof(p);
        }

        // angle
        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          angle = atof(p);
        }

        p = strchr(p, ',')+1;
        if (',' != *p)
        {
          uint32_t fulldate = atof(p);
          day = fulldate / 10000;
          month = (fulldate % 10000) / 100;
          year = (fulldate % 100);
        }
        // we dont parse the remaining, yet!
        return true;
      }

      return false;


}

uint8_t GPSParser::parseHex(char c) {
    if (c < '0')
      return 0;
    if (c <= '9')
      return c - '0';
    if (c < 'A')
       return 0;
    if (c <= 'F')
       return (c - 'A')+10;
    // if (c > 'F')
    return 0;
}

void GPSParser::editKML()
{
    if (altitude==altitude && fix){
        if (firstPoint){
            initKml();
            launchMap();
            firstPoint = false;
        }
        else
            updateKml();
    }

}

void GPSParser::initKml(){
    kml.setFileName(fileLocation);
    kmlPre = "<?xml version='1.0' encoding='UTF-8'?><kml xmlns='http://earth.google.com/kml/2.2'><Document><Placemark><name>flight</name><LineString><extrude>1</extrude><altitudeMode>relativeToGround</altitudeMode><coordinates>";
    kmlPost = "</coordinates></LineString></Placemark></Document></kml>";
    updateKml();
}
void GPSParser::updateKml(){
    qDebug()<<longitude_fixed;
    qDebug()<<QString::number((int)longitude_fixed/10000000, 'g', 10) + "." + QString::number((int)longitude_fixed%10000000, 'g', 10);
    kmlMain.append( QString::number((int)longitude_fixed/10000000, 'g', 10) + "." + QString::number((int)longitude_fixed%10000000, 'g', 10) + "," + QString::number((int)latitude_fixed/10000000, 'g', 10) + "." + QString::number((int)latitude_fixed%10000000, 'g', 10) + "," + QString::number(altitude, 'g', 10) + "\r\n");
    kml.open(QIODevice::WriteOnly);
    kml.write(kmlPre.toUtf8());
    kml.write(kmlMain.toUtf8());
    kml.write(kmlPost.toUtf8());
    kml.close();
}


void GPSParser::launchMap()
{

    QFile a("live_flight_tracking.kml");
    QString temp;
    a.open(QIODevice::WriteOnly);
    a.write("<?xml version=\"1.0\" encoding=\"utf-8\" ?><kml xmlns=\"http://earth.google.com/kml/2.1\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\"><Document><gx:Tour><name>Play me!</name><gx:Playlist><gx:FlyTo><gx:duration>0.1</gx:duration><LookAt><longitude>");
    temp = QString("%1").arg(longitude_double/10000000);
    a.write(temp.toUtf8());
    a.write("</longitude><latitude>");
    temp = QString("%1").arg(latitude_double/10000000);
    a.write(temp.toUtf8());
    a.write("</latitude><altitude>");
    temp = QString::number(altitude);
    a.write(temp.toUtf8());
    a.write("</altitude><heading>112.87</heading><tilt>68.065</tilt><range>6811.884</range><altitudeMode>absolute</altitudeMode></LookAt></gx:FlyTo></gx:Playlist>\r\n</gx:Tour>");
    a.write("<NetworkLink id=\"thrustMIT\">\r\n<name>thrustMIT</name>\r\n<open>1</open>\r\n<Link>\r\n<href>");

    a.write(fileLocation.toUtf8());
    a.write("</href>\r\n<refreshMode>onInterval</refreshMode>\r\n<refreshInterval>5</refreshInterval>\r\n</Link>\r\n<flyToView>0</flyToView>\r\n</NetworkLink>\r\n</Document>\r\n</kml>");


    a.flush();
    a.close();

    a.setFileName("GEarth.bat");
    a.open(QIODevice::WriteOnly);
    a.write("Start /B live_flight_tracking.kml");
    a.flush();
    a.close();

    system("GEarth.bat");
}

