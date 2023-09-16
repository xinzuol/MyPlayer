#ifndef LYIRCCLASS_H
#define LYIRCCLASS_H

#include <QMap>
#include <QWidget>
class LyircClass : public QWidget
{
    Q_OBJECT
public:
    explicit LyircClass(QWidget *parent = 0);
    bool initLyirc(QString Musicname,int position);
    QMap<qint64,QString>* showlyirc(int positon);
signals:

public slots:
private:
    QMap<qint64,QString> LyircMap;
    QStringList musiclyirc;
};

#endif // LYIRCCLASS_H
