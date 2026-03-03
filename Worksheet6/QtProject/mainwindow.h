#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Model/ModelPartList.h"
#include "Model/ModelPart.h"
#include <QMainWindow>

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

    void on_actionOpen_File_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;

//    void onButton3Released();

};
#endif // MAINWINDOW_H
