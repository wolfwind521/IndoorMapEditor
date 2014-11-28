#ifndef JSONLOADER_H
#define JSONLOADER_H

#include<QString>

class QGraphicsObject;

class JsonLoader
{
public:
    JsonLoader();
    static QGraphicsObject* loadFile(const QString & fileName);
};

#endif // JSONLOADER_H
