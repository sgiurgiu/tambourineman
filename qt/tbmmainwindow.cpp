#include "tbmmainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QScreen>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QTreeView>
#include "protomessageoperationswidget.h"

TBMMainWindow::TBMMainWindow(QWidget *parent) : QMainWindow(parent,Qt::Window)
{
    qDebug() << "Creating main window";
    setMinimumSize(640,480);
    setWindowTitle(QCoreApplication::applicationName());
    centerOnScreen();
    createActions();
    createMenus();
    addStatusBar();
    addWindowWidgets();
}

void TBMMainWindow::createActions()
{
    fileNewAction = new QAction(this);
    fileNewAction->setText( tr("&New") );
    fileNewAction->setShortcuts(QKeySequence::New);
    fileNewAction->setToolTip(tr("Create new proto file"));
    fileNewAction->setStatusTip(fileNewAction->toolTip());
//    newAction->setIcon(QtAwesome::getInstance()->icon(fa::fileo));
//    connect(newAction, SIGNAL(triggered()), SLOT(newProject()) );

    fileOpenAction = new QAction(this);
    fileOpenAction->setText( tr("&Open") );
    fileOpenAction->setShortcuts(QKeySequence::Open);
    fileOpenAction->setToolTip(tr("Open proto file"));
    fileOpenAction->setStatusTip(fileOpenAction->toolTip());

    quitApplicationAction = new QAction(this);
    quitApplicationAction->setText( tr("&Quit") );
    quitApplicationAction->setShortcuts(QKeySequence::Quit);
    quitApplicationAction->setToolTip(tr("Quit application"));
    quitApplicationAction->setStatusTip(quitApplicationAction->toolTip());
    connect(quitApplicationAction, SIGNAL(triggered()), SLOT(close()));
}
void TBMMainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu( tr("&File") );
    fileMenu->addAction( fileNewAction );
    fileMenu->addAction( fileOpenAction );
    fileMenu->addSeparator();
    fileMenu->addAction(quitApplicationAction);

    viewMenu = menuBar()->addMenu(tr("View"));
}
void TBMMainWindow::addStatusBar()
{
    statusBar()->showMessage("");
}
void TBMMainWindow::addWindowWidgets()
{
    QDockWidget* messagesTreeDock = new QDockWidget(tr("Messages"),this);
    messagesTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    messagesTreeDock->setObjectName("messagesTree");
    addDockWidget(Qt::LeftDockWidgetArea, messagesTreeDock);
    messagesTreeDock->setWidget(new QTreeView(this));

    QDockWidget* messagePropertiesDock = new QDockWidget(tr("Properties"),this);
    messagePropertiesDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    messagePropertiesDock->setObjectName("messagesTree");
    addDockWidget(Qt::RightDockWidgetArea, messagePropertiesDock);
    messagePropertiesDock->setWidget(new QTreeView(this));


    viewMenu->addAction(messagesTreeDock->toggleViewAction());
    viewMenu->addAction(messagePropertiesDock->toggleViewAction());

    ProtoMessageOperationsWidget* messageOperationsWidget = new ProtoMessageOperationsWidget(this);
    messageOperationsWidget->setObjectName("protomessageoperationswidget");
    setCentralWidget(messageOperationsWidget);
}


void TBMMainWindow::centerOnScreen()
{
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width()-width()) / 2;
    int y = (screenGeometry.height()-height()) / 2;
    move(x, y);
}
void TBMMainWindow::closeEvent(QCloseEvent * event)
{
    QSettings settings;
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
    QMainWindow::closeEvent(event);
}
