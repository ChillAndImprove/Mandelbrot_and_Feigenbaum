#include "controller.h"


// Controller


// Konstruktor
Controller::Controller( Model** rMod, MainWindow* pWin)
{
mpMod = rMod;
mpWin = pWin;

// Dialog fuer der Mandelbrot:
diaMand = new DialogForMandelbrot("Mandelbrot",new QFormLayout(), mpMod);

// Dialog fuer das Feigenbaum:
diaFeig = new DialogForFeigenbaum("Feigenbaum", new QFormLayout(), mpMod);

// Menu:
QMenu* pMenuOptions = pWin->menuBar()->addMenu("Einstellungen");

// 3 - Menueeintraege:
// 1 - Schalter zwischen den Strukturen:
pAction1 = pMenuOptions->addAction("Mandelbrot/Feigenbaum");
// Strg + c (nehme ich an) TODO: ueberpruefen
pAction1->setShortcut(QKeySequence::Copy);
// wechsel zwischen Feigenbaum und Mandelbrot in den Model-Threads
QObject::connect(pAction1, &QAction::triggered, [this]()
{
    // wechsel zwischen Feigenbaum und Mandelbrot im Fenster und dem View
    this->mpWin->feigenConstants = !this->mpWin->feigenConstants;
    this->mpWin->m_pView->feigencon = !this->mpWin->m_pView->feigencon;

    // Wenn Feigenbaum im Fenster gezeichnet, dann sollte Menueeintrag "this->pAction3->setEnabled(true)" sein
    // und beim "this->pAction2->setEnabled(false)"
    if(this->mpWin->feigenConstants == true)
    {
        this->pAction2->setEnabled(false);
        this->pAction3->setEnabled(true);
    }
    else
    {
        this->pAction2->setEnabled(true);
        this->pAction3->setEnabled(false);
    }
});
for(int i = 0; i < 4; ++i)
    QObject::connect(pAction1, &QAction::triggered, mpMod[i], &Model::changer);
//separotor
pMenuOptions->addSeparator();
// 2 - Einstellungen Mandelbrot:
pAction2 = pMenuOptions->addAction("Einstel. Mandelbrot");
// Strg + x (nehme ich an) TODO: ueberpruefen
pAction2->setShortcut(QKeySequence::Cut);
// Zeige Fractalen-Dialog an
QObject::connect(pAction2, &QAction::triggered, diaMand, &DialogForMandelbrot::show);
// 3 - Einstellungen Feigenbaum:
pAction3 = pMenuOptions->addAction("Einstel. Feigenbaum");
// Strg + n (nehme ich an) TODO: ueberpruefen
pAction3->setShortcut(QKeySequence::New);
// Zeige Feigen-Dialog an
QObject::connect(pAction3, &QAction::triggered, diaFeig, &DialogForFeigenbaum::show);
// am anfang "enanable(false)"
pAction3->setEnabled(false);


// update-Connect's:
for (int var = 0; var < 4; ++var)
    QObject::connect(mpMod[var],&Model::dataChanged,pWin,static_cast<void (QMainWindow::*)()>(&QMainWindow::update));
}

// Destruktor
Controller::~Controller()
{
//    std::cout << "Schluss mit Controller" << std::endl;
}
