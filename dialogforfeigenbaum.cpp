#include "dialogforfeigenbaum.h"


// Dialog fuer Feigenbaum


// Konstruktor
DialogForFeigenbaum::DialogForFeigenbaum(const QString& crTitle, QFormLayout* pLayout, Model** rMod)
{
    setWindowTitle(crTitle);

    mpMod = rMod;
    setModal(true);

    // Iterationen zeichnen ab
    lineIterationFrom = new QLineEdit(this);
    lineIterationFrom->setText(QString::number(mpMod[0]->k));
    this->k = static_cast<unsigned>(mpMod[0]->k);
    pLayout->addRow("k",lineIterationFrom);
    QObject::connect(lineIterationFrom, &QLineEdit::textChanged, [this]()
    {
        if(!isNumber(lineIterationFrom))
            deleteLastChar(lineIterationFrom);
        if(lineIterationFrom->text().isEmpty())
            this->k = static_cast<unsigned>(this->mpMod[0]->k);
        else
            this->k = (lineIterationFrom->text()).toUInt();
    });

    // Iterationen zeichnen bis
    lineIterationTo = new QLineEdit(this);
    lineIterationTo->setText(QString::number(mpMod[0]->m));
    this->m = static_cast<unsigned>(mpMod[0]->m);
    pLayout->addRow("m",lineIterationTo);
    QObject::connect(lineIterationTo, &QLineEdit::textChanged, [this]()
    {
        if(!isNumber(lineIterationTo))
            deleteLastChar(lineIterationTo);
        if(lineIterationTo->text().isEmpty()){
            this->m = static_cast<unsigned>(this->mpMod[0]->m);
        }
        else
            this->m = (lineIterationTo->text()).toUInt();
    });

    // Anteil der gruener Farbe
    lineStartValue = new QLineEdit(this);
    lineStartValue->setText(QString::number(static_cast<double>(mpMod[0]->x_0)));
    this->startvalue = static_cast<double>(mpMod[0]->x_0);
    pLayout->addRow("x_0",lineStartValue);
    QObject::connect(lineStartValue, &QLineEdit::textChanged, [this]()
    {
        if(!isFloat(lineStartValue))
            deleteLastChar(lineStartValue);
        if(lineStartValue->text().isEmpty())
            this->startvalue = static_cast<double>(this->mpMod[0]->x_0);
        else
            this->startvalue = (lineStartValue->text()).toFloat();
    });

    // Uebernehm-Button
    btn = new QPushButton("Uebernehmen");
    pLayout->addRow(btn);
    QObject::connect(btn, &QPushButton::clicked, [this]()
    {
        for(int i = 0; i < 4; ++i)
        {
            if(!lineIterationFrom->text().isEmpty())
                this->mpMod[i]->k = static_cast<int>(this->k);
            if(!lineIterationTo->text().isEmpty())
                this->mpMod[i]->m = static_cast<int>(this->m);
            if(!lineStartValue->text().isEmpty())
                this->mpMod[i]->x_0 = static_cast<double>(this->startvalue);
        }
    });
    for(int i = 0; i < 4; ++i)
    {
        QObject::connect(btn, &QPushButton::clicked,[this, i](){this->mpMod[i]->sleepWait.wakeOne();});
    }
    setLayout(pLayout);
}

// ist eine Nimmer
bool DialogForFeigenbaum::isNumber(QLineEdit* string)
{
    for (int var = 0; var < string->text().size(); ++var)
    {
        if(!string->text().at(var).isDigit() )
            return 0;
    }
    return 1;
}

// loesche Symbol
void DialogForFeigenbaum::deleteLastChar(QLineEdit* string)
{
    string->setText(string->text().remove(string->text().size()-1,string->text().size()));

}

// ist Fliesskommazahl
bool DialogForFeigenbaum::isFloat(QLineEdit* string)
{

    bool point = 0;
    for (int var = 0; var < string->text().size(); ++var)
    {
        if(!string->text().at(var).isDigit() && (string->text().at(var) != '.') ^ ( point != 0))
            return 0;
        if((string->text().at(var) == '.'))
            point = 1;
    }
    return 1;
}


