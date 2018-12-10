#include "schedule.h"

schedule::schedule(QWidget *parent)
    : QWidget(parent)
{
    bIsLock = true;
    lightUpOrOffScreen();

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

    m_frmParaSetting = new Frm_parameterSettings();
    m_pMainWindow->addChildWgt(m_frmParaSetting);
    /*将几个窗口放在加入到 lst 中*/
    lstWgtFrm.clear();
    lstWgtFrm <<m_pMainWindow <<m_frmChain <<m_frmSettingMenu <<m_frmTestMenu <<m_frmPatternManage<<m_frmTimings<<m_frmParaSetting;

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

    switch (iValue) {
    case Key_Fun7:
        lightUpOrOffScreen();
        break;
    default:
    {
        if(!bIsLock)
            baseClassWgt::g_pCurentDealWgt->keyPressEvent(iValue);
        break;
    }
    }
}

/*处理XDDP发过来的数据*/
void schedule::handleXddpData(QByteArray data)
{
    //    qDebug()<<"处理XDDP发过来的数据:"<<data;

    //处理测试界面相关的消息
    if(data.contains("working_state") || data.contains("func_key"))
    {
        m_pMainWindow->handleXddpData(data);

    }
    else if(data.contains("_test"))
    {
        m_frmTestMenu->handleXddpData(data);
    }
    else if(data.contains("\"zeroing\""))
    {
        m_frmParaSetting->handleXddpData(data);
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
    qDebug()<<"发送给XDDP的数据:"<<data;
}

/*点亮或熄灭屏幕*/
void schedule::lightUpOrOffScreen()
{
    int lightLv;
    if(bIsLock == false)
    {
        lightLv = 0;
        bIsLock = true;
    }
    else
    {
        lightLv = 7;
        bIsLock = false;
    }

    QFile file(PATH_BACKLIGHT);
    if(file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Truncate))
    {
        QTextStream in(&file);
        in << lightLv;
        file.close();
    }
}
