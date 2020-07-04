#ifndef MSGBOX_H
#define MSGBOX_H

#include <QObject>
#include <QMainWindow>
#include "dialogbox.h"

class MsgBox {

public:

    enum MsgIcon {
        idea = 0,
        info,
        question,
        warning,
        critical
    };

    static void init(QMainWindow* mainWindow);
    static int popup(MsgIcon icon, const QString &text, const QString& cancelButton, const QString& okButton, int id=-1);
    static void startSpinner(const QString &text);
    static void stopSpinner();

private:

    static QMainWindow* _mainWindow;
    static QWidget* _curtain;
    static int _currentId;
    static bool _spinning;
    static DialogBox* _spinningBox;

};

#endif // MSGBOX_H
