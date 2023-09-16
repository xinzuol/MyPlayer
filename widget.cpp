#include "widget.h"
#include "ui_widget.h"
#include "unistd.h"
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#define MUSIC "D:\\Qt\\Qtproject\\MusicProje_1\\MusicPlayer"
#define PICTURE ":/src/Picture/"
#include <QTime>
#include <random>
#include <algorithm>
#include <QPropertyAnimation>
#include <QMediaMetaData>
#include <QAudioProbe>
#include <QAudioDecoder>
Widget::Widget(QWidget *parent) : QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //    mkfifo(FIFO_CMD, 0666);
    player = new QMediaPlayer(this);
    //标题
    this->setWindowTitle ("MusicPlayer");
    //初始化歌词
    lyirc = new LyircClass;
    //音乐结束切换
    connect(player, &QMediaPlayer::positionChanged, this, [=](qint64 position)
    {
        //单曲循环
        if(pattern==CYCLE)
        {
            ui->Progressbar->setValue (0);
            player->play ();
        }
        else if (position==duration&&position!=0)
        {
            emit on_MusicSpeed_clicked();
        } });
    //音乐变化 时间
    connect(player,&QMediaPlayer::positionChanged,this ,&updatePosition);
    //歌词变化 时间
    connect(player,&QMediaPlayer::positionChanged,this ,&setLyircShow);
    //音乐总持续时间
    connect(player,&QMediaPlayer::durationChanged,this,&updateDuration);
    //进度条拖动
    connect(ui->Progressbar,&ui->Progressbar->sliderMoved,this,&setPosition);
    //进度条跳转
    connect(ui->Progressbar,&SliberBar::SliderClicked,this,&sliderClicked);
    //音量控制
    connect(ui->Volumebar,&SliberBar::SliderClicked,this,[=](){
        emit setVolumeber (ui->Volumebar->value ());
    });
    //音量拖动
    connect(ui->Volumebar,&SliberBar::valueChanged,this,[=](qint64 position){
        player->setVolume (position);
        ui->Volumevalue->setText (QString::number (position)+"%");
    });
    //读取音乐信息
    connect(player,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
        emit MusicInformation(duration);
        ui->MusicStop->setText ("暂停");
        qInfo()<<probe->setSource(player);
    });
    //选择音乐源
    player->setMedia(QUrl::fromLocalFile(MUSIC));
    //设置音量
    player->setVolume(20);
    //音量隐藏
    ui->Volumebar->hide ();
    ui->Volumevalue->hide ();
    //初始化选择
    playingIndex=0;
    //初始化音量
    ui->Volumebar->setRange(0,100);
    ui->Volumebar->setPageStep(1);
    ui->Volumebar->setValue (player->volume ());
    ui->Volumevalue->setText (QString::number(player->volume ())+"%");
    //初始化模式
    pattern = LIST;
    //设置界面固定
    this->setFixedSize(500,550);
    //设置控制栏背景透明
    this->ui->widget->setObjectName("wid");
    this->ui->widget->setStyleSheet("QWidget#wid{background-color: rgba(175, 200, 225,0.5);}");
    //隐藏滚动条
    ui->MusicList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->MusicList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //初始化按钮
    ui->MusicStop->setText ("播放");
    //添加图标
    this->setWindowIcon (QIcon("://src/Icon/IconPicture.png"));
    // 音频获取
    probe = new QAudioProbe;
    qInfo()<<probe->setSource(player);
    connect(probe, &QAudioProbe::audioBufferProbed, this,&processBuffer);
    connect(player,&QMediaPlayer::mediaStatusChanged,[=](QMediaPlayer::MediaStatus state){
        if(state==QMediaPlayer::BufferedMedia)
            qInfo()<<"缓冲#107";
        else
            qInfo()<<state;
    });
    //    probe->record()
}

