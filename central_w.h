#ifndef CENTRAL_W_H
#define CENTRAL_W_H

#include <QWidget>
#include<QScrollArea>
#include"music_window.h"
#include"key_board.h"
#include"export_check_btn.h"

namespace Ui {
class central_w;
}

class central_w : public QWidget
{
    Q_OBJECT

public:
    QScrollArea *area;//交给mainwindow设定窗口尺寸
    music_window *musicWindow;//交给mainwindow设定窗口尺寸
    export_check_btn *exportCheckBtn[60];//60个exportCheckBtn

    explicit central_w(QWidget *parent = 0);
    ~central_w();

private:

    key_board *keyBoard_octave[5];//5组八度的声明,这里从keyBoard_octave[0]到keyBoard_octave[4]
    int key_board_excursion;//键盘的图像偏移以便对齐:取节点距的一半
    int export_check_btn_excursion=-10;//检查按钮的偏移

    Ui::central_w *ui;

public slots:
    void key_board_move(int);//根据滚动条的值移动键盘
    void export_check_btn_move(int);//根据滚动条的值移动检查按钮

};

#endif // CENTRAL_W_H
