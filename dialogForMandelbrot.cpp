#include "dialogForMandelbrot.h"


// Dialog fuer Mandelbrot


// Konstruktor
DialogForMandelbrot::DialogForMandelbrot(const QString& crTitle, QFormLayout* pLayout, Model** rMod)
{
    setWindowTitle(crTitle);

    mpMod = rMod;
    setModal(true);

    // rgb-Startwerte
    this->r = mpMod[0]->changeRed;
    this->g = mpMod[0]->changeGreen;
    this->b = mpMod[0]->changeBlue;

    // Apfelmaenchen-Slider
    QSlider* sAppleman = new QSlider( Qt::Orientation::Horizontal, this);

    QString s1A = "QSlider::groove:horizontal";
    QString s2A = "{background-color:rgb(";
    QString s3A = QString::number((static_cast<int>(mpMod[0]->n * mpMod[0]->nMultValueR) % mpMod[0]->changeRed));
    QString s4A = ", ";
    QString s5A = QString::number((static_cast<int>(mpMod[0]->n * mpMod[0]->nMultValueG) % mpMod[0]->changeGreen));
    QString s6A = ", ";
    QString s7A = QString::number(((static_cast<int>(mpMod[0]->n * mpMod[0]->nMultValueB) % mpMod[0]->changeBlue)));
    QString s8A = ");";
    QString s9A = "}";
    QString s10A = s1A+s2A+s3A+s4A+s5A+s6A+s7A+s8A+s9A;
    QString final = s10A + "QSlider::handle:horizontal "+s2A+s3A+s4A+s5A+s6A+s7A+s8A+" height:16px; width: 16px;}";

    sAppleman->setStyleSheet(final);
    sAppleman->setEnabled(false);

    // geadded sAppleman
    pLayout->addRow(sAppleman);


    // Iterationen
    lineIteratSize = new QLineEdit(this);
    lineIteratSize->setText(QString::number(mpMod[0]->n));
    this->n = mpMod[0]->n;
    // geadded IteratSize
    pLayout->addRow("Iterationswert",lineIteratSize);
    QObject::connect(lineIteratSize, &QLineEdit::textChanged, [this, sAppleman]()
    {
        if(!isNumber(lineIteratSize))
            deleteLastChar(lineIteratSize);
        if(lineIteratSize->text().isEmpty())
            this->n = this->mpMod[0]->n;
        else
            this->n = (lineIteratSize->text()).toUInt();


        // Apfelmaenchen Farbe
        appleman(sAppleman);
    });


    // Roter-Slider
    QSlider* sRed = new QSlider( Qt::Orientation::Horizontal, this);

    sRed->setValue(mpMod[0]->changeRed);
    sRed->setMaximum(255);
    sRed->setMinimum(1);


    QString s1 = "QSlider::groove:horizontal {background-color:rgb(";

    QString s2 = QString::number(sRed->value());

    QString s3 = ", 0, 0);}";

    QString s4 = s1+s2+s3;

    sRed->setStyleSheet(s4 + "QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");


    // geadded Red
    pLayout->addRow(sRed);
    QObject::connect(sRed, &QSlider::valueChanged, [this, sRed, sAppleman]()
    {
        QString s1 = "QSlider::groove:horizontal {background-color:rgb(";

        QString s2 = QString::number(sRed->value());

        QString s3 = ", 0, 0);}";

        QString s4 = s1+s2+s3;

        sRed->setStyleSheet(s4 + "QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");


        this->r = sRed->value();


        // Apfelmaenchen Farbe
        appleman(sAppleman);
    });

    // Gruener-Slider
    QSlider* sGreen = new QSlider( Qt::Orientation::Horizontal, this);

    sGreen->setValue(mpMod[0]->changeGreen);
    sGreen->setMaximum(255);
    sGreen->setMinimum(1);


    QString s1G = "QSlider::groove:horizontal {background-color:rgb(0, ";

    QString s2G = QString::number(sGreen->value());

    QString s3G = ", 0);}";

    QString s4G = s1G+s2G+s3G;

    sGreen->setStyleSheet(s4G + "QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");


    // geadded Green
    pLayout->addRow(sGreen);
    // connect
    QObject::connect(sGreen, &QSlider::valueChanged, [this, sGreen, sAppleman]()
    {
        QString s1G = "QSlider::groove:horizontal {background-color:rgb(0, ";

        QString s2G = QString::number(sGreen->value());

        QString s3G = ", 0);}";

        QString s4G = s1G+s2G+s3G;

        sGreen->setStyleSheet(s4G + "QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");

        this->g = sGreen->value();

        // Apfelmaenchen Farbe
        appleman(sAppleman);
    });

    // Blauer-Slider
    QSlider* sBlue = new QSlider( Qt::Orientation::Horizontal, this);

    sBlue->setValue(mpMod[0]->changeBlue);
    sBlue->setMaximum(255);
    sBlue->setMinimum(1);


    QString s1B = "QSlider::groove:horizontal {background-color:rgb(0, 0, ";

    QString s2B = QString::number(sBlue->value());

    QString s3B = ");}";

    QString s4B = s1B+s2B+s3B;

    sBlue->setStyleSheet(s4B + "QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");


    // geadded Blue
    pLayout->addRow(sBlue);
    QObject::connect(sBlue, &QSlider::valueChanged, [this, sBlue, sAppleman]()
    {

        QString s1B = "QSlider::groove:horizontal {background-color:rgb(0, 0, ";

        QString s2B = QString::number(sBlue->value());

        QString s3B = ");}";

        QString s4B = s1B+s2B+s3B;

        sBlue->setStyleSheet(s4B + "QSlider::handle:horizontal {background-color:white; height:16px; width: 16px;}");

        this->b = sBlue->value();

        // Apfelmaenchen Farbe
        appleman(sAppleman);
    });


    // Uebernehm-Button
    btn = new QPushButton("Uebernehmen");
    pLayout->addRow(btn);
    QObject::connect(btn, &QPushButton::clicked, [this]()
    {
        for(int i = 0; i < 4; ++i)
        {
            if(!lineIteratSize->text().isEmpty())
                this->mpMod[i]->n = this->n;
            this->mpMod[i]->changeRed = this->r;
            this->mpMod[i]->changeGreen = this->g;
            this->mpMod[i]->changeBlue = this->b;
            this->mpMod[i]->restart = 1;
        }
    });
    for(int i = 0; i < 4; ++i)
    {
        QObject::connect(btn, &QPushButton::clicked,[this, i](){this->mpMod[i]->sleepWait.wakeOne();});
    }
    setLayout(pLayout);
}

