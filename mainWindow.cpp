#include "mainWindow.h"
#include "ui_mainWindow.h"

mainWindow::mainWindow(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::mainWindow)
{
    g_mapIndexToWgt[PAGE_RUNNING] = this;
    ui->setupUi(this);
    init();
}

mainWindow::~mainWindow()
{
    delete ui;
}

//初始化
void mainWindow::init()
{
    initIco();
    initData();
}

//数据初始化
void mainWindow::initData()
{
    g_lstRightButton.clear();
    g_lstRightButton <<ui->m_btnRight_1 <<ui->m_btnRight_2 <<ui->m_btnRight_3 <<ui->m_btnRight_4 <<ui->m_btnRight_5 <<ui->m_btnRight_6;

    g_pStackedWgt = ui->m_stackedWidget;

    /*新建定时器*/
    m_timer = new QTimer(this);

    /*设置系统时间控件*/
    initSystemTime();

    setNextDealWgt(PAGE_RUNNING);

}

//图标初始化
void mainWindow::initIco()
{
    m_ON_status  = new QPixmap(":/image/start.png");             //运行状态；
    m_OFF_status = new QPixmap(":/image/stop.png");              //停止状态；

    ui->m_pngRunState->setPixmap(*m_OFF_status);

    /*编织位置的显示图片*/
    m_chain_01  = new QMovie(":/image/chain_01.gif");
    m_chain_02  = new QMovie(":/image/chain_02.gif");
    m_chain_03  = new QMovie(":/image/chain_03.gif");
    m_chain_04  = new QMovie(":/image/chain_04.gif");
    m_chain_05  = new QMovie(":/image/chain_05.gif");
    m_chain_06  = new QMovie(":/image/chain_06.gif");
    m_chain_07  = new QMovie(":/image/chain_07.gif");
    m_chain_08  = new QMovie(":/image/chain_08.gif");
    m_chain_09  = new QMovie(":/image/chain_09.gif");
    m_chain_10  = new QMovie(":/image/chain_10.gif");
    m_chain_11  = new QMovie(":/image/chain_11.gif");
    m_chain_12  = new QMovie(":/image/chain_12.gif");

    ui->m_gifKInt->setMovie(m_chain_01);
    m_chain_01->start();
}

/*系统时间控件定时刷新*/
void mainWindow::initSystemTime()
{
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    m_timer->start(100);
}

/*系统时间控件定时刷新*/
void mainWindow::timerUpDate()
{
    QDateTime QDateCurentTime= QDateTime::currentDateTime();//获取系统现在的时间
    QString strCurentTime = QDateCurentTime.toString("yyyy-MM-dd hh:mm:ss dddd"); //设置显示格式
    ui->m_labSysTime->setText(strCurentTime);//在标签上显示时间
}

void mainWindow::addChildWgt(QWidget *w)
{
    ui->m_stackedWidget->addWidget(w);
}

/*处理串口数据*/
void mainWindow::keyPressEvent(int key)
{
    qDebug()<<"当前正在运行界面,key="<<key;

    /*键盘锁定不允许操作*/
    if(m_bKeyLock && key != Key_PageUp) return;

    switch (key) {
    case Key_F9:
    {
        myMessageBox::getInstance()->setMessage(tr("确定转到设置界面么?"), BoxQuesion);
        m_iIndex=1;
        break;
    }
    case Key_F8:
    {
        myMessageBox::getInstance()->setMessage(tr("确定转到测试界面么?"), BoxQuesion);
        m_iIndex=2;
        break;
    }
    case Key_9:

        break;
    case Key_8:

        break;
    case Key_7:

        break;
    case Key_PageUp:
    {
        m_bKeyLock = !m_bKeyLock;
        ui->m_btnKeyLock->setEnabled(m_bKeyLock);
        g_lstRightButton.at(5)->setText(m_bKeyLock ? tr("键盘解锁"):tr("键盘锁定"));
        break;
    }


    case Key_plus:
    {
        if(myMessageBox::getInstance()->isVisible())
        {
            m_iIndex==1 ? setNextDealWgt(PAGE_SETTING):setNextDealWgt(PAGE_TESTINGMENU);
            myMessageBox::getInstance()->hide();
        }
        break;
    }
    case Key_minus:
        myMessageBox::getInstance()->hide();



    default:
        break;
    }
}

/*数据初始化*/
void mainWindow::initShowFrmConfig()
{
    ui->m_stackedWidget->setCurrentIndex(0);
    freshRightButtonContent(QStringList()<<tr("设置")<<tr("测试")<<tr("")<<tr("")<<tr("")<<tr("键盘锁定"));
    m_bKeyLock = false;
}
