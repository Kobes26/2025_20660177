#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui { class OptionDialogue; }

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

private:
    Ui::OptionDialogue *ui;
};

#endif
