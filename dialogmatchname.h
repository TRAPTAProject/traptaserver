#ifndef DIALOGMATCHNAME_H
#define DIALOGMATCHNAME_H

#include <QDialog>

namespace Ui {
class DialogMatchName;
}

class DialogMatchName : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMatchName(QWidget *parent = 0);
    ~DialogMatchName();
    QString getRoundName() const;

private:
    Ui::DialogMatchName *ui;
};

#endif // DIALOGMATCHNAME_H
