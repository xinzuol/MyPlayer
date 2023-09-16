#ifndef VOLUMECLASS_H
#define VOLUMECLASS_H

#include <QWidget>
#include <QMouseEvent>
class VolumeClass : public QWidget
{
    Q_OBJECT
public:
    explicit VolumeClass(QWidget *parent = 0);
    void mouseMoveEvent (QMouseEvent *event);
signals:

public slots:
};

#endif // VOLUMECLASS_H
