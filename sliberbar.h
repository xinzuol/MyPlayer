#ifndef SLIBERBAR_H
#define SLIBERBAR_H

#include <QSlider>
#include <QMouseEvent>
class SliberBar : public QSlider
{
    Q_OBJECT
public:
    explicit SliberBar(QWidget *parent = 0);

signals:
//点击信号
void SliderClicked();

public slots:

protected:
    void mousePressEvent (QMouseEvent *e);
//    void mouseReleaseEvent (QMouseEvent *e);
//    void mouseMoveEvent (QMouseEvent *e);
};

#endif // SLIBERBAR_H
