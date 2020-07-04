#include "ipchooser.h"
#include "ui_ipchooser.h"

IPChooser::IPChooser(QStringList &stringList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPChooser) {

    ui->setupUi(this);
    setWindowTitle(tr(" Sélection adresse réseau"));
    ui->comboBox->addItems(stringList);
}

QString IPChooser::getSelectedItem() {

    return ui->comboBox->currentText();

}

IPChooser::~IPChooser()
{
    delete ui;
}
