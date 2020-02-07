#include "protofileloadermodel.h"
#include <QDebug>

#include <iostream>
#include <protofile.h>

ProtoFileLoaderModel::ProtoFileLoaderModel(QObject* parent) : QAbstractItemModel(parent)
{
}

void ProtoFileLoaderModel::loadFile(const QString& file)
{
    qWarning() << "Loading file "<<file;

}
QHash<int, QByteArray> ProtoFileLoaderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FieldNameRole] = "name";
    roles[FieldTypeRole] = "type";
    return roles;
}
int ProtoFileLoaderModel::rowCount(const QModelIndex & parent) const
{

    return 0;
}
QVariant ProtoFileLoaderModel::data(const QModelIndex & index, int role) const
{
    return QVariant();
}
int ProtoFileLoaderModel::columnCount(const QModelIndex &parent) const
{
    return  0;
}
QModelIndex ProtoFileLoaderModel::parent(const QModelIndex &child) const
{
    return  QModelIndex();
}
QModelIndex ProtoFileLoaderModel::index(int row, int column,
                          const QModelIndex &parent ) const
{
    return  QModelIndex();
}
