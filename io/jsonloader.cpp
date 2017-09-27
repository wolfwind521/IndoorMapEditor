#include "jsonloader.h"
#include "../core/feature.h"
#include "../core/building.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


JsonLoader::JsonLoader(){
}

Feature* JsonLoader::loadFile(const QString & fileName) {
       QFile file(fileName);
       if (!file.open(QIODevice::ReadOnly)) {
           qWarning("Couldn't open the file.");
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

bool JsonLoader::saveFile(const QString &fileName, const Feature *mapFeature) {

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open the file.");
        return false;
    }

    QJsonObject jsonObject;
    if(mapFeature->save(jsonObject)){
        QJsonDocument saveDoc(jsonObject);
        file.write(saveDoc.toJson());
        return true;
    }else{
        return false;
    }
}
