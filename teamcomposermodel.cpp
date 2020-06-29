#include "teamcomposermodel.h"
#include <QDebug>


TeamComposerModel::TeamComposerModel(DB* db, QObject *parent) :
    _db(db),
    QStandardItemModel(parent)
{

    setColumnCount(2);
    setRowCount(0);
    // set header labels
    QStringList header;
    header << tr("Catégorie d'équipe");
    header << tr("Règle de composition");
    setHorizontalHeaderLabels(header);

    // load rules from DB
    if (db!=0) {
        QList<QStringList> ruleList = _db->ruleMap();
        QStringList ruleNameList = ruleList.at(0);
        QStringList ruleExpressionList = ruleList.at(1);
        int index = 0;
        foreach (QString ruleName, ruleNameList) {
            QString ruleExpression = ruleExpressionList.at(index++);
            QStandardItem* categString = new QStandardItem(ruleName);
            QStandardItem* ruleString = new QStandardItem(ruleExpression);
            QList<QStandardItem*> list;
            list << categString << ruleString;
            appendRow(list);
        }
    }
    updateCompositionRules();
    connect(this, SIGNAL(itemChanged(QStandardItem*)), SLOT(processItemChanged(QStandardItem*)));

}



QHash<QString, QList<TeamRule> > TeamComposerModel::ruleMap() const {
    return _ruleMap;
}

void TeamComposerModel::addRule(const QString& rule) {
    QStandardItem* categString = new QStandardItem("Nom catégorie");
    QStandardItem* ruleString = new QStandardItem("Règle de composition");
    QList<QStandardItem*> list;
    list << categString << ruleString;
    appendRow(list);
    updateCompositionRules();
}


void TeamComposerModel::updateCompositionRules() {
    _ruleMap.clear();

    QStringList ruleName;
    QStringList ruleExpression;
    for (int row=0; row<rowCount(); row++) {
        QStandardItem* itemName = item(row, 0);
        QStandardItem* itemRule = item(row, 1);
        ruleName << itemName->text();
        ruleExpression << itemRule->text();
        QList<TeamRule> list;
        QStringList tab = itemRule->text().split('+');
        foreach (QString str, tab) {
            str = str.trimmed();
            list << TeamRule(str);
        }
        _ruleMap.insert(itemName->text(), list);
    }
    QList<QStringList> list;
    list << ruleName;
    list << ruleExpression;
    if (_db!=0) _db->setRuleMap(list);

}

void TeamComposerModel::removeRule(int row) {

    removeRow(row);
    updateCompositionRules();
}


void TeamComposerModel::processItemChanged(QStandardItem* item) {
    qDebug() << "Item changed at row " << item->row();
    updateCompositionRules();
}
