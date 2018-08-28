#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <baseClassWgt.h>
#include <QMovie>
#include <api/myMessageBox.h>

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
private:
    Ui::mainWindow *ui;

    void init();     //初始化
    void initIco();
    void initData();
    void initSystemTime();

    QMovie *m_chain_01, *m_chain_02, *m_chain_03, *m_chain_04, *m_chain_05, *m_chain_06,*m_chain_07, *m_chain_08, *m_chain_09, *m_chain_10, *m_chain_11, *m_chain_12;
    QPixmap *m_ON_status,*m_OFF_status,*m_Forward,*m_Reversal;
    int m_iIndex;
    bool m_bKeyLock;

    QTimer *m_timer;
};

#endif // MAINWINDOW_H
