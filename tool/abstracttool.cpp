#include "abstracttool.h"

AbstractTool::AbstractTool(DocumentView *doc)
    :m_doc(doc)
{
}

AbstractTool::~AbstractTool(){

}

bool AbstractTool::isClassOf(const QString &className) const {
    QString myClassName;
    myClassName = this->metaObject()->className();
    return (myClassName == className);
}
