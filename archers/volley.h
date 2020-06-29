#ifndef VOLLEY_H
#define VOLLEY_H

#include <QList>
#include <QJsonArray>

#define VOLLEY_MAX_ARROW 6

class Volley {

public:

    Volley();
    Volley(const QJsonArray& json);
    Volley(const QList<int>& list);
    Volley(const Volley &copy);
    Volley& operator = (const Volley& copy);

    void setArrow(int index, int value);

    int score() const;
    int codeCount(int code) const;
    QList<int> arrowList() const;
    bool isVoid() const;
    bool missedOnce() const;

    QJsonArray jsonArray() const;

    static QString arrowLabelFromValue(int value);
    
private:

    QList<int> _arrowList;

};

#endif // VOLLEY_H
