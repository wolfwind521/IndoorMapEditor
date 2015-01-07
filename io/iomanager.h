#ifndef IOMANAGER_H
#define IOMANAGER_H

#include<QString>

class DocumentView;

class IOManager
{
public:
    IOManager();
    static bool loadFile(const QString & fileName, DocumentView * doc);
    static bool saveFile(const QString & fileName, const DocumentView * doc);
};

#endif // IOMANAGER_H
