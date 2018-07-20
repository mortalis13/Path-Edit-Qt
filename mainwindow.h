#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <string>

using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void showBytes(string str);
private:
  void addActions();
  void addShortcuts();
  void init();
  
  void status(QString msg, bool permanent=false);

  QString getPathVal();
  QString formatPath(QString path);
  void showPath(QString path);
  
  QString getPathText();
  QString joinPath(QString path);
  void setPathVal(QString path);
  void updateEnvironment();
  
private slots:
  void loadPath();
  void savePath();

    
private:
  Ui::MainWindow *ui;
  QSettings* setting;


};

#endif // MAINWINDOW_H
