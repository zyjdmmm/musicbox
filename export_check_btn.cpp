#include "export_check_btn.h"
#include "ui_export_check_btn.h"

export_check_btn::export_check_btn(QWidget *parent) :
    QRadioButton(parent),//3
    ui(new Ui::export_check_btn)
{
    ui->setupUi(this);



}
//QRadioButton
export_check_btn::~export_check_btn()
{
    delete ui;
}
