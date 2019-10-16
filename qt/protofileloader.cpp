#include "protofileloader.h"
#include <QDebug>

#include <iostream>

ProtoFileLoader::ProtoFileLoader(QObject* parent) : QObject(parent)
{
}

void ProtoFileLoader::loadFile(const QString& file)
{
    qWarning() << "Loading file "<<file;

}
