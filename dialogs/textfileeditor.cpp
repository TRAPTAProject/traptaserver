#include "textfileeditor.h"
#include "ui_textfileeditor.h"
#include "msgbox.h"
#include <QTextStream>
#include <QDebug>

TextFileEditor::TextFileEditor(const QString& defaultFilename, const QString &filename, const QString &title, const QString& label, QWidget *parent) :
    QDialog(parent),
    _filename(filename),
    _defaultFilename(defaultFilename),
    ui(new Ui::TextFileEditor)
{
    ui->setupUi(this);
    setWindowTitle(" "+title);
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    connect(ui->okButton, SIGNAL(clicked()), SLOT(processSaveButton()));
    connect(ui->defaultButton, SIGNAL(clicked(bool)), SLOT(processDefaultValues()));
    ui->titleLabel->setText(label);
    QFile file(_filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->textEdit->setPlainText(in.readAll());
    }
}

TextFileEditor::~TextFileEditor()
{
    delete ui;
}

void TextFileEditor::processDefaultValues() {
    QFile file(_defaultFilename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->textEdit->setPlainText(in.readAll());
    }
}

void TextFileEditor::processSaveButton() {
    QFile file(_filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.close();
        qDebug() << "File saved to " << _filename;
        accept();
    }
    else {
        qCritical() << "Cannot save file to " << _filename;
        reject();
    }

}
