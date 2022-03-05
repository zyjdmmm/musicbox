#ifndef READ_MIDI_H
#define READ_MIDI_H
#include"music_window.h"
#include <QWidget>

class read_midi: public QWidget
{
    Q_OBJECT

signals:
    void point_move_signal(int ,int);

public:

    explicit read_midi(QWidget *parent = nullptr);

    void getpoint(music_window *);//获得music_window入口地址

public slots:
    void openFile();//读取并解析文件

private:
    music_window *m;//接收从接口函数中传来的入口地址

    quint8 buffer[4];//专用缓冲区移位，存储文件读来的数据，存储命令符前的时间差（delta-time）
    quint32 deltaTime;//计算的时间差的值
    int DDT;//4分音符所占时间
    float read_note_X=0;//这个指针用于读取文件，存储音符在X方向的坐标,初始位置X=0

    int bar=4;//每小节拍数
    int beat=4;//以？音符为一拍
    int tempo=500000;//4分音符时间：默认500000us=0.5s(通过这个计算出：BPM=60000000/microTempo)

    quint8 *data1=new quint8[1];//准备缓冲区从文件读取数据，,它能放1字节   //注意：quint8是无符号的！那么它能存8位
    quint8 *data2=new quint8[2];//准备缓冲区从文件读取数据,它能放2字节
    quint8 *data3=new quint8[3];//准备缓冲区从文件读取数据,它能放3字节
    quint8 *data4=new quint8[4];//准备缓冲区从文件读取数据,它能放4字节

    bool readErrorMark=0;//读取出错的标志位
    int num_track=0;//定义音轨数量
    quint32 mtrkByte;//本次mtrk字节数
    int a_mtrk;//记录当前是第几个mtrk

    void Mthd_check(QDataStream &);//头块校验
    void Mtrk_check(QDataStream &);//音轨块校验
    void delta_time(QDataStream &);//
    void read_cmd_0x00_0x7F(QDataStream &);//
    void read_cmd_0x8x(QDataStream &);//0x8x松开 操作指令 (省略了形参名)
    void read_cmd_0x9x(QDataStream &);//0x9x打开 操作指令
    void read_cmd_0xAx(QDataStream &);//
    void read_cmd_0xBx(QDataStream &);//
    void read_cmd_0xCx(QDataStream &);//
    void read_cmd_0xDx(QDataStream &);//
    void read_cmd_0xEx(QDataStream &);//
    void read_cmd_0xFO(QDataStream &);//
    void read_cmd_0XFF_0x00(QDataStream &);// 非MIDI事件
    void read_cmd_0XFF_0x01(QDataStream &);//
    void read_cmd_0XFF_0x02(QDataStream &);//
    void read_cmd_0XFF_0x03(QDataStream &);//
    void read_cmd_0XFF_0x04(QDataStream &);//
    void read_cmd_0XFF_0x05(QDataStream &);//
    void read_cmd_0XFF_0x06(QDataStream &);//
    void read_cmd_0XFF_0x07(QDataStream &);//
    void read_cmd_0XFF_0x08(QDataStream &);//
    void read_cmd_0XFF_0x09(QDataStream &);//
    void read_cmd_0XFF_0x20(QDataStream &);//
    void read_cmd_0XFF_0x21(QDataStream &);//
    void read_cmd_0XFF_0x2F(QDataStream &);//
    void read_cmd_0XFF_0x51(QDataStream &);//
    void read_cmd_0XFF_0x54(QDataStream &);//
    void read_cmd_0XFF_0x58(QDataStream &);//
    void read_cmd_0XFF_0x59(QDataStream &);//
    void read_cmd_0XFF_0x7F(QDataStream &);//
};

#endif // READ_MIDI_H
