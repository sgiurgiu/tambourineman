#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <QMetaType>
#include <QtDebug>
#include <QQuickStyle>
#include <QLoggingCategory>

#include "protofileloadermodel.h"

int main(int argc, char *argv[])
{

    QLoggingCategory::setFilterRules("*.debug=true\n"
    "qt.*.debug=false");
    //QuickStyle::setStyle("Material");
    //qmlRegisterType<AudioOutputItemModel>("com.zergiu.qas", 1, 0, "AudioOutputItemModel");
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/qml/actions.qml")),
                             "com.zergiu.tambourineman", 1, 0, "Actions");
    qmlRegisterType<ProtoFileLoaderModel>("com.zergiu.tambourineman.proto",
                                      1, 0, "ProtoFileLoaderModel");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //engine.addImageProvider("audio",new IconProvider());

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}


