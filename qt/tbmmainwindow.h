#ifndef TBMMAINWINDOW_H
#define TBMMAINWINDOW_H

#include <QMainWindow>

class TBMMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TBMMainWindow(QWidget *parent = nullptr);

signals:
protected:
  void closeEvent(QCloseEvent * event);
private:
  void centerOnScreen();
  void createActions();
  void createMenus();
  void addStatusBar();
  void addWindowWidgets();
private:
  QMenu* viewMenu;
  QAction* fileNewAction;
  QAction* fileOpenAction;
  QAction* quitApplicationAction;

};

#endif // TBMMAINWINDOW_H
