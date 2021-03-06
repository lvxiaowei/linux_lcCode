#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <baseClassWgt.h>
#include <QMovie>
#include "ui_mainWindowPop.h"
#include "api/AlarmBox.h"

namespace Ui {
class mainWindow;
}

class mainWindow : public baseClassWgt
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

    void addChildWgt(QWidget *w);  //主窗口添加子窗口
    void keyPressEvent(int key);  //处理串口数据
    void initShowFrmConfig();
    void handleXddpData(QByteArray data);
public slots:
    void writeToXddp();
    void timerUpDate();
    void marqueeTitle();
    void focusAirValveChanged();
private:
    Ui::mainWindow *ui;
    Ui::mainWindowPop ui_pop;    /*运行界面的弹出界面对应的窗口*/
    QDialog *w;                  /*运行界面的弹出界面对应的窗口*/

    AlarmBox *m_AlarmBox;
    void init();     //初始化
    void initIco();
    void initData();
    void initSystemTime();
    void processingPopup(int key);
    void keyPressEventInputBox(int key);
    void keyPressEventPopSet(int key);
    void keyPressEventPopSet_cut(int key);
    void keyPressEventPopSet_airValve(int key);
    void keyPressEventPopSet_airFeeder(int key);
    bool readAirValveConfig();
    void addAlarmRecord(QString str);
    void initCutterTable();
    void initManualTable();

    void writeToXddp(const int operType, QString operMode="");  /*向XDDP发送数据*/
    /*16个宏处理的函数*/
    typedef void (mainWindow::*myfun)();
    QMap<int, myfun> m_mapFun;
    void macroFun_Reset();       //01 强制初始状态
    void macroFun_YFALLOUT();    //02 梭子全出
    void macroFun_CutterSet();   //03 剪刀抬起
    void macroFun_PatSetStop();  //04 选针器停止
    void macroFun_TakeDown();    //05 牵拉
    void macroFun_AirFeeder();   //06 进线吹气
    void macroFun_ManualCmd();   //07 气阀命令
    void macroFun_BackLight();   //08 背光
    void macroFun_MiniCycle();   //09 快编
    void macroFun_EndCycle();    //10 单只自停
    void macroFun_LowSpeed();    //11 低速
    void macroFun_ChainBlock();  //12 空转
    void macroFun_Zeroing();     //13 归零
    void macroFun_FanOnOff();    //14 吸风马达
    void macroFun_ParaMod();     //15 密度/速度/圈数
    void macroFun_Oiler();       //16 加油

    QMovie *m_chain_01, *m_chain_02, *m_chain_03, *m_chain_04, *m_chain_05, *m_chain_06,*m_chain_07, *m_chain_08, *m_chain_09, *m_chain_10, *m_chain_11, *m_chain_12;
    QPixmap *m_ON_status,*m_OFF_status,*m_Forward,*m_Reversal;
    int m_iIndex;
    int m_iAirFeederIndex;
    bool m_bKeyLock;
    bool m_isRunning;
    QStringList m_lstAirValve;              //配置文件的气阀名称；
    QString m_strTitle;
    QTimer *m_timer;

    QJsonArray m_arryYF, m_arryTake, m_arryAirFeefer, m_arryFan, m_arryOiler, m_arryCutter;
    QBitArray m_bitValveStates, m_bitMacroState, m_bitAirFeederState;

    QStringList lst_segment;
    QList<QMovie*> lst_move;
};

#endif // MAINWINDOW_H
