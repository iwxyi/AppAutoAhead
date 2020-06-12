#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    task_timer = new QTimer(this);
    task_timer->setInterval(min_duration);

    settings = new QSettings("appautoahead.ini", QSettings::IniFormat, this);


}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    auto apps = taskbar.get();
    foreach (auto app, apps)
    {
        qDebug() << app.name;
    }
    manager.analyzeApplications(apps);
}
