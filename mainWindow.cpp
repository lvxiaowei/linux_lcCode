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
    ui->m_wgtTitle->setProperty("form", "title");
    initIco();
    initData();

    m_AlarmBox = new AlarmBox(this);
}

//数据初始化
void mainWindow::initData()
{
    g_lstRightButton.clear();
    g_lstRightButton <<ui->m_btnRight_1 <<ui->m_btnRight_2 <<ui->m_btnRight_3 <<ui->m_btnRight_4 <<ui->m_btnRight_5 <<ui->m_btnRight_6;

    g_pStackedWgt = ui->m_stackedWidget;

    m_bitValveStates.resize(96);
    m_bitValveStates.fill(false);

    m_bitMacroState.resize(32);
    m_bitMacroState.fill(false);

    m_bitAirFeederState.resize(4);
    m_bitAirFeederState.fill(false);
    /*---------------------------------------------获取配置文件中各类气阀的阀号和名称等--------------------------------------------*/
    QDomDocument document;
    if(!getXmlConfig(document))
        return;
    QDomNodeList lstItem,node;
    QJsonValue val;
    /*******************************************cutter*******************************************/
    node = document.elementsByTagName("cutter");
    if(node.count()!=0)
    {
        lstItem = node.at(0).childNodes();
    }
    for(int i=0;i<lstItem.count();i++)
    {
        val = lstItem.at(i).toElement().attribute("airVaveNo").toInt();
        m_arryCutter.append(val);
    }
    /********************************************YF*******************************************/
    node = document.elementsByTagName("YF");
    if(node.count()!=0)
    {
        lstItem = node.at(0).childNodes();
    }
    for(int i=0;i<lstItem.count();i++)
    {
        val = lstItem.at(i).toElement().attribute("airVaveNo").toInt();
        m_arryYF.append(val);
    }
    /********************************************TakeDown*************************************/
    node = document.elementsByTagName("TakeDown");
    if(node.count()!=0)
    {
        lstItem = node.at(0).childNodes();
    }
    val = lstItem.at(0).toElement().attribute("airVaveNo").toInt();
    m_arryTake.append(val);
    /********************************************AirFeeder************************************/
    node = document.elementsByTagName("AirFeeder");
    if(node.count()!=0)
    {
        lstItem = node.at(0).childNodes();
    }
    for(int i=0; i<lstItem.count(); ++i)
    {
        val = lstItem.at(i).toElement().attribute("airVaveNo").toInt();
        m_arryAirFeefer.append(val);
    }
    /********************************************Fan******************************************/
    node = document.elementsByTagName("Fan");
    if(node.count()!=0)
    {
        lstItem = node.at(0).childNodes();
    }
    val = lstItem.at(0).toElement().attribute("airVaveNo").toInt();
    m_arryFan.append(val);
    /********************************************Oiler****************************************/
    node = document.elementsByTagName("Oiler");
    if(node.count()!=0)
    {
        lstItem = node.at(0).childNodes();
    }
    val = lstItem.at(0).toElement().attribute("airVaveNo").toInt();
    m_arryOiler.append(val);
    /*---------------------------------------------获取配置文件中各类气阀的阀号和名称等--------------------------------------------*/

    /*初始化宏键和函数的映射关系*/
    m_mapFun[Key_Fun0]  = &mainWindow::macroFun_Reset;
    m_mapFun[Key_Fun1]  = &mainWindow::macroFun_YFALLOUT;
    m_mapFun[Key_Fun2]  = &mainWindow::macroFun_CutterSet;
    m_mapFun[Key_Fun3]  = &mainWindow::macroFun_PatSetStop;
    m_mapFun[Key_Fun4]  = &mainWindow::macroFun_TakeDown;
    m_mapFun[Key_Fun5]  = &mainWindow::macroFun_AirFeeder;
    m_mapFun[Key_Fun6]  = &mainWindow::macroFun_ManualCmd;
    m_mapFun[Key_Fun7]  = &mainWindow::macroFun_BackLight;
    m_mapFun[Key_Fun8]  = &mainWindow::macroFun_MiniCycle;
    m_mapFun[Key_Fun9]  = &mainWindow::macroFun_EndCycle;
    m_mapFun[Key_Fun10] = &mainWindow::macroFun_LowSpeed;
    m_mapFun[Key_Fun11] = &mainWindow::macroFun_ChainBlock;
    m_mapFun[Key_Fun12] = &mainWindow::macroFun_Zeroing;
    m_mapFun[Key_Fun13] = &mainWindow::macroFun_FanOnOff;
    m_mapFun[Key_Fun14] = &mainWindow::macroFun_ParaMod;
    m_mapFun[Key_Fun15] = &mainWindow::macroFun_Oiler;

    lst_segment.clear();
    lst_segment<<tr("开始") <<tr("包口") <<tr("橡筋") <<tr("上统") <<tr("袜跟上") <<tr("袜跟下") <<tr("下统")
              <<tr("过桥") <<tr("袜头上") <<tr("袜头下") <<tr("缝头线") <<tr("机头线") <<tr("结束");
    /*<< tr("START") << tr("WELT") <<("ELASTIC") <<tr("LEG") <<tr("HEEL_UP") <<tr("HELL_DOWN") <<tr("FOOT")
                <<tr("PRE_TOE") <<tr("TOE_UP") <<tr("TOE_DOWN") <<tr("ROSSO") <<tr("TAKE OFF") <<tr("END")*/
    /*新建定时器*/
    m_timer = new QTimer(this);

    /*设置系统时间控件*/
    initSystemTime();

    /*初始化事件记录表格*/
    ui->m_tabEventRecord->setColumnWidth(0,250);
    addAlarmRecord(QString("设备启动成功!"));

    /*初始化剪刀表格*/
    initCutterTable();

    setNextDealWgt(PAGE_RUNNING);

}

