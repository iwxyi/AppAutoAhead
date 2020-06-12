#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QSettings>
#include "taskbargetter.h"
#include "applicationruntimemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    TaskBarGetter taskbar;
    QTimer* task_timer;
    QSettings* settings;
    ApplicationRuntimeManager manager;

    const int min_duration = 1000; // 检测间隔/最短运行时间（时间太短没必要，例如查看图片）
};
#endif // MAINWINDOW_H
