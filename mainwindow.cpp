#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QTextEdit>
#include <QMenuBar>
#include<QToolBar>
#include"read_midi.h"
#include"export_image.h"
#include"note_stretch.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    resize(1500,800);//打开时的尺寸
    setFixedSize(1500,800);//设置最小尺寸
    setWindowIcon(QIcon(":/res/大白兔小.png"));
    setWindowTitle("大白兔测试0.1");

    centralWidget=new central_w(this);//类名和对象名一样会出问题
    setCentralWidget(centralWidget);//将这个窗口设置为中心窗口                          //果然这里只能写指针不能写非指针
    centralWidget->area->setGeometry(115,0,this->width()-115,this->height()-40);//窥视窗尺寸(放这里是要参考this尺寸)
/**********************************************************************************************************/
    QMenuBar *Bar=menuBar();
    setMenuBar(Bar);//将菜单栏放入窗口中
    QMenu *fileMenu=Bar->addMenu("文件");//增加文件窗口

    QAction *openAction=fileMenu->addAction("打开");
    fileMenu->addSeparator();//分割线
    QAction *outAction=fileMenu->addAction("输出为图片");

    QMenu *noteMenu=Bar->addMenu("音符数");//
    QAction *note_15=noteMenu->addAction("15音");
    QAction *note_20=noteMenu->addAction("20音");
    QAction *note_30=noteMenu->addAction("30音");

    QMenu *stretchMenu=Bar->addMenu("扩张音符");//
    QAction *expand=stretchMenu->addAction("扩展为2倍");
    QAction *shrink=stretchMenu->addAction("缩小为一半");
/***********************************************************************************************************/
    //定义读取文件对象
//    read_midi readMidi;//不允许使用局部变量，connect虽然成功但是生命变量会被销毁，必须将这句放在头文件
    read_midi *readMidi=new read_midi;
//    readMidi->getpoint(centralWidget->musicWindow);//获得musicWindow入口地址

    //定义输出图片对象
    export_image *exportImage=new export_image;
    exportImage->get_export_check_btn(centralWidget);//获得检查按钮的数据
    exportImage->getpoint(centralWidget->musicWindow);//获得point数据

    //打开并解析文件
    connect(openAction,&QAction::triggered,readMidi,read_midi::openFile);//connect不如说成是connect(实例，&类：方法)

    //解析文件的结果数据交给move_point_slot槽函数处理
    connect(readMidi,SIGNAL(point_move_signal(int,int)),centralWidget->musicWindow,SLOT(move_point_slot(int,int)));

    //输出图片
    connect(outAction,&QAction::triggered,exportImage,&export_image::drawPicture);

    //15音模式
    connect(note_15,&QAction::triggered,this,MainWindow::export_check_btn_set_15);
    //20音模式
    connect(note_20,&QAction::triggered,this,MainWindow::export_check_btn_set_20);
    //30音模式
    connect(note_30,&QAction::triggered,this,MainWindow::export_check_btn_set_30);
    //默认加载15音
    export_check_btn_set_15();//槽函数居然也可以直接用

    //15音参数设置
    connect(note_15,&QAction::triggered,exportImage,&export_image::export_image_set_15);
    //20音参数设置
    connect(note_20,&QAction::triggered,exportImage,&export_image::export_image_set_20);
    //30音参数设置
    connect(note_30,&QAction::triggered,exportImage,&export_image::export_image_set_30);
    //默认加载15音参数设置
    exportImage->export_image::export_image_set_15();
/***********************************************************************************************************/
    //定义扩大一倍距离对象
    note_stretch *noteExpand=new note_stretch;
    noteExpand->getpoint(centralWidget->musicWindow);//获得point数据

    //扩大一倍
    connect(expand,&QAction::triggered,noteExpand,&note_stretch::note_expand);
}