//图标初始化
void mainWindow::initIco()
{
    m_ON_status  = new QPixmap(":/image/start.png");             //运行状态；
    m_OFF_status = new QPixmap(":/image/stop.png");              //停止状态；
    m_Forward    = new QPixmap(":/image/RotationReversal.png");  //正转；
    m_Reversal   = new QPixmap(":/image/RotationForward.png");   //反转；
    //    ui->m_pngRunState->setPixmap(*m_OFF_status);
    ui->m_pngRunState->setProperty("state", "stop");
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

    lst_move <<m_chain_01 <<m_chain_02 <<m_chain_03 <<m_chain_04 <<m_chain_05 <<m_chain_06 <<m_chain_07 <<m_chain_08 <<m_chain_09
            <<m_chain_10 <<m_chain_11 <<m_chain_12 <<m_chain_01;

    ui->m_gifKInt->setMovie(m_chain_01);
    m_chain_01->start();

}

/*系统时间控件定时刷新*/
void mainWindow::initSystemTime()
{
    m_timer->start(100);
}

/*系统时间控件定时刷新*/
void mainWindow::timerUpDate()
{
    QDateTime QDateCurentTime= QDateTime::currentDateTime();//获取系统现在的时间
    QString strCurentTime = QDateCurentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    ui->m_labSysTime->setText(strCurentTime);//在标签上显示时间
}

