#include "myqlabel.h"


MyQLabel::MyQLabel(QWidget *parent):QLabel(parent)
{
    graphicStyle = GraphicStyle::PingPu;
}

void MyQLabel::drawAudioPxm(const QAudioBuffer &buffer)
{
    m_buffer = buffer;

    update();
}

void MyQLabel::paintEvent(QPaintEvent *ev)
{
    int count = m_buffer.sampleCount() / 2;
    if(count <= 0)
        return;

    QPainter painter(this);
    // 抗锯齿
    painter.setRenderHints(QPainter::Antialiasing);

    if(graphicStyle == GraphicStyle::PingPu )
        drawWaveformFigure(painter);
    else if (graphicStyle == GraphicStyle::ZhuZhuangTu)
        drawHistogram(painter);
    else if (graphicStyle == GraphicStyle::ZhengTi)
        drawOverallVolatility(painter);
}

// 频谱效果
void MyQLabel::drawWaveformFigure(QPainter &painter)
{
    const qint32 *data = m_buffer.constData<qint32>();
    int count = m_buffer.sampleCount() / 2;

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.setColor(Qt::blue);
    painter.setPen(pen);

    float xScale = (float)this->width() / count;
    float center = (float)this->height() / 2;
    for(int i = 1; i < count; i++)
    {
        painter.drawLine(
                    (i - 1) * xScale,
                    center + ((float)(data[i-1]) / INT_MAX * center),
                i * xScale,
                center + ((float)(data[i]) / INT_MAX * center) );

    }
}

//  柱状图效果
void MyQLabel::drawHistogram(QPainter &painter)
{
    const qint32 *data = m_buffer.constData<qint32>();
    int count = m_buffer.sampleCount() / 2;

    int step = count / 50;
    QVector <qreal> vecData;
    for(int i = 0; i < 51; i++)
    {
        int number = 0;
        qreal sum = 0;
        for (int j = 0; j < step; j++)
        {
            if(i * 50 + j >= count)
                break;

            qreal value = (qreal)(data[i * 50 + j]) / INT_MAX * this->height();

            if(value > 0)
            {
                sum += value;
                number++;
            }
        }

        if(number > 0)
            vecData.push_back(sum / number);
        else
            vecData.push_back(0);
    }
    // 图形加间隙的宽度
    qreal rectangleWidth = (qreal)this->width() / vecData.count();
    // 间隙宽度
    qreal clearanceWidth = rectangleWidth / 4;
    // 图形宽度
    qreal shapeWidth = rectangleWidth - clearanceWidth;

    QPen pen = painter.pen();
    pen.setWidth(shapeWidth);
    pen.setColor(Qt::red);
    painter.setPen(pen);

    for(int i = 0; i < vecData.count(); i++)
    {
        painter.drawLine(
                    i * rectangleWidth + rectangleWidth* 0.5,
                    this->height() - vecData[i],
                    i * rectangleWidth + rectangleWidth* 0.5,
                    this->height());
    }
}

// 整体效果
void MyQLabel::drawOverallVolatility(QPainter &painter)
{
    const qint32 *data = m_buffer.constData<qint32>();
    int count = m_buffer.sampleCount() / 2;
    // 取最大值
    qreal maxValue = 0;
    for(int i = 1; i < count; i++)
    {
        qreal value = (qreal)(data[i]) / INT_MAX * this->height();
        maxValue = qMax(value,maxValue);
    }
    // 图形加间隙的宽度
    qreal rectangleWidth = (qreal)this->width() / 10;
    // 间隙宽度
    qreal clearanceWidth = rectangleWidth / 4;
    // 图形宽度
    qreal shapeWidth = rectangleWidth - clearanceWidth;

    for(int i = 0; i < 10; i++)
    {
        QRect rect(i * rectangleWidth + clearanceWidth*0.5, 0, shapeWidth, this->height());

        QLinearGradient gradient(rect.x(), rect.y(),rect.x(), rect.y() + rect.height());
        gradient.setColorAt(0.0, Qt::red);
        gradient.setColorAt(0.3, Qt::green);
        gradient.setColorAt(0.6, Qt::yellow);
        gradient.setColorAt(1.0, Qt::blue);

        painter.setPen(Qt::NoPen);
        gradient.setSpread(QGradient::ReflectSpread);
        // QBrush(const QGradient &gradient)
        painter.setBrush(gradient);
        painter.drawRect(rect);

        // 去掉顶部
        QBrush brush = painter.brush();
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::white);
        painter.setBrush(brush);

        painter.drawRect(i * rectangleWidth + clearanceWidth*0.5, 0, shapeWidth, this->height() - maxValue);
    }
}

void MyQLabel::setGraphicStyle(const GraphicStyle &value)
{
    graphicStyle = value;
}