//设置15音
void MainWindow::export_check_btn_set_15()
{
    int a[15]={11,12,14,16,18,19,21,23,24,26,28,30,31,33,35};//指定要打印的音
    for(int i=0;i<15;i++)//这些音设为true      //要改1
    {
        centralWidget->exportCheckBtn[a[i]]->setChecked(true);
    }
    int j=0;//临时变量遍历数组a中所有值
    for(int i=0;i<60;i++)//其余音设为false
    {
        if(a[j]!=i)
        {
            centralWidget->exportCheckBtn[i]->setChecked(false);
        }
        else
        {
            if(j<14)//最大加到14        //要改2
            j++;
        }
    }
}

//设置20音
void MainWindow::export_check_btn_set_20()
{
    int a[20]={14,16,18,19,21,23,24,26,28,30,31,33,35,36,38,40,42,43,45,47};//指定要打印的音
    for(int i=0;i<20;i++)//这些音设为true      //要改1
    {
        centralWidget->exportCheckBtn[a[i]]->setChecked(true);
    }
    int j=0;//临时变量遍历数组a中所有值
    for(int i=0;i<60;i++)//其余音设为false
    {
        if(a[j]!=i)
        {
            centralWidget->exportCheckBtn[i]->setChecked(false);
        }
        else
        {
            if(j<19)//最大加到19        //要改2
            j++;
        }
    }
}
//设置30音
void MainWindow::export_check_btn_set_30()
{
    int a[30]={7,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,33,35,36,38,40,45,47};//指定要打印的音
    for(int i=0;i<30;i++)//这些音设为true    //要改1
    {
        centralWidget->exportCheckBtn[a[i]]->setChecked(true);
    }
    int j=0;//临时变量遍历数组a中所有值
    for(int i=0;i<60;i++)//其余音设为false
    {
        if(a[j]!=i)
        {
            centralWidget->exportCheckBtn[i]->setChecked(false);
        }
        else
        {
            if(j<29)//最大加到29   //要改2
            j++;
        }
    }
//    centralWidget->exportCheckBtn[19]->setChecked(true);
//    centralWidget->exportCheckBtn[21]->setChecked(true);
//    centralWidget->exportCheckBtn[23]->setChecked(true);
//    centralWidget->exportCheckBtn[24]->setChecked(true);
//    centralWidget->exportCheckBtn[25]->setChecked(true);
//    centralWidget->exportCheckBtn[26]->setChecked(true);
//    centralWidget->exportCheckBtn[27]->setChecked(true);
//    centralWidget->exportCheckBtn[28]->setChecked(true);
//    centralWidget->exportCheckBtn[29]->setChecked(true);
//    centralWidget->exportCheckBtn[30]->setChecked(true);
//    centralWidget->exportCheckBtn[31]->setChecked(true);
//    centralWidget->exportCheckBtn[32]->setChecked(true);
//    centralWidget->exportCheckBtn[33]->setChecked(true);
//    centralWidget->exportCheckBtn[34]->setChecked(true);
//    centralWidget->exportCheckBtn[35]->setChecked(true);
//    centralWidget->exportCheckBtn[36]->setChecked(true);
//    centralWidget->exportCheckBtn[37]->setChecked(true);
//    centralWidget->exportCheckBtn[38]->setChecked(true);
//    centralWidget->exportCheckBtn[39]->setChecked(true);
//    centralWidget->exportCheckBtn[40]->setChecked(true);
//    centralWidget->exportCheckBtn[41]->setChecked(true);
//    centralWidget->exportCheckBtn[42]->setChecked(true);
//    centralWidget->exportCheckBtn[43]->setChecked(true);
//    centralWidget->exportCheckBtn[45]->setChecked(true);
//    centralWidget->exportCheckBtn[47]->setChecked(true);
//    centralWidget->exportCheckBtn[48]->setChecked(true);
//    centralWidget->exportCheckBtn[50]->setChecked(true);
//    centralWidget->exportCheckBtn[52]->setChecked(true);
//    centralWidget->exportCheckBtn[57]->setChecked(true);
//    centralWidget->exportCheckBtn[59]->setChecked(true);
}
MainWindow::~MainWindow()
{
    delete ui;
}