/*标题栏滚动效果*/
void mainWindow::marqueeTitle()
{
    ui->m_wgtTitle->update();
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
    if(m_bKeyLock && key != Key_F5)
    {
        myHelper::showMessageBoxInfo(tr("键盘已锁定，请先解锁！"), 1);
        return;
    }

    /*处理确认的弹出框窗口信息*/
    if(myHelper::isMessageBoxShow())
    {
        processingPopup(key);
        return;
    }

    /*处理输入密码的弹出框*/
    if(myHelper::isInputBoxShow())
    {
        keyPressEventInputBox(key);
        return;
    }

    /*处理设置的弹出框信息*/
    if(w->isVisible())
    {
        keyPressEventPopSet(key);
        return;
    }
    switch (key) {
    case Key_F0:
    case Key_F1:
    {
        if(m_isRunning)
        {
            myHelper::showMessageBoxInfo(tr("请先停止设备运行！"), 1);
            return;
        }
        setNextDealWgt(key==Key_F0 ?  PAGE_SETTING:PAGE_TESTINGMENU);
        disconnect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
        disconnect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
        disconnect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
        break;
    }
    case Key_F5:
    {
        m_bKeyLock = !m_bKeyLock;
        g_lstRightButton.at(5)->setText(m_bKeyLock ? tr(""):tr("键盘锁定"));
        setObjProperty(g_lstRightButton.at(5), STATUS, m_bKeyLock ? "lock":"unlock");
        break;
    }
    case Key_Fun0:
    case Key_Fun1:
    case Key_Fun2:
    case Key_Fun4:
    case Key_Fun6:
    case Key_Fun7:
    case Key_Fun12:
    case Key_Fun14:
    {
        if(m_isRunning)
        {
            myHelper::showMessageBoxInfo(tr("请先停止设备运行！"), 1);
            return;
        }
    }
    case Key_Fun3:
    case Key_Fun5:
    case Key_Fun8:
    case Key_Fun9:
    case Key_Fun10:
    case Key_Fun11:
    case Key_Fun13:
    case Key_Fun15:
    {
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
    connect(m_timer,SIGNAL(timeout()),this,SLOT(marqueeTitle()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
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

    QJsonArray arrySet;

    switch (operType) {
    case runningState:
    {
        json.insert("mesg_type", "working_state");
        break;
    }
    case macroFu_ManualCmd:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        arrySet.append(ui_pop.m_tabAirValve->currentItem()->text().toInt());
        jsContent.insert("valve_set", arrySet);
        break;
    }
    case macroFu_CutterSet:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        if(operMode.isEmpty())
        {
            QLabel *m_labLed = dynamic_cast<QLabel*>(ui_pop.m_tabCut->cellWidget(ui_pop.m_tabCut->currentRow(),2));
            setObjProperty(m_labLed, STATUS, m_labLed->property(STATUS)==STATUS_OUT ? STATUS_IN:STATUS_OUT);

            arrySet.append(ui_pop.m_tabCut->item(ui_pop.m_tabCut->currentRow(), 3)->text().toInt());
            jsContent.insert("valve_set", arrySet);
        }
        else {
            jsContent.insert("valve_set", m_arryCutter);
        }

        break;
    }
    case macroFu_YFALLOUT:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        jsContent.insert("valve_set", m_arryYF);
        break;
    }
    case macroFu_TakeDown:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        jsContent.insert("valve_set", m_arryTake);
        break;
    }
    case macroFu_AirFeeder:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        if(operMode.isEmpty())
        {
            QJsonArray airVal;
            airVal.append(m_arryAirFeefer.at(m_iAirFeederIndex));
            jsContent.insert("valve_set", airVal);
        }
        else {
            jsContent.insert("valve_set", m_arryAirFeefer);
        }
        break;
    }
    case macroFu_Oiler:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        jsContent.insert("valve_set", m_arryOiler);
        break;
    }
    case macroFu_FanOnOff:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);

        jsContent.insert("valve_set", m_arryFan);
        break;
    }
    case macroFu_PatSetStop:
    case macroFu_ChainBlock:
    case macroFu_Reset:
    case macroFu_MiniCycle:
    case macroFu_LowSpeed:
    case macroFu_Zeroing:
    case macroFu_EndCycle:
    case macroFu_0:
    {
        json.insert("mesg_type", "func_key");
        jsContent.insert("key", operType);
        break;
    }
    default:
        break;
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
            if (!object.contains("mesg_type") || object.value("mesg_dir").toString() != "ret") {
                return;
            }

            QJsonValue strVal = object.value("content");
            if(!strVal.isObject()) return;
            QJsonObject obj = strVal.toObject();

            if(object.value("mesg_type").toString() == "working_state")
            {
                QBitArray bitarr(32);
                for(int i=0;i<32; ++i)
                {
                    bitarr.setBit(i, (obj.value("key_stat").toInt()>>i)&1);
                }
                ui->btnMacro_1->setChecked(bitarr.at(0));   //强制初始状态
                ui->btnMacro_2->setChecked(bitarr.at(1));   //梭子全部退出
                ui->btnMacro_3->setChecked(bitarr.at(2));   //剪刀抬起
                ui->btnMacro_4->setChecked(bitarr.at(3));   //选针器停止
                ui->btnMacro_5->setChecked(bitarr.at(4));   //牵拉
                ui->btnMacro_6->setChecked(bitarr.at(5));   //进线吹气
                ui->btnMacro_7->setChecked(bitarr.at(6));   //气阀命令
                //                ui->btnMacro_8->setChecked(bitarr.at(7));   //背光
                ui->btnMacro_9->setChecked(bitarr.at(8));   //最小循环
                ui->btnMacro_10->setChecked(bitarr.at(9)); //单只自停
                ui->btnMacro_11->setChecked(bitarr.at(10)); //低速
                ui->btnMacro_12->setChecked(bitarr.at(11)); //空转
                ui->btnMacro_13->setChecked(bitarr.at(12)); //归零
                ui->btnMacro_14->setChecked(bitarr.at(13)); //吸风马达
                //                ui->btnMacro_15->setChecked(bitarr.at(14)); //密度速度圈数
                ui->btnMacro_16->setChecked(bitarr.at(15)); //加油

                ui->m_pngRunState->setText(QString("%1").arg(obj.value("state") == true ? "RUN":"STOP"));
                setObjProperty(ui->m_pngRunState, "state", obj.value("state") == true ? "run":"stop");
                ui->m_edtTitle->setText(QString("%1").arg(obj.value("chain_name").toString()));
                ui->m_pngDirection->setPixmap(obj.value("dir") == true? *m_Forward:*m_Reversal);      //电机转动方向
                ui->m_edtStep->setText(QString("%1/%2").arg(obj.value("step_no").toInt()).arg(obj.value("step_nr").toInt()));  //当前步号/总步数
                ui->m_edtNeedle->setText(QString("%1").arg(obj.value("needle_no").toInt())); //当前针号
                ui->m_edtCircle->setText(QString("%1/%2").arg(obj.value("rep_no").toInt()).arg(obj.value("rep_nr").toInt())); //当前圈数/总圈数

                ui->m_edtOutput->setText(QString("%1/%2").arg(obj.value("volume_no").toInt()).arg(obj.value("volume_nr").toInt()));
                ui->m_edtPattern->setText(QString("%1").arg(obj.value("pattern_name").toString())); //当前激活花型名
                ui->m_edtSpeed->setText(QString("%1").arg(obj.value("speed").toInt())); //速度
                ui->m_pngPart->setText(QString("%1").arg(lst_segment[obj.value("segment").toInt()]));
                ui->m_edtTime->setText(QString("%1").arg(obj.value("time").toString()));
                ui->m_edtMotor->setText(QString("%1").arg(obj.value("motor").toString()));

                if(ui->m_gifKInt->movie() !=lst_move.at(obj.value("segment").toInt()))
                {
                    ui->m_gifKInt->movie()->stop();
                    ui->m_gifKInt->setMovie(lst_move.at(obj.value("segment").toInt()));
                    lst_move.at(obj.value("segment").toInt())->start();
                }

                m_isRunning = (obj.value("state")==true ? true:false);


            }
            else if(object.value("mesg_type").toString() == "func_key")
            {
                QJsonValue jsonBit = obj.value("valve_state");
                if(jsonBit.isArray())
                {
                    QJsonArray array = jsonBit.toArray();
                    m_bitValveStates.fill(false);
                    for (int i = 0; i < array.size(); ++i) {
                        m_bitValveStates.setBit(array.at(i).toInt(),true);
                    }
                }

                switch (obj.value("key").toInt())
                {
                case macroFu_Reset:   //01 强制初始状态
                    break;
                case macroFu_YFALLOUT:   //02 梭子全出
                    break;
                case macroFu_CutterSet:   //03 剪刀抬起
                {
                    for(int i=0; i<ui_pop.m_tabCut->rowCount(); ++i)
                    {
                        int index = ui_pop.m_tabCut->item(i,3)->text().toInt();
                        QLabel * lab=dynamic_cast<QLabel*>(ui_pop.m_tabCut->cellWidget(i,2));
                        if(lab != NULL)
                        {
                            setObjProperty(lab, STATUS, m_bitValveStates.at(index) ? STATUS_IN:STATUS_OUT);
                        }
                    }
                    break;
                }
                case macroFu_PatSetStop:  //04 选针器停止
                    break;
                case macroFu_TakeDown:    //05 牵拉
                    break;
                case macroFu_AirFeeder:   //06 进线吹气
                {
                    for(int i=0; i<m_arryAirFeefer.count(); ++i)
                    {
                        m_bitAirFeederState.setBit(i, m_bitValveStates.at(m_arryAirFeefer.at(i).toInt()) ? true: false);

                        QString name = QString("pushButton_%1").arg(i+1);
                        QPushButton* btn = w->findChild<QPushButton*>(name);
                        if(btn != NULL)
                        {
                            btn->setChecked(m_bitAirFeederState.at(i));
                        }
                    }
                    break;
                }
                case macroFu_ManualCmd:   //07 气阀命令
                case macroFu_0:
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0; j<12; ++j)
                        {
//                            ui_pop.m_tabAirValve->setItem(j,i,new QTableWidgetItem(QIcon(":/image/out.png"),(QString::number(i+j*8))));
                            ui_pop.m_tabAirValve->item(j,i)->setIcon(m_bitValveStates.at(i+j*8) ? QIcon(":/image/in.png"):QIcon(":/image/out.png"));
                        }
                    }
                    break;
                }
                case macroFu_BackLight:   //08 背光
                    break;
                case macroFu_MiniCycle:   //09 快编
                    break;
                case macroFu_EndCycle:    //10 单只自停
                    break;
                case macroFu_LowSpeed:    //11 低速
                    break;
                case macroFu_ChainBlock:  //12 空转
                    break;
                case macroFu_Zeroing:     //13 归零
                    break;
                case macroFu_FanOnOff:    //14 吸风马达

                    break;
                case macroFu_ParaMod:     //15 密度/速度/圈数

                    break;
                case macroFu_Oiler:        //16 加油

                    break;
                default:
                    break;
                }
            }
            else if(object.value("mesg_type").toString() == "alarm")
            {
                QString strAlarm = obj.value("description").toString();
                QString stat = obj.value("stat").toString();

                if("trigger"==stat)
                {
                    qDebug()<<"------"<<strAlarm;
                    m_AlarmBox->adAlarm(strAlarm);
                    addAlarmRecord(strAlarm);
                }
                else if("cancel"==stat)
                {
                    m_AlarmBox->rmAlarm(strAlarm);
                }
            }
        }
    }
}

