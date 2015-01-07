#ifndef SCENETREEMODEL_H
#define SCENETREEMODEL_H

#include <QAbstractItemModel>

class MapEntity;

class SceneModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    SceneModel(MapEntity *root, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

private:
    MapEntity *m_root;

    QIcon *m_buildingIcon;
    QIcon *m_floorIcon;
    QIcon *m_funcAreaIcon;
    QIcon *m_pubPointIcon;
};

#endif // SCENETREEMODEL_H
