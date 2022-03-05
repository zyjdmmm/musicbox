#ifndef NOTE_STRETCH_H
#define NOTE_STRETCH_H

#include <QWidget>
#include"music_window.h"

class note_stretch : public QWidget
{
    Q_OBJECT
public:
    explicit note_stretch(QWidget *parent = nullptr);
    void getpoint(music_window *);//获得point数据
private:
    music_window *m;//获得point数据

signals:


public slots:
    void note_expand();//音符间距扩大一倍
    void note_shrink();//音符间距缩小一倍
};

#endif // NOTE_STRETCH_H
