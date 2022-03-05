#include "music_window.h"
#include "ui_music_window.h"

//#include<QVBoxLayout>
//#include<QHBoxLayout>
//#include<QPushButton>
#include<QPainter>
#include<QDebug>

music_window::music_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::music_window)
{
    ui->setupUi(this);

   setGeometry(0,0,(col-1)*point_distance_X,(row-1)*point_distance_Y);//幕布尺寸//-1是为了少留一个间隔

    //在堆空间定义动态二维数组（目前只发现这种方法）
    point = new music_point* [row];
    for (int i = 0; i < row; i++)
    {
        point[i] = new music_point[col];
    }

       for(int i=0;i<row;i++)
       {
           for(int j=0;j<col;j++)
           {
//               mainlayout->addWidget((music_point *)&point[i][j],i,j);//将控件放入布局（因为幕布中必须使用布局）//看来强转是有用的
               point[i][j].setParent(this);
//              connect(&point[i][j],&QPushButton::clicked,&point[i][j],&music_point::reversal_music_point);//点击后翻转,就不传参了
           }
       }



}

music_window::~music_window()
{
    delete ui;
}

void music_window::paintEvent(QPaintEvent *)//还是无法解决一直绘制的问题，很占cpu
{

    //背景图
//    QPainter painter(this);//创建画家,之后的画线也要提前设置好画家
//    QPixmap pix;//画布
//    pix.load(":/res/289fadd6277f9e2f6ecbc7dd1230e924b999f37a.jpg");//再画布上放入图片
//    painter.drawPixmap(0,0,this->width(),this->height(),pix);//画家将画布展示出来
//背景图标
//    pix.load(":/res/Title.png");
//    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);//缩放
//    painter.drawPixmap(10,30,pix);

//    qDebug()<<"执行绘制";

    //背景色
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(221,220,113)));//第四个是透明度//fl studio感觉不好看52,68,78
    painter.drawRect(this->rect());

    //设置画笔颜色
    QPen pen(QColor(255,255,255));
    //在拿起笔前就要设置宽度
    pen.setWidth(2);
    //画家拿起这支笔
    painter.setPen(pen);

    for(int i=0;i<row;i++)//画多少次横线
    {
        painter.drawLine(QPoint(0,0+point_distance_Y*i),
                         QPoint(point_distance_X*col,0+point_distance_Y*i)
                        );
    }

    for(int j=0;j<col;j++)//画多少次竖线
    {
        painter.drawLine(QPoint(0+j*point_distance_X,0),
                         QPoint(0+j*point_distance_X,row*point_distance_Y)
                        );
    }
}