Widget::~Widget()
{
    delete ui;
}
//选择文件
void Widget::on_MusicFile_clicked()
{

    ui->MusicList->show();
    //获取目录所有歌曲
    auto path = QFileDialog::getExistingDirectory (this,"请选择音乐所在目录",MUSIC);
    QDir dir(path);
    //对文件内容进行筛选
    auto musicList = dir.entryList (QStringList()<<"*.mp3"<<"*.wav"<<"*.ogg"<<"*.flac");
    //音乐放到显示列表中
    if(musicList.size ()==0)
        return;
    //插入顺序
    int i=0;
    for(auto file: musicList)
    {
        //如果不存在则添加
        if(MusicnameList.indexOf (file)==-1||MusicnameList.size ()==0)
        {
            //将文件歌曲读取到文件列表
            ui->MusicList->addItem (file);
            MusicnameList.append (file);
            playlist.append (QUrl::fromLocalFile (path+"\\"+file));
            order.append (i++);
        }
    }
    //默认选中第一个
    ui->MusicList->setCurrentRow (0);
}
//播放/暂停
void Widget::on_MusicStop_clicked()
{
    if(!playlist.size ())
        return;
    switch (player->state ()) {
    //停止状态
    case QMediaPlayer::State::StoppedState:
    {
        //播放当前选中音乐
        playingIndex = ui->MusicList->currentRow ();
        //播放对应下标
        player->setMedia (playlist[playingIndex]);
        player->setVolume(player->volume ());
        player->play ();
        //歌曲名选择歌词
        lyirc->initLyirc(MusicnameList.at (playingIndex),0);
        break;
    }
        //播放状态
    case QMediaPlayer::State::PlayingState:
    {
        ui->MusicStop->setText ("播放");
        player->pause ();

        break;
    }
        //暂停状态
    case QMediaPlayer::State::PausedState:
    {
        ui->MusicStop->setText ("暂停");
        player->play ();
        break;
    }
    default:
        break;
    }
}
//上一曲
void Widget::on_MusicBack_clicked()
{
    //内容为空时
    if(playlist.size ()==0)
        return;
    //防止越界
    playingIndex=(--playingIndex)% playlist.size ();
    if(playingIndex<0)
        playingIndex=playlist.size ()-1;
    //选定下一首
    ui->MusicList->setCurrentRow (order.at (playingIndex));
    //播放下一曲
    player->setMedia (playlist[order.at (playingIndex)]);
    player->play ();
    //歌曲名选择歌词
    lyirc->initLyirc(MusicnameList.at (order.at (playingIndex)),0);
}
//下一曲
void Widget::on_MusicSpeed_clicked()
{
    if(playlist.size ()==0)
        return;
    //防止越界
    playingIndex=(++playingIndex)% playlist.size ();
    //播放下一曲
    ui->MusicList->setCurrentRow (order.at (playingIndex));
    player->setMedia (playlist[order.at (playingIndex)]);
    player->play ();
    //歌曲名选择歌词
    lyirc->initLyirc(MusicnameList.at (order.at (playingIndex)),0);
}

//双击切歌
void Widget::on_MusicList_doubleClicked(const QModelIndex &index)
{
    //双击播放指定歌曲
    playingIndex=index.row ();
    player->setMedia (playlist[playingIndex]);
    player->play ();
    lyirc->initLyirc(MusicnameList.at (playingIndex),0);
}
//进度条根据音乐变化
void Widget::updatePosition(qint64 position)
{
    ui->Progressbar->setValue (position);
    //显示时常
    QTime times(0, position / 60000, qRound((position % 60000) / 1000.0));
    ui->time->setText (times.toString (tr("mm:ss")));

}
//进度条总时长初始化
void Widget::updateDuration(qint64 duration)
{
    this->duration=duration;
    ui->Progressbar->setRange (0,duration);
    ui->Progressbar->setEnabled (duration>0);
    //设置步长
    ui->Progressbar->setPageStep (duration/10);
    //显示时常
    QTime times(0, duration / 60000, qRound((duration % 60000) / 1000.0));
    ui->timeAll->setText (times.toString (tr("mm:ss")));
}
//进度条拖动
void Widget::setPosition(qint64 position)
{
    if(qAbs(player->position ()-position)>99)
        player->setPosition (position);
}
//控制音量
void Widget::setVolumeber(int volume)
{
    player->setVolume (volume);
    ui->Volumevalue->setText (QString::number (volume)+"%");
}
//音量显示
void Widget::on_MusicVolume_clicked()
{
    //点击显示再点击隐藏
    if(ui->Volumebar->isVisible ())
    {
        ui->Volumebar->hide ();
        ui->Volumevalue->hide ();
    }
    else
    {
        ui->Volumebar->show ();
        ui->Volumevalue->show ();
    }
}
//进度条跳转
void Widget::sliderClicked()
{
    player->setPosition (ui->Progressbar->value());
    player->play ();
}
//显示歌词列表
void Widget::on_MusicLyric_clicked()
{
    ui->MusicList->hide ();

}
//显示歌单
void Widget::on_MusicFile_2_clicked()
{
    ui->MusicList->show ();
}
//显示歌词
void Widget::setLyircShow(qint64 position)
{
    //返回map容器
    auto lyircmap = lyirc->showlyirc (1);
    //判断歌曲歌词是否存在
    if(lyircmap->size ()<10)
    {
        ui->Lyric_3->setText ("歌词不存在");
        ui->Lyric_4->setText ("");
        ui->Lyric_5->setText ("");
        ui->Lyric_6->setText ("");
        return;
    }
    if(lyircmap->lastKey ()<position)
    {
        return;
    }

    QList<QString> showlyirc;
    //it表示当前歌词
    auto it = lyircmap->lowerBound (position);
    //歌词链表
    --it;
    showlyirc.insert (1,(--it).value ());
    showlyirc.insert (0,(--it).value ());
    it++;
    showlyirc.insert (2,(++it).value ());
    showlyirc.insert (3,(++it).value ());
    showlyirc.insert (4,(++it).value ());
    showlyirc.insert (5,(++it).value ());
    //歌词插入
    ui->Lyric_1->setText (showlyirc.at (0));
    ui->Lyric_2->setText (showlyirc.at (1));
    ui->Lyric_3->setText (showlyirc.at (2));
    ui->Lyric_4->setText (showlyirc.at (3));
    ui->Lyric_5->setText (showlyirc.at (4));
    ui->Lyric_6->setText (showlyirc.at (5));
    //歌词样式
    ui->Lyric_3->setStyleSheet("QLabel { font: 25 10pt \"Microsoft YaHei\";\
                               border-radius: 8px; \
            background-color: argb(255,244, 162, 121); \
color: rgb(255, 255, 255);text-align: center;}");
}