/*处理弹出框窗口信息*/
void mainWindow::processingPopup(int key)
{
    switch (key) {
    case Key_Set:
    {
        myHelper::messageBoxHide();
        switch (m_iIndex) {
        case macroFu_Reset:
        {
            writeToXddp(macroFu_Reset);
            break;
        }
        case macroFu_Zeroing:
        {
            writeToXddp(macroFu_Zeroing);
            break;
        }
        default:
            break;
        }

        break;
    }
    case Key_Esc:
        myHelper::messageBoxHide();
    default:
        break;
    }
}

/*处理输入密码的弹出框*/
void mainWindow::keyPressEventInputBox(int key)
{
    QLineEdit* p = myHelper::getInputPoint();
    if(p==NULL) return;

    switch (key) {
    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    case Key_7:
    case Key_8:
    case Key_9:
    {
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = p->text() + strInputValue;
        p->setText(strCurentValue);
    }
        break;
    case Key_minus:
    {
        QString strCurentValue = p->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        p->setText(strCurentValue);
        break;
    }
    case Key_Set:
    {
        if(p->text()=="000000")
        {
            /*读气阀的配置文件*/
            if(!readAirValveConfig())
            {
                myHelper::showMessageBoxInfo(tr("读取气阀配置文件失败！"), 1);
                return;
            }

            writeToXddp(macroFu_0);

            m_bitMacroState.setBit(macroFu_ManualCmd, !m_bitMacroState.at(macroFu_ManualCmd));
            ui->btnMacro_7->setChecked(!m_bitMacroState.at(macroFu_ManualCmd));

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

            ui_pop.stackedWidget->setCurrentIndex(1);
            w->show();
        }
        else {
            myHelper::showMessageBoxInfo(tr("密码错误！"), 1);
            break;
        }
    }
    case Key_Esc:
    {
        myHelper::inputBoxHIde();
        break;
    }
    default:
        break;
    }
    qDebug()<<"=========================="<<p->text();
}


