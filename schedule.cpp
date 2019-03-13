#include "schedule.h"

schedule::schedule(QWidget *parent)
    : QWidget(parent)
{
    bIsLock = true;
    lightUpOrOffScreen();

    m_pMainWindow = new mainWindow();
    m_pMainWindow->show();

    connect(m_pMainWindow, SIGNAL(initNewWgt(int)), this, SLOT(initNewWgt(int)));
    connect(m_pMainWindow, SIGNAL(serialDataToScheduler(QByteArray)),this,SLOT(writeToSerial(QByteArray))); /*向串口发送数据*/
    connect(m_pMainWindow, SIGNAL(xddpDataToScheduler(QByteArray)),this,SLOT(writeToXddp(QByteArray)));     /*向管理口发送数据*/

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
        {
            baseClassWgt::g_pCurentDealWgt->keyPressEvent(iValue);
        }
        break;
    }
    }
}

/*处理XDDP发过来的数据*/
void schedule::handleXddpData(QByteArray data)
{
    qDebug()<<"处理XDDP发过来的数据:"<<data;

    //处理测试界面相关的消息
    if(data.contains("working_state") || data.contains("func_key") || data.contains("alarm"))
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

/*新增界面*/
void schedule::initNewWgt(int index)
{
    baseClassWgt* pWgtFrm=NULL;
    qDebug()<<"initNewWgt";
    switch (index) {
    case PAGE_SETTING:
    {
        m_frmSettingMenu = new Frm_settingMenu();
        m_pMainWindow->addChildWgt(m_frmSettingMenu);
        pWgtFrm = m_frmSettingMenu;
        break;
    }
    case PAGE_CHAINPROCESSING:
    {
        m_frmChain = new Frm_chainProcessingClass();
        m_pMainWindow->addChildWgt(m_frmChain);
        pWgtFrm = m_frmChain;
        break;
    }
    case PAGE_TESTINGMENU:
    {
        m_frmTestMenu = new Frm_testingMenu();
        m_pMainWindow->addChildWgt(m_frmTestMenu);
        pWgtFrm = m_frmTestMenu;
        break;
    }
    case PAGE_PATTERNMANAGE:
    {
        m_frmPatternManage = new Frm_patternManage();
        m_pMainWindow->addChildWgt(m_frmPatternManage);
        pWgtFrm = m_frmPatternManage;
        break;
    }
    case PAGE_TIMINGS:
    {
        m_frmTimings = new Frm_timingTable();
        m_pMainWindow->addChildWgt(m_frmTimings);
        pWgtFrm = m_frmTimings;
        break;
    }
    case PAGE_PARASETTING:
    {
        m_frmParaSetting = new Frm_parameterSettings();
        m_pMainWindow->addChildWgt(m_frmParaSetting);
        pWgtFrm = m_frmParaSetting;
        break;
    }
    default:
        break;
    }

    if(NULL != pWgtFrm)
    {
        baseClassWgt::g_mapIndexToWgt[index]= pWgtFrm;
        baseClassWgt::g_pStackedWgt->setCurrentWidget(pWgtFrm);
        baseClassWgt::g_pCurentDealWgt = (baseClassWgt*)pWgtFrm;
        pWgtFrm->initShowFrmConfig();
        connect(pWgtFrm,SIGNAL(serialDataToScheduler(QByteArray)),this,SLOT(writeToSerial(QByteArray))); /*向串口发送数据*/
        connect(pWgtFrm,SIGNAL(xddpDataToScheduler(QByteArray)),this,SLOT(writeToXddp(QByteArray)));     /*向管理口发送数据*/
        connect(pWgtFrm, SIGNAL(initNewWgt(int)), this, SLOT(initNewWgt(int)));
    }
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
