#include "matchvolleydialog.h"
#include "ui_matchvolleydialog.h"
#include <QScrollBar>

MatchVolleyDialog::MatchVolleyDialog(const QString& title0,
                                     const QList<Volley> &volleyList0,
                                     const QString& title1,
                                     const QList<Volley>& volleyList1,
                                     QWidget *parent) :
    QDialog(parent),
    _scorecard0(volleyList0),
    _scorecard1(volleyList1),
    ui(new Ui::MatchVolleyDialog)

{

    ui->setupUi(this);
    setWindowTitle("  Détails des volées");
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    ui->label0->setText(title0);
    ui->label1->setText(title1);
    ui->tableView0->setModel(&_scorecard0);
    ui->tableView1->setModel(&_scorecard1);
    connect(ui->tableView0->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->tableView1->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->tableView1->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->tableView0->verticalScrollBar(), SLOT(setValue(int)));

}

MatchVolleyDialog::~MatchVolleyDialog()
{
    delete ui;
}
