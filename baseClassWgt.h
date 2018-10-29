#ifndef BASECLASSWGT_H
#define BASECLASSWGT_H

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QDebug>
#include "api/myhelper.h"
#include "api/publicData.h"
#include "api/comConfigClass.h"
#include "api/DBProcessingClass.h"
#include "api/myMessageBox.h"

/*操作界面*/
enum operatePage
{
    PAGE_RUNNING =0 ,     //运行界面
    PAGE_SETTING,         //设置菜单界面
    PAGE_MSGBOX,          //消息弹出框
    PAGE_CHAINPROCESSING, //链条管理
    PAGE_TESTINGMENU,     //测试菜单界面
    PAGE_PATTERNMANAGE,    //花型管理界面
    PAGE_TIMINGS,           //时序管理界面
};

/*键盘key值对应表*/
enum Key {
    Key_0      = 48,
    Key_1      = 49,
    Key_2      = 50,
    Key_3      = 51,
    Key_4      = 52,
    Key_5      = 53,
    Key_6      = 54,
    Key_7      = 55,
    Key_8      = 56,
    Key_9      = 57,
    Key_PageDn = 22,
    Key_Up     = 23,
    Key_PageUp = 24,
    Key_Right  = 25,
    Key_Set    = 32,
    Key_Left   = 33,
    Key_Esc    = 34,
    Key_Down   = 35,
    Key_F0     = 16,
    Key_F1     = 17,
    Key_F2     = 18,
    Key_F3     = 19,
    Key_F4     = 20,
    Key_F5     = 21,
    Key_F6     = 86,
    Key_F7     = 87,
    Key_F8     = 88,
    Key_F9     = 89,
    Key_Ins    = 91,
    Key_Del    = 90,
    Key_plus   = 93,
    Key_minus  = 92,
    Key_FF     = 255
};

class baseClassWgt : public QWidget
{
    Q_OBJECT

public:
    explicit baseClassWgt(QWidget *parent = 0);
    ~baseClassWgt();

    virtual void keyPressEvent(int key=0){Q_UNUSED(key);}
    virtual void initShowFrmConfig(){}

    void freshRightButtonContent(QStringList lstStr);
    void setNextDealWgt(int index);

    QMap<int,int> m_mapNoKeyToValue;
    QMap<int,QKeyEvent> m_mapKeyToKeyboard;
    static QList<QPushButton*> g_lstRightButton;     //界面右侧操作按钮的lst
    static QStackedWidget* g_pStackedWgt;
    static QMap<int, baseClassWgt*> g_mapIndexToWgt;
    static baseClassWgt* g_pCurentDealWgt;

    bool getXmlConfig(QDomDocument& document);
    void setObjProperty(QWidget* pwgt, const char *name, const QVariant &value);
signals:
    void serialDataToScheduler(QByteArray data);
    void xddpDataToScheduler(QByteArray data);

};

#endif // BASECLASSWGT_H
