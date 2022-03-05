#ifndef EXPORT_CHECK_BTN_H
#define EXPORT_CHECK_BTN_H

#include <QRadioButton>//1

namespace Ui {
class export_check_btn;
}

class export_check_btn : public QRadioButton//2
{
    Q_OBJECT

public:
    explicit export_check_btn(QWidget *parent = 0);
    ~export_check_btn();

private:
    Ui::export_check_btn *ui;
};

#endif // EXPORT_CHECK_BTN_H
