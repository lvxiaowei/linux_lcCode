#include "schedule.h"

schedule::schedule(QWidget *parent)
    : QWidget(parent)
{
    m_pMainWindow = new mainWindow();
    m_pMainWindow->show();

    m_frmChain = new Frm_chainProcessingClass();
    m_pMainWindow->addChildWgt(m_frmChain);

    m_frmSettingMenu = new Frm_settingMenu();
    m_pMainWindow->addChildWgt(m_frmSettingMenu);

    m_frmTestMenu = new Frm_testingMenu();
    m_pMainWindow->addChildWgt(m_frmTestMenu);

    m_frmPatternManage = new Frm_patternManage();
    m_pMainWindow->addChildWgt(m_frmPatternManage);

    m_frmTimings = new Frm_timingTable();
    m_pMainWindow->addChildWgt(m_frmTimings);
    /*将几个窗口放在加入到 lst 中*/
    lstWgtFrm.clear();
    lstWgtFrm <<m_pMainWindow <<m_frmChain <<m_frmSettingMenu <<m_frmTestMenu <<m_frmPatternManage<<m_frmTimings;

    foreach (QWidget* pWgtFrm, lstWgtFrm) {
        connect(pWgtFrm,SIGNAL(serialDataToScheduler(QByteArray)),this,SLOT(writeToSerial(QByteArray))); /*向串口发送数据*/
        connect(pWgtFrm,SIGNAL(xddpDataToScheduler(QByteArray)),this,SLOT(writeToXddp(QByteArray)));     /*向管理口发送数据*/
    }

//    baseClassWgt::g_pStackedWgt->setCurrentIndex(4);
}

schedule::~schedule()
{

}

/*处理串口发过来的数据*/
void schedule::handleSerialData(QByteArray data)
{
    bool ok;
    int iValue = data.toHex().toInt(&ok, 16);
    baseClassWgt::g_pCurentDealWgt->keyPressEvent(iValue);
}

/*处理XDDP发过来的数据*/
void schedule::handleXddpData(QByteArray data)
{
    qDebug()<<"处理XDDP发过来的数据:"<<data;

    //处理测试界面相关的消息
    if(data.contains("_test"))
    {
        m_frmTestMenu->handleXddpData(data);
    }
    Q_UNUSED(data);
}

/*向串口发送数据*/
void schedule::writeToSerial(QByteArray data)
{
    Q_UNUSED(data);
}

/*向XDDP发送数据*/
void schedule::writeToXddp(QByteArray data)
{
    emit toXddpData(data);
}
