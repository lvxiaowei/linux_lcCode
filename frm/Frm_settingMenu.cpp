#include "Frm_settingMenu.h"
#include "ui_Frm_settingMenu.h"

Frm_settingMenu::Frm_settingMenu(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_menu)
{
    ui->setupUi(this);
}

Frm_settingMenu::~Frm_settingMenu()
{
    delete ui;
}

/*处理串口数据*/
void Frm_settingMenu::keyPressEvent(int key)
{
    qDebug()<<"当前正在菜单界面,key="<<key;
    switch (key) {
    case Key_F0:
        setNextDealWgt(PAGE_RUNNING);
        break;
    case Key_F1:
        setNextDealWgt(PAGE_CHAINPROCESSING);
        break;
    case Key_F2:
        setNextDealWgt(PAGE_PATTERNMANAGE);
        break;
    case Key_F3:
        setNextDealWgt(PAGE_TIMINGS);
        break;
    case Key_F4:

        break;
    case Key_F5:
        setNextDealWgt(PAGE_PARASETTING);
        break;
    default:
        break;
    }
}

/*数据初始化*/
void Frm_settingMenu::initShowFrmConfig()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("链条管理")<<tr("花型管理")<<tr("梭子设置")<<tr("")<<tr("设定"));
}
