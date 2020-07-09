#ifndef CLASSFISH_H
#define CLASSFISH_H
#include <QString>


class classfish
{
public:
    classfish();

private:
    QString namefish;       //name of the fish
    qint32 minactivtemp;    //minimum (water) temperature at which the fish is active
    qint32 maxactivtemp;    //maximum (water) temperature at which the fish is active
    qint32 minactivpress;   //minimum pressure at which the fish is active
    qint32 maxactivpress;   //maximum pressure at which the fish is active
    QString areafish;       //fish habitat(river, lake, sea, ocean)
    QString typefish;       //freshwater or marine fish
    QString aboutfish;      //description of the fish
public:
    QString getNameFish() const{return namefish;}
    QString getTypeFish() const{return typefish;}
    qint32  getMinActivTemp() const{return minactivtemp;}
    qint32  getMaxActivTemp() const{return maxactivtemp;}
    qint32  getMinActivPress() const{return minactivpress;}
    qint32  getMaxActivPress() const{return maxactivpress;}
    QString aboutFish() const{return aboutfish;}

};

#endif // CLASSFISH_H
