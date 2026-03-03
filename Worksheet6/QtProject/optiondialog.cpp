#include "optiondialog.h"
#include "ui_optiondialog.h"

#include <QDialogButtonBox>

OptionDialogue::OptionDialogue(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OptionDialogue)
{
    ui->setupUi(this);

    // Make OK/Cancel work
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

OptionDialogue::~OptionDialogue()
{
    delete ui;
}

void OptionDialogue::setName(const QString& name)
{
    ui->lineEditName->setText(name);
}

QString OptionDialogue::name() const
{
    return ui->lineEditName->text();
}

void OptionDialogue::setVisibleChecked(bool v)
{
    ui->checkBoxVisible->setChecked(v);
}

bool OptionDialogue::visibleChecked() const
{
    return ui->checkBoxVisible->isChecked();
}
