#include "export_image.h"
#include<QPixmap>
#include<QPainter>
#include <QPaintEvent>
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include"music_window.h"

export_image::export_image(QWidget *parent) :
    QWidget(parent)
{

}

void export_image::drawPicture()
{
/******************************************检查哪些音将被绘制****************************************************/
    //检查哪些音将被绘制
    print_note_Num=0;//将其置0，清除上次的值
    for(int i=0;i<60;i++)
    {
        if(w->exportCheckBtn[i]->isChecked())
        {
            qDebug()<<"当前被按下的按钮是:"<<i;
            print_note_Num++;//打印列数加1
        }
    }

    //向用户提示
    if(!(print_note_Num==15||print_note_Num==20||print_note_Num==30))
    {
        qDebug()<<"真的要打印"<<print_note_Num<<"个音么？似乎还没有这种产品";
    }

/**********************************************计算数据***********************************************************/
    //一列纸带加左右间隔之和
    tape_right_left_width=left_gap+right_gap+(print_note_Num-1)*exportImage_point_X;//-1是对的间隔比音符数少1

    //记录一张A4纸可打印多少列纸带
    A4_tape_Num=(exportImage_X-image_excursion)/tape_right_left_width;//这里x方向总长减去偏移的原因是加上正数相当于将x轴范围加宽
    qDebug()<<"一张A4纸可打印多少列纸带"<<A4_tape_Num;
    qDebug()<<"多少列音符"<<print_note_Num;

    //记录需要打印多少行音符
    for(int i=0;i<m->row;i++)
    {
        if(w->exportCheckBtn[i]->isChecked())//只检查被选中的行的最远音，其它行忽略
        {
            qDebug()<<"这一行已检查最远音符且将被打印："<<i;
            for(int j=0;j<m->col;j++)
            {
                if(m->point[i][j].noteFlag==1&&export_rank_Num<j)//当前音符标志位是1且export_rank_Num没更新到最大值
                {
                    export_rank_Num=j;
                }
            }
        }
    }
    qDebug()<<"最远需要打印多少列音符"<<export_rank_Num;

    //需要打印多少列纸带=总共需要打印的长度/一列纸带的高度+1(+1是零头的音符也需要打印)
    tape_Num=(exportImage_point_Y*export_rank_Num)/(exportImage_Y-top_gap)+1;
    qDebug()<<"需要打印多少列纸带"<<tape_Num;

    //计算需要多少张纸
    A4_paper_Num=tape_Num/A4_tape_Num;
    if(tape_Num%A4_tape_Num!=0)//如果剩余纸带列数并不铺满一张A4纸，还要加一张纸
    {
        A4_paper_Num++;
    }
    qDebug()<<"需要A4纸张数"<<A4_paper_Num;

    //计算一列纸带能容纳多少行音符
    tape_note_rank=(exportImage_Y-top_gap)/exportImage_point_Y-3;//-3是给一个保险值
    qDebug()<<"一列纸带能容纳多少行音符"<<tape_note_rank;

/******************************************设置绘图属性（每次1张A4）***************************************************************/
    for(int A4_tape_Num_n=0;A4_tape_Num_n<A4_paper_Num;A4_tape_Num_n++)//每一个image都开辟了新的地址，所以名字一样其实是不同变量
    {
//        QImage exportImage;//定义输出图片
        QImage image(QSize(exportImage_X,exportImage_Y),QImage::Format_ARGB32);
        image.fill("white");
        QPainter *painter = new QPainter(&image);
        painter->save();

        //画笔
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(21,85,154));

        //设置画刷
        QBrush brush(QColor(0,0,0));
        //画家拿起这个画刷
        painter->setBrush(brush);
        painter->setPen(pen);

        //图标
        QPixmap pix;//画布
        pix.load(":/res/大白兔大.png");//再画布上放入图片

        //设置字体
        QFont font;//字体属性类对象，可以设置字体的属性
       //设置字体属性
       font.setPixelSize(60);//设置字体大小
//      font.setUnderline(true);//设置下划线
//      font.setOverline(true);//设置上划线
       font.setItalic(true);//设置斜体
       font.setBold(true);//设置加粗
       font.setCapitalization(QFont::SmallCaps);//设置字母大小写
       font.setLetterSpacing(QFont::AbsoluteSpacing,10);//设置字符间距
       painter->setFont(font);//使用字体属性

/******************************************绘制虚线和logo(每次1张A4)**************************************************/
        //上侧裁剪虚线
//        painter->setPen(QPen(QBrush(QColor(99,187,208)),2,Qt::DashLine));//虚线
        //设置虚线属性
        pen.setStyle(Qt::CustomDashLine);
        QVector<qreal> dashes;
        qreal space = 10;
        dashes << 5 << space;//实线宽-虚线宽-实线宽依次循环
        pen.setDashPattern(dashes);
        painter->setPen(pen);

        painter->drawLine(QPoint(0,
                                 exportImage_point_Y*(-0.5)+top_gap),
                         QPoint(tape_right_left_width*A4_tape_Num+image_excursion,
                              exportImage_point_Y*(-0.5)+top_gap)
                          );
        //下侧裁剪虚线
        painter->drawLine(QPoint(0,
                                 (tape_note_rank+0.5)*exportImage_point_Y+top_gap),
                         QPoint(tape_right_left_width*A4_tape_Num+image_excursion,
                              (tape_note_rank+0.5)*exportImage_point_Y+top_gap)
                          );
        painter->setPen(QPen(QBrush(QColor(99,187,208)),2,Qt::SolidLine));//恢复实线

        //绘制图标（必须一次绘制完否则后面节点会被盖住）
//       painter->setOpacity(0.9);//透明度设置
        for(int A4_tape_Num_t=0;A4_tape_Num_t<A4_tape_Num;A4_tape_Num_t++)
        {
            painter->drawPixmap(0+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion+icon_X,
                                top_gap+icon_Y,
                                200,200,pix);//画家将画布内容展示出来
        }

        //绘制标题
//        QString str=QString("%1").arg(A4_tape_Num_t+A4_tape_Num_n*A4_tape_Num);//设置文字
//        painter->drawText(0+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion+image_number_X,
//                          top_gap+image_number_Y,
//                              str
//                              );


        for(int A4_tape_Num_t=0;A4_tape_Num_t<A4_tape_Num;A4_tape_Num_t++)
        {
/******************************************绘制网格(每次1列)**************************************************/
            qDebug()<<"这张纸将打印第"<<A4_tape_Num_t+1<<"条纸带网格";
            //竖线
            for(int i=0;i<print_note_Num;i++)
            {
                painter->drawLine(QPoint(i*exportImage_point_X+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion,0+top_gap),
                                 QPoint(i*exportImage_point_X+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion,top_gap+exportImage_point_Y*tape_note_rank)
                                );
            }
            //横线
            for(int i=0;i<tape_note_rank+1;i++)//补上加1是因为最底下会缺一行
            {
                painter->drawLine(QPoint(0+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion,i*exportImage_point_Y+top_gap),
                                 QPoint((print_note_Num-1)*exportImage_point_X+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion,i*exportImage_point_Y+top_gap)
                                );
            }
            //只打印右侧分隔线
            painter->drawLine(QPoint(tape_right_left_width*(A4_tape_Num_t+1)+image_excursion,exportImage_point_Y*(-0.5)+top_gap),//向上多0.5exportImage_point_Y
                              QPoint(tape_right_left_width*(A4_tape_Num_t+1)+image_excursion,top_gap+exportImage_point_Y*(tape_note_rank+0.5))//向下多0.5exportImage_point_Y
                            );
            //左侧分隔线（第一列左边）
            painter->drawLine(QPoint(1,exportImage_point_Y*(-0.5)+top_gap),
                              QPoint(1,top_gap+exportImage_point_Y*(tape_note_rank+0.5))
                            );

/***********************************************绘制小节记号（每次1列）********************************************/

            //打印小节记号
           QString str=QString("%1").arg(A4_tape_Num_t+A4_tape_Num_n*A4_tape_Num);//设置文字

           //循环打印小节符
           painter->drawText(0+left_gap+A4_tape_Num_t*tape_right_left_width+image_excursion+image_number_X,
                             top_gap+image_number_Y,
                                 str
                                 );

           }//for(int A4_tape_Num_t=0;A4_tape_Num_t<A4_tape_Num;A4_tape_Num_t++)//打印完当前A4纸的网格

/************************************************绘制音符（每次1张A4）**********放在后面是因为音符要显示在图标之上******/
           //打印此次这张A4纸的音符
           for(int A4_tape_Num_c=0;A4_tape_Num_c<A4_tape_Num;A4_tape_Num_c++)//这张纸能容纳多少列纸带就循环多少次
           {
               int col_id=-1;//依次打印选中行的音符（从-1开始是为了正好使1条行音对应0）
               for(int i=59;i>=0;i--)//检查并打印被选中的行(反向i的原因是将音符从左向右排列)
               {
                   if(w->exportCheckBtn[i]->isChecked())
                   {
                       col_id++;//选中行的音符映射到从0到30（30是举例）

                       //打印这列纸带中的第col_id列音符
                       for(int j=output_pointer;j<tape_note_rank+output_pointer;j++)//从上一次的输出光标位置继续打印
                       {

                           //假如这个点有音符就准备打印
                           if(m->point[i][j].noteFlag==1)
                           {
                               painter->drawEllipse(QPoint(left_gap+A4_tape_Num_c*tape_right_left_width+col_id*exportImage_point_X+image_excursion,
                                                           top_gap+(j-output_pointer)*exportImage_point_Y)
                                                   ,12,12);//画圈半径
                               qDebug()<<"有一个音符被打印i j"<<i<<j;
                               qDebug()<<"这个音符的位置是x y"<<left_gap+A4_tape_Num_c*tape_right_left_width+col_id*exportImage_point_X+image_excursion<<top_gap+(j-output_pointer)*exportImage_point_Y;
                               qDebug()<<"此时A4_tape_Num_c是"<<A4_tape_Num_c;
//                               qDebug()<<"col_id"<<col_id;
                           }
                       }
                   }
               }
               output_pointer=output_pointer+tape_note_rank;//输出光标向后移动
           }//for(int A4_tape_Num_c=0;A4_tape_Num_c<A4_tape_Num;A4_tape_Num_c++)//打印完当前A4纸的节点

/************************************************保存（1张A4）*************************************************************/
        painter->restore();
        painter->end();
        exportImage = image;//每一次exportImage都收到了新的数据

        qDebug()<<"image的值"<<image;

        if(A4_paper_Num==1)
        {
            save_image();//输出一张图片
        }
        else
        {
            save_image(A4_tape_Num_n);//输出多张图片
        }
    }//for(int A4_tape_Num_n=0;A4_tape_Num_n<A4_paper_Num;A4_tape_Num_n++)//绘制完所有图片
    /***************************************完成绘制数据归位***************************************/
    output_pointer=0;//光标重新置0
    save_image_mark=0;//弹出选择标志位框置0
}


