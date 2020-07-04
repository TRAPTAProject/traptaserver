#ifndef STRINGLISTDIALOG_H
#define STRINGLISTDIALOG_H

#include <QDialog>

namespace Ui {
class StringListDialog;
}

class StringListDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StringListDialog(const QString &title, const QString &message, QStringList stringList, bool displayOKButton, bool displayCancelButton, QWidget *parent = 0);
    ~StringListDialog();
    
private:
    Ui::StringListDialog *ui;
};

#endif // STRINGLISTDIALOG_H
