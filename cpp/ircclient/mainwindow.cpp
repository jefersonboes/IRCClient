#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ircClient = IRCClientInterface::New();
    userIRCCMDProcessor = UserIRCCMDProcessorInterface::New(ircClient);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRead()));
    timer->start(1);
}

MainWindow::~MainWindow()
{
    IRCClientInterface::Delete(ircClient);
    UserIRCCMDProcessorInterface::Delete(userIRCCMDProcessor);
    delete timer;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(!ircClient->connect("irc.rizon.net", 6667, "Nitronix_Ex", "Nitronix_Ex"))
        QMessageBox::critical(this, "Error", "Cannot connect.");
}

void MainWindow::on_pushButton_2_clicked()
{
    userIRCCMDProcessor->processUserCMD(ui->lineEdit->text());
}

void MainWindow::updateRead()
{
    QString data;

    ircClient->getData(data);
    if (data.length() > 0) {
        ui->plainTextEdit->appendPlainText(data);
        QScrollBar *scroll = ui->plainTextEdit->verticalScrollBar();
        scroll->setSliderPosition(scroll->maximum());
    }
}
