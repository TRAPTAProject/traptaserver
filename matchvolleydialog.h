#ifndef MATCHVOLLEYDIALOG_H
#define MATCHVOLLEYDIALOG_H

#include <QDialog>
#include "matchscorecard.h"

namespace Ui {
class MatchVolleyDialog;
}

class MatchVolleyDialog : public QDialog {
    Q_OBJECT

public:
    explicit MatchVolleyDialog(const QString& title0,
                               const QList<Volley>& volleyList0,
                               const QString& title1,
                               const QList<Volley>& volleyList1,
                               QWidget *parent = 0);
    ~MatchVolleyDialog();

private:
    Ui::MatchVolleyDialog *ui;
    MatchScoreCard _scorecard0;
    MatchScoreCard _scorecard1;
};

#endif // MATCHVOLLEYDIALOG_H
