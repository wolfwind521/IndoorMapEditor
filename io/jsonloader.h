#ifndef JSONLOADER_H
#define JSONLOADER_H

#include<QString>

class MapEntity;

class JsonLoader
{
public:
    JsonLoader();
    static MapEntity* loadFile(const QString & fileName);
    static bool saveFile(const QString & fileName, const MapEntity * mapEntity);
};

#endif // JSONLOADER_H
