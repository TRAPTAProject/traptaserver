#ifndef IPCHOOSER_H
#define IPCHOOSER_H

#include <QDialog>

namespace Ui {
class IPChooser;
}

class IPChooser : public QDialog {
    Q_OBJECT
    
public:
    explicit IPChooser(QStringList& stringList, QWidget *parent = 0);
    ~IPChooser();

    QString getSelectedItem();
    
private:
    Ui::IPChooser *ui;
};

#endif // IPCHOOSER_H
