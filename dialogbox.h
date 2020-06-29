#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QDialog>


namespace Ui {
class DialogBox;
}

class DialogBox : public QDialog
{
    Q_OBJECT

public:

    explicit DialogBox(int icon, const QString& text, const QString& cancelButton, const QString& okButton, QWidget* parent=0);
    DialogBox(const QString& text, QWidget* parent=0);
    ~DialogBox();

protected:
    void keyPressEvent(QKeyEvent* event);

private:

    Ui::DialogBox *ui;

    void init(QWidget *parent);

};

#endif // DIALOGBOX_H
