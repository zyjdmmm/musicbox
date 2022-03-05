#ifndef key_board_H
#define key_board_H

#include <QWidget>

namespace Ui {
class key_board;
}

class key_board : public QWidget
{
    Q_OBJECT

public:
    key_board(int o);
    ~key_board();

private:
    Ui::key_board *ui;
};

#endif // key_board_H
