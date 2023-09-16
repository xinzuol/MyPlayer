#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QAudioBuffer>
class MyQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyQLabel(QWidget *parent = 0);
    enum GraphicStyle{
    PingPu,
    ZhuZhuangTu,
    ZhengTi
    };
    void drawAudioPxm(const QAudioBuffer &buffer);
    void drawHistogram(QPainter &painter);
    void drawOverallVolatility(QPainter &painter);
    void setGraphicStyle(const GraphicStyle &value);
    void paintEvent(QPaintEvent *ev);
    void drawWaveformFigure(QPainter &painter);
signals:

public slots:
private:
QAudioBuffer m_buffer;
GraphicStyle graphicStyle;
};

#endif // MYQLABEL_H
