#include "mylrc.h"
#include <QtGui>
MyLrc::MyLrc(QWidget *parent) : QLabel(parent)
{
//设置窗体                         无边框                       置顶
setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
setAttribute(Qt::WA_TranslucentBackground);        //背景透明
//还没播的
    linearGradient.setStart(0, 10);
    linearGradient.setFinalStop(0, 40);
    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
    linearGradient.setColorAt(0.5, QColor(114, 32, 255));
    linearGradient.setColorAt(0.9, QColor(14, 179, 255));

    //已经播过的
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

}
//绘图事件
void MyLrc::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setFont(myFont);　　//如果绘图的画家指定字体的话，那么该类也应该设置字体，这样用adjustSize()函数后宽度和高度才是正确的
    //绘制底层黑色阴影图层
    painter.setPen(Qt::black);
    painter.drawText(1, 1, this->width(), this->height(), Qt::AlignLeft, text());
    //绘制未播放图层
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, this->width(), this->height(), Qt::AlignLeft, text());
    //绘制已播放图层
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, drawWidth, this->height(), Qt::AlignLeft, text());
}
//鼠标点击事件
void MyLrc::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        movePointData.setX(e->pos().x());
        movePointData.setY(e->pos().y());
    }
}

//鼠标移动事件
void MyLrc::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        this->move(e->globalPos() - movePointData);
    }
}

//鼠标松开事件　　　　这个是保证歌词永远完整的在屏幕显示
void MyLrc::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(this->x() < 0)
            this->setGeometry(0, this->y(), this->width(), this->height());
        if(this->y() < 0)
            this->setGeometry(this->x(), 0, this->width(), this->height());
        if(this->x() + this->width() > QApplication::desktop()->screenGeometry().width())
            this->setGeometry(QApplication::desktop()->screenGeometry().width() - this->width()
                              , this->y(), this->width(), this->height());
        if(this->y() + this->height() > QApplication::desktop()->screenGeometry().height())
            this->setGeometry(this->x(), QApplication::desktop()->screenGeometry().height() - this->height(),
                              this->width(), this->height());
    }
}
