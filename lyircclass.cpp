#include "lyircclass.h"
#include <QFileDialog>
#include <QDebug>
#include <QRegularExpression>
#include <QFile>
#include <QMessageBox>
#include <QTime>
#define LYIRC "D:\\Qt\\Qtproject\\MusicProje_1\\MusicPlayer\\src\\Lyircs\\"

LyircClass::LyircClass(QWidget *parent) : QWidget(parent)
{
    QDir dir(LYIRC);
    //遍历获取所有歌词
    musiclyirc = dir.entryList (QStringList()<<"*.lrc");
}
//根据音乐名和时间同步
bool LyircClass::initLyirc(QString Musicname, int position)
{
    qInfo()<<Musicname;
    LyircMap.clear ();
    //查找是否存在改音乐歌词文件
    int flag =0;
    // 匹配"-"后面的中文字符
    QRegularExpression regexname("-\\s*([^.]*)\\.");
    for(QString name:musiclyirc)
    {
        QRegularExpressionMatch match = regexname.match(Musicname);
        QString matchedText = match.captured(1);
        if(name.contains (matchedText))
        {
            qInfo()<<name<<"#37";
            goto input;
        }
        flag++;
    }
    qInfo()<<"未找到";
    return -1;
input:
    //打开指定文件
    QFile file(LYIRC+musiclyirc[flag]);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return -1;
    }

    //文件流
    QTextStream out(&file);
    //读取文件
    QRegularExpression regex("\\[(\\d+:\\d+\\.\\d+)\\](.*)");
    QStringList list = out.readAll().split('\n',QString::SkipEmptyParts);
    for(QString line:list)
    {
        //读取歌词
        QRegularExpressionMatch match = regex.match(line);
        if (match.hasMatch())
        {
            //获取时间毫秒
            QString timeString = match.captured(1);
            if(timeString.length ()!=strlen ("03:21.370"))
                timeString= timeString+"0";
            QTime time = QTime::fromString(timeString, "mm:ss.zzz");
            qint64 milliseconds = time.msecsSinceStartOfDay();
            //获取歌词
            QString lyrics = match.captured(2);
            LyircMap.insertMulti (milliseconds,lyrics);
        }
    }
    file.close();
    return true;
}

QMap<qint64,QString>* LyircClass::showlyirc(int position)
{
    LyircMap.insert (-2,"");
    LyircMap.insert (-1,"");
    LyircMap.insert (-3,"");
    LyircMap.insertMulti (LyircMap.lastKey (),"");
    LyircMap.insertMulti (LyircMap.lastKey (),"");
    return &LyircMap;
}
