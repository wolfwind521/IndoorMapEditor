#include "scenemodel.h"
#include "../core/mapentity.h"
#include "../core/funcarea.h"
#include <QIcon>
SceneModel::SceneModel(MapEntity *root, QObject *parent) :
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

    MapEntity *parentObject;
    if(!parent.isValid()){
        parentObject = m_root;
    }else{
        parentObject = static_cast<MapEntity *>(parent.internalPointer());
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

    MapEntity *childObject = static_cast<MapEntity *>(child.internalPointer());
    MapEntity *parentObject = static_cast<MapEntity *>(childObject->parent());

    if(parentObject == m_root){
        return QModelIndex();
    }

    MapEntity *grandParentObject = static_cast<MapEntity *>(parentObject->parent());

    return createIndex(grandParentObject->children().indexOf(parentObject), 0, parentObject);

}

int SceneModel::rowCount(const QModelIndex &parent) const
{
    if(m_root == NULL)
        return 0;
    MapEntity *parentObject;
    if( !parent.isValid() ){
        parentObject = m_root;
    }else{
        parentObject = static_cast<MapEntity *>(parent.internalPointer());
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
            QString className = static_cast<MapEntity *>( index.internalPointer() )->metaObject()->className();
            if(className == "Building"){
                return *m_buildingIcon;
            }else if(className == "Floor") {
                return *m_floorIcon;
            } else if(className == "FuncArea") {
                return *m_funcAreaIcon;
            } else if (className == "PubPoint") {
                return *m_pubPointIcon;
            }
        }
    }
    if(role == Qt::DisplayRole){
        if(index.column() == 0){
            MapEntity * entity = static_cast<MapEntity *>( index.internalPointer() );
            QString className = entity->metaObject()->className();
            if(className == "FuncArea"){
                FuncArea *funcArea = static_cast<FuncArea*>(entity);
                if(funcArea->dianpingId() != 0 && funcArea->dianpingId() != -1){
                    return funcArea->objectName() + '(' + QString::number(funcArea->dianpingId()) + ')';
                }
            }
            return entity->objectName();
        }
    }
    return QVariant();
}
