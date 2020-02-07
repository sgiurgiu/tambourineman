#ifndef PROTOFILELOADERMODEL_H
#define PROTOFILELOADERMODEL_H

#include <QAbstractItemModel>
#include <QString>
#include <protofileloader.h>

class ProtoFileLoaderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum ProtoMessageRoles {
            FieldNameRole = Qt::UserRole + 1,
            FieldTypeRole
    };
    explicit ProtoFileLoaderModel(QObject* parent = nullptr);
    Q_INVOKABLE void loadFile(const QString& file = "");
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    tbm::ProtoFileLoader fileLoader;
};

#endif // PROTOFILELOADER_H
