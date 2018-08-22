#include "Frm_settingMenu.h"
#include "ui_Frm_settingMenu.h"

Frm_settingMenu::Frm_settingMenu(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_menu)
{
    g_mapIndexToWgt[PAGE_SETTING]= this;
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
    case Key_F9:
        setNextDealWgt(PAGE_RUNNING);
        break;
    case Key_F8:
        setNextDealWgt(PAGE_CHAINPROCESSING);
        break;
    case Key_9:
        qDebug()<<"1111";
         setNextDealWgt(PAGE_TIMINGS);
         qDebug()<<"2222";
        break;
    case Key_8:

        break;
    case Key_7:
        setNextDealWgt(PAGE_PATTERNMANAGE);
        break;
    case Key_PageUp:

        break;
    default:
        break;
    }
}

/*数据初始化*/
void Frm_settingMenu::initShowFrmConfig()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("链条编辑")<<tr("梭子设置")<<tr(" ")<<tr("花型管理")<<tr("设定"));
}
