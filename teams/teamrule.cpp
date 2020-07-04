#include "teamrule.h"

TeamRule::TeamRule(const QString &rule, QObject *parent) :
    QObject(parent),
    _everybody(false),
    _stringRule(rule)
{

    QStringList tab = rule.split('|');
    foreach (QString str, tab) {
        str = str.trimmed();
        if (str.indexOf('*')>-1) {
            _everybody = true;
            _categList = QStringList();
            _categList << str.remove(str.indexOf('*'), 1);
            break;
        }
        _categList << str;
    }

}

TeamRule::TeamRule(const TeamRule &copy) {
    _everybody = copy.everybody();
    _stringRule = copy.stringRule();
    _categList = copy.categList();
}

TeamRule& TeamRule::operator = (const TeamRule& copy) {
    _everybody = copy.everybody();
    _stringRule = copy.stringRule();
    _categList = copy.categList();
    return *this;
}

QString TeamRule::stringRule() const {
    return _stringRule;
}

QStringList TeamRule::categList() const {
    return _categList;
}

bool TeamRule::everybody() const {
    return _everybody;
}
