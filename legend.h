#pragma once

#include <QwtLegend>
#include <QwtPlot>

class Legend : public QwtLegend
{
public:
    explicit Legend(QwtPlot *parent)
        : QwtLegend()
        , _parent(parent)
    {
        this->setFont(QFont("Bahnschrift"));
        this->setDefaultItemMode(QwtLegendData::Clickable);
        parent->insertLegend(this);
    }

private:
    QwtPlot *_parent;

};
