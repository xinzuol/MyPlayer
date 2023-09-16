#include "sliberbar.h"

SliberBar::SliberBar(QWidget *parent) : QSlider(parent)
{

}

void SliberBar::mousePressEvent(QMouseEvent *e)
{
    QSlider::mousePressEvent(e);
    double pos;
    if(width()>height())
    {
        pos = e->pos().x() / (double)width();
        setValue(pos * (maximum() - minimum()) + minimum());
    }
    else{
        pos = e->pos().y() / (double)height();
        setValue(maximum()-pos * (maximum() - minimum()));
    }
    emit SliderClicked();
}

