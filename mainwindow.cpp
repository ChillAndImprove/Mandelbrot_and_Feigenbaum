#include "mainwindow.h"


// Das Fenster


// Konstruktor
MainWindow::MainWindow(Model** rMod){
m_pMod = rMod;

setWindowTitle("Strukturen");
setAttribute(Qt::WA_DeleteOnClose);
setFixedSize(QSize(rMod[0]->width, rMod[0]->height));

// Die View initialisieren etc.
m_pView = new View(rMod[0]->pixels, rMod[0]->width,rMod[0]->height);
// Strings in View veraendern
m_pView->qstrmandel = "imagin.: -1.0f <= y <= 1.0f                    reel.: -2.f <= x <= 1.0f";
m_pView->qstrfeige = "y-Achse: " + QString::number(static_cast<double>(m_pMod[0]->minY)) + " <= x <= " +
        QString::number(static_cast<double>(m_pMod[0]->maxY)) + "                    " + "x-Achse: " +
        QString::number(static_cast<double>(m_pMod[0]->minX)) + " <= r <= " +
        QString::number(static_cast<double>(m_pMod[0]->maxX));
// feigencon von der View initialisieren
m_pView->feigencon = 0;

setCentralWidget(m_pView);
show();
}

// Destruktor
MainWindow::~MainWindow()
{
    for(int i = 0; i < 4; ++i)
    {
        delete m_pMod[i];
    }
    //std::cout << "Schluss mit MainWindow" << std::endl;
}

// Maus gedrueckt
void MainWindow::mousePressEvent(QMouseEvent * event)
{
    m_pView->xBeginning = event->pos().rx();
    m_pView->yBeginning = event->pos().ry() - 20;

    update();
}

// setze die Koordiantenvariable min/max neu
void MainWindow::setModelVariables()
{
    int modelnr = 0;
    if(!feigenConstants)
    {
         // Zoom-Schritt, wird erhoeht, wenn Zoom-Rechteck gezoomt
        minIArr[mandelNextFree] = m_pMod[0]->minI;
        minRArr[mandelNextFree] = m_pMod[0]->minR;
        maxIArr[mandelNextFree] = m_pMod[0]->maxI;
        maxRArr[mandelNextFree] = m_pMod[0]->maxR;
        mandelNextFree++;

         // Mandelbrot
         minR = m_pMod[modelnr]->mapToReal(m_pView->xBeginning, m_pMod[modelnr]->width, minR, maxR);
         minI = m_pMod[modelnr]->mapToImaginary(m_pView->yBeginning, m_pMod[modelnr]->height, minI, maxI);
         maxR = m_pMod[modelnr]->mapToReal(m_pView->xEnd,m_pMod[modelnr]->width,minR,maxR);
         maxI = m_pMod[modelnr]->mapToImaginary(m_pView->yEnd, m_pMod[modelnr]->height, minI, maxI);

         for(int i = 0; i < 4; ++i)
         {
             m_pMod[i]->minR = minR;
             m_pMod[i]->minI = minI;
             m_pMod[i]->maxR = maxR;
             m_pMod[i]->maxI = maxI;
         }

         // String in View veraendern
         m_pView->qstrmandel = "imagin.: " + QString::number(static_cast<double>(m_pMod[0]->minI)) + " <= y <= " +
                 QString::number(static_cast<double>(m_pMod[0]->maxI)) + "                    " +
                 "reel.: " + QString::number(static_cast<double>(m_pMod[0]->minR)) +
                 " <= x <= " + QString::number(static_cast<double>(m_pMod[0]->maxR));

    }
    else
    {
        // Zoom-Schritt, wird erhoeht, wenn Zoom-Rechteck Zoomt
        minXArr[feigenNextFree] = m_pMod[0]->minX;
        minYArr[feigenNextFree] = m_pMod[0]->minY;
        maxXArr[feigenNextFree] = m_pMod[0]->maxX;
        maxYArr[feigenNextFree] = m_pMod[0]->maxY;
        feigenNextFree++;

        // Feigenbaum
        double tmp = minX;
        double tmp2 =minY;
        minX  =  minX + (static_cast<double>(m_pView->xBeginning)/static_cast<double>(m_pMod[modelnr]->width))* (maxX-minX);
        minY  =  maxY - (static_cast<double>(m_pView->yEnd)/static_cast<double>(m_pMod[modelnr]->height)) * (maxY-minY);
        maxX  =  tmp  + (static_cast<double>(m_pView->xEnd)/static_cast<double>(m_pMod[modelnr]->width)) * (maxX-tmp);
        maxY  =  maxY - (static_cast<double>(m_pView->yBeginning)/static_cast<double>(m_pMod[modelnr]->height)) *  (maxY-tmp2);

        // Modele befuehlen
        for(int i = 0; i < 4; ++i)
        {
            m_pMod[i]->minX = minX;
            m_pMod[i]->minY = minY;
            m_pMod[i]->maxX = maxX;
            m_pMod[i]->maxY = maxY;
        }

        // String in View veraendern
        m_pView->qstrfeige = "y-Achse: " + QString::number(static_cast<double>(m_pMod[0]->minY)) +
                " <= x <= " + QString::number(static_cast<double>(m_pMod[0]->maxY)) +
                "                    " + "x-Achse: " + QString::number(static_cast<double>(m_pMod[0]->minX)) +
                " <= r <= " + QString::number(static_cast<double>(m_pMod[0]->maxX));

    }
}

