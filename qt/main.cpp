#include <QApplication>
#include <QMetaType>
#include <QtDebug>
#include <QSettings>
#include "tbmmainwindow.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Zergiu");
    QCoreApplication::setOrganizationDomain("zergiu.com");
    QCoreApplication::setApplicationName("Tambourine Man");
    QCoreApplication::setApplicationVersion(QString("%1").arg(TBM_QT_VERSION));

    //QLoggingCategory::setFilterRules("*.debug=true\nqt.*.debug=false");
    //QLoggingCategory::setFilterRules("*.debug=true");

    QApplication app(argc, argv);

    TBMMainWindow mainWindow;
    QSettings settings;
    mainWindow.restoreGeometry(settings.value("mainwindow/geometry").value<QByteArray>());
    mainWindow.restoreState(settings.value("mainwindow/state").value<QByteArray>());
    mainWindow.show();

    return app.exec();
}


