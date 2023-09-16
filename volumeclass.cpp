#include "volumeclass.h"

VolumeClass::VolumeClass(QWidget *parent) : QWidget(parent)
{

}

void VolumeClass::mouseMoveEvent(QMouseEvent *event)
{
    this->mousePressEvent (event);

}
