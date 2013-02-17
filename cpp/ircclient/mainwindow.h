#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ircclient.h"
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void updateRead();
private:
    Ui::MainWindow *ui;
    IRCClientInterface *ircClient;
    UserIRCCMDProcessorInterface *userIRCCMDProcessor;
    QTimer *timer;
};

#endif // MAINWINDOW_H
