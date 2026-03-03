#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Model/ModelPart.h"
#include "Model/ModelPartList.h"

#include <QTreeView>
#include <QPushButton>
#include <QStatusBar>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---------------- Exercise 5: Tree click ----------------
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // ---------------- Exercise 4: Model setup ----------------
    this->partList = new ModelPartList("Parts List", this);

    ui->treeView->setModel(this->partList);
    ui->treeView->setAnimated(true);

    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top level items
    for (int i = 0; i < 3; ++i)
    {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; ++j)
        {
            QString childName = QString("Item %1,%2").arg(i).arg(j);
            QString childVisible("true");

            ModelPart* childChildItem = new ModelPart({ childName, childVisible });
            childItem->appendChild(childChildItem);
        }
    }

    ui->treeView->expandAll();

    // ---------------- Exercise 3: Status bar ----------------
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // ---------------- Exercise 2: Buttons ----------------
    connect(ui->pushButton,   &QPushButton::released, this, &MainWindow::onButton1Released);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::onButton2Released);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------- Exercise 2 ----------------
void MainWindow::onButton1Released()
{
    emit statusUpdateMessage("Button 1 released -> status bar updated.", 2000);
}

void MainWindow::onButton2Released()
{
    emit statusUpdateMessage("Button 2 released -> status bar updated.", 0);
}

// ---------------- Exercise 5 ----------------
void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid())
        return;

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

// ---------------- Exercise 6 ----------------
void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open file",
        QString(),
        "All Files (*.*)"
        );

    if (fileName.isEmpty())
    {
        emit statusUpdateMessage("Open file cancelled.", 2000);
        return;
    }

    emit statusUpdateMessage(QString("Selected file: ") + fileName, 0);
}
