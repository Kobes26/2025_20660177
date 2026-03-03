#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model/ModelPart.h"
#include "Model/ModelPartList.h"

#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Exercise 5
    connect(ui->treeView, &QTreeView::clicked,
            this, &MainWindow::handleTreeClicked);

    // ---------------- Exercise 4: create/allocate the model list ----------------
    this->partList = new ModelPartList("Parts List", this);

    // Link it to the TreeView in the GUI
    ui->treeView->setModel(this->partList);
    ui->treeView->setAnimated(true);

    // Manually create a model tree (worksheet starter example)
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
    // ---------------------------------------------------------------------------

    // ---------------- Exercise 3: Status bar signal hookup ----------------
    connect(this, &MainWindow::statusUpdateMessage,
            ui->statusbar, &QStatusBar::showMessage);

    // ---------------- Exercise 2: Button connections ----------------
    connect(ui->pushButton,   &QPushButton::released, this, &MainWindow::onButton1Released);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::onButton2Released);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButton1Released()
{
    emit statusUpdateMessage("Button 1 released -> status bar updated.", 2000);
}

void MainWindow::onButton2Released()
{
    emit statusUpdateMessage("Button 2 released -> status bar updated.", 0);
}

void MainWindow::handleTreeClicked()
{
    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* Retrieve the name string from the internal QVariant array */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}
