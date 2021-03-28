#include <QObject>
#include "model.h"
#include <iostream>


//Fraktale


//Konstruktor
Model::Model(int w, int h, QImage& pix, unsigned threadIndex)
{   
    start();
    // Zeichenbereich
    pixels = &pix;

    // breite und hoehe setzen
    width = w;
    height = h;

    // Thread-Index
    index = threadIndex;

    // 4 Threads(Mandelbrot)
    // Quadrant 1
    if(threadIndex== 0)
    {
        index_x = 0;
        index_y = 0;
        index_width = w/2;
        index_height = h/2;
    }
    // Quadrant 2
    else if(threadIndex == 1)
    {
        index_x = w/2;
        index_y = 0;
        index_width = w;
        index_height = h/2;
    }
    // Quadrant 3
    else if(threadIndex == 2)
    {
        index_x = 0;
        index_y = h/2;
        index_width = w/2;
        index_height = h;
    }
    // Quadrant 4
    else if(threadIndex == 3)
    {
        index_x = w/2;
        index_y = h/2;
        index_width = w;
        index_height = h;
    }

    // 4 Threads(Feigenbaum)
    index_x_feigen = (threadIndex * static_cast<unsigned>(width))/4;
    index_feigen_width = index_x_feigen + width/4;
}

// Destruktor
Model::~Model(){
    // Thread
    running = false;
    // weckt auf bis nicht jeder der Threads gemerkt das "running = false"
    forever
    {
        if(!QThread::isFinished())
            sleepWait.wakeOne();
        else
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
//  std::cout << "zerstoere Model" << std::endl;
}

// f index c(z) = z ^ 2 + c
// (a + bi) ^ 2 = (a + bi)(a + bi) = a^2 + abi + abi + (bi)^2 = a ^ 2 - b ^ 2 + 2abi
unsigned Model::findMandelbrot(double cr, double ci, unsigned max_iterations)
{
    unsigned i = 0;
    double zr = 0.0f, zi = 0.0f;
    while(i < max_iterations && zr * zr + zi * zi <= 4.0f)
    {
        //                                c
        double temp = zr * zr - zi * zi + cr;
        // 2 * a * b * i
        zi = 2.0f * zr * zi + ci;
        zr = temp;
        ++i;
    }
    return i;
}

// neuen x berechnen:
// newX = newLeft + ((oldX - oldLeft)/(oldRight -  oldLeft))*(newRight - newLeft)
double Model::mapToReal(int x, int imageWidth, double minR, double maxR)
{
    double range = maxR - minR;
    // [0, width]
    // [0, maxR - minR] - val * range / width
    // [minR, maxR] - last step + minR
    //                          0.0f -> oldLeft
    return minR + (range/(imageWidth - 0.0f)) * (x - 0.0f);
}

// neuen y berechnen:
double Model::mapToImaginary(int y, int imageHeight, double minI, double maxI)
{

    double range = maxI - minI;
    //                          0.0f -> oldRight
    return minI + (range/(imageHeight)) * (y - 0.0f);
}

// laesst die Koordinaten berechnen und fuegt bei QImage die Farben ein
void Model::genNewKoor()
{
    if(!feigenConstants)
    {
        // Mandelbrot

        // setze
        unsigned maxN = n;
        for(int y = index_y; y < index_height; y++)
        {
            if(restart)
            {
                y = index_y;
                restart = 0;
             }
             // Pixel in Reihen...
             for(int x = index_x; x < index_width; x++)
             {
                //finde dazugehoerigen Realen- und den Imagineren-Wert, zu dem x, y Pixel des Bildes
                double cr = mapToReal( x, width, minR, maxR);
                double ci = mapToImaginary( y, height, minI, maxI);
                //finde die Anzahl der Iterationen in der Mandelbaumformel unter Verwendung von c
                unsigned n = findMandelbrot(cr, ci, maxN);
                //...kartiere den resultierenden Nummer zu einem R,G,B-Wert
                int r = (static_cast<int>(n * nMultValueR) % changeRed);
                int g = (static_cast<int>(n * nMultValueG) % changeGreen);
                int b = (static_cast<int>(n * nMultValueB) % changeBlue);

                //...fuege der "pixels"-Variable hinzu
                //Schneller als setPixel(evtl. mit Bugs)
                *((QRgb*)pixels->scanLine(y)+x) = qRgb(r,g,b);
            }
        }
    }
    else if(feigenConstants)
    {
        // Feigenbaum

        // bereinige die 4 Thread-Bereiche
        clear(index_x_feigen,index_feigen_width);

            // Standartwerte:
            //x_0 = 0.5 Startwert
            //k ist der Intervall also bis zu 600 und dann einzeichnen
            //m ist 900 das Ende des Intervalls bei dem eingezeichnet werden soll
            double scaleFactorX = (maxX-minX) / width;
            double scaleFactorH = height/ (maxY-minY);

            for(int x = index_x_feigen; x < index_feigen_width;++x)
            {
                if(restart)
                {
                  x = index_x_feigen;
                  restart = 0;
                }
                double lambda = minX + scaleFactorX*x;
                double x_1 = x_0;
                int conversion_y = 0;

                for(int i = 0 ; i < m;++i)
                {
                    x_1 = lambda * x_1 * (1-x_1);
                    conversion_y = static_cast<int>(scaleFactorH * (maxY-x_1));
                    if(height > (conversion_y) && (conversion_y) >= 0 )
                       if(i >= k && i < m)
                       {
                            //...kartiere den resultierenden Nummer
                            //arr[x][conversion_y] = 0xFF000000;
                           *((QRgb*)pixels->scanLine(conversion_y)+x) = 0xFF000000;

                       }
                }
            }
        }
}

// Imagebereiche wieder auf weiss setzen
void Model::clear(int beginning, int end)
{
    for(int i = beginning; i < end; ++i)
        for(int j = 0; j < height; ++j)
            *((QRgb*)pixels->scanLine(j)+i) = 0xFFFFFFFF;
}

void Model::run()
{
    while(running)
    {
        genNewKoor();
        emit dataChanged();
        mutex.lock();
        sleepWait.wait(&mutex);
        mutex.unlock();
    }
}

// Slot -> wechsel zwischen den Strukturen
void Model::changer()
{
    feigenConstants = !feigenConstants;
    sleepWait.wakeOne();
}


