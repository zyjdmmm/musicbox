#include "music_point.h"
#include "ui_music_point.h"
#include<QDebug>
music_point::music_point(QWidget *parent) :
    QPushButton(parent),//3
    ui(new Ui::music_point)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TransparentForMouseEvents,true);//设置鼠标穿透
    this->hide();//看来是影响的，设置隐藏后反应特别快
}

//标志位置0
void music_point::set_music_point_0()
{
    this->show();
    noteFlag=0;

    QPixmap pix;//图片加载从这句开始
    bool ret=pix.load(":/res/空圆.png");
    if(!ret)//ret为0失败
        {
        qDebug()<<"图片加载失败";
        return;//失败直接跳出函数
        }
    this->setFixedSize(pix.width(),pix.height());//设置图片固定大小
    this->setStyleSheet("QPushButton{border:0px;}");//设置不规则图片
    this->setIcon(pix);//设置图标
    this->setIconSize(QSize(pix.width(),pix.height()));//设置图标大小
}


//标志位置1
void music_point::set_music_point_1()
{
    this->show();
    noteFlag=1;

    QPixmap pix;//图片加载从这句开始
    bool ret=pix.load(":/res/绿圆.png");
    if(!ret)//ret为0失败
        {
        qDebug()<<"图片加载失败";
        return;//失败直接跳出函数
        }
    this->setFixedSize(pix.width(),pix.height());//设置图片固定大小
    this->setStyleSheet("QPushButton{border:0px;}");//设置不规则图片
    this->setIcon(pix);//设置图标
    this->setIconSize(QSize(pix.width(),pix.height()));//设置图标大小
}
//标志位置2
void music_point::set_music_point_2()
{
    this->show();
    noteFlag=2;

    QPixmap pix;//图片加载从这句开始
    bool ret=pix.load(":/res/红圆.png");
    if(!ret)//ret为0失败
        {
        qDebug()<<"图片加载失败";
        return;//失败直接跳出函数
        }
    this->setFixedSize(pix.width(),pix.height());//设置图片固定大小
    this->setStyleSheet("QPushButton{border:0px;}");//设置不规则图片
    this->setIcon(pix);//设置图标
    this->setIconSize(QSize(pix.width(),pix.height()));//设置图标大小
}




music_point::~music_point()
{
    delete ui;
}
