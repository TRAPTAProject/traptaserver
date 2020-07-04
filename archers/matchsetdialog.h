#ifndef MATCHSETDIALOG_H
#define MATCHSETDIALOG_H

#include <QDialog>

namespace Ui {
class MatchSetDialog;
}

class MatchSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatchSetDialog(const QString& tabName,
                            const QStringList& categList,
                            const QStringList& previousMatchRoundList,
                            const QList<int>& previousMatchTabId,
                            QWidget *parent);
    ~MatchSetDialog();

    int round();
    int previousMatchTabId();
    bool source();
    int target();
    int mode();
    bool trispot();
    QString categ() const;


private:
    Ui::MatchSetDialog *ui;
    QList<int> _previousTabId;


};

#endif // MATCHRUNDIALOG_H
