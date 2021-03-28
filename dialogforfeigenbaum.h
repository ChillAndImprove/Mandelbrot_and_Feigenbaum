#ifndef DIALOGFORFEIGENBAUM_H
#define DIALOGFORFEIGENBAUM_H
#include "model.h"
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QString>
#include <iostream>

class DialogForFeigenbaum:public QDialog
{
public:
    DialogForFeigenbaum(const QString& crTitle, QFormLayout* pLayout, Model** rMod);
// Methoden:
    bool isNumber(QLineEdit* string);
    void deleteLastChar(QLineEdit* string);
    bool isFloat(QLineEdit* string);

// Member:
private:
    Model** mpMod;
    QLineEdit* lineIterationFrom;
    QLineEdit* lineIterationTo;
    QLineEdit* lineStartValue;
    // uebernehme-Button
    QPushButton* btn;

    // aenderungen in dem Model
    // iterationen (k)ab - (m)bis
    unsigned k;
    unsigned m;
    // Startwert
    double startvalue;
};

#endif // DIALOGFORFEIGENBAUM_H
