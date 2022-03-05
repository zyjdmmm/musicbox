#include "central_w.h"
#include "ui_central_w.h"
#include<QDebug>
#include<QScrollBar>




central_w::central_w(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::central_w)
{
    ui->setupUi(this);


    area=new QScrollArea(this);//不知道为什么只有设为指针时才能用？
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//保持滚动条常开
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    musicWindow=new music_window;//变为子部件，为什么这里不加this？
    area->setWidget(musicWindow);//设置幕布
//    area->setWidgetResizable(true);//似乎是调整控件多余的间距？  //感觉无法理解
//    area->setMinimumSize(1000,1000);

    key_board_excursion=musicWindow->point_distance_Y/2;//键盘的图像偏移以便对齐:取节点距的一半

    //建立5个键盘对象
    int a=5-1;//a用于确定键盘位置，因为键盘从下至上摆放，所以顺序是反的
    for(int o=0;o<5;o++)//一共有5组八度，o是octave的手首字母,octave是八度的意思
    {
        keyBoard_octave[o]=new key_board(o+3);//参数o代表是第几组八度，//o+3是因为参数o会直接显示第几组八度
                                                                                /*
                                                                            keyBoard_octave[0]对应C3
                                                                            keyBoard_octave[1]对应C4
                                                                            keyBoard_octave[2]对应C5
                                                                            keyBoard_octave[3]对应C6
                                                                            keyBoard_octave[4]对应C7
                                                                            */
        keyBoard_octave[o]->setParent(this);//设置父类
        keyBoard_octave[o]->move(20,a*keyBoard_octave[o]->height()-key_board_excursion);//初始化位置
        a--;
    }

    //export_check_btn检查哪些音要被打印
    for(int i=0;i<60;i++)
    {
         exportCheckBtn[i]=new export_check_btn;
         exportCheckBtn[i]->setParent(this);
         exportCheckBtn[i]->setAutoExclusive(false);//取消互斥
         exportCheckBtn[i]->setMaximumSize(20,20);//必须设置最小大小不然出问题
         exportCheckBtn[i]->move(0,i*30+export_check_btn_excursion);
    }







//    QScrollBar *rightBar=new QScrollBar;  //可以自定义一个滚动条
//    area->setVerticalScrollBar(rightBar);
    //根据右侧滚动条改变键盘位置
    connect(area->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(key_board_move(int)));//左边valueChanged不会告诉是哪个形参，自己在槽中随便搞一个好了

    //根据右侧滚动条改变检查按钮位置
    connect(area->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(export_check_btn_move(int)));//左边valueChanged不会告诉是哪个形参，自己在槽中随便搞一个好了

}

central_w::~central_w()
{
    delete ui;
}

void central_w::key_board_move(int b)//这里传过来的值是滚动条移动的像素位移b
{

    int a=5-1;//a用于确定键盘位置，因为键盘从下至上摆放，所以顺序是反的
    for(int o=0;o<5;o++)//一共有5组八度，o是octave的手首字母,octave是八度的意思
    {
        keyBoard_octave[o]->move(20,a*keyBoard_octave[o]->height()-b-key_board_excursion);//
        a--;
    }
}

void central_w::export_check_btn_move(int b)//这里传过来的值是滚动条移动的像素位移b
{
    for(int i=0;i<60;i++)
    {
         exportCheckBtn[i]->move(0,i*30-b+export_check_btn_excursion);
    }

}




