#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include <QObject>
#include "archers/archer.h"

class PDFCreator : public QObject {
    Q_OBJECT
public:
    explicit PDFCreator(QObject *parent = 0);
    bool create(const QString& filename,
                const QString &title,
                const QString &imagePath,
                QList<Archer*> archerList,
                int roundCount,
                bool x10 = false);

signals:

public slots:

private:

    static int _margin;

    float _pageWidth;
    float _pageHeight;
    float _margin0;
    float w(float mm);
    float h(float mm);
    float x(float mm);
    float y(float mm);
    float w0(float pc);
    float h0(float pc);
    float x0(float pc);
    float y0(float pc);


};

#endif // PDFCREATOR_H
