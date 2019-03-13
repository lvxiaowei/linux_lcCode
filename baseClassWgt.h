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
    PAGE_PATTERNMANAGE,   //花型管理界面
    PAGE_TIMINGS,         //时序管理界面
    PAGE_PARASETTING,     //参数设置
};

/*键盘key值对应表*/
enum Key1 {
    Key_0      = 23,
    Key_1      = 38,
    Key_2      = 39,
    Key_3      = 40,
    Key_4      = 35,
    Key_5      = 36,
    Key_6      = 37,
    Key_7      = 30,
    Key_8      = 31,
    Key_9      = 32,
    Key_plus   = 22,
    Key_minus  = 24,

    Key_Up     = 28,
    Key_Down   = 20,
    Key_Left   = 19,
    Key_Right  = 21,
    Key_Set    = 27,
    Key_Esc    = 29,

    Key_F0     = 34,
    Key_F1     = 33,
    Key_F2     = 26,
    Key_F3     = 25,
    Key_F4     = 18,
    Key_F5     = 17,

    Key_Fun0   = 9,
    Key_Fun1   = 10,
    Key_Fun2   = 11,
    Key_Fun3   = 12,
    Key_Fun4   = 13,
    Key_Fun5   = 14,
    Key_Fun6   = 15,
    Key_Fun7   = 16,
    Key_Fun8   = 1,
    Key_Fun9   = 2,
    Key_Fun10  = 3,
    Key_Fun11  = 4,
    Key_Fun12  = 5,
    Key_Fun13  = 6,
    Key_Fun14  = 7,
    Key_Fun15  = 8,

    Key1_FF     = 255
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

    QKeyEvent *key_0, *key_1, *key_2, *key_3 , *key_4, *key_5, *key_6, *key_7, *key_8, *key_9,
    *key_plus, *key_minus, *key_up, *key_down, *key_left, *key_right;

    bool getXmlConfig(QDomDocument& document);
    void setObjProperty(QWidget* pwgt, const char *name, const QVariant &value);
signals:
    void serialDataToScheduler(QByteArray data);
    void xddpDataToScheduler(QByteArray data);
    void initNewWgt(int index);

};

#endif // BASECLASSWGT_H
