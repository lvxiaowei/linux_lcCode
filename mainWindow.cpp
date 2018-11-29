#include "mainWindow.h"
#include "ui_mainWindow.h"

mainWindow::mainWindow(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::mainWindow)
{
    g_mapIndexToWgt[PAGE_RUNNING] = this;
    ui->setupUi(this);
    w = new QDialog(this);
    ui_pop.setupUi(w);

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

    /*初始化宏键和函数的映射关系*/
    m_mapFun[Key_F7]    = &mainWindow::macroFun_Reset;
    m_mapFun[Key_F6]    = &mainWindow::macroFun_YFALLOUT;
    m_mapFun[Key_F5]    = &mainWindow::macroFun_CutterSet;
    m_mapFun[Key_F4]    = &mainWindow::macroFun_PatSetStop;
    m_mapFun[Key_F3]    = &mainWindow::macroFun_TakeDown;
    m_mapFun[Key_F2]    = &mainWindow::macroFun_AirFeeder;
    m_mapFun[Key_F1]    = &mainWindow::macroFun_ManualCmd;
    m_mapFun[Key_F0]    = &mainWindow::macroFun_BackLight;
    m_mapFun[Key_Ins]   = &mainWindow::macroFun_MiniCycle;
    m_mapFun[Key_Del]   = &mainWindow::macroFun_EndCycle;
    m_mapFun[Key_6]     = &mainWindow::macroFun_LowSpeed;
    m_mapFun[Key_5]     = &mainWindow::macroFun_ChainBlock;
    m_mapFun[Key_4]     = &mainWindow::macroFun_Zeroing;
    m_mapFun[Key_Left]  = &mainWindow::macroFun_FanOnOff;
    m_mapFun[Key_Set]   = &mainWindow::macroFun_ParaMod;
    m_mapFun[Key_Right] = &mainWindow::macroFun_Oiler;

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
    m_Forward    = new QPixmap(":/image/RotationReversal.png");  //正转；
    m_Reversal   = new QPixmap(":/image/RotationForward.png");   //反转；
    ui->m_pngRunState->setPixmap(*m_OFF_status);
    ui->m_pngDirection->setPixmap(*m_Reversal);
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
    QString strCurentTime = QDateCurentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
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

    /*处理确认的弹出框窗口信息*/
    if(myMessageBox::getInstance()->isVisible())
    {
        processingPopup(key);
        return;
    }

    /*处理设置的弹出框信息*/
    if(w->isVisible())
    {
        keyPressEventPopSet(key);
        return;
    }
    switch (key) {
    case Key_F9:
    {
        myMessageBox::getInstance()->setMessage(tr("确定转到设置界面么?"), BoxQuesion);
        m_iIndex=pop_settting;
        break;
    }
    case Key_F8:
    {
        myMessageBox::getInstance()->setMessage(tr("确定转到测试界面么?"), BoxQuesion);
        m_iIndex=pop_testing;
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

    case Key_F7:
    case Key_F6:
    case Key_F5:
    case Key_F4:
    case Key_F3:
    case Key_F2:
    case Key_F1:
    case Key_F0:
    case Key_Ins:
    case Key_Del:
    case Key_6:
    case Key_5:
    case Key_4:
    case Key_Left:
    case Key_Set:
    case Key_Right:
    {
        if(m_isRunning)
        {
            myMessageBox::getInstance()->setMessage(tr("请先停止设备运行！"), BoxInfo);
            return;
        }

        if(m_mapFun.keys().contains(key))
            (this->*m_mapFun[key])();
        break;
    }

    default:
        break;
    }
}

/*数据初始化*/
void mainWindow::initShowFrmConfig()
{
    ui->m_stackedWidget->setCurrentIndex(0);
    freshRightButtonContent(QStringList()<<tr("设置")<<tr("测试")<<tr("")<<tr("")<<tr("")<<tr("键盘锁定"));
//        connect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
    m_bKeyLock = false;
    m_isRunning = false;
}

/*向XDDP发送数据*/
void mainWindow::writeToXddp()
{
    writeToXddp(runningState);
}

/*向XDDP发送数据*/
void mainWindow::writeToXddp(const int operType, QString operMode)
{
    QJsonObject json;
    json.insert("mesg_dir", "req");
    QJsonObject jsContent;
    if(!operMode.isEmpty())
        jsContent.insert("operMode", operMode);

    switch (operType) {
    case runningState:
    {
        json.insert("mesg_type", "working_state");
        break;
    }
    case macroFu_ManualCmd:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", macroFu_ManualCmd);
        jsContent.insert("airVaveNo", ui_pop.m_tabAirValve->currentItem()->text().toInt());
        break;
    }
    case macroFu_CutterSet:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", macroFu_CutterSet);
        jsContent.insert("airVaveNo", ui_pop.m_tabCut->item(ui_pop.m_tabCut->currentRow(), 3)->text().toInt());
        break;
    }
    default:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);
        break;
    }
    }
    json.insert("content", jsContent);
    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Indented);
    emit xddpDataToScheduler(byteArray);
}

