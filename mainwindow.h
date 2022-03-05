#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<central_w.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    central_w *centralWidget;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:


    Ui::MainWindow *ui;
public slots:
    void export_check_btn_set_15();//设置15音
    void export_check_btn_set_20();//设置20音
    void export_check_btn_set_30();//设置30音
};

#endif // MAINWINDOW_H
