#include "jsonloader.h"
#include "../core/building.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


JsonLoader::JsonLoader()
{
}

QGraphicsObject* JsonLoader::loadFile(const QString & fileName)
{
       QFile file(fileName);
       if (!file.open(QIODevice::ReadOnly)) {
           return NULL;
       }

       QByteArray jsonData = file.readAll();
       QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));

       Building * building = new Building("");
       if(building->load(loadDoc.object())){
           return building;
       }else{
           delete building;
           building = NULL;
       }
       return building;
}
