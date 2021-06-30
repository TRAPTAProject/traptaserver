#include "pdfcreator.h"
#include <QPdfWriter>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QPdfWriter>
#include "utils/util.h"

#define A4_WIDTH 210
#define A4_HEIGHT 297
#define MARGIN 20

// with margin of 20 mm, the page size is 170x257

int PDFCreator::_margin;


PDFCreator::PDFCreator(QObject *parent) :
    QObject(parent)
{
    _margin = MARGIN;
    _pageWidth = 9583;
    _pageHeight = 13699;
    _margin0 = 912;

}


float PDFCreator::w(float mm) {
    return (_pageWidth*mm)/A4_WIDTH;
}


float PDFCreator::h(float mm) {
    return (_pageHeight*mm)/A4_HEIGHT;
}


float PDFCreator::x(float mm) {
    return w(mm+_margin);
}


float PDFCreator::y(float mm) {
    return h(mm+_margin);
}

float PDFCreator::w0(float r) {
    return ((_pageWidth-2*w(_margin))*r)/100;
}


float PDFCreator::h0(float r) {
    return ((_pageHeight-2*h(_margin))*r)/100;
}


float PDFCreator::x0(float r) {

    return w(_margin)+w0(r);
}


float PDFCreator::y0(float r) {

    return h(_margin)+h0(r);

}

