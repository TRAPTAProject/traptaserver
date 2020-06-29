#ifndef TEAMCOMPOSERMODEL_H
#define TEAMCOMPOSERMODEL_H

#include <QStandardItemModel>
#include "teamrule.h"

class Database;

class TeamComposerModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TeamComposerModel(Database* db);
    QHash<QString, QList<TeamRule> > ruleMap() const;


signals:

private slots:

    void processItemChanged(QStandardItem* item);

public slots:

    void addRule(const QString& rule = QString());
    void removeRule(int row);


private:

    QHash<QString, QList<TeamRule> > _ruleMap;
    Database* _db;

    void updateCompositionRules();

};

#endif // TEAMCOMPOSERMODEL_H
