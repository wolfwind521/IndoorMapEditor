#include "objecttreemodel.h"

ObjectTreeModel::ObjectTreeModel(QObject *root, QObject *parent )
    :QAbstractItemModel(parent), m_root(root)
{
}

Qt::ItemFlags    ObjectTreeModel::flags( const QModelIndex &index ) const{
    if(!index.isValid()) return Qt::ItemIsEnabled;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ObjectTreeModel::headerData(int section,
                                     Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole || orientation != Qt::Horizontal ) return QVariant();
    switch( section ) {
    case 0:
        return QString( "Object" ); case 1:
        return QString( "Class" ); default:
        return QVariant();
    }
}

QModelIndex ObjectTreeModel::index(int row, int column,const QModelIndex &parent ) const
{
    QObject *parentObject;
    if( !parent.isValid() )
        parentObject = m_root;
    else
        parentObject = static_cast<QObject*>( parent.internalPointer() );
    if( row >= 0 && row < parentObject->children().count() )
        return createIndex( row, column, parentObject->children().at( row ) );
    else
        return QModelIndex();
}

int ObjectTreeModel::rowCount(const QModelIndex &parent ) const {
    QObject *parentObject;
    if( !parent.isValid() )
        parentObject = m_root;
    else
        parentObject = static_cast<QObject*>( parent.internalPointer() );
    return parentObject->children().count();
}

int ObjectTreeModel::columnCount(const QModelIndex &parent ) const {
    return 2;
}

QVariant ObjectTreeModel::data( const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();
    if( role == Qt::DisplayRole ) {
        switch( index.column() ) {
        case 0:
            return static_cast<QObject*>( index.internalPointer() )->objectName();
        case 1:
            return static_cast<QObject*>( index.internalPointer() )-> metaObject()->className();
        default: break;
        }
    }
    else if( role == Qt::ToolTipRole ) {
    }
    return QVariant();
}


QModelIndex ObjectTreeModel::parent(const QModelIndex &index) const {
    if( !index.isValid() )
        return QModelIndex();
    QObject *indexObject = static_cast<QObject*>( index.internalPointer() );
    QObject *parentObject = indexObject->parent();
    if( parentObject == m_root )
        return QModelIndex();
    QObject *grandParentObject = parentObject->parent();
    return createIndex( grandParentObject->children().indexOf( parentObject ), 0, parentObject );
}

