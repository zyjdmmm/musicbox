#ifndef MUSIC_POINT_H
#define MUSIC_POINT_H

#include <QPushButton>//1

namespace Ui {
class music_point;
}

class music_point : public QPushButton//2
{
    Q_OBJECT

public:
    qint8 noteFlag=0;//标志位置1表示已翻转

//    int lastNoteX=0;//存储当前点的最后一次坐标//用不上了
//    int lastNoteY=0;
    explicit music_point(QWidget *parent = 0);
    ~music_point();

private:
    Ui::music_point *ui;

public slots:
        void set_music_point_0();//无图片
        void set_music_point_1();//正确显示节点
        void set_music_point_2();//标记错误节点
};

#endif // MUSIC_POINT_H
