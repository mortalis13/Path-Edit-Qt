#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QShortcut>

#include <windows.h>
#include <iostream>


// ------------------------------------------------- consts -------------------------------------------------

const QString regPath = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
const QString regSubPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\";

const QString pathName = "Path";


// ------------------------------------------------- slots -------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  
  addActions();
  addShortcuts();
  init();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::init() {
  setting = new QSettings(regPath, QSettings::NativeFormat);
  ui->tePath->document()->setDocumentMargin(8);
}

void MainWindow::addActions() {
  connect( ui->bLoad, SIGNAL(clicked()), this, SLOT(loadPath()) );
  connect( ui->bSave, SIGNAL(clicked()), this, SLOT(savePath()) );
}

void MainWindow::addShortcuts() {
  QShortcut* load = new QShortcut(QKeySequence("F1"), this);
  connect( load, SIGNAL(activated()), ui->bLoad, SLOT(click()) );
  
  QShortcut* save = new QShortcut(QKeySequence("F2"), this);
  connect( save, SIGNAL(activated()), ui->bSave, SLOT(click()) );

  QShortcut *quit = new QShortcut(QKeySequence("Esc"), this);
  connect( quit, SIGNAL(activated()), this, SLOT(close()) );
}

// ------------------------------------------------- slots -------------------------------------------------

void MainWindow::loadPath()
{
  QString path = "";
  path = getPathVal();
  path = formatPath(path);
  showPath(path);
}

void MainWindow::savePath()
{
   QString path = "";
   path = getPathText();
   path = joinPath(path);
   setPathVal(path);
   updateEnvironment();
  
   status("Path updated");
}

// ------------------------------------------------- service -------------------------------------------------

QString MainWindow::getPathVal() {
  return setting->value(pathName, "no-path").toString();
}

QString MainWindow::formatPath(QString path) {
  path = path.replace(";", ";\n");
  return path;
}

void MainWindow::showPath(QString path) {
  ui->tePath->setPlainText(path);
  ui->tePath->setFocus();
}


QString MainWindow::getPathText() {
  return ui->tePath->toPlainText();
}

QString MainWindow::joinPath(QString path) {
  path = path.replace(";\n", ";");
  return path;
}

void MainWindow::setPathVal(QString path) {
  // setting->setValue(pathName, path);
  HKEY key;

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), 0, KEY_SET_VALUE, &key) != ERROR_SUCCESS) {
    status("Unable to open registry key");
    return;
  }

  wstring pathStr = path.toStdWString();

  const BYTE* lpcBuffer = reinterpret_cast<const BYTE*>(pathStr.c_str());
  DWORD cbData = static_cast<DWORD>(pathStr.size() * sizeof(wchar_t));

  if (RegSetValueEx(key, TEXT("Path"), 0, REG_EXPAND_SZ, lpcBuffer, cbData) != ERROR_SUCCESS) {
    status("Unable to write registry key");
    return;
  }
}

void MainWindow::updateEnvironment() {
  SendMessageA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment");
}

// ------------------------------------------------- other -------------------------------------------------

void MainWindow::status(QString msg, bool permanent) {
  int timeout = 5000;
  if (permanent) timeout = 0;
  ui->statusBar->showMessage(msg, timeout);
}
