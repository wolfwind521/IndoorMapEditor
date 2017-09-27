#include "scenemodel.h"
#include "../core/feature.h"
#include "../core/room.h"
#include <QIcon>
SceneModel::SceneModel(Feature *root, QObject *parent) :
    QAbstractItemModel(parent), m_root(root)
{
    m_buildingIcon = new QIcon(":/src/icon/building.png");
    m_floorIcon = new QIcon(":/src/icon/floor.png");
    m_funcAreaIcon = new QIcon(":/src/icon/shop.png");
    m_pubPointIcon = new QIcon(":/src/icon/pubpoint.png");
}

Qt::ItemFlags SceneModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::ItemIsEnabled;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QModelIndex SceneModel::index(int row, int column, const QModelIndex &parent) const
{
    if(m_root == NULL)
        return QModelIndex();

    Feature *parentObject;
    if(!parent.isValid()){
        parentObject = m_root;
    }else{
        parentObject = static_cast<Feature *>(parent.internalPointer());
    }

    if(row >= 0 && row < parentObject->children().count()){
        return createIndex(row, column, parentObject->children().at(row));
    }else{
        return QModelIndex();
    }
}

QModelIndex SceneModel::parent(const QModelIndex &child) const
{
    if(m_root == NULL)
        return QModelIndex();

    if( !child.isValid() ){
        return QModelIndex();
    }

    Feature *childObject = static_cast<Feature *>(child.internalPointer());
    Feature *parentObject = static_cast<Feature *>(childObject->parent());

    if(parentObject == m_root){
        return QModelIndex();
    }

    Feature *grandParentObject = static_cast<Feature *>(parentObject->parent());

    return createIndex(grandParentObject->children().indexOf(parentObject), 0, parentObject);

}

int SceneModel::rowCount(const QModelIndex &parent) const
{
    if(m_root == NULL)
        return 0;
    Feature *parentObject;
    if( !parent.isValid() ){
        parentObject = m_root;
    }else{
        parentObject = static_cast<Feature *>(parent.internalPointer());
    }
    int count = parentObject->children().count();
    return count;
}

int SceneModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant SceneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::DisplayRole && orientation == Qt::Horizontal ) {
        if(section == 0)
            return QString("Scene");
    }
    return QVariant();
}
QVariant SceneModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() ){
        return QVariant();
    }
    if(role == Qt::DecorationRole){
        if(index.column() == 0) {
            QString className = static_cast<Feature *>( index.internalPointer() )->metaObject()->className();
            if(className == "Building"){
                return *m_buildingIcon;
            }else if(className == "Floor") {
                return *m_floorIcon;
            } else if(className == "Room") {
                return *m_funcAreaIcon;
            } else if (className == "PubPoint") {
                return *m_pubPointIcon;
            }
        }
    }
    if(role == Qt::DisplayRole){
        if(index.column() == 0){
            Feature * feature = static_cast<Feature *>( index.internalPointer() );
            QString className = feature->metaObject()->className();
            if(className == "Room"){
                Room *room = static_cast<Room*>(feature);
                if(room->dianpingId() != 0 && room->dianpingId() != -1){
                    return room->objectName() + '(' + QString::number(room->dianpingId()) + ')';
                }
            }
            return feature->objectName();
        }
    }
    return QVariant();
}
