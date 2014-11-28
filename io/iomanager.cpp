#include "iomanager.h"
#include "gui/documentview.h"
#include "core/building.h"
#include "jsonloader.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>


IOManager::IOManager()
{
}

bool IOManager::loadFile(const QString & fileName, DocumentView * doc)
{
    QString suffix = QFileInfo(fileName).suffix();
    if(!suffix.compare("json", Qt::CaseInsensitive))
    {
        Building * building = dynamic_cast<Building*>(JsonLoader::loadFile(fileName));
        if(building != NULL){ //loaded successfully
            doc->setBuilding(building);
            return true;
        }else{
            return false;
        }
    }else if( (!suffix.compare("jpg", Qt::CaseInsensitive)) ||
             (!suffix.compare("png", Qt::CaseInsensitive)) ||
             (!suffix.compare("bmp", Qt::CaseInsensitive)) ||
             (!suffix.compare("gif", Qt::CaseInsensitive)) )
    {
        QImage image(fileName);
        if (image.isNull()) {
            return false;
        }else{
            return true;
        }

    }else{
        return false;
    }
}
