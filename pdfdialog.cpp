#include "pdfdialog.h"
#include "ui_pdfdialog.h"
#include <QPixmap>
#include <QFileDialog>

PDFDialog::PDFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PDFDialog)
{
    ui->setupUi(this);
    setWindowTitle("  Impression des feuilles de marques  ");
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    connect(ui->loadImageButton, SIGNAL(clicked()), SLOT(onLoadImageClick()));
    connect(ui->voidImageButton, SIGNAL(clicked()), SLOT(onVoidImageClick()));
    ui->iconLabel->setPixmap(QPixmap());
}

void PDFDialog::onVoidImageClick() {
    ui->iconLabel->setPixmap(QPixmap());
    _imagePath="";
}

void PDFDialog::onLoadImageClick() {
    setImagePath(QFileDialog::getOpenFileName(this, tr("Charger une image"),
                                                    "",
                                                    tr("Images (*.png *.jpg);; Tout fichier (*.*)")),
                 ui->iconLabel->width(),
                 ui->iconLabel->height()
                 );
}

QString PDFDialog::getTitle() const {
    return ui->titleText->toPlainText();
}

QString PDFDialog::getImagePath() const {
    return _imagePath;
}

void PDFDialog::setTitle(const QString& title) {
    ui->titleText->setPlainText(title);
}

void PDFDialog::setImagePath(const QString& imagePath, int width, int height) {
    _imagePath = imagePath;
    ui->iconLabel->setPixmap(QPixmap(_imagePath).scaled(width, height, Qt::KeepAspectRatio));


}

PDFDialog::~PDFDialog()
{
    delete ui;
}
