#ifndef VIEW_H
#define VIEW_H
#include "model.h"
#include <QWidget>
#include <iostream>
#include <QPainter>

class View : public QWidget
{
public:
    View(QImage *, int w, int h);
    ~View() override;
protected:
void paintEvent(QPaintEvent*) override;

private:
    std::string minRMaxR;
    int width;
    int height;
public:
    // Rechteck des Zoom's
    // Rechteck zum Zoomen
    // klickX und klickY
    int clickX = 0;
    int clickY = 0;
    // klickWidth und klickHeight
    int clickX2 = 0;
    int clickY2 = 0;

    // Rechteck zum Zoomen 2
    int xBeginning = 0;
    int yBeginning = 0;
    int xEnd = 0;
    int yEnd = 0;
    // zeichne Rechteck um den die Fractale groesser werden sollte
    QString qstrfeige;
    QString qstrmandel;
    bool feigencon;
    volatile bool bDrawRect;
    QImage* pixels;


};

#endif // VIEW_H
