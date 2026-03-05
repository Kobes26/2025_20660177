#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Model/ModelPart.h"
#include "Model/ModelPartList.h"

#include "optiondialog.h"
#include <QTreeView>
#include <QTreeView>
#include <QPushButton>
#include <QStatusBar>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QModelIndex>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <qvtkopenglnativewidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItem_Options);
    // Double-click opens dialog
    //connect(ui->treeView, &QTreeView::doubleClicked,
    //        this, &MainWindow::handleTreeDoubleClicked);
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
    {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart)
    {
        emit statusUpdateMessage("Selection invalid", 0);
        return;
    }

    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

// ---------------- Exercise 6 ----------------
void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("STL Files (*.stl);;All Files (*.*)")
        );

    if (fileName.isEmpty())
        return;

    // Show file path in status bar
    emit statusUpdateMessage(fileName, 0);

    // Get currently selected tree item
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid())
    {
        emit statusUpdateMessage("No tree item selected", 0);
        return;
    }

    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart)
        return;

    // Extract just the filename (not full path)
    QFileInfo info(fileName);
    QString shortName = info.fileName();

    // Update the name column in the tree
    selectedPart->set(0, shortName);

    // Refresh tree view display
    partList->dataChanged(index.siblingAtColumn(0),
                          index.siblingAtColumn(1));
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!index.isValid())
    {
        emit statusUpdateMessage("No tree item selected", 0);
        return;
    }

    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if(!selectedPart)
    {
        emit statusUpdateMessage("Selection invalid", 0);
        return;
    }

    OptionDialogue dialog(this);

    dialog.setFromModelPart(selectedPart);

    if(dialog.exec() == QDialog::Accepted)
    {
        dialog.applyToModelPart(selectedPart);

        // refresh the tree display
        QModelIndex idx = ui->treeView->currentIndex();
        partList->dataChanged(idx.siblingAtColumn(0), idx.siblingAtColumn(1));
        emit statusUpdateMessage("Dialog accepted", 0);
    }
    else
    {
        emit statusUpdateMessage("Dialog cancelled", 0);
    }
}
