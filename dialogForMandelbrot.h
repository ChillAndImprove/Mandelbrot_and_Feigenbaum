#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include <model.h>
#include <QFormLayout>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include <iostream>
#include <QString>

class DialogForMandelbrot:public QDialog
{
public:
    DialogForMandelbrot(const QString& crTitle,QFormLayout* pLayout, Model** rMod);
// Metthoden:
    bool isNumber(QLineEdit* string);
    void deleteLastChar(QLineEdit* string);
    void appleman(QSlider*);

// Member:
private:
    Model** mpMod;
    QLineEdit* lineIteratSize;
    QLineEdit* lineR;
    QLineEdit* lineG;
    QLineEdit* lineB;
    // uebernehme-Button
    QPushButton* btn;

    // aenderungen in dem Model
    // iterationen
    unsigned n;
    // farben
    int r;
    int g;
    int b;
};

#endif // DIALOG_H
