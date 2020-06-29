#ifndef LCDSPINNER_H
#define LCDSPINNER_H

#include <QObject>
#include <QPushButton>
#include <QLCDNumber>

class LCDSpinner : public QObject
{
    Q_OBJECT
public:
    explicit LCDSpinner(int min, int max, QLCDNumber* lcdNumber, QPushButton* upButton, QPushButton* downButton, QObject *parent = 0);
    int value() const;
    void setValue(int value);

signals:
    void valueChanged(int value);

public slots:
    void inc();
    void dec();

private :

    QLCDNumber* _lcdNumber;
    QPushButton* _upButton;
    QPushButton* _downButton;
    int _min;
    int _max;

};

#endif // LCDSPINNER_H