/*处理XDDP数据*/
void mainWindow::handleXddpData(QByteArray data)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        if (doucment.isObject()) {
            QJsonObject object = doucment.object();
            if (object.contains("mesg_type")) {
                if (object.value("mesg_type").toString() != "working_state"
                        || object.value("mesg_dir").toString() != "ret") {
                    return;
                }
            }
            QJsonValue strVal = object.value("content");
            if(!strVal.isObject()) return;
            QJsonObject obj = strVal.toObject();

            ui->m_pngRunState->setPixmap(obj.value("state") == true ? *m_ON_status:*m_OFF_status);   //运行状态
            ui->m_pngDirection->setPixmap(obj.value("dir") == true? *m_Forward:*m_Reversal);      //电机转动方向
            ui->m_edtStep->setText(QString("%1/%2").arg(obj.value("step_no").toInt()).arg(obj.value("step_nr").toInt()));  //当前步号/总步数
            ui->m_edtNeedle->setText(QString("%1").arg(obj.value("needle_no").toInt())); //当前针号
            ui->m_edtCircle->setText(QString("%1/%2").arg(obj.value("rep_no").toInt()).arg(obj.value("rep_nr").toInt())); //当前圈数/总圈数
            //            ui->m_labStepTotal->setText(QString("%1").arg(obj.value("rep_nr").toInt()));//总循环数
            //            ui->m_labStepCurent->setText(QString("%1").arg(obj.value("rep_no").toInt()));//当前循环数
            //            ui->m_labRowTotal->setText(QString("%1").arg(obj.value("pattern_line_nr").toInt()));//花形总行数
            //            ui->m_labRowCrunt->setText(QString("%1").arg(obj.value("pattern_line_no").toInt()));//当前花形行号
            ui->m_edtOutput->setText(QString("%1/%2").arg(obj.value("volume_no").toInt()).arg(obj.value("volume_nr").toInt()));
            ui->m_edtChain->setText(QString("%1").arg(obj.value("chain_name").toString())); //当前激活链条名
            ui->m_edtPattern->setText(QString("%1").arg(obj.value("pattern_name").toString())); //当前激活花型名
            ui->m_edtSpeed->setText(QString("%1").arg(obj.value("speed").toInt())); //速度

            m_isRunning = (obj.value("state")==true ? true:false);
        }
    }
}

/*处理弹出框窗口信息*/
void mainWindow::processingPopup(int key)
{
    switch (key) {
    case Key_plus:
    {
        myMessageBox::getInstance()->hide();
        switch (m_iIndex) {
        case pop_settting:
            setNextDealWgt(PAGE_SETTING);
            disconnect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
            break;
        case pop_testing:
            setNextDealWgt(PAGE_TESTINGMENU);
            disconnect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
            break;
        case macroFu_Reset:
        {
            writeToXddp(macroFu_Reset);
            break;
        }
        default:
            break;
        }

        break;
    }
    case Key_minus:
        myMessageBox::getInstance()->hide();
    default:
        break;
    }
}

/*二级弹出框处理，包括剪刀设置等*/
void mainWindow::keyPressEventPopSet(int key)
{

    switch (ui_pop.stackedWidget->currentIndex()) {
    case 0:
        keyPressEventPopSet_cut(key);
        break;
    case 1:
        keyPressEventPopSet_airValve(key);
        break;
    default:
        break;
    }
}

