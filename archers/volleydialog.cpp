#include "volleydialog.h"
#include "ui_volleydialog.h"
#include "volley.h"

VolleyDialog::VolleyDialog(Archer* archer, int heatIndex, int arrowCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolleyDialog)
{
    ui->setupUi(this);
    QString windowTitle = QString("SERIE %0").arg(heatIndex+1);
    setWindowTitle(" "+windowTitle);
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);

    if (archer!=0) {
        ui->label->setText(archer->toTitleString());
        _scoreCard = archer->scoreCard(heatIndex);
        ui->tableView->setModel(&_scoreCard);
        for (int index=arrowCount; index<VOLLEY_MAX_ARROW; index++) ui->tableView->setColumnHidden(index, true);

    }


}


VolleyDialog::~VolleyDialog()
{
    delete ui;
}
