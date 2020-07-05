#ifndef VOLLEYDIALOG_H
#define VOLLEYDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "archers/archer.h"
#include "archers/heatscorecard.h"

namespace Ui {
class VolleyDialog;
}

class VolleyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VolleyDialog(Archer* archer, int heatIndex, int arrowCount, QWidget *parent = 0);


    ~VolleyDialog();
    
private:
    Ui::VolleyDialog *ui;
    HeatScoreCard _scoreCard;

};

#endif // VOLLEYDIALOG_H
