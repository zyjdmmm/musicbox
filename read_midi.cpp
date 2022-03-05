#include "read_midi.h"
#include<QFile>
#include<QFileDialog>
#include<QDebug>
#include<QString>

read_midi::read_midi(QWidget *parent) : QWidget(parent)
{

}



/***********************************读取并解析文件*******************************************************/
//从定义它的父类处获得music_window入口地址
void read_midi::getpoint(music_window *a)//传值竟如此简简单单
{
    m=a;
}

void read_midi::openFile()
{

    QString fileter = "midi文件(*.mid);;专用格式文件(*.wmid)";
    QString path=QFileDialog::getOpenFileName((QWidget *)this,"老佛爷，请选择您的MIDI文件！","C:/Users/zyjdm/Desktop",fileter);
//   QString path=QFileDialog::getOpenFileName(this,"老佛爷，请选择您的MIDI文件！","C:/Users/zyjdm/Desktop",tr("*.mid *.wmid"));//path存放路径
    QFile file(path);

//        if(file.open(QIODevice::WriteOnly))
//        {
//            QDataStream out(&file);//向外写
//            qint8 data1[256];
//            memset(data1,0,256*sizeof(qint8));
//            const qint8* p = "this is QDataStream demo";//tian添加了const
//            strcpy(data1,p);
//            out.writeRawData(data1,256);
//            file.close();
//        }
//        else
//        {
//            qDebug()<<file.errorString();//反过来也不错
//        }

    if(!file.open(QIODevice::ReadOnly))
    {
       qDebug()<<"文件打开失败";
    }
    else
    {
//        //清屏,标志位全置0        不知道为什么会错
//        for(int i=0;i<m->row;i++)
//        {
//            for(int j=0;j<m->col;j++)
//            {
//                qDebug()<<"ij"<<i<<j;
//                m->point[i][j].noteFlag=0;
//            }
//        }

        QDataStream in(&file);//输入流的定义
        Mthd_check(in);//mthd头块校验
        for(a_mtrk=0;a_mtrk<num_track;a_mtrk++) //循环num_track次，音轨块的数量
        {
            read_note_X=0;//每次读取当前前将光标清0
            Mtrk_check(in);//mtrk音轨块校验
            while(1)
            {
                if(readErrorMark==1){qDebug()<<"错误标志位被置1";return;}
                delta_time(in);//计算时间差
                in.readRawData((char*)data1,1);//再读1字节
                if(data1[0]<=0x7f){read_cmd_0x00_0x7F(in);}//0x00-0x7F省略操作指令音符(此时要提前打开指定通道)
                else if(data1[0]>=0x80&&data1[0]<0x90){read_cmd_0x8x(in);}//0x8x 松开 指令(仅仅移动read_note_X坐标，什么也不做)
                else if(data1[0]>=0x90&&data1[0]<0xA0){read_cmd_0x9x(in);}//0x9x 按下 指令
                else if(data1[0]>=0xA0&&data1[0]<0xB0){read_cmd_0xAx(in);}//0xAx  指令
                else if(data1[0]>=0xB0&&data1[0]<0xC0){read_cmd_0xBx(in);}//0xBx  指令
                else if(data1[0]>=0xC0&&data1[0]<0xD0){read_cmd_0xCx(in);}//0xCx  指令
                else if(data1[0]>=0xD0&&data1[0]<0xE0){read_cmd_0xDx(in);}//0xDx  指令
                else if(data1[0]>=0xE0&&data1[0]<0xF0){read_cmd_0xEx(in);}//0xEx  指令
                else if(data1[0]==0xF0){read_cmd_0xFO(in);}//0xF0  指令
                else if(data1[0]==0xFF)//0xff操作指令
                {
                    in.readRawData((char*)data1,1);//再读1字节
                    if(data1[0]==0x00){read_cmd_0XFF_0x00(in);}
                    else if(data1[0]==0x01){read_cmd_0XFF_0x01(in);}
                    else if(data1[0]==0x02){read_cmd_0XFF_0x02(in);}
                    else if(data1[0]==0x03){read_cmd_0XFF_0x03(in);}//歌曲标题或音轨名称
                    else if(data1[0]==0x04){read_cmd_0XFF_0x04(in);}
                    else if(data1[0]==0x05){read_cmd_0XFF_0x05(in);}
                    else if(data1[0]==0x06){read_cmd_0XFF_0x06(in);}
                    else if(data1[0]==0x07){read_cmd_0XFF_0x07(in);}
                    else if(data1[0]==0x08){read_cmd_0XFF_0x08(in);}
                    else if(data1[0]==0x09){read_cmd_0XFF_0x09(in);}
                    else if(data1[0]==0x20){read_cmd_0XFF_0x20(in);}
                    else if(data1[0]==0x21){read_cmd_0XFF_0x21(in);}
                    else if(data1[0]==0x2F){read_cmd_0XFF_0x2F(in);break;} //结束本次mtrk循环
                    else if(data1[0]==0x51){read_cmd_0XFF_0x51(in);}//速度tempo
                    else if(data1[0]==0x54){read_cmd_0XFF_0x54(in);}
                    else if(data1[0]==0x58){read_cmd_0XFF_0x58(in);}//节拍
                    else if(data1[0]==0x59){read_cmd_0XFF_0x59(in);}//调号
                    else if(data1[0]==0x7F){read_cmd_0XFF_0x7F(in);}
                    else{qDebug()<<"错误：0xff指令后的字节无法识别";return;}
                }
                else{qDebug()<<"操作指令错误：可能是操作指令为0xF1-0XFE";return;}//直接返回
            }//while(1)
        }//for(a_mtrk=0;a_mtrk<num_track;a_mtrk++)
        file.close();
    }//if(!file.open(QIODevice::ReadOnly))
}//void read_midi::openFile()
/**********************************************************************************************************/
//读取时间差或动态字节
void read_midi::delta_time(QDataStream &in)
{
    int stop=in.readRawData((char*)data1,1);
    //检查字节是否已经被读取完（如果是readRawData会返回0）
    if(stop==0)
    {
        qDebug()<<"已经没有字节可读取";
        readErrorMark=1;
        return;
    }

    if(data1[0]<=0x7f)                                            //只有一个字节时，数据在buffer[0]，其余置128
    {
        buffer[0]=data1[0];//第一个字节的数据存入buffer[0]
        buffer[1]=128; //因为其（假设字节拓宽的话）最高位一直为1，为了保证后面计算时间差时减128为0
        buffer[2]=128;
        buffer[3]=128;
        qDebug()<<"当前时间差1字节";
    }
    else
    {
        buffer[1]=data1[0];//存储if中没存的数据
        in.readRawData((char*)data1,1);//再读1字节

        if(data1[0]<=0x7f)//检查是否为时间差的最后一个字节            //有两个字节时，数据在buffer[0]buffer[1]，其余置128
        {
            buffer[0]=data1[0];
            buffer[2]=128; //其余2字节置128
            buffer[3]=128;
            qDebug()<<"当前时间差2字节";
        }
        else
        {
            buffer[2]=buffer[1];//向后移动1字节
            buffer[1]=data1[0];//存储if中没存的数据
            in.readRawData((char*)data1,1);//再读1字节

            if(data1[0]<=0x7f)//检查是否为时间差的最后一个字节         //有三个字节时，数据在buffer[0]buffer[1]buffer[2]，其余置128
            {
                buffer[0]=data1[0];
                buffer[3]=128; //其余1字节置128
                qDebug()<<"当前时间差3字节";
            }
            else
            {
                buffer[3]=buffer[2];
                buffer[2]=buffer[1];//向后移动1字节
                buffer[1]=data1[0];//存储2字节
                in.readRawData((char*)data1,1);//再读1字节

                if(data1[0]<=0x7f)//检查是否为时间差的最后一个字节     //有四个字节时，数据在buffer[0]buffer[1]buffer[2]buffer[3]
                {
                    buffer[0]=data1[0];
                    qDebug()<<"当前时间差4字节";
                }
                else
                {
                    qDebug()<<"时间差计算有误:第4字节大于0x7f";
                    readErrorMark=1;
                    return;//直接返回
                }
             }
       }
   }
    //最后来计算时间差
   deltaTime=(quint32)buffer[0]+ //注意buffer[0]是最低字节，最高位为0，本来就小于128，所以不用减128
                       (quint32)(buffer[1]-128)*128+//buffer[1]减128是因为其最高位为1
                       (quint32)(buffer[2]-128)*128*128+//时间差不可能是负值，所以用无符号，还有有符号向无符号转换有些问题
                       (quint32)(buffer[3]-128)*128*128*128;
    qDebug()<<"当前时间差是"<<deltaTime;
}
void read_midi::read_cmd_0x00_0x7F(QDataStream &in)
{
    qDebug()<<"省略操作指令：直接写音符";
    int b;//临时存储计算出音符在哪一行（列直接用read_note_X）
    float x;//临时存储x方向的相对节点位移      //这里x可能是32位？
    x=deltaTime/((float)DDT);//x：占多少个四分音符
    qDebug()<<"当前相对节点位移x是"<<4*x;

    read_note_X=read_note_X+4*x;//现在定义两个节点距为一个四分音符x=1，则一个节点距为一个8分音符x=0.5,16音符等一下考虑
    b=95-data1[0];//如果要更改音域宽度这里就要改!

    in.readRawData((char*)data1,1);//再读1字节
    if(b>=60||b<0)//如果超过音域宽度的音符就忽略掉  //如果要更改音域宽度这里就要改!
    {
        qDebug()<<"音符不在C3-C7";
        return;
    }
    if(data1[0]==0x00)
    {
                     //力度为0什么也不做：代表这个音符结束
    }
    else if(data1[0]>0x00&&data1[0]<=0x7f)//力度在1-127
    {
//        m->point[b][read_note_X].noteFlag=1;//力度大于0表示一个音符开始，noteflag置1//用发送出去的数据置1
        emit point_move_signal(b,(int)read_note_X);//移动第b行read_note_X列，把数据发送出去
    }
    else
    {
          qDebug()<<"错误：力度不在0-127之间";
          readErrorMark=1;
          return;
    }
}
void read_midi::read_cmd_0x8x(QDataStream &in)
{
    qDebug()<<"操作指令识别：0x8x松开";
    in.readRawData((char*)data1,1);//再读1字节

    float x;//临时存储x方向的相对节点位移      //这里x可能是32位？
    x=deltaTime/((float)DDT);//x：占多少个四分音符
    qDebug()<<"当前相对节点位移x是"<<4*x;
    read_note_X=read_note_X+4*x;//现在定义四个节点距为一个四分音符x=1，则一个节点距为一个8分音符x=0.5,16分音符等一下考虑

    in.readRawData((char*)data1,1);//再读1字节：吞掉 力度 字节（因为没什么用）
    //错误：力度不在0-127之间
    if(data1[0]>0x7f)
    {
        qDebug()<<"错误：力度不在0-127之间";
        readErrorMark=1;
        return;
    }
}
void read_midi::read_cmd_0x9x(QDataStream &in)
{
    qDebug()<<"操作指令识别：0x9x按下";
    in.readRawData((char*)data1,1);//再读1字节:音符号

    int b;//临时存储计算出哪个对象应该被按下
    float x;//临时存储x方向的相对节点位移      //这里x可能是32位？
    x=deltaTime/((float)DDT);//x：占多少个四分音符
    qDebug()<<"当前相对节点位移x是"<<4*x;

    read_note_X=read_note_X+4*x;//现在定义两个节点距为一个四分音符x=1，则一个节点距为一个8分音符x=0.5,16分音符等一下考虑

    b=95-data1[0];//如果要更改音域宽度这里就要改!


    if(b>=60||b<0)//如果超过音域宽度的音符就忽略掉  //如果要更改音域宽度这里就要改!
    {
        in.readRawData((char*)data1,1);//吞掉 力度
        qDebug()<<"音符不在C3-C7";
        return;
    }

//    m->point[b][read_note_X].noteFlag=1;//力度大于0表示一个音符开始，noteflag置1 //用发送出去的数据置1
    emit point_move_signal(b,(int)read_note_X);//移动第b行read_note_X列，把数据发送出去

    in.readRawData((char*)data1,1);//吞掉 力度
    //错误：力度不在0-127之间
    if(data1[0]>0x7f)
    {
        qDebug()<<"错误：力度不在0-127之间";
        readErrorMark=1;
        return;
    }
}
void read_midi::read_cmd_0xAx(QDataStream &in)
{
    in.readRawData((char*)data2,2);
}
void read_midi::read_cmd_0xBx(QDataStream &in)
{
    in.readRawData((char*)data2,2);
}
void read_midi::read_cmd_0xCx(QDataStream &in)
{
    in.readRawData((char*)data1,1);
}
void read_midi::read_cmd_0xDx(QDataStream &in)
{
    in.readRawData((char*)data1,1);
}
void read_midi::read_cmd_0xEx(QDataStream &in)
{
    in.readRawData((char*)data2,2);
}
void read_midi::read_cmd_0xFO(QDataStream &in)
{
    qDebug()<<"警告：出现了0xf0系统码";
    in.readRawData((char*)data1,1);

    //读取到0xf7结束
    while(data1[0]!=0xf7)
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x00(QDataStream &in)
{
    in.readRawData((char*)data3,3);
}
void read_midi::read_cmd_0XFF_0x01(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x02(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x03(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x04(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x05(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x06(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x07(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x08(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x09(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }
}
void read_midi::read_cmd_0XFF_0x20(QDataStream &in)
{
    in.readRawData((char*)data2,2);
}
void read_midi::read_cmd_0XFF_0x21(QDataStream &in)
{
    in.readRawData((char*)data2,2);
}
void read_midi::read_cmd_0XFF_0x2F(QDataStream &in)
{
    in.readRawData((char*)data1,1);
    if(data1[0]==0x00)
    {
        QString sta=QString("执行00 ff 2f 00指令，结束第%1次mtrk循环：").arg(a_mtrk+1);
        qDebug()<<sta;
    }
    else
    {
        qDebug()<<"结束指令有误：应该为0x00";
        readErrorMark=1;
        return;
    }
}
void read_midi::read_cmd_0XFF_0x51(QDataStream &in)
{
    in.readRawData((char*)data1,1);//去掉“03”字节
    in.readRawData((char*)data3,3);
    tempo=data1[0]*256*256+data1[1]*256+data1[2];
    qDebug()<<"设置速度tempo：当前4分音符的时间为："<<tempo;
}
void read_midi::read_cmd_0XFF_0x54(QDataStream &in)
{
    in.readRawData((char*)data1,1);//去掉“05”字节
    in.readRawData((char*)data4,4);
}
void read_midi::read_cmd_0XFF_0x58(QDataStream &in)
{
    qDebug()<<"设置节拍";
    in.readRawData((char*)data1,1);//去掉“04”字节
    in.readRawData((char*)data4,4);
    //第一个参数是分子：每小节拍数（bar：每小节）
    bar=data1[0];
    //第二个参数是分母：以？音符为一拍（beat：节拍）
    beat=pow(2,data1[1]);
    qDebug()<<"当前节拍设置是"<<bar<<beat;
    //后面两个参数被忽略
}
void read_midi::read_cmd_0XFF_0x59(QDataStream &in)
{
    //读取字节3字节，因为还有字节数字节02
     in.readRawData((char*)data3,3);//大小调是个被忽略的参数，所以什么也不干
     qDebug()<<"设置调号";
}
void read_midi::read_cmd_0XFF_0x7F(QDataStream &in)
{
    delta_time(in);//这里将时间差算法计算动态字节
    qDebug()<<"将要读取的动态字节数："<<deltaTime;
    for(quint32 a=0;a<deltaTime;a++)//消耗多余字节将来再改
    {
        in.readRawData((char*)data1,1);
    }

    qDebug()<<"警告：0x7f特定信息";
}
void read_midi::Mthd_check(QDataStream &in)
{
    //判断是否为Mthd标志（0x4d 0x54 0x68 0x64）
    in.readRawData((char*)data4,4);
    if(data4[0]==0x4d&&data4[1]==0x54&&data4[2]==0x68&&data4[3]==0x64)
    {
        qDebug()<<"mthd验证成功";
    }
    else
    {
        qDebug()<<"mthd验证失败";
        return;//直接返回
        readErrorMark=1;
    }

    //检验00 00 00 06字节
    in.readRawData((char*)data4,4);
    if(data4[0]==0x00&&data4[1]==0x00&&data4[2]==0x00&&data4[3]==0x06)
    {
        qDebug()<<"检验00 00 00 06字节验证成功";
    }
    else
    {
        qDebug()<<"检验00 00 00 06字节验证验证失败";
        return;//直接返回
        readErrorMark=1;
    }

    //检验ff ff字节:音轨格式
    in.readRawData((char*)data2,2);
    if(data4[0]==0x00&&data4[1]==0x01)//这个是用的最多的
    {
        qDebug()<<"当前音轨：同步 多音轨";
    }
    else if(data4[0]==0x00&&data4[1]==0x00)
    {
        qDebug()<<"当前音轨：单音轨";
    }
    else if(data4[0]==0x00&&data4[1]==0x02)
    {
        qDebug()<<"当前音轨：异步多音轨";
    }
    else
    {
        qDebug()<<"音轨检测错误";
        return;//直接返回
        readErrorMark=1;
    }

    //检验音轨数量: nn nn
    in.readRawData((char*)data2,2);

    if(data2[0]==0x00)//这个是用的最多的  //这里还是有些问题：音轨数最多是多少？
    {
        num_track=data2[1];
        qDebug()<<"当前音轨数量"<<num_track;
    }
    else
    {
        qDebug()<<"错误：音轨数量超过15";
        return;//直接返回
        readErrorMark=1;
    }

    //检验分区: dd dd (DTT:delta-time ticks微小时间事件片))//四分音符所占tick？
    in.readRawData((char*)data2,2);
    DDT=data2[0]*256+data2[1];
    qDebug()<<"当前DDT："<<DDT;
}
void read_midi::Mtrk_check(QDataStream &in)
{
    //检测音轨头标志：mtrk
    in.readRawData((char*)data4,4);
    if(data4[0]==0x4d&&data4[1]==0x54&&data4[2]==0x72&&data4[3]==0x6b)
    {
        qDebug()<<"Mtrk音轨头校验成功";
    }
    else
    {
        QString str=QString("Mtrk第%1个音轨头校验失败:可能是前一个音轨块字节超出所定义的（即mtrkByte的值不对）").arg(a_mtrk+1);
        qDebug()<<str;
        return;//直接返回
        readErrorMark=1;
    }

    //不检测音轨字节数量，因为有结束指令
    in.readRawData((char*)data4,4);
    quint32 mtrkByte=data4[0]*256*256*256+data4[1]*256*256+data4[2]*256+data4[3];
    qDebug()<<"本次Mtrk音轨字节数量:"<<mtrkByte;
}
