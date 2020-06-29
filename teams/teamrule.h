#ifndef TEAMRULE_H
#define TEAMRULE_H

#include <QObject>
#include <QStringList>

class TeamRule : public QObject
{
    Q_OBJECT
public:
    explicit TeamRule(const QString& rule, QObject *parent = 0);

    TeamRule(const TeamRule &copy);
    TeamRule& operator = (const TeamRule& copy);

    QString stringRule() const;
    QStringList categList() const;
    bool everybody() const;

private:

    QString _stringRule; // rule as a string (passed as parameter)
    bool _everybody; // cardinalite. How many people do we select with this rule.
    QStringList _categList; // SHCL | SFCL

};

#endif // TEAMRULE_H