//重写鼠标事件
void music_window::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        //偏移鼠标位置：(event->x()+point_excursion_X)
        int a_X=event->x()+point_excursion_X;
        int a_Y=event->y()+point_excursion_Y;

        if(a_X%point_distance_X<30&&a_Y%point_distance_Y<30)//选定点击区域:30X30像素大小  //修改点击范围只改这里即可
        {
            //选择并绘制节点
            if(point[a_Y/point_distance_Y][a_X/point_distance_X].noteFlag==0)
            {
                bool a=0;//错误节点标志位
                for(int i=1;i<4;i++)//检测前面三个节点是否为空
                {
                    if(a_X/point_distance_X-i<0)//防止前方数组越界
                    {
                        break;
                    }
                    if(point[a_Y/point_distance_Y][a_X/point_distance_X-i].noteFlag!=0)//前面有节点，错误标志位置1
                    {
                        a=1;
                    }
                }
                if(a==0)//错误标志位未被置1，节点置1正常
                {
                    point[a_Y/point_distance_Y][a_X/point_distance_X].set_music_point_1();
                }
                else//错误标志位置1，节点置2错误
                {
                    point[a_Y/point_distance_Y][a_X/point_distance_X].set_music_point_2();
                }
                //最后移动到正确位置
                point[a_Y/point_distance_Y][a_X/point_distance_X].move(point_distance_X*(a_X/point_distance_X)-point_excursion_X,
                                                                         point_distance_Y*(a_Y/point_distance_Y)-point_excursion_Y
                                                                         );

                    for(int i=1;i<4;i++)//检测后面三个节点是否为空                                  //后侧的数组越界还未检测！
                    {
                        if(point[a_Y/point_distance_Y][a_X/point_distance_X+i].noteFlag==1)//3格内有节点全变错误
                        {
                            point[a_Y/point_distance_Y][a_X/point_distance_X+i].set_music_point_2();
                            point[a_Y/point_distance_Y][a_X/point_distance_X+i].move(point_distance_X*(a_X/point_distance_X+i)-point_excursion_X,
                                                                                     point_distance_Y*(a_Y/point_distance_Y)-point_excursion_Y
                                                                                     );
                        }
                    }
//                }
            }
            else if(point[a_Y/point_distance_Y][a_X/point_distance_X].noteFlag==1||2)//1、如果该节点有正常图片，使其消失，2、如果后面有节点，使其正常
            {
                point[a_Y/point_distance_Y][a_X/point_distance_X].set_music_point_0();

                for(int i=1;i<4;i++)//检测后面三个节点是否为空
                {
                    if(point[a_Y/point_distance_Y][a_X/point_distance_X+i].noteFlag==2)//找到3格内第一个错误节点p(现在想将其还原)
                    {
                        bool a=0;//前方有错误节点标志位
                        for(int j=1;j<4;j++)//再从p向前寻找最近的节点，只要3格内没有节点，节点p就变正确
                        {
                            if(a_X/point_distance_X+i-j<0)//防止前方数组越界
                            {
                                break;
                            }

                            if(point[a_Y/point_distance_Y][a_X/point_distance_X+i-j].noteFlag!=0)//查找节点p前3格节点
                            {
                                a=1;//有节点就置1
                            }
                        }
                        if(a==0)//错误标志位未被置1，节点p置1正常
                        {
                            point[a_Y/point_distance_Y][a_X/point_distance_X+i].set_music_point_1();
                            point[a_Y/point_distance_Y][a_X/point_distance_X+i].move(point_distance_X*(a_X/point_distance_X+i)-point_excursion_X,
                                                                                     point_distance_Y*(a_Y/point_distance_Y)-point_excursion_Y
                                                                                     );
                        }
                    }
                }
            }

            //必须要移动到正确位置
            point[a_Y/point_distance_Y][a_X/point_distance_X].move(point_distance_X*(a_X/point_distance_X)-point_excursion_X,
                                                                     point_distance_Y*(a_Y/point_distance_Y)-point_excursion_Y
                                                                     );
        }
    }   
}

//接收来自读取文件的数据，移动节点（因为move只能在这里用）
void music_window::move_point_slot(int a,int b)//b接收read_note_X的值
{
    //检测
    if(b==col)
    {
        qDebug()<<"读取音符过多数组越界";
        return;
    }

    //检测前面是否有节点，如果是当前节点置2表示错误
    bool c=0;//错误节点标志位
    for(int j=1;j<4;j++)//从节点b向前寻找最近的节点，只要3格内没有节点，节点b就是正确
    {
        if(b-j<0)//防止前方数组越界
        {
            break;
        }
        if(point[a][b-j].noteFlag!=0)//查找节点b前3格节点
        {
            c=1;//有节点标志位置1
        }
    }

    if(c==0)//计满3表示前面3格无节点，节点b置1正常
    {
        point[a][b].set_music_point_1();//标志位置1
    }
    else//前面有节点，标记错误
    {
        point[a][b].set_music_point_2();//标志位置2
    }

    //移动到正确位置
    point[a][b].move(point_distance_X*b-point_excursion_X,point_distance_Y*a-point_excursion_Y);

}