/*二级弹出框处理，包括剪刀设置等*/
void mainWindow::keyPressEventPopSet(int key)
{

    switch (ui_pop.stackedWidget->currentIndex()) {
    case 0:
        /*剪刀弹出框操作*/
        keyPressEventPopSet_cut(key);
        break;
    case 1:
        /*气阀弹出框操作*/
        keyPressEventPopSet_airValve(key);
        break;
    case 2:
        /*吹气弹出框设置*/
        keyPressEventPopSet_airFeeder(key);
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
        QCoreApplication::sendEvent(ui_pop.m_tabCut, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(ui_pop.m_tabCut, key_down);
        break;
    }
    case Key_Set:
    {
        writeToXddp(macroFu_CutterSet);
        break;
    }
    case Key_F0:
    case Key_Fun2:
    case Key_Esc:
    {
        m_bitMacroState.setBit(macroFu_CutterSet, !m_bitMacroState.at(macroFu_CutterSet));
        ui->btnMacro_3->setChecked(!m_bitMacroState.at(macroFu_CutterSet));

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
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, key_down);
        break;
    }
    case Key_Left:
    {
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, key_left);
        break;
    }
    case Key_Right:
    {
        QCoreApplication::sendEvent(ui_pop.m_tabAirValve, key_right);
        break;
    }
    case Key_Set:
    {
        writeToXddp(macroFu_ManualCmd);
        break;
    }
    case Key_F0:
    case Key_Fun6:
    case Key_Esc:
    {/*
        m_bitMacroState.setBit(macroFu_ManualCmd, !m_bitMacroState.at(macroFu_ManualCmd));
        ui->btnMacro_7->setChecked(!m_bitMacroState.at(macroFu_ManualCmd));*/
        w->hide();
    }

    default:
        break;
    }
}

