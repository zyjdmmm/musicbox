#ifndef MUSIC_WINDOW_H
#define MUSIC_WINDOW_H

#include<music_point.h>
#include <QWidget>
#include<QGridLayout>
#include<QMouseEvent>

namespace Ui {
class music_window;
}

class music_window : public QWidget
{
    Q_OBJECT

public:
    music_point** point;//定义多个按钮对象
    int row = 60;//行数
    int col = 2000;//列数
    int point_distance_X=35;//节点间距
    int point_distance_Y=30;
    int point_excursion_X=16;//调整节点的位置以对齐网格（note_stretch.cpp要访问）
    int point_excursion_Y=16;


    explicit music_window(QWidget *parent = 0);
    ~music_window();

private:
//    QGridLayout *mainlayout;//画布
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);


    Ui::music_window *ui;
public slots:
    void move_point_slot(int,int);
};

#endif // MUSIC_WINDOW_H
