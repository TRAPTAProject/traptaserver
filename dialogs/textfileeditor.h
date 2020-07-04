#ifndef TEXTFILEEDITOR_H
#define TEXTFILEEDITOR_H

#include <QDialog>

namespace Ui {
class TextFileEditor;
}

class TextFileEditor : public QDialog
{
    Q_OBJECT

public:
    explicit TextFileEditor(const QString& defaultFilename, const QString& filename, const QString& title, const QString &label, QWidget *parent = 0);
    ~TextFileEditor();

private slots:
    void processSaveButton();
    void processDefaultValues();

private:
    Ui::TextFileEditor *ui;

    QString _filename;
    QString _defaultFilename;
};

#endif // TEXTFILEEDITOR_H
