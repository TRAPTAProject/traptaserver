#include "msgbox.h"
#include "dialogbox.h"
#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <QLayout>

QMainWindow* MsgBox::_mainWindow = 0;
QWidget* MsgBox::_curtain = 0;
int MsgBox::_currentId = -1;
bool MsgBox::_spinning = false;
DialogBox* MsgBox::_spinningBox = 0;

void MsgBox::init(QMainWindow* mainWindow) {
    _mainWindow = mainWindow;
    _curtain = new QWidget();
    _curtain->setGraphicsEffect(new QGraphicsOpacityEffect);
    QLayout* layout = mainWindow->layout();
    if (layout==0) {
        qCritical() << "MsgBox: there is no layout for the widget. The message box is not initialized !";
        return;
    }
    layout->addWidget(_curtain);
    _curtain->move(0,0);
    _curtain->setVisible(false);
    _curtain->setFixedWidth(2000);
    _curtain->setFixedHeight(2000);
    _currentId = -1;
    _spinning = false;
}

int MsgBox::popup(MsgIcon icon, const QString &text, const QString& cancelButton, const QString& okButton, int id) {
    if (_currentId>-1 && _currentId==id) return QDialog::Accepted; // do not pop up window if already displayed
    if (_curtain!=0) _curtain->setVisible(true);
    _currentId = id;
    DialogBox box(icon, text, cancelButton, okButton, _mainWindow);
    int r = box.exec();
    if (_curtain!=0) _curtain->setVisible(false);
    _currentId = -1;
    return r;

}

void MsgBox::startSpinner(const QString &text) {
    if (_spinning) {
        qDebug() << "Already spinning";
        return;
    }
    if (_curtain!=0) _curtain->setVisible(true);
    _spinning = true;
    _spinningBox = new DialogBox(text, _mainWindow);
    _spinningBox->open();
    qDebug() << "start spinning";

}

void MsgBox::stopSpinner() {
    if (!_spinning) {
        qDebug() << "Not currently spinning";
        return;
    }
    if (_curtain!=0) _curtain->setVisible(false);
    if (_spinningBox!=0) {
        _spinningBox->close();
        _spinningBox->deleteLater();
    }
    _spinning = false;
    qDebug() << "stop spinning";
}