/*吹气设置*/
void mainWindow::keyPressEventPopSet_airFeeder(int key)
{
    switch (key) {
    case Key_1:
    case Key_2:
    case Key_3:
    {
        m_iAirFeederIndex = key-Key_1;
        goto skip;
    }
    case Key_4:
    {
        m_iAirFeederIndex = 3;
skip:
        m_bitAirFeederState.setBit(m_iAirFeederIndex, !m_bitAirFeederState.at(m_iAirFeederIndex));
        writeToXddp(macroFu_AirFeeder);
        break;
    }
    case Key_F0:
    case Key_Fun5:
    case Key_Esc:
    {
        m_bitMacroState.setBit(macroFu_AirFeeder, !m_bitMacroState.at(macroFu_AirFeeder));
        ui->btnMacro_6->setChecked(!m_bitMacroState.at(macroFu_AirFeeder));
        w->setFixedSize(800,480);
        w->hide();
        writeToXddp(macroFu_AirFeeder, "out");
    }

    default:
        break;
    }
}

//01 强制初始状态
void mainWindow::macroFun_Reset()
{
    myHelper::showMessageBoxQuestion(tr("确定强制恢复初始状态么?"));
    m_iIndex = macroFu_Reset;
    qDebug()<<"-------------------------"<<"强制初始状态";
}

