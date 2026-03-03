#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Connect our custom signal to the status bar
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);
    connect(ui->pushButton,   &QPushButton::released, this, &MainWindow::onButton1Released);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::onButton2Released);
//    connect(ui->pushButton_3, &QPushButton::released, this, &MainWindow::onButton3Released);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButton1Released()
{
    emit statusUpdateMessage("Button 1 released -> status bar updated.", 2000); // 2 seconds
}

void MainWindow::onButton2Released()
{
    emit statusUpdateMessage("Button 2 released -> status bar updated.", 0);    // 0 = stays until replaced
}

/*void MainWindow::onButton3Released()
{
    QMessageBox::information(this,
                             "exercise 2",
                             "button 3 released -> slot executed.");
}*/
