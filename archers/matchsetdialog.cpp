#include "matchsetdialog.h"
#include "ui_matchsetdialog.h"
#include <QDebug>

MatchSetDialog::MatchSetDialog(const QString &tabName, const QStringList& categList, const QStringList& previousMatchRoundList, const QList<int>& previousMatchTabId, QWidget *parent) :
    QDialog(parent),
    _previousTabId(previousMatchTabId),
    ui(new Ui::MatchSetDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString(" Générer des matches par catégorie pour %0").arg(tabName));
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->matchCombo->addItems(previousMatchRoundList);
    ui->categBox->addItems(categList);
    ui->matchCombo->setEnabled(false);
    QStringList list;
    for (int i=1; i<80; i++) list << QString::number(i);
    ui->targetBox->addItems(list);

}


int MatchSetDialog::round() {
    return ui->roundCombo->count()-ui->roundCombo->currentIndex();
}

int MatchSetDialog::previousMatchTabId() {
    int currentIndex = ui->matchCombo->currentIndex();
    if (currentIndex<0 || currentIndex>=_previousTabId.count()) return -1;
    return _previousTabId.at(currentIndex);
}

bool MatchSetDialog::source() {
    if (ui->heatRadio->isChecked()) return 0;
    return 1;
}

QString MatchSetDialog::categ() const {
    return ui->categBox->currentText();
}

int MatchSetDialog::target() {
    return ui->targetBox->currentIndex()+1;
}

int MatchSetDialog::mode() {
    return ui->modeBox->currentIndex();
}

bool MatchSetDialog::trispot() {
    return ui->trispotBox->isChecked();
}

MatchSetDialog::~MatchSetDialog() {
    delete ui;
}