//02 梭子全出
void mainWindow::macroFun_YFALLOUT()
{
    //    m_bitMacroState.setBit(macroFu_YFALLOUT, !m_bitMacroState.at(macroFu_YFALLOUT));


    //    writeToXddp(macroFu_YFALLOUT, m_bitMacroState.at(macroFu_YFALLOUT) ? "out":"in");
    writeToXddp(macroFu_YFALLOUT, ui->btnMacro_2->isChecked()  ? "out":"in");

    //    ui->btnMacro_2->setChecked(!m_bitMacroState.at(macroFu_YFALLOUT));

    qDebug()<<"-------------------------"<<"梭子全出";
}

//03 剪刀抬起
void mainWindow::macroFun_CutterSet()
{
    //    m_bitMacroState.setBit(macroFu_CutterSet, !m_bitMacroState.at(macroFu_CutterSet));
    //    ui->btnMacro_3->setChecked(!m_bitMacroState.at(macroFu_CutterSet));

    writeToXddp(macroFu_CutterSet, "in");

    ui_pop.stackedWidget->setCurrentIndex(0);
    w->show();
    qDebug()<<"-------------------------"<<"剪刀抬起";
}

//04 选针器停止
void mainWindow::macroFun_PatSetStop()
{

    m_bitMacroState.setBit(macroFu_PatSetStop, !m_bitMacroState.at(macroFu_PatSetStop));

    writeToXddp(macroFu_PatSetStop, m_bitMacroState.at(macroFu_PatSetStop) ? "false":"true");


    qDebug()<<"-------------------------"<<"选针器停止";
}

//05 牵拉
void mainWindow::macroFun_TakeDown()
{
    //    m_bitMacroState.setBit(macroFu_TakeDown, !m_bitMacroState.at(macroFu_TakeDown));

    //    ui->btnMacro_5->setChecked(!m_bitMacroState.at(macroFu_TakeDown));

    writeToXddp(macroFu_TakeDown);
    qDebug()<<"-------------------------"<<"牵拉";
}

//06 进线吹气
void mainWindow::macroFun_AirFeeder()
{
    //    m_bitMacroState.setBit(macroFu_AirFeeder, !m_bitMacroState.at(macroFu_AirFeeder));

    //    ui->btnMacro_6->setChecked(!m_bitMacroState.at(macroFu_AirFeeder));

    writeToXddp(macroFu_AirFeeder, "in");

    ui_pop.stackedWidget->setCurrentIndex(2);
    w->setFixedSize(200,120);
    w->show();

    //    writeToXddp(macroFu_AirFeeder);
    qDebug()<<"-------------------------"<<"进线吹气";
}

//07 气阀命令
void mainWindow::macroFun_ManualCmd()
{
    myHelper::showInputBox("请输入密码:", 0, 0, "登录气阀测试界面密码", true);

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
    //    m_bitMacroState.setBit(macroFu_MiniCycle, !m_bitMacroState.at(macroFu_MiniCycle));

    //    ui->btnMacro_9->setChecked(!m_bitMacroState.at(macroFu_MiniCycle));

    writeToXddp(macroFu_MiniCycle, m_bitMacroState.at(macroFu_MiniCycle)? "false":"true");

    qDebug()<<"-------------------------"<<"快编";
}

