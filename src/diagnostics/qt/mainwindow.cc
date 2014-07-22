#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the window title
    setWindowTitle("draco_info-gui");

    diw = new diWidget(this);
    setCentralWidget(diw);
}
