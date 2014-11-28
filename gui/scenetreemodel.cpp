#include "scenetreemodel.h"
#include "core/mapentity.h"

SceneTreeModel::SceneTreeModel(MapEntity *root, QObject *parent) :
    QAbstractItemModel(parent), m_root(root)
{
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::ItemIsEnabled;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(m_root == NULL)
        return QModelIndex();

    MapEntity *parentObject;
    if(!parent.isValid()){
        parentObject = m_root;
    }else{
        parentObject = static_cast<MapEntity *>(parent.internalPointer());
    }

    if(row >= 0 && row < parentObject->childItems().count()){
        return createIndex(row, column, parentObject->childItems().at(row));
    }else{
        return QModelIndex();
    }
}

QModelIndex SceneTreeModel::parent(const QModelIndex &child) const
{
    if(m_root == NULL)
        return QModelIndex();

    if( !child.isValid() ){
        return QModelIndex();
    }

    MapEntity *childObject = static_cast<MapEntity *>(child.internalPointer());
    MapEntity *parentObject = static_cast<MapEntity *>(childObject->parentItem());

    if(parentObject == m_root){
        return QModelIndex();
    }

    MapEntity *grandParentObject = static_cast<MapEntity *>(parentObject->parentItem());

    return createIndex(grandParentObject->childItems().indexOf(parentObject), 0, parentObject);

}

int SceneTreeModel::rowCount(const QModelIndex &parent) const
{
    if(m_root == NULL)
        return 0;
    MapEntity *parentObject;
    if( !parent.isValid() ){
        parentObject = m_root;
    }else{
        parentObject = static_cast<MapEntity *>(parent.internalPointer());
    }

    return parentObject->childItems().count();
}

int SceneTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant SceneTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if( role == Qt::DisplayRole && orientation == Qt::Horizontal ) {
        return QString("Scene");
    }else{
        return QVariant();
    }
}
QVariant SceneTreeModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() ){
        return QVariant();
    }
    if(role == Qt::DisplayRole){
        MapEntity * entity = static_cast<MapEntity *>( index.internalPointer() );
        return entity->name();
    }else{
        return QVariant();
    }
}
