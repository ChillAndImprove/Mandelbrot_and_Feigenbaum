#include <QApplication>
#include "mainwindow.h"
#include "controller.h"
#include <QImage>

// die Main-Methode
int main(int argc, char *argv[]) {
QApplication app(argc, argv);

// breite und höhe vom Fenster festlegen
int w = 800;
int h = 600;

// fuer die Farben der Strukturen(Fraktalen)
QImage pixels(QSize(w,h),QImage::Format_ARGB32_Premultiplied);
for(int y = 0; y < h; ++y)
    for (int x = 0; x< w; ++x)
        pixels.setPixel(x,y,0xFF000000);

// Model arr
Model* modArr[4];
modArr[0] = new Model(w, h, pixels, 0);
modArr[1] = new Model(w, h, pixels, 1);
modArr[2] = new Model(w, h, pixels, 2);
modArr[3] = new Model(w, h, pixels, 3);

//Fenster
MainWindow* pWin = new MainWindow(modArr);

//Controller
new Controller(modArr, pWin);

return app.exec();
}