bool DialogForMandelbrot::isNumber(QLineEdit* string)
{
    for (int var = 0; var < string->text().size(); ++var)
    {
        if(!string->text().at(var).isDigit() )
            return 0;
    }
    return 1;
}

void DialogForMandelbrot::deleteLastChar(QLineEdit* string)
{
    string->setText(string->text().remove(string->text().size()-1,string->text().size()));

}

void DialogForMandelbrot::appleman(QSlider* sAple)
{
    QString s1A = "QSlider::groove:horizontal";
    QString s2A = "{background-color:rgb(";
    QString s3A = QString::number((static_cast<int>(this->n * mpMod[0]->nMultValueR) % this->r));
    QString s4A = ", ";
    QString s5A = QString::number((static_cast<int>(this->n * mpMod[0]->nMultValueG) % this->g));
    QString s6A = ", ";
    QString s7A = QString::number(((static_cast<int>(this->n * mpMod[0]->nMultValueB) % this->b)));
    QString s8A = ");";
    QString s9A = "}";
    QString s10A = s1A+s2A+s3A+s4A+s5A+s6A+s7A+s8A+s9A;
    QString final = s10A + "QSlider::handle:horizontal "+s2A+s3A+s4A+s5A+s6A+s7A+s8A+" height:16px; width: 16px;}";

    sAple->setStyleSheet(final);
}
