#ifndef PDFDIALOG_H
#define PDFDIALOG_H

#include <QDialog>

namespace Ui {
class PDFDialog;
}

class PDFDialog : public QDialog {
    Q_OBJECT

public:
    explicit PDFDialog(QWidget *parent = 0);
    ~PDFDialog();

    void setTitle(const QString& title);
    void setImagePath(const QString& imagePath, int width=226, int height=130);
    QString getTitle() const;
    QString getImagePath() const;

private slots:
    void onVoidImageClick();
    void onLoadImageClick();

private:
    Ui::PDFDialog *ui;

    QString _imagePath;

};

#endif // PDFDIALOG_H
