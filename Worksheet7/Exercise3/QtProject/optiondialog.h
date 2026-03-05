#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_optiondialog.h"

class ModelPart;   // forward declaration

class OptionDialogue : public QDialog
{
    Q_OBJECT
public:
    explicit OptionDialogue(QWidget *parent = nullptr);
    ~OptionDialogue();

    void setName(const QString& name);
    QString name() const;

    void setVisibleChecked(bool v);
    bool visibleChecked() const;

    // NEW:
    void setFromModelPart(ModelPart* part);
    void applyToModelPart(ModelPart* part) const;

    void setRGB(int r, int g, int b);
    int r() const;
    int g() const;
    int b() const;

private:
    Ui::OptionDialogue *ui;
};

#endif
