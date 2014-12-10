#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>

class AbstractTool : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTool(QObject *parent = 0);

signals:

public slots:

};

#endif // ABSTRACTTOOL_H
