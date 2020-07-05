#include "lcdspinner.h"

LCDSpinner::LCDSpinner(int min, int max, QLCDNumber *lcdNumber, QPushButton *upButton, QPushButton *downButton, QObject *parent) :
    _lcdNumber(lcdNumber),
    _upButton(upButton),
    _downButton(downButton),
    _min(min),
    _max(max),
    QObject(parent)
{
    connect(upButton, SIGNAL(clicked()), SLOT(inc()));
    connect(downButton, SIGNAL(clicked()), SLOT(dec()));
}

int LCDSpinner::value() const {
    return (int)(_lcdNumber->value());
}

void LCDSpinner::setValue(int value) {
    _lcdNumber->display(value);
    emit valueChanged(value);
}


void LCDSpinner::inc() {
    int newValue = value()+1;
    if (newValue>_max) newValue = _max;
    setValue(newValue);
}

void LCDSpinner::dec() {
    int newValue = value()-1;
    if (newValue<_min) newValue = _min;
    setValue(newValue);
}
