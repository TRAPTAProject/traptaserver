#include "dialogmatchname.h"
#include "ui_dialogmatchname.h"

DialogMatchName::DialogMatchName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMatchName)
{
    ui->setupUi(this);
    setWindowTitle(tr(" Tour de matches"));
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);

}

QString DialogMatchName::getRoundName() const {
    return ui->lineEdit->text();

}

DialogMatchName::~DialogMatchName()
{
    delete ui;
}
