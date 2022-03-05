#include "key_board.h"
#include "ui_key_board.h"
#include<QString>
#include<QFile>

key_board::key_board(int o) :
//    QWidget(parent),这句加了之后就错了，似乎不能再设立父类了
    ui(new Ui::key_board)
{
    ui->setupUi(this);

    QFile file(":/res/octave_white.qss");            //统一给全音设置样式
    //只读方式打开文件
    file.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet = tr(file.readAll());



    QString str1=QString("C %1").arg(o);
    ui->pushButton_C->setText(str1);
    //给当前对象设置样式表
//    ui->pushButton_C->setStyleSheet(styleSheet);//C在样式表，不用再设置

    QString str3=QString("D %1").arg(o);
    ui->pushButton_D->setText(str3);
    //给当前对象设置样式表
    ui->pushButton_D->setStyleSheet(styleSheet);


    QString str5=QString("E %1").arg(o);
    ui->pushButton_E->setText(str5);
    //给当前对象设置样式表
    ui->pushButton_E->setStyleSheet(styleSheet);

    QString str6=QString("F %1").arg(o);
    ui->pushButton_F->setText(str6);
    //给当前对象设置样式表
    ui->pushButton_F->setStyleSheet(styleSheet);

    QString str8=QString("G %1").arg(o);
    ui->pushButton_G->setText(str8);
    //给当前对象设置样式表
    ui->pushButton_G->setStyleSheet(styleSheet);

    QString str10=QString("A %1").arg(o);
    ui->pushButton_A->setText(str10);
    //给当前对象设置样式表
    ui->pushButton_A->setStyleSheet(styleSheet);

    QString str12=QString("B %1").arg(o);
    ui->pushButton_B->setText(str12);
    //给当前对象设置样式表
    ui->pushButton_B->setStyleSheet(styleSheet);

    //关闭qss文件
    file.close();//其实不用写，离开作用域自动关闭
/***********************************************************************************************/
    QFile file1(":/res/octave_black.qss");            //统一给半音设置样式(C1,D1这种都是表示C#，D#)
    //只读方式打开文件
    file1.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet1 = tr(file1.readAll());

    QString str2=QString("C#%1").arg(o);
    ui->pushButton_C1->setText(str2);
    ui->pushButton_C1->setStyleSheet(styleSheet1);

    QString str4=QString("D#%1").arg(o);
    ui->pushButton_D1->setText(str4);
    ui->pushButton_D1->setStyleSheet(styleSheet1);

    QString str7=QString("F#%1").arg(o);
    ui->pushButton_F1->setText(str7);
    ui->pushButton_F1->setStyleSheet(styleSheet1);

    QString str9=QString("G#%1").arg(o);
    ui->pushButton_G1->setText(str9);
    ui->pushButton_G1->setStyleSheet(styleSheet1);

    QString str11=QString("A#%1").arg(o);
    ui->pushButton_A1->setText(str11);
    ui->pushButton_A1->setStyleSheet(styleSheet1);

    //关闭qss文件
    file1.close();





}

key_board::~key_board()
{
    delete ui;
}
