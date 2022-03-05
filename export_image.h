#ifndef EXPORT_IMAGE_H
#define EXPORT_IMAGE_H

#include <QWidget>
#include"music_window.h"
#include "mainwindow.h"
#include"central_w.h"


class export_image : public QWidget
{
    Q_OBJECT
public slots:
    void drawPicture();//绘制图片


public:
    void getpoint(music_window *);//获得point数据
    void get_export_check_btn(central_w *);//获得检查按钮的数据

    central_w *w;//获得检查按钮的数据
    music_window *m;//获得point数据

    explicit export_image(QWidget *parent = 0);

private:
    void save_image();//保存图片
    void save_image(int);//保存图片,如果多余一张纸就调用这个

    int save_image_mark=0;//用于void save_image(int);只弹出一次保存框

    int print_note_Num=0;//将有这么多行之上的音符将被打印，(初始为0，有几个检查按钮就为几)
    int export_rank_Num=0;//记录最远的音符在第几行
    int A4_paper_Num=0;//记录需要输出几张A4纸
    int tape_Num=0;//记录需要打印多少列纸带
    int A4_tape_Num=0;//记录一张A4纸可打印多少列纸带
    int tape_note_rank=0;//记录一列纸带能容纳多少行音符

    int output_pointer=0;//记录当前已经输出了多少行音符
//    int output_rank_pointer=0;//记录已经输出了多少行，用于绘制小节标记

    QImage exportImage;//定义输出图片
    QString file_path;//存储保存路径
    int exportImage_point_X=25;//打印节点在X轴方向间隔
    int exportImage_point_Y=48;//打印节点在Y轴方向间隔
    int exportImage_X=2481;//输出图片宽
    int exportImage_Y=3508;//输出图片高
    int left_gap=78;//左侧间隔
    int right_gap=72;//右侧间隔
    int top_gap=400;//顶部间隔
    int tape_right_left_width;//一列纸带加左右间隔之和
    int image_excursion=0;//图像平移以最左侧纸带处于合适位置//在槽函数设置其值
    int image_number_X=150;//输出标记数字的位置偏移
    int image_number_Y=150;
    int icon_X=80;//图标偏移位置
    int icon_Y=200;

    int grid_add_width=0;//由于网格总宽很难调到厂家生产的宽度，所以每隔grid_add_Num个横线小格增加grid_add_width个像素
    int grid_add_Num=5;

public slots:
        void export_image_set_15();
        void export_image_set_20();
        void export_image_set_30();
};

#endif // EXPORT_IMAGE_H