//10 单只自停
void mainWindow::macroFun_EndCycle()
{
    //    m_bitMacroState.setBit(macroFu_EndCycle, !m_bitMacroState.at(macroFu_EndCycle));

    //    ui->btnMacro_10->setChecked(!m_bitMacroState.at(macroFu_EndCycle));

    writeToXddp(macroFu_EndCycle, m_bitMacroState.at(macroFu_EndCycle)? "false":"true");

    qDebug()<<"-------------------------"<<"单只自停";
}

//11 低速
void mainWindow::macroFun_LowSpeed()
{
    //    m_bitMacroState.setBit(macroFu_LowSpeed, !m_bitMacroState.at(macroFu_LowSpeed));

    //    ui->btnMacro_11->setChecked(!m_bitMacroState.at(macroFu_LowSpeed));

    writeToXddp(macroFu_LowSpeed, m_bitMacroState.at(macroFu_LowSpeed)? "false":"true");

    qDebug()<<"-------------------------"<<"低速";
}

//12 空转
void mainWindow::macroFun_ChainBlock()
{
    //    m_bitMacroState.setBit(macroFu_ChainBlock, !m_bitMacroState.at(macroFu_ChainBlock));

    //    ui->btnMacro_12->setChecked(!m_bitMacroState.at(macroFu_ChainBlock));

    writeToXddp(macroFu_ChainBlock, m_bitMacroState.at(macroFu_ChainBlock)? "false":"true");

    qDebug()<<"-------------------------"<<"空转";
}

//13 归零
void mainWindow::macroFun_Zeroing()
{
    myHelper::showMessageBoxQuestion(tr("确定进行归零操作吗?"));
    m_iIndex = macroFu_Zeroing;

    qDebug()<<"-------------------------"<<"归零";
}

//14 吸风马达
void mainWindow::macroFun_FanOnOff()
{
    m_bitMacroState.setBit(macroFu_FanOnOff, !m_bitMacroState.at(macroFu_FanOnOff));

    writeToXddp(macroFu_FanOnOff);

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

/*初始化事件记录表格*/
void mainWindow::addAlarmRecord(QString str)
{
    ui->m_tabEventRecord->setRowCount(ui->m_tabEventRecord->rowCount()+1);
    ui->m_tabEventRecord->setItem(ui->m_tabEventRecord->rowCount()-1,0,new QTableWidgetItem(QString("%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))));
    ui->m_tabEventRecord->setItem(ui->m_tabEventRecord->rowCount()-1,1,new QTableWidgetItem(str));
    ui->m_tabEventRecord->item(ui->m_tabEventRecord->rowCount()-1,0)->setTextAlignment(Qt::AlignCenter);
    ui->m_tabEventRecord->item(ui->m_tabEventRecord->rowCount()-1,1)->setTextAlignment(Qt::AlignCenter);

    ui->m_tabEventRecord->scrollToBottom();
}

/*初始化剪刀表格*/
void mainWindow::initCutterTable()
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
    ui_pop.m_tabCut->setHorizontalHeaderLabels(QStringList()<<tr("编号")<<tr("剪刀名称")<<tr("动作状态")<<"气阀编号"); //设置表头名称；

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
        ui_pop.m_tabCut->item(i,3)->setTextAlignment(Qt::AlignCenter);
        ui_pop.m_tabCut->setRowHeight(i,51);                                   //每行的高度；

        QLabel *m_labLed = new QLabel;
        m_labLed->setAlignment(Qt::AlignCenter);
        m_labLed->setObjectName(QStringLiteral("m_labLed"));
        m_labLed->setProperty(STATUS,STATUS_IN);
        m_labLed->setScaledContents(true);
        ui_pop.m_tabCut->setCellWidget(i,2,m_labLed);
    }
}

/*初始化气阀表格*/
void mainWindow::initManualTable()
{

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
