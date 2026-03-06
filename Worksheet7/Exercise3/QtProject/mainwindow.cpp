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
#include <vtkNew.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//link renderer to qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);
//add renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    renderer->SetBackground(0.1, 0.2, 0.4);
    //cylinder from before
/*    vtkNew<vtkCylinderSource> cylinder;
    cylinder -> SetResolution(8);
    //mapper
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
    //actor: colour and rotation
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.,0.,0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);*/

//    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//    ui->widget->setRenderWindow(renderWindow);

//    renderer = vtkSmartPointer<vtkRenderer>::New();
//    renderWindow->AddRenderer(renderer);

//reset camera - may need function
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    renderWindow->Render();

    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->addAction(ui->actionItem_Options);
    // click opens dialog
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
        tr("Open STL File"),
        "",
        tr("STL Files (*.stl);;All Files (*.*)")
        );

    if (fileName.isEmpty())
        return;

    QFileInfo info(fileName);
    QString shortName = info.fileName();

    ModelPart* newPart = new ModelPart({ shortName, QString("true") });

    QModelIndex index = ui->treeView->currentIndex();

    if (index.isValid())
    {
        ModelPart* selectedPart =
            static_cast<ModelPart*>(index.internalPointer());

        if (selectedPart)
        {
            selectedPart->appendChild(newPart);
        }
        else
        {
            partList->getRootItem()->appendChild(newPart);
        }
    }
    else
    {
        partList->getRootItem()->appendChild(newPart);
    }

    newPart->loadSTL(fileName);

    ui->treeView->expandAll();

    updateRender();

    emit statusUpdateMessage(QString("Loaded STL: ") + shortName, 0);
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
    {
        emit statusUpdateMessage("No tree item selected", 0);
        return;
    }

    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart)
    {
        emit statusUpdateMessage("Selection invalid", 0);
        return;
    }

    OptionDialogue dialog(this);

    dialog.setFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.applyToModelPart(selectedPart);

        QModelIndex idx = ui->treeView->currentIndex();
        partList->dataChanged(idx.siblingAtColumn(0), idx.siblingAtColumn(1));

        updateRender();

        emit statusUpdateMessage("Dialog accepted", 0);
    }
    else
    {
        emit statusUpdateMessage("Dialog cancelled", 0);
    }
}
void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();

    int rows = partList->rowCount(QModelIndex());
    for (int i = 0; i < rows; ++i)
    {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }

    renderer->ResetCamera();
    renderer->ResetCameraClippingRange();
    renderWindow->Render();
}
void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid())
    {
        ModelPart* selectedPart =
            static_cast<ModelPart*>(index.internalPointer());

        if (selectedPart)
        {
            vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
            if (actor)
            {
                renderer->AddActor(actor);
            }
        }
    }

    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
    {
        return;
    }

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; ++i)
    {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
