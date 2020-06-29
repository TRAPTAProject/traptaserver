#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(" Réglage des ports");
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    QSettings settings;
    bool doBroadcast = settings.value("broadcast", true).toBool();
    bool manualport = settings.value("manualport", false).toBool();
    int udpPort = settings.value("udpport", 5433).toInt();
    int tabletPort = settings.value("tabletport", 8092).toInt();
    int displayPort = settings.value("displayport", 8093).toInt();
    ui->udpPortEdit->setText(QString::number(udpPort));
    ui->tabletPortEdit->setText(QString::number(tabletPort));
    ui->displayPortEdit->setText(QString::number(displayPort));
    ui->groupBox->setChecked(manualport);
    if (!doBroadcast) {
        ui->checkBox->setChecked(false);
        ui->udpPortEdit->setEnabled(false);
    }

    connect(ui->okButton, SIGNAL(clicked()), SLOT(processOKButton()));
    //connect(ui->checkBox, SIGNAL(clicked(bool)), ui->udpPortEdit, SLOT();

}

void SettingsDialog::processOKButton() {
    QSettings settings;
    settings.setValue("manualport", ui->groupBox->isChecked());
    bool ok;
    settings.setValue("broadcast", ui->checkBox->isChecked());
    int udpPort = ui->udpPortEdit->text().toInt(&ok);
    if (ok) settings.setValue("udpport", udpPort);
    int tabletPort = ui->tabletPortEdit->text().toInt(&ok);
    if (ok) settings.setValue("tabletport", tabletPort);
    int displayPort = ui->displayPortEdit->text().toInt(&ok);
    if (ok) settings.setValue("displayport", displayPort);


    close();
}


SettingsDialog::~SettingsDialog()
{
    delete ui;
}
