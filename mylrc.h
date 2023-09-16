#ifndef MYLRC_H
#define MYLRC_H

#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
class MyLrc : public QLabel
{
    Q_OBJECT
public:
    explicit MyLrc(QWidget *parent = 0);
    void MyLrc::paintEvent (QPaintEvent *);
    void MyLrc::mousePressEvent (QMouseEvent *ev);
    void MyLrc::mouseMoveEvent (QMouseEvent *ev);
    void MyLrc::mouseReleaseEvent (QMouseEvent *ev);
signals:

public slots:
};

#endif // MYLRC_H