/*剪刀设置*/
void mainWindow::keyPressEventPopSet_cut(int key)
{
    switch (key) {
    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui_pop.m_tabCut, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui_pop.m_tabCut, &keyPress);
        break;
    }
    case Key_Set:
    {
        writeToXddp(macroFu_CutterSet);
        break;
    }
    case Key_F5:
    {
        writeToXddp(macroFu_CutterSet, "out");
        w->hide();
    }

    default:
        break;
    }
}

/*气阀设置*/
void mainWindow::keyPressEventPopSet_airValve(int key)
{
    switch (key) {
    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, &keyPress);
        break;
    }
    case Key_Left:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, &keyPress);
        break;
    }
    case Key_Right:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, &keyPress);
        break;
    }
    case Key_Set:
    {
        writeToXddp(macroFu_ManualCmd);
        break;
    }
    case Key_F1:
    {
        writeToXddp(macroFu_ManualCmd, "out");
        w->hide();
    }

    default:
        break;
    }
}

//01 强制初始状态
void mainWindow::macroFun_Reset()
{
    myMessageBox::getInstance()->setMessage(tr("确定强制恢复初始状态么?"), BoxQuesion);
    m_iIndex = macroFu_Reset;
    qDebug()<<"-------------------------"<<"强制初始状态";
}

//02 梭子全出
void mainWindow::macroFun_YFALLOUT()
{

    if(m_mapEnable.keys().contains(macroFu_YFALLOUT))
    {
        m_mapEnable[macroFu_YFALLOUT] = !m_mapEnable[macroFu_YFALLOUT];
    }
    else {
        m_mapEnable[macroFu_YFALLOUT] = true;
    }

    writeToXddp(macroFu_YFALLOUT, m_mapEnable[macroFu_YFALLOUT] ? "true":"false");

    qDebug()<<"-------------------------"<<"梭子全出";
}

//03 剪刀抬起
void mainWindow::macroFun_CutterSet()
{
    /*初始化剪刀弹出表格*/
    QDomDocument document;
    if(!getXmlConfig(document))
        return;
    QDomNodeList nodeCut = document.elementsByTagName("cutter");
    QDomNodeList lstItem;
    if(nodeCut.count()!=0)
    {
        lstItem = nodeCut.at(0).childNodes();
    }

    /*初始化表格数据*/
    ui_pop.m_tabCut ->setRowCount(0);
    ui_pop.m_tabCut->clearContents();
    ui_pop.m_tabCut->setColumnCount(4); //设置总列数；
    ui_pop.m_tabCut->setRowCount(lstItem.count());//初始化总行数；
    ui_pop.m_tabCut->setColumnWidth(0,50); //设置第1列宽度；
    ui_pop.m_tabCut->setColumnWidth(1,477); //设置第2列宽度；
    ui_pop.m_tabCut->setColumnWidth(2,145); //设置第3列宽度；
    ui_pop.m_tabCut->setColumnWidth(3,0); //设置第4列宽度；
    ui_pop.m_tabCut->setHorizontalHeaderLabels(QStringList()<<tr("编号")<<tr("剪刀名称")<<tr("动作状态")<<""); //设置表头名称；

    QFont font=ui_pop.m_tabCut->horizontalHeader()->font();  //设置表头的字体为粗体；
    font.setBold(true);
    font.setPixelSize(20);
    ui_pop.m_tabCut->horizontalHeader()->setFont(font);

    ui_pop.m_tabCut->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui_pop.m_tabCut->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui_pop.m_tabCut->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));

    QFont fontContent;
    fontContent.setPointSize(12);
    ui_pop.m_tabCut->setFont(fontContent);

    ui_pop.m_tabCut->hasFocus();   //设置具有焦点；
    ui_pop.m_tabCut->selectRow(0); //设置默认选择行为第1行；

    for(int i=0;i<lstItem.count();i++)
    {
        ui_pop.m_tabCut->setItem(i,0,new QTableWidgetItem((QString::number(i))));
        ui_pop.m_tabCut->item(i,0)->setTextAlignment(Qt::AlignCenter);         //对齐方式；

        ui_pop.m_tabCut->setItem(i,1,new QTableWidgetItem(QString("  %1 %2").arg(tr("剪刀")).arg(lstItem.at(i).toElement().attribute("name"))));
        ui_pop.m_tabCut->setItem(i,3,new QTableWidgetItem(QString("%1").arg(lstItem.at(i).toElement().attribute("airVaveNo"))));
        ui_pop.m_tabCut->setRowHeight(i,51);                                   //每行的高度；

        QLabel *m_labLed = new QLabel;
        m_labLed->setAlignment(Qt::AlignCenter);
        m_labLed->setObjectName(QStringLiteral("m_labLed"));
        m_labLed->setProperty(STATUS,STATUS_OUT);
        m_labLed->setScaledContents(true);
        ui_pop.m_tabCut->setCellWidget(i,2,m_labLed);
    }

    writeToXddp(macroFu_CutterSet, "in");

    ui_pop.stackedWidget->setCurrentIndex(0);
    w->show();
    qDebug()<<"-------------------------"<<"剪刀抬起";
}

