#include "stringlistdialog.h"
#include "ui_stringlistdialog.h"

StringListDialog::StringListDialog(const QString& title, const QString& message, QStringList stringList, bool displayOKButton, bool displayCancelButton, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringListDialog) {

    ui->setupUi(this);
    setWindowTitle(" "+title);
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->label->setText(message);
    ui->listWidget->addItems(stringList);
    ui->cancelButton->setVisible(displayCancelButton);
    ui->okButton->setVisible(displayOKButton);
    setModal(true);

}

StringListDialog::~StringListDialog() {

    delete ui;
}
