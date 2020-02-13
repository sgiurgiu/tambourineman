#include "protofileloadermodel.h"
#include <QDebug>

#include <iostream>
#include <protofile.h>

ProtoFileLoaderModel::ProtoFileLoaderModel(QObject* parent) : QAbstractItemModel(parent)
{
}

void ProtoFileLoaderModel::loadFile(const QString& file)
{
    qDebug() << "Loading file "<<file;
    emit layoutAboutToBeChanged();
    std::vector<std::string> paths = {"test/data/"};
#ifdef TBM_QT_LINUX
    paths.emplace_back("/usr/include/");
#endif
    auto protoFile = fileLoader.loadFile("test/data/addressbook.proto", paths);
    auto messages = protoFile.messages();
    std::copy(messages.begin(),messages.end(),std::back_inserter(loadedMessages));
    emit layoutChanged();
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
    Q_UNUSED(parent);
    return loadedMessages.size();
}
QVariant ProtoFileLoaderModel::data(const QModelIndex & index, int role) const
{
    auto row = index.row();
    if(row < 0 || row >= (int)loadedMessages.size()) return QVariant();
    switch (role) {
    case FieldNameRole:
        return QVariant::fromValue(QString(loadedMessages.at(row).name().c_str()));
    case FieldTypeRole:
        return QVariant::fromValue(tr("Message"));
    default:
        return QVariant();
    }
}
int ProtoFileLoaderModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  0;
}
QModelIndex ProtoFileLoaderModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return  QModelIndex();
}
QModelIndex ProtoFileLoaderModel::index(int row, int column,
                          const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    auto index = createIndex(row,column);
    return  index;
}
