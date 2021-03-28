#include "view.h"


// View


// Konstruktor
View::View(QImage* pixel, int w, int h){
width = w;
height = h;
setFixedSize(QSize(w, h));
pixels = pixel;
}

// Destruktor
View::~View(){
//std::cout << "Schluss mit View" << std::endl;
}

void View::paintEvent(QPaintEvent*)
{
    // QPainter wird als p deklariert und bezieht sich auf die "View"
    QPainter p(this);
    p.drawImage(0, 0, *pixels);

    if(!feigencon)
    {
        p.setPen(QColor(Qt::white));
        p.drawText(width/2 - 200, height-30, qstrmandel);
    }
    else
    {
        p.setPen(QColor(Qt::black));
        p.drawText(width/2 - 200, height-30, qstrfeige);
    }

    if(bDrawRect)
    {
        p.drawRect(xBeginning, yBeginning, xEnd - xBeginning, yEnd - yBeginning);
    }
}