//输出一张图片
void export_image::save_image()
{
    QString file_path = QFileDialog::getSaveFileName(this, tr("输出图片"),
                               "C:/Users/zyjdm/Desktop/white_rabbit",
                               tr("Images (*.png)"));

    qDebug()<<"file_path"<<file_path;

        //如果没有写后缀就自动加上
        if(QFileInfo(file_path).suffix().isEmpty())
        {
            file_path.append(".png");
        }
        exportImage.save(file_path);
}

//输出多张图片
void export_image::save_image(int a)
{
    //打开选择框只执行一次
    if(save_image_mark==0)
    {
        file_path = QFileDialog::getSaveFileName(this, tr("输出图片"),
                                   "C:/Users/zyjdm/Desktop/white_rabbit",
                                   tr(""));
        save_image_mark=1;//置1
    }

    qDebug()<<"file_path"<<file_path;

    QString file_path_a ;//这个临时变量存储file_path的值，不然后缀越加越长
    file_path_a=file_path;//这样真正file_path的值就不会变了

    QString str=QString("(%1).png").arg(a);//也就是说这里只能输出png图片
    file_path_a.append(str);//添加后缀
    exportImage.save(file_path_a);
}

//获得检查按钮的数据
void export_image::get_export_check_btn(central_w *a)
{
    w=a;
}

//获得point数据
void export_image::getpoint(music_window *a)
{
    m=a;
}

//15音设置
void export_image::export_image_set_15()
{
    left_gap=78;//左侧间隔
    right_gap=72;//右侧间隔
    exportImage_point_X=25;//打印节点在X轴方向间隔
    exportImage_point_Y=24;//打印节点在Y轴方向间隔48
    image_excursion=-30;//图像平移以最左侧纸带处于合适位置-34
}

//20音设置
void export_image::export_image_set_20()
{
    left_gap=78;//左侧间隔
    right_gap=72;//右侧间隔
    exportImage_point_X=25;//打印节点在X轴方向间隔
    exportImage_point_Y=24;//打印节点在Y轴方向间隔48
    image_excursion=0;//图像平移以最左侧纸带处于合适位置
}

//30音设置
void export_image::export_image_set_30()
{
    left_gap=62;//左侧间隔66
    right_gap=60;//右侧间隔
    exportImage_point_X=25;//打印节点在X轴方向间隔
    exportImage_point_Y=24;//打印节点在Y轴方向间隔
    image_excursion=-22;//图像平移以最左侧纸带处于合适位置
}