//切换模式
void Widget::on_playset_clicked()
{
    if(order.size ()==0)
        return;
    //切换随机
    switch (pattern) {
    case LIST:
    {
        pattern=RANDOM;
        ui->playset->setText ("随机");
        std::random_device rd;
        std::mt19937 eng(rd());
        std::shuffle(order.begin (),order.end (),eng);
        break;
    }
    case RANDOM:
    {
        pattern=CYCLE;
        ui->playset->setText ("单曲");
        playingIndex=order.at (playingIndex);
        std::sort(order.begin (),order.end ());
        break;
    }
    case CYCLE:
    {
        pattern=LIST;
        ui->playset->setText ("列表");
        playingIndex=order.at (playingIndex);
        std::sort(order.begin (),order.end ());
        break;
    }
    default:
        break;
    }
}
//获取音乐信息
void Widget::MusicInformation(qint64 duration)
{
    if(duration==0)
        return;
    // 强制触发元数据读取
    player->metaDataChanged();
    //获取作者
    QVariant artist = player->metaData(QMediaMetaData::Author);
    qDebug() << "Artist:" << artist.toString();
    ui->AuthorLabel->setText (artist.toString());

    //获取标题
    QVariant name = player->metaData(QMediaMetaData::Title);
    qDebug() << "tite:" << name.toString();
    if(name.toString().length ()==0)
    {
        QString namestr = MusicnameList[playingIndex];
        ui->TirleLabel->setText (namestr.left (namestr.length ()-4));
    }
    else
        ui->TirleLabel->setText (name.toString());

    //获取格式
    QString type = MusicnameList[playingIndex];
    ui->FormatLabel->setText (type.right (4)+"格式");

    //获取专辑
    QVariant album = player->metaData(QMediaMetaData::AlbumTitle);
    qDebug() << "Album:" << album.toString();
    ui->DiscLabel->setText (album.toString());
    //图片
    QVariant coverImage = player->metaData(QMediaMetaData::ThumbnailImage);
    if (coverImage.isValid()) {
        QImage image6 = coverImage.value<QImage>();
        qDebug() << "ThumbnailImage Image Size:" << image6.size();
        ui->Picture->setPixmap (QPixmap::fromImage (image6).scaled (ui->Picture->size (),\
                                                                    Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    //如果音乐图不存在 播放随机图片
    else
    {
        QDir Picturedir(PICTURE);
        //对文件内容进行筛选
        QStringList picturelist = Picturedir.entryList (QStringList()<<"*.jpg");
        QPixmap pixmap(PICTURE+picturelist[playingIndex%(picturelist.size())]);
        ui->Picture->setPixmap (pixmap.scaled (ui->Picture->size (),\
                                               Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
//背景
void Widget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//声频
void Widget::processBuffer(const QAudioBuffer &buffer)
{
    qInfo()<<"#420";
    if(buffer.sampleCount() / 2 > 0)
        ui->Spectrum->drawAudioPxm(buffer);
}

