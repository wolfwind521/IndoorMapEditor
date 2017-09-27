#ifndef JSONLOADER_H
#define JSONLOADER_H

#include<QString>
#include<QJsonObject>

class Feature;
class Building;
class Floor;
class Room;
class PubPoint;

class JsonLoader
{
public:
    JsonLoader();
    static Feature* loadFile(const QString & fileName);
    static bool saveFile(const QString & fileName, const Feature * mapFeature);

private:
    //TODO:
    Building * loadBuilding(const QJsonObject & jsonObject){};
    Floor * loadFloor(const QJsonObject & jsonObject){};
    Room * loadRoom(const QJsonObject & jsonObject){};
    PubPoint * loadPubPoint(const QJsonObject & jsonObject){};

};

#endif // JSONLOADER_H
