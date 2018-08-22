#ifndef FRM_MENU_H
#define FRM_MENU_H

#include <baseClassWgt.h>

namespace Ui {
class Frm_menu;
}

class Frm_settingMenu : public baseClassWgt
{
    Q_OBJECT

public:
    explicit Frm_settingMenu(QWidget *parent = 0);
    ~Frm_settingMenu();

    void keyPressEvent(int key);
    void initShowFrmConfig();
private:
    Ui::Frm_menu *ui;
};

#endif // FRM_MENU_H
