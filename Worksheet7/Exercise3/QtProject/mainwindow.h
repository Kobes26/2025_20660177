#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Model/ModelPartList.h"
#include "Model/ModelPart.h"
#include <QMainWindow>
#include <QModelIndex>
#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkGenericOpenGLRenderWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void statusUpdateMessage(const QString& message, int timeoutMs);

private slots:
    void onButton1Released();
    void onButton2Released();

    void handleTreeClicked();   // EX5
//    void handleTreeDoubleClicked(const QModelIndex& index);
    void on_actionItem_Options_triggered();
    void on_actionOpen_File_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

//    void onButton3Released();

};
#endif // MAINWINDOW_H