//04 选针器停止
void mainWindow::macroFun_PatSetStop()
{
    if(m_mapEnable.keys().contains(macroFu_PatSetStop))
    {
        m_mapEnable[macroFu_PatSetStop] = !m_mapEnable[macroFu_PatSetStop];
    }
    else {
        m_mapEnable[macroFu_PatSetStop] = true;
    }

    writeToXddp(macroFu_PatSetStop, m_mapEnable[macroFu_PatSetStop] ? "true":"false");


    qDebug()<<"-------------------------"<<"选针器停止";
}

//05 牵拉
void mainWindow::macroFun_TakeDown()
{
    if(m_mapEnable.keys().contains(macroFu_TakeDown))
    {
        m_mapEnable[macroFu_TakeDown] = !m_mapEnable[macroFu_TakeDown];
    }
    else {
        m_mapEnable[macroFu_TakeDown] = true;
    }

    writeToXddp(macroFu_TakeDown, m_mapEnable[macroFu_TakeDown] ? "true":"false");
    qDebug()<<"-------------------------"<<"牵拉";
}

//06 进线吹气
void mainWindow::macroFun_AirFeeder()
{
    qDebug()<<"-------------------------"<<"进线吹气";
}

//07 气阀命令
void mainWindow::macroFun_ManualCmd()
{
    /*读气阀的配置文件*/
    if(!readAirValveConfig())
    {
        myMessageBox::getInstance()->setMessage(tr("读取气阀配置文件失败！"), BoxInfo);
    }
    /*初始化表格数据*/
    ui_pop.m_tabAirValve->clearContents();
    ui_pop.m_tabAirValve->setRowCount(0);
    ui_pop.m_tabAirValve->setColumnCount(8); //设置总列数；
    ui_pop.m_tabAirValve->setRowCount(12);   //初始化总行数；
    ui_pop.m_tabAirValve->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    ui_pop.m_tabAirValve->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分行
    ui_pop.m_tabAirValve->setIconSize(QSize(32,32));
    for(int i=0;i<8;i++)
    {
        for(int j=0; j<12; ++j)
        {
            ui_pop.m_tabAirValve->setItem(j,i,new QTableWidgetItem(QIcon(":/image/out.png"),(QString::number(i+j*8))));
        }
    }
    connect(ui_pop.m_tabAirValve, SIGNAL(itemSelectionChanged()), this, SLOT(focusAirValveChanged()));
    ui_pop.m_tabAirValve->setCurrentCell(0,0);

    writeToXddp(macroFu_ManualCmd, "in");

    ui_pop.stackedWidget->setCurrentIndex(1);
    w->show();

    qDebug()<<"-------------------------"<<"气阀命令";
}

//08 背光
void mainWindow::macroFun_BackLight()
{
    qDebug()<<"-------------------------"<<"背光";
}

