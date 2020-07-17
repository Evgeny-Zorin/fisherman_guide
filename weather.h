#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>

class weather : public QObject
{
    Q_OBJECT
public:
    explicit weather(QObject *parent = nullptr);

private:
//coord
    double coordLon;   //City geo location, longitude
    double coordLat;   //City geo location, latitude
//weather
    qint32 weatherid;  //Weather condition id
    QString weatherMain;   //roup of weather parameters (Rain, Snow, Extreme etc.)
    QString weatherDescription; //Weather condition within the group. You can get the output in your language. Learn more
    QString weatherIcon;    //Weather icon id
    //base Internal parameter
//main
    double mainTtemp;       //Temperature. Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit.
    double mainFeels_like;  //Temperature. This temperature parameter accounts for the human perception of weather. Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit.
    double mainPressure;    //Atmospheric pressure (on the sea level, if there is no sea_level or grnd_level data), hPa
    double mainHumidity;    //Humidity, %
    double mainTemp_min;    //Minimum temperature at the moment. This is minimal currently observed temperature (within large megalopolises and urban areas). Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit.
    double mainTemp_max;    //Maximum temperature at the moment. This is maximal currently observed temperature (within large megalopolises and urban areas). Unit Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit.
    double mainSea_level;   //Atmospheric pressure on the sea level, hPa
    double mainGrnd_level;  //Atmospheric pressure on the ground level, hPa
//wind
    double windSpeed;   //Wind speed. Unit Default: meter/sec, Metric: meter/sec, Imperial: miles/hour.
    double windDeg;    //Wind direction, degrees (meteorological)
    double windGust;   //Wind gust. Unit Default: meter/sec, Metric: meter/sec, Imperial: miles/hour
//clouds
    double cloudsAll;   //Cloudiness, %
//rain
    double rain1h;  //Rain volume for the last 1 hour, mm
    double rain3h;  //Rain volume for the last 3 hours, mm
//snow
    double snow1h;  //Snow volume for the last 1 hour, mm
    double snow3h;  //Snow volume for the last 3 hours, mm

    double dt;  //Time of data calculation, unix, UTC
//sys
    quint32 sysType; //Internal parameter
    quint32 sysId;  //Internal parameter
    double sysMessage;  //Internal parameter
    QString sysCountry; //Country code (GB, JP etc.)
    quint32 sysSunrise; //Sunrise time, unix, UTC
    quint32 sysSunset;  //Sunset time, unix, UTC
    qint32 timezone;    //Shift in seconds from UTC
    quint32 id; //City ID
    QString name;   //City name
    quint32 cod;    //Internal parameter

public:
        QString getname()const{return name;}

};

#endif // WEATHER_H
