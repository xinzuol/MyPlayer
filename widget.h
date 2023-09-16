#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <pthread.h>
#include <QSlider>
#include <QMouseEvent>
#include "lyircclass.h"
#include <QPainter>
#include <myqlabel.h>
class QAudioProbe;
class QMediaPlayer;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    protected:
    enum playPattern{
     LIST = 0,
     RANDOM,
     CYCLE
};
private slots:
    void on_MusicFile_clicked();

    void on_MusicStop_clicked();

    void on_MusicBack_clicked();

    void on_MusicSpeed_clicked();

    void on_MusicList_doubleClicked(const QModelIndex &index);
    //进度条拖动
    void updatePosition(qint64 position);
    //进度条时常
    void updateDuration(qint64 duration);
    //进度条拖动
    void setPosition(qint64 position);
    //音量
    void setVolumeber(int volume);
    //音量键
    void on_MusicVolume_clicked();
    //进度条跳转
    void sliderClicked();

    void on_MusicLyric_clicked();

    void on_MusicFile_2_clicked();
    //歌词
    void setLyircShow(qint64 position);
    //模式
    void on_playset_clicked();
    //歌曲信息
    void MusicInformation(qint64 duration);
    //背景
    void paintEvent(QPaintEvent *e);

    void processBuffer(const QAudioBuffer &buffer);
private:
  Ui::Widget *ui;
    //歌曲地址列表
    QList<QUrl> playlist;
    //播放
    QMediaPlayer *player;
    //播放歌曲下表
    int playingIndex;
    //总时常
    qint64 duration;
    //歌词
    LyircClass *lyirc;
    //歌名
    QStringList MusicnameList;
    //模式
    playPattern pattern;
    //顺序
    QVector<int> order;
    //音频
    QAudioProbe *probe;
};
#endif // WIDGET_H
