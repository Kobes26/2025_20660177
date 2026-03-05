#include "optiondialog.h"
#include "Model/modelpart.h"

#include <QDialogButtonBox>

OptionDialogue::OptionDialogue(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::OptionDialogue)
{
    ui->setupUi(this);

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

// -------------------- NEW: load dialog from selected ModelPart --------------------
void OptionDialogue::setFromModelPart(ModelPart* part)
{
    if(!part) return;

    // Column 0 is the name in the tree
    setName(part->data(0).toString());

    // Use the real visibility flag (preferred)
    setVisibleChecked(part->visible());
    setRGB(part->getColourR(), part->getColourG(), part->getColourB());
}

// -------------------- NEW: apply dialog values back to ModelPart -----------------
void OptionDialogue::applyToModelPart(ModelPart* part) const
{
    if(!part) return;

    // Update name shown in tree (column 0)
    part->set(0, name());

    // Update visibility boolean
    part->setVisible(visibleChecked());

    // Optional: also update column 1 text (if you are using it as "true/false" display)
    part->set(1, visibleChecked() ? QString("true") : QString("false"));
    part->setColour(
        static_cast<unsigned char>(r()),
        static_cast<unsigned char>(g()),
        static_cast<unsigned char>(b())
        );
}

void OptionDialogue::setRGB(int r, int g, int b)
{
    ui->spinBoxR->setValue(r);
    ui->spinBoxG->setValue(g);
    ui->spinBoxB->setValue(b);
}

int OptionDialogue::r() const { return ui->spinBoxR->value(); }
int OptionDialogue::g() const { return ui->spinBoxG->value(); }
int OptionDialogue::b() const { return ui->spinBoxB->value(); }
