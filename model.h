#ifndef MODEL_H
#define MODEL_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>

namespace Ui {
class Model;
}


class Model: public QThread
{
    Q_OBJECT
public:
    Model();
    Model(int w, int h, QImage& pix, unsigned threadIndex);
    ~Model() override;

    // laesst die Werte fuer die Strukturen generieren und laedt in QImage rein
    void genNewKoor();

    // finde Mandelbrot(N)
    unsigned findMandelbrot(double cr, double ci, unsigned max_iterations);
    // kartiere zu Realer Zahl
    double mapToReal(int x, int imageWidth, double minR, double maxR);
    // kartiere zur Imaginären Zahl
    double mapToImaginary(int y, int imageHeight, double minI, double maxI);

    // bereinige QImage pixels(bei Feigenbaum notwendig)
    void clear(int, int);

    // run-Methode(Thread's "main-Methode")
    void run() override;

    // Slots/Signals
    public slots:
        void changer();
    signals:
        void dataChanged();

// Member
public:
    // Statt eigenes rgb-Array's(zum Darstellen der Strukturen)
    QImage* pixels;

    // Breite und Hoehe von dem View und Frame
    int width;
    int height;


    // Feigenbaum boolean ob Feigenbaum oder Fractale
    bool feigenConstants = false;


    // Koordinaten Mandelbrot
    double minR = -2.f, maxR = 1.0f, minI = -1.0f, maxI = 1.0f;

    // zum aendern ueber den Dialog
    // Iterationsmaximum Standartwert 100
    unsigned n = 100;
    // rotanteil
    int changeRed = 255;
    // gruenanteil
    int changeGreen = 255;
    // blauanteil
    int changeBlue = 255;
    // Randungskontrast(Randschaerfe) fuer R,G,B
    unsigned nMultValueR = 255;
    unsigned nMultValueG = 1;
    unsigned nMultValueB = 1;


    // Koordinaten Feigenbaum
    // 1, 4, 0, 1
    double minX =  1.f, maxX =  4.f, minY = 0.f, maxY = 1.f;

    // zum aendern ueber den Dialog
    // Definitionsbereich bei dem "gezeichnet" wird
    int k = 600;
    int m = 900;
    // Startwert
    double x_0 = 0.5f;


    // Threads Abteilungen
    // Mandelbrot
    int index_x;
    int index_y;
    int index_width;
    int index_height;
    // Feigenbaum
    int index_x_feigen;
    int index_feigen_width;

    // Thread
    volatile bool running = 1;
    QWaitCondition sleepWait;
    QMutex mutex;
    unsigned index;

    // falls die Threads noch am Arbeiten beim Zoomen und man zoomt, dann restart
    volatile bool restart = 0;
};

#endif // MODEL_H

