#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <iostream>
#include <QMenuBar>
#include "dialogForMandelbrot.h"
#include "dialogforfeigenbaum.h"
#include "mainwindow.h"

namespace Ui {
class Controller;
}

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller();
    Controller( Model** rMod, MainWindow* pWin);
    ~Controller() override;


// Member's
private:
// das Fenster
MainWindow* mpWin;
// Model Fractale
Model** mpMod;
// Mandelbrot-Dialog
DialogForMandelbrot* diaMand;
// Feigenbaum-Dialog
DialogForFeigenbaum* diaFeig;

// Schalter zwischen Strukturen
QAction* pAction1;
// Menu-Eintraege: Einstel. Feigenbaum / Einstel. Mandelbrot
QAction* pAction2;
QAction* pAction3;
};

#endif // CONTROLLER_H