//09 快编
void mainWindow::macroFun_MiniCycle()
{
    if(m_mapEnable.keys().contains(macroFu_MiniCycle))
    {
        m_mapEnable[macroFu_MiniCycle] = !m_mapEnable[macroFu_MiniCycle];
    }
    else {
        m_mapEnable[macroFu_MiniCycle] = true;
    }

    writeToXddp(macroFu_MiniCycle, m_mapEnable[macroFu_MiniCycle] ? "true":"false");

    qDebug()<<"-------------------------"<<"快编";
}

//10 单只自停
void mainWindow::macroFun_EndCycle()
{
    if(m_mapEnable.keys().contains(macroFu_EndCycle))
    {
        m_mapEnable[macroFu_EndCycle] = !m_mapEnable[macroFu_EndCycle];
    }
    else {
        m_mapEnable[macroFu_EndCycle] = true;
    }

    writeToXddp(macroFu_EndCycle, m_mapEnable[macroFu_EndCycle] ? "true":"false");

    qDebug()<<"-------------------------"<<"单只自停";
}

//11 低速
void mainWindow::macroFun_LowSpeed()
{
    if(m_mapEnable.keys().contains(macroFu_LowSpeed))
    {
        m_mapEnable[macroFu_LowSpeed] = !m_mapEnable[macroFu_LowSpeed];
    }
    else {
        m_mapEnable[macroFu_LowSpeed] = true;
    }

    writeToXddp(macroFu_LowSpeed, m_mapEnable[macroFu_LowSpeed] ? "true":"false");

    qDebug()<<"-------------------------"<<"低速";
}

//12 空转
void mainWindow::macroFun_ChainBlock()
{
    if(m_mapEnable.keys().contains(macroFu_ChainBlock))
    {
        m_mapEnable[macroFu_ChainBlock] = !m_mapEnable[macroFu_ChainBlock];
    }
    else {
        m_mapEnable[macroFu_ChainBlock] = true;
    }

    writeToXddp(macroFu_ChainBlock, m_mapEnable[macroFu_ChainBlock] ? "true":"false");

    qDebug()<<"-------------------------"<<"空转";
}

//13 归零
void mainWindow::macroFun_Zeroing()
{
    if(m_mapEnable.keys().contains(macroFu_Zeroing))
    {
        m_mapEnable[macroFu_Zeroing] = !m_mapEnable[macroFu_Zeroing];
    }
    else {
        m_mapEnable[macroFu_Zeroing] = true;
    }

    writeToXddp(macroFu_Zeroing, m_mapEnable[macroFu_Zeroing] ? "true":"false");

    qDebug()<<"-------------------------"<<"归零";
}

//14 吸风马达
void mainWindow::macroFun_FanOnOff()
{
    if(m_mapEnable.keys().contains(macroFu_FanOnOff))
    {
        m_mapEnable[macroFu_FanOnOff] = !m_mapEnable[macroFu_FanOnOff];
    }
    else {
        m_mapEnable[macroFu_FanOnOff] = true;
    }

    writeToXddp(macroFu_FanOnOff, m_mapEnable[macroFu_FanOnOff] ? "true":"false");
    qDebug()<<"-------------------------"<<"吸风马达";
}

//15 密度/速度/圈数
void mainWindow::macroFun_ParaMod()
{
    qDebug()<<"-------------------------"<<"密度/速度/圈数";
}

//16 加油
void mainWindow::macroFun_Oiler()
{
    writeToXddp(macroFu_Oiler);

    qDebug()<<"-------------------------"<<"加油";
}

/*读气阀的配置文件*/
bool mainWindow::readAirValveConfig()
{
    QFile file(AIRVALUECONFIG_CH);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    m_lstAirValve.clear();
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    while(!file.atEnd())
    {
        QByteArray line=file.readLine();
        QString str=codec->toUnicode(line).trimmed();
        if(!str.isEmpty())
        {
            m_lstAirValve<<str;
        }
    }
    file.close();
    return true;
}

/*当前需要测试的气阀变化的槽函数*/
void mainWindow::focusAirValveChanged()
{
    ui_pop.m_labValveName->setText(m_lstAirValve.at(ui_pop.m_tabAirValve->currentItem()->text().toInt()));
}
