#include "note_stretch.h"

note_stretch::note_stretch(QWidget *parent) : QWidget(parent)
{

}
//获得point数据
void note_stretch::getpoint(music_window *a)
{
    m=a;
}

//音符间距扩大一倍
void note_stretch::note_expand()
{
    //间距扩大一倍
    for(int i=m->row-1;i>-1;i--)//从m->row-1到0                                   //注意这里不能简单使用改变标志位的方法，
    {                                                                         //必须函数置位后move，不然节点还是在原位没有改变
        for(int j=m->col-1;j>-1;j--)//从m->col-1到0
        {
            if(m->point[i][j].noteFlag!=0)//只要不为空就扩大一倍
            {
                m->point[i][j].set_music_point_0();
                m->point[i][2*j].set_music_point_1();//列坐标扩大为两倍
                m->point[i][2*j].move(m->point_distance_X*(2*j)-m->point_excursion_X,//移动到正确位置
                                      m->point_distance_Y*(i)-m->point_excursion_Y
                                      );
            }
        }
    }

    //完成移动后进行检查错误点
    for(int i=0;i<m->row;i++)
    {
        for(int j=0;j<m->col;j++)
        {
            if(m->point[i][j].noteFlag!=0)
            {
                bool a=0;//错误标志位
                for(int z=1;z<4;z++)//从当前节点p向前寻找最近的节点，只要3格内没有节点，节点p就是正确
                {
                    if(j-z<0)//防止前方数组越界
                    {
                        break;
                    }

                    if(m->point[i][j-z].noteFlag!=0)//查找节点b前3格节点
                    {
                        a=1;//有节点错误标志位就置1
                    }
                }
                if(a==0)//前面3格无节点，节点p置1正常
                {
//                    m->point[i][j-z].set_music_point_1();//标志位置1//不用这句话，因为本来当前节点就是正确的
                }
                else//前面3格有节点，标记错误
                {
                    m->point[i][j].set_music_point_2();//标志位置2
                    //移动到正确位置
                    m->point[i][j].move(m->point_distance_X*j-m->point_excursion_X,m->point_distance_Y*i-m->point_excursion_Y);
                }
            }
        }
    }
}

//音符间距缩小一倍
void note_stretch::note_shrink()
{

}
