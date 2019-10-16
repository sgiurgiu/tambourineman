#ifndef PROTOFILELOADER_H
#define PROTOFILELOADER_H

#include <QObject>
#include <QString>

class ProtoFileLoader : public QObject
{
    Q_OBJECT

public:
    explicit ProtoFileLoader(QObject* parent = nullptr);
    Q_INVOKABLE void loadFile(const QString& file = "");
};

#endif // PROTOFILELOADER_H