// Maus losgelassen
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{   
    m_pView->xEnd = event->pos().rx();
    m_pView->yEnd = event->pos().ry() - 20;

    if(m_pView->xBeginning > m_pView->xEnd)
    {
        int tmp = m_pView->xEnd;
        m_pView->xEnd = m_pView->xBeginning;
        m_pView->xBeginning = tmp;
    }
    if(m_pView->yBeginning > m_pView->yEnd)
    {
        int tmp = m_pView->yEnd;
        m_pView->yEnd = m_pView->yBeginning;
        m_pView->yBeginning = tmp;
    }

    // neu berechnen Zoom zustand
    setModelVariables();

    // zeichne Rechteck nicht mehr
    m_pView->bDrawRect = false;

    for (int i = 0; i < 4; ++i)
    {
        m_pMod[i]->sleepWait.wakeOne();
        m_pMod[i]->restart=1;

    }
}

// Maus bewegt
// zum zeichnen von dem Rechteck(fuer das Zoomen)
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    m_pView->xEnd = event->pos().rx();
    m_pView->yEnd = event->pos().ry() - 20;
    m_pView->bDrawRect = true;
    update();
}

// Scroll-Event
// fuer das rauszoomen
void MainWindow::wheelEvent(QWheelEvent *)
{
    if(feigenConstants)
    {
        // setze und weke die Model-Threads
        if(feigenNextFree > 0)
        {
               feigenNextFree--;
               minX = minXArr[feigenNextFree];
               maxX = maxXArr[feigenNextFree];
               minY = minYArr[feigenNextFree];
               maxY = maxYArr[feigenNextFree];
            for(int i = 0; i < 4; ++i)
            {
                m_pMod[i]->minX = minXArr[feigenNextFree];
                m_pMod[i]->minY = minYArr[feigenNextFree];
                m_pMod[i]->maxX = maxXArr[feigenNextFree];
                m_pMod[i]->maxY = maxYArr[feigenNextFree];
                m_pMod[i]->sleepWait.wakeOne();
                m_pMod[i]->restart = 1;
            }

        }
         // String in View veraendern
         m_pView->qstrfeige = "y-Achse: " + QString::number(static_cast<double>(m_pMod[0]->minY)) +
                 " <= x <= " + QString::number(static_cast<double>(m_pMod[0]->maxY)) + "                    " +
                 "x-Achse: " + QString::number(static_cast<double>(m_pMod[0]->minX)) +
                 " <= r <= " + QString::number(static_cast<double>(m_pMod[0]->maxX));

    }
    else
    {
        if(mandelNextFree > 0)
        {
            mandelNextFree--;

            minR = minRArr[mandelNextFree];
            maxR = maxRArr[mandelNextFree];
            minI = minIArr[mandelNextFree];
            maxI = maxIArr[mandelNextFree];
            // setze und weke die Model-Threads
            for(int i = 0; i < 4; ++i)
            {
                m_pMod[i]->minR = minRArr[mandelNextFree];
                m_pMod[i]->minI = minIArr[mandelNextFree];
                m_pMod[i]->maxR = maxRArr[mandelNextFree];
                m_pMod[i]->maxI = maxIArr[mandelNextFree];
                m_pMod[i]->sleepWait.wakeOne();
                m_pMod[i]->restart = 1;
            }
        }
         // String in View veraendern
         m_pView->qstrmandel = "imagin.: " + QString::number(static_cast<double>(m_pMod[0]->minI)) +
                 " <= y <= " + QString::number(static_cast<double>(m_pMod[0]->maxI)) +"                    " +
                 "reel.: " + QString::number(static_cast<double>(m_pMod[0]->minR)) +
                 " <= x <= " + QString::number(static_cast<double>(m_pMod[0]->maxR));

    }
}

