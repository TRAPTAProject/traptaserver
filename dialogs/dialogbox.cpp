#include "dialogbox.h"
#include "ui_dialogbox.h"
#include <QDebug>
#include <QKeyEvent>


DialogBox::DialogBox(int icon, const QString& text, const QString& cancelButton, const QString& okButton, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DialogBox)
{
    init(parent);
    if (okButton.isEmpty()) ui->okButton->setVisible(false);
    else ui->okButton->setText(okButton);
    if (cancelButton.isEmpty()) ui->cancelButton->setVisible(false);
    else ui->cancelButton->setText(cancelButton);

    ui->label->setText(text);
    switch (icon) {
        case 0 : {
            ui->iconLabel->setPixmap(QPixmap(":/images/tango-idea.png"));
            break;
        }
        case 1 : {
            ui->iconLabel->setPixmap(QPixmap(":/images/tango-info.png"));
            break;
        }
        case 2 : {
            ui->iconLabel->setPixmap(QPixmap(":/images/tango-question.png"));
            break;
        }

        case 3 : {
            ui->iconLabel->setPixmap(QPixmap(":/images/tango-warning.png"));
            break;
        }
        case 4 : {
            ui->iconLabel->setPixmap(QPixmap(":/images/tango-critical.png"));
            break;
        }
    }
}

DialogBox::DialogBox(const QString& text, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DialogBox)
{
    init(parent);
    ui->buttonContainer->hide();
    ui->iconLabel->hide();
    ui->label->setText(text);



}

void DialogBox::init(QWidget* parent) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    resize(parent->width(), parent->height()/2);
    move(0,parent->height()/4);
    setFocus(Qt::PopupFocusReason);

}

//################################ Key handling
void DialogBox::keyPressEvent(QKeyEvent* event) {

    qDebug() << "Pressing key " << event->key();
    if (event->key()==Qt::Key_Escape) reject();

}


DialogBox::~DialogBox()
{
    delete ui;
}
