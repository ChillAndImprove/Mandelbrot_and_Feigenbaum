#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMouseEvent>
#include <iostream>
#include "model.h"
#include "view.h"

class MainWindow: public QMainWindow
{
public:
    MainWindow(Model**);
    ~MainWindow() override;
    void setModelVariables();
    // Maus gedrueckt
    void mousePressEvent(QMouseEvent *event) override;
    // Maus losgelassen
    void mouseReleaseEvent(QMouseEvent *event) override;
    // zeichner
    //void paintEvent(QPaintEvent*) override;
    // Maus Event
    void mouseMoveEvent(QMouseEvent *event) override;
    // Scroll Event
    void wheelEvent(QWheelEvent *) override;

public:
    Model** m_pMod;
    View* m_pView;

    // Arrays fuer den Zoom
    double minXArr[1000];
    double minYArr[1000];
    double maxXArr[1000];
    double maxYArr[1000];
    unsigned feigenNextFree = 0;

    double minRArr[1000];
    double minIArr[1000];
    double maxRArr[1000];
    double maxIArr[1000];
    unsigned mandelNextFree = 0;

    bool feigenConstants = false;
    double minX =  1.f ;
    // 1
    double maxX =  4.f ;
    // 4
    double minY = 0.f  ;
    // 1
    double  maxY = 1.f  ;

    //uhrsprung die Daten
    // 0
    double uMinX =  1.f ;
    // 1
    double uMaxX =  4.f ;
    // 4
    double uMinY = 0.f  ;
    // 1
    double  uMaxY = 1.f  ;

    // steps Koordinaten
    double sMinX = 0.f ;
    // 1
    double sMaxX =  0.f ;
    // 4
    double sMinY = 0.f;
    // 1
    double  sMaxY = 0.f  ;
    // zoom-Schritt
    int zoomis = 0;

    // Fraktalenwerte
    double minR = -2.f, maxR = 1.0f, minI = -1.0f, maxI = 1.0f;
    //uhrsprung die Daten
    double uMinR = -2.f, uMaxR = 1.0f, uMinI = -1.0f, uMaxI = 1.0f;
    //steps Koordinaten
    double sMinR = 0.f, sMaxR = 0.f, sMinI = 0.f, sMaxI = 0.f;
    //zoomis2-Schritt
    int zoomis2 = 0;
};

#endif // MAINWINDOW_H