bool PDFCreator::create(const QString& filename,
                        const QString& title,
                        const QString& imagePath,
                        QList<Archer *> archerList,
                        int roundCount,
                        bool x10) {

    QPdfWriter printer(filename);
    printer.setPageSize(QPageSize::A4);
    _pageWidth = printer.width();
    _pageHeight = printer.height();
    qDebug() << "Page width=" << _pageWidth;
    qDebug() << "Page height=" << _pageHeight;

    QPainter painter(&printer);
    if (!painter.isActive()) {
        qDebug() << "Couldn't create PDF in file " << filename << ". Maybe this file is locked (already opened) in another application ?";
        return false;
    }
    QFont fontBody;
    fontBody.setPointSize(12);
    QFont fontTitle;
    fontTitle.setPointSize(18);
    QFont fontSig;
    fontSig.setPointSize(6);
    QColor gray(200,200,200);
    QPixmap image(imagePath);
    float targetWidth = w0(25);
    float targetHeight = w0(25);
    float sourceWidth = image.width();
    float sourceHeight = image.height();
    float scale = targetHeight / sourceHeight;
    if (sourceWidth*scale>targetWidth) scale = targetWidth / sourceWidth;
    QString stringCrit1 = "Nb 10";
    QString stringCrit2 = "Nb 9";
    QString stringSum1 = "Total 10";
    QString stringSum2 = "Total 9";
    if (x10) {
        stringCrit1 = "Nb 10+10X";
        stringCrit2 = "Nb 10X";
        stringSum1 = "Total 10+10X";
        stringSum2 = "Total 10X";
    }

    foreach (Archer* archer, archerList) {

        painter.drawPixmap(QRectF(x0(75),y0(0), sourceWidth*scale, sourceHeight*scale),
                           image,
                           QRectF(0,0,sourceWidth, sourceHeight));

        painter.setFont(fontTitle);
        painter.setBrush(QBrush(gray));
        painter.setPen(QPen(Qt::white));
        QRectF rect(x0(0), y0(0), w0(65), h(30));
        painter.drawRoundedRect(rect, 40, 40);
        painter.setPen(QPen(Qt::black));
        QString titleshoot = QString("%0\nDEPART %1").arg(title.trimmed()).arg(archer->shootId());
        painter.drawText(rect, Qt::AlignCenter, titleshoot);

        painter.setFont(fontBody);
        int line = 37;
        painter.drawText(QPointF(x(0), y(line)), Util::targetLabelFromPosition(archer->position()));
        painter.drawText(QPointF(x(0), y(line+6)), archer->name());
        painter.drawText(QPointF(x(0), y(line+12)), archer->license());
        painter.drawText(QPointF(x(0), y(line+18)), archer->noc());
        painter.drawText(QPointF(x(0), y(line+24)), archer->categ());

        line = -10;
        for (int heatIndex=0; heatIndex<roundCount; heatIndex++) {
            if (heatIndex==2) {
                line = -10;
                painter.restore();
                printer.newPage();
            }
            else {
                line+= 74;
            }
            painter.setPen(QPen(Qt::white));
            painter.setBrush(QBrush(Qt::black));
            painter.drawRoundedRect(QRectF(x(0), y(line), w0(100), h(6)),30,30);
            painter.drawText(QRectF(x(0), y(line), w0(50), h(6)), QString("   SÉRIE %0").arg(heatIndex+1));
            painter.setPen(QPen(Qt::black));
            painter.setBrush(QBrush(gray));
            line+=6;
            int volleyId = 0;
            // draw header
            painter.setPen(gray);
            painter.drawRect(QRectF(x0(18), y(line), w0(63), h(6)));
            painter.drawRect(QRectF(x0(9), y(line+6), w0(9), h(60)));
            painter.drawRect(QRectF(x0(70), y(line+6), w0(11), h(66)));
            painter.setPen(QPen(Qt::black));
            painter.drawText(QRectF(x0(18), y(line), w0(63), h(6)), Qt::AlignCenter, "Flèches");
            painter.drawText(QRectF(x0(72), y(line), w0(9), h(6)), Qt::AlignCenter, "Volée");
            painter.drawText(QRectF(x0(81), y(line), w0(9), h(6)), Qt::AlignCenter, "Cumul");
            int volleyCount = 0;
            foreach (Volley volley, archer->scoreCard(heatIndex).volleyList()) {
                volleyId++;
                painter.drawText(QRectF(x0(9), y(line+volleyId*6), w0(9), h(6)), Qt::AlignCenter, QString::number(volleyId));
                int col = 0;
                foreach (int arrow, volley.arrowList()) {
                    col++;
                    painter.drawText(QRectF(x0(9+col*9), y(line+volleyId*6), w0(9), h(6)), Qt::AlignCenter, Volley::arrowLabelFromValue(arrow));
                }
                if (!volley.isVoid()) {
                    volleyCount++;
                    int score = volley.score();
                    int sum = archer->scoreCard(heatIndex).score(volleyId-1);
                    painter.drawText(QRectF(x0(72), y(line+volleyId*6), w0(8), h(6)), Qt::AlignRight, QString::number(score));
                    painter.drawText(QRectF(x0(81), y(line+volleyId*6), w0(9), h(6)), Qt::AlignRight, QString::number(sum));
                }
            }

            painter.drawText(QRectF(x0(10), y(line+66), w0(20), h(6)), Qt::AlignLeft, stringCrit1);
            QRectF rect1(x0(28), y(line+66), w0(8), h(6));
            painter.fillRect(rect1, QBrush(gray));
            painter.drawText(rect1, Qt::AlignCenter, QString::number(archer->scoreCard(heatIndex).criteria1()));
            painter.drawText(QRectF(x0(40), y(line+66), w0(20), h(6)), Qt::AlignLeft, stringCrit2);
            QRectF rect2(x0(52), y(line+66), w0(8), h(6));
            painter.fillRect(rect2, QBrush(gray));
            painter.drawText(rect2, Qt::AlignCenter, QString::number(archer->scoreCard(heatIndex).criteria2()));
            painter.drawText(QRectF(x0(71), y(line+66), w0(10), h(6)), Qt::AlignRight, QString("Série %0:").arg(heatIndex+1));
            painter.drawText(QRectF(x0(81), y(line+66), w0(9), h(6)), Qt::AlignRight, QString::number(archer->scoreCard(heatIndex).score()));
            QPen pen(Qt::black);
            pen.setWidth(h(0.3));
            painter.setPen(pen);
            for (int row=0; row<volleyCount; row++) painter.drawLine(QPointF(x0(9), y(line+6+row*6)), QPointF(x0(90), y(line+6+row*6)));

        }
        painter.setPen(QPen(Qt::black));
        painter.drawText(QRectF(x0(10), y(line+72), w0(20), h(6)), Qt::AlignLeft, stringSum1);
        QRectF rect1(x0(28), y(line+72), w0(8), h(6));
        painter.fillRect(rect1, QBrush(gray));
        painter.drawText(rect1, Qt::AlignCenter, QString::number(archer->criteria1()));
        painter.drawText(QRectF(x0(40), y(line+72), w0(20), h(6)), Qt::AlignLeft, stringSum2);
        QRectF rect2(x0(52), y(line+72), w0(8), h(6));
        painter.fillRect(rect2, QBrush(gray));
        painter.drawText(rect2, Qt::AlignCenter, QString::number(archer->criteria2()));
        painter.drawText(QRectF(x0(20), y(line+72), w0(60), h(6)), Qt::AlignRight, "Total points");
        painter.setPen(QPen(Qt::white));
        painter.setBrush(QBrush(Qt::black));
        painter.drawRect(QRectF(x0(81), y(line+72), w0(10), h(6)));
        painter.drawText(QRectF(x0(81), y(line+72), w0(9), h(6)), Qt::AlignRight, QString::number(archer->score()));
        QPen pen(Qt::black);
        pen.setWidth(h(0.3));
        painter.setPen(pen);
        //painter.drawLine(QPointF(x0(9), y(line+80)), QPointF(x0(90), y(line+80)));
        painter.drawLine(QPointF(x0(9), y(line+72)), QPointF(x0(90), y(line+72)));
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(QRectF(x0(0), y(235), w0(40), h(25)));
        painter.drawRect(QRectF(x0(45), y(235), w0(40), h(25)));
        painter.setFont(fontSig);
        painter.drawText(QRectF(x0(0), y(235), w0(38), h(25)), Qt::AlignRight | Qt::AlignBottom, "Signature ARBITRE");
        painter.drawText(QRectF(x0(45), y(235), w0(38), h(25)), Qt::AlignRight | Qt::AlignBottom, "Signature ARCHER");
        painter.save();
        painter.translate(x0(100), y0(100));
        painter.rotate(-90);
        painter.drawPixmap(0,-h(7),w(7),h(7), QPixmap(":/images/icon.png"));
        painter.drawText(QPointF(w(11), -w(1)), "https://github.com/TRAPTAProject");
        painter.restore();
        printer.newPage();
    }

    return true;
}

