#include "iomanager.h"
#include "../gui/documentview.h"
#include "../core/building.h"
#include "../core/floor.h"
#include "../core/scene.h"
#include "../core/imagelayer.h"
#include "jsonloader.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>


IOManager::IOManager() {
}

bool IOManager::loadFile(const QString & fileName, DocumentView * doc) {
    QString suffix = QFileInfo(fileName).suffix();
    if(!suffix.compare("json", Qt::CaseInsensitive))
    {
        Building * building = dynamic_cast<Building*>(JsonLoader::loadFile(fileName));
        if(building != NULL){ //loaded successfully
            doc->scene()->setBuilding(building);
            return true;
        }else{
            return false;
        }
    }else if( (!suffix.compare("jpg", Qt::CaseInsensitive)) ||
             (!suffix.compare("png", Qt::CaseInsensitive)) ||
             (!suffix.compare("bmp", Qt::CaseInsensitive)) ||
             (!suffix.compare("gif", Qt::CaseInsensitive)) ||
             (!suffix.compare("jpeg", Qt::CaseInsensitive)) )
    {
        ImageLayer * imageLayer = new ImageLayer();
        if(imageLayer->setImage(fileName)){
            doc->scene()->addImageLayer(imageLayer);
            return true;
        }else{
            delete imageLayer;
            return false;
        }
    }else{
        return false;
    }
}

bool IOManager::saveFile(const QString &fileName, const DocumentView *doc) {
    QString suffix = QFileInfo(fileName).suffix();
    if(!suffix.compare("json", Qt::CaseInsensitive)) {
        return JsonLoader::saveFile(fileName, doc->scene()->building());
    }else{
        return false;
    }
}
