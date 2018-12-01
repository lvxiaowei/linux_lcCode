#include "Frm_testingMenu.h"
#include "ui_Frm_testingMenu.h"

Frm_testingMenu::Frm_testingMenu(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_testingMenu)
{
    g_mapIndexToWgt[PAGE_TESTINGMENU]= this;
    ui->setupUi(this);

    dataInit();
}

Frm_testingMenu::~Frm_testingMenu()
{
    delete ui;
}

/*数据初始化*/
void Frm_testingMenu::dataInit()
{
    m_lstMotoVal.clear();
    m_lstMotoVal<< ui->m_edtLow << ui->m_edtUp <<ui->m_edtElastic;

    m_lstMotoRadio.clear();
    m_lstMotoRadio<< ui->m_rdioLow <<ui->m_rdioUp << ui->m_rdioElastic;
}

/*处理串口数据*/
void Frm_testingMenu::keyPressEvent(int key)
{
    qDebug()<<"当前正在测试界面,key="<<key;

    switch (ui->m_stackWgt->currentIndex()) {
    case 0:
        dealPg1(key);
        break;
    case 1:
        dealPg2(key);
        break;
    case 2:
        dealPg3(key);
        break;
    case 3:
        dealPg4(key);
        break;
    case 4:
        dealPg5(key);
        break;
    default:
        break;
    }
}

/*数据初始化*/
void Frm_testingMenu::initShowFrmConfig()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("气阀测试")<<tr("选针器\n测试")<<tr("马达")<<tr("梭子测试")<<tr("闸刀测试"));
    ui->m_stackWgt->setCurrentIndex(0);
}

/*处理串口数据-菜单界面*/
void Frm_testingMenu::dealPg1(int key)
{
    switch (key) {
    case Key_F0:
        setNextDealWgt(PAGE_RUNNING);
        break;
    case Key_F1:
    {
        initAirValvePage();
        break;
    }
    case Key_F2:
    {
        initActuatorPage();
        break;
    }
    case Key_F3:
    {
        initMotoPage();
        break;
    }
    case Key_F4:
    {
        initYFPage();
        break;
    }
    case Key_F5:
        break;
    default:
        break;
    }
}

/*处理串口数据-page2--气阀测试*/
void Frm_testingMenu::dealPg2(int key)
{
    switch (key) {
    case Key_F0:
        initShowFrmConfig();
        emit writeToXddp(AIRVALVETEST, "out");
        break;
    case Key_F1:
        break;
    case Key_F2:
    {
        emit writeToXddp(AIRVALVETEST);
        break;
    }
    case Key_F3:
        break;
    case Key_F4:
        break;
    case Key_F5:
    {
        int curtFrq = ui->m_labFre->text().split("(").at(0).toInt()+1;
        ui->m_labFre->setText(QString("%1(秒)").arg(curtFrq%5==0 ? 5:curtFrq%5));
        break;
    }






    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabAirValve, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabAirValve, &keyPress);
        break;
    }
    case Key_Left:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabAirValve, &keyPress);
        break;
    }
    case Key_Right:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabAirValve, &keyPress);
        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page2--马达测试*/
void Frm_testingMenu::dealPg3(int key)
{
    switch (key) {
    case Key_F0:
        if(m_bIsTesting)
            return;
        initShowFrmConfig();
        emit writeToXddp(MOTOTEST, "out");
        break;
    case Key_F1:
    {
        m_bIsTesting = !m_bIsTesting;
        if(m_bIsTesting && m_iPos==2)
        {
            freshRightButtonContent(QStringList()<<tr("")<<tr("停止")<<tr("")<<tr("")<<tr("")<<tr(""));
        }
        else if(m_bIsTesting && (m_iPos==0 || m_iPos==1)) {
            freshRightButtonContent(QStringList()<<tr("")<<tr("回到\n原位")<<tr("")<<tr("")<<tr("")<<tr(""));
        }
        else if(!m_bIsTesting)
        {
            freshRightButtonContent(QStringList()<<tr("返回")<<tr("开始")<<tr("")<<tr("")<<tr("")<<tr(""));
        }

        writeToXddp(MOTOTEST);
        break;
    }



    case Key_Up:
    {
        if( m_iPos ==0 || m_bIsTesting) return;
        m_iPos--;
        m_lstMotoRadio.at(m_iPos)->setChecked(true);
        m_lstMotoVal.at(m_iPos)->setFocus();
        break;
    }
    case Key_Down:
    {
        if( m_iPos==2|| m_bIsTesting) return;
        m_iPos++;
        m_lstMotoRadio.at(m_iPos)->setChecked(true);
        m_lstMotoVal.at(m_iPos)->setFocus();
        break;
    }
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
        if(!m_mapNoKeyToValue.contains(key) || m_bIsTesting) break;

        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = m_lstMotoVal.at(m_iPos)->text() + strInputValue;
        m_lstMotoVal.at(m_iPos)->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_minus:
    {
        bool ok;
        QString strCurentValue = m_lstMotoVal.at(m_iPos)->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        m_lstMotoVal.at(m_iPos)->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page2--选针器测试*/
void Frm_testingMenu::dealPg4(int key)
{
    switch (key) {
    case Key_F0:
        if(g_lstRightButton.at(0)->text().isEmpty())
            return;
        initShowFrmConfig();
        emit writeToXddp(SELECTIONTEST, "out");
        break;
    case Key_F1:
    {
        if(g_lstRightButton.at(1)->text().isEmpty())
        {
            return;
        }
        else if(g_lstRightButton.at(1)->text() == tr("停止"))
        {
            freshRightButtonContent(QStringList()<<tr("返回")<<tr("循环")<<tr("一格一")<<tr("全上全下")<<tr("单刀")<<tr("频率"));
            m_bIsTesting = false;
        }
        else
        {
            freshRightButtonContent(QStringList()<<tr("")<<tr("停止")<<tr("")<<tr("")<<tr("")<<tr(""));
            m_bIsTesting = true;
        }
        m_strTestMode = "rep";
        writeToXddp(SELECTIONTEST);
        break;
    }
    case Key_F2:
    {
        if(g_lstRightButton.at(2)->text().isEmpty())
        {
            return;
        }
        else if(g_lstRightButton.at(2)->text() == tr("停止"))
        {
            freshRightButtonContent(QStringList()<<tr("返回")<<tr("循环")<<tr("一格一")<<tr("全上全下")<<tr("单刀")<<tr("频率"));
            m_bIsTesting = false;
        }
        else
        {
            freshRightButtonContent(QStringList()<<tr("")<<tr("")<<tr("停止")<<tr("")<<tr("")<<tr(""));
            m_bIsTesting = true;
        }
        m_strTestMode = "onebyone";
        writeToXddp(SELECTIONTEST);
        break;
    }
    case Key_F3:
    {
        if(g_lstRightButton.at(3)->text().isEmpty())
        {
            return;
        }
        else if(g_lstRightButton.at(3)->text() == tr("停止"))
        {
            freshRightButtonContent(QStringList()<<tr("返回")<<tr("循环")<<tr("一格一")<<tr("全上全下")<<tr("单刀")<<tr("频率"));
            m_bIsTesting = false;
        }
        else
        {
            freshRightButtonContent(QStringList()<<tr("")<<tr("")<<tr("")<<tr("停止")<<tr("")<<tr(""));
            m_bIsTesting = true;
        }
        m_strTestMode = "all";
        writeToXddp(SELECTIONTEST);
        break;
    }
    case Key_F4:
    {
        m_bIsTesting = true;
        m_strTestMode = "single";
        writeToXddp(SELECTIONTEST);
        break;
    }
    case Key_F5:
    {
        if(g_lstRightButton.at(5)->text().isEmpty())
            return;

        int curtFrq = ui->m_labNO_4->text().split("(").at(0).toInt()+1;
        ui->m_labNO_4->setText(QString("%1(秒)").arg(curtFrq%5==0 ? 5:curtFrq%5));
        break;
    }

    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabActuator, &keyPress);
        ui->m_labKnifNO->setText(ui->m_tabActuator->currentItem()->text());
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabActuator, &keyPress);
        ui->m_labKnifNO->setText(ui->m_tabActuator->currentItem()->text());
        break;
    }

    default:
        break;
    }
}

/*处理串口数据-page2--梭子测试*/
void Frm_testingMenu::dealPg5(int key)
{
    switch (key) {
    case Key_F0:
        if(g_lstRightButton.at(0)->text().isEmpty())
            return;
        initShowFrmConfig();
        emit writeToXddp(YFTEST, "out");
        break;
    case Key_F1:
    {
        if(g_lstRightButton.at(1)->text() == tr("停止"))
        {
            freshRightButtonContent(QStringList()<<tr("返回")<<tr("自动")<<tr("手动")<<tr("")<<tr("")<<tr("频率"));
            m_bIsTesting = false;
        }
        else
        {
            freshRightButtonContent(QStringList()<<tr("")<<tr("停止")<<tr("")<<tr("")<<tr("")<<tr(""));
            m_bIsTesting = true;
        }
        m_strTestMode = "true";
        writeToXddp(YFTEST);
        break;
    }
    case Key_F2:
    {
        if(g_lstRightButton.at(2)->text().isEmpty())
        {
            return;
        }
        m_strTestMode = "false";
        writeToXddp(YFTEST);
        break;
    }
    case Key_F3:
        break;
    case Key_F4:
        break;
    case Key_F5:
    {
        if(g_lstRightButton.at(5)->text().isEmpty())
        {
            return;
        }
        int curtFrq = ui->m_labNO_5->text().split("(").at(0).toInt()+1;
        ui->m_labNO_5->setText(QString("%1(秒)").arg(curtFrq%5==0 ? 5:curtFrq%5));
        break;
    }

    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabYF1, &keyPress);
        ui->m_labYFNO->setText(QString("%1").arg(ui->m_tabYF1->currentIndex().row()+1));
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabYF1, &keyPress);
        ui->m_labYFNO->setText(QString("%1").arg(ui->m_tabYF1->currentIndex().row()+1));
        break;
    }

    default:
        break;
    }
}

/*气阀测试界面初始化*/
void Frm_testingMenu::initAirValvePage()
{
    m_bIsTesting = false;
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("自动")<<tr("翻转")<<tr("清除")<<tr("连续")<<tr("频率"));
    initAirValve();
    ui->m_stackWgt->setCurrentIndex(1);

    emit writeToXddp(AIRVALVETEST, "in");
}

/*电机测试界面初始化*/
void Frm_testingMenu::initMotoPage()
{
    m_bIsTesting = false;
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("开始")<<tr("")<<tr("")<<tr("")<<tr(""));
    ui->m_stackWgt->setCurrentIndex(2);
    m_iPos =0;
    ui->m_rdioLow->setChecked(true);
    ui->m_edtLow->setFocus();

    emit writeToXddp(MOTOTEST, "in");
}

/*选针器测试界面初始化*/
void Frm_testingMenu::initActuatorPage()
{
    ui->m_stackWgt->setCurrentIndex(3);
    /*初始化表格数据*/
    ui->m_tabActuator->clearContents();
    ui->m_tabActuator->setRowCount(0);
    ui->m_tabActuator->setColumnCount(1); //设置总列数；
    ui->m_tabActuator->setRowCount(12);   //初始化总行数；
    ui->m_tabActuator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    ui->m_tabActuator->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分行
    ui->m_tabActuator->setIconSize(QSize(32,32));
    for(int i=0;i<12;++i)
    {
        ui->m_tabActuator->setItem(i,0,new QTableWidgetItem(QIcon(":/image/out.png"),QString("%1").arg(i+1)));
    }
    ui->m_tabActuator->setCurrentCell(0,0);

    m_bIsTesting = false;
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("循环")<<tr("一格一")<<tr("全上全下")<<tr("单刀")<<tr("频率"));

    emit writeToXddp(SELECTIONTEST, "in");
}

/*初始化气阀测试界面*/
void Frm_testingMenu::initAirValve()
{
    /*读气阀的配置文件*/
    if(!readAirValveConfig())
    {
        myMessageBox::getInstance()->setMessage(tr("读取气阀配置文件失败！"), BoxInfo);
    }
    /*初始化表格数据*/
    ui->m_tabAirValve->clearContents();
    ui->m_tabAirValve->setRowCount(0);
    ui->m_tabAirValve->setColumnCount(8); //设置总列数；
    ui->m_tabAirValve->setRowCount(12);   //初始化总行数；
    ui->m_tabAirValve->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    ui->m_tabAirValve->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分行
    ui->m_tabAirValve->setIconSize(QSize(32,32));
    for(int i=0;i<8;i++)
    {
        for(int j=0; j<12; ++j)
        {
            ui->m_tabAirValve->setItem(j,i,new QTableWidgetItem(QIcon(":/image/out.png"),(QString::number(i+j*8))));
        }
    }
    connect(ui->m_tabAirValve, SIGNAL(itemSelectionChanged()), this, SLOT(focusAirValveChanged()));
    ui->m_tabAirValve->setCurrentCell(0,0);
}

/*初始化梭子测试界面*/
void Frm_testingMenu::initYFPage()
{
    ui->m_stackWgt->setCurrentIndex(4);
    /*初始化表格数据*/
    ui->m_tabYF1->clearContents();
    ui->m_tabYF1->setRowCount(0);
    ui->m_tabYF1->setColumnCount(1); //设置总列数；
    ui->m_tabYF1->setRowCount(6);   //初始化总行数；
    ui->m_tabYF1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分列
    ui->m_tabYF1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均分行
    for(int i=0;i<6;++i)
    {
        ui->m_tabYF1->setItem(i,0,new QTableWidgetItem(QString("梭子—1F%1").arg(i+1)));
    }
    ui->m_tabYF1->setCurrentCell(0,0);

    m_bIsTesting = false;
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("自动")<<tr("手动")<<tr("")<<tr("")<<tr("频率"));

    emit writeToXddp(YFTEST, "in");
}

/*当前需要测试的气阀变化的槽函数*/
void Frm_testingMenu::focusAirValveChanged()
{
    ui->m_labNO->setText(ui->m_tabAirValve->currentItem()->text());
    ui->m_labName->setText(m_lstAirValve.at(ui->m_labNO->text().toInt()));
}

/*读气阀的配置文件*/
bool Frm_testingMenu::readAirValveConfig()
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

/*向XDDP发送数据*/
void Frm_testingMenu::writeToXddp(const int testType, QString operMode)
{
    QJsonObject json;
    json.insert("mesg_dir", "req");
    QJsonObject jsContent;
    if(!operMode.isEmpty())
        jsContent.insert("operMode", operMode);
    switch (testType) {
    case AIRVALVETEST:
    {
        json.insert("mesg_type", "valve_test");
        jsContent.insert("valve_no",ui->m_labNO->text().toInt());  //测试的阀号

        break;
    }
    case SELECTIONTEST:
    {
        json.insert("mesg_type", "actuator_test");
        jsContent.insert("actuator_no",1);  //选针器号（目前机型上只有一个）
        jsContent.insert("state",m_bIsTesting);  // true表示开始，false表示停止
        jsContent.insert("pin",ui->m_labKnifNO->text().toInt());    //选针的值
        jsContent.insert("mode",m_strTestMode);         //single(单个选针测试)，“rep”: (循环测试)，all(全起全沉) ”freq“: 自动测试的频率
        jsContent.insert("freq",1000*ui->m_labNO_4->text().split("(").at(0).toInt());  //自动模式下的测试频率
        break;
    }
    case MOTOTEST:
    {
        json.insert("mesg_type", "motor_test");
        jsContent.insert("motor_no", m_iPos);  //电机号。0表示下针筒，1表示上针筒，2表示橡筋马达
        jsContent.insert("pos", m_lstMotoVal.at(m_iPos)->text().toInt());
        jsContent.insert("state", g_lstRightButton.at(1)->text()==tr("开始")? false:true);
        break;
    }
    case YFTEST:
    {
        json.insert("mesg_type", "yf_test");
        jsContent.insert("yf_no",1);  //选针器号（目前机型上只有一个）
        jsContent.insert("state",m_bIsTesting);  // true表示开始，false表示停止
        jsContent.insert("pin",ui->m_labYFNO->text().toInt());    //选针的值
        jsContent.insert("mode",m_strTestMode);         //false表示手动　true表示自动
        jsContent.insert("freq",1000*ui->m_labNO_5->text().split("(").at(0).toInt());  //自动模式下的测试频率

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
void Frm_testingMenu::handleXddpData(QByteArray data)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        if (doucment.isObject()) {
            QJsonObject object = doucment.object();
            if (object.value("mesg_dir").toString() != "ret") {
                return;
            }
            QJsonValue strVal = object.value("content");
            if(!strVal.isObject()) return;
            QJsonObject obj = strVal.toObject();

            //处理气阀测试界面相关的数据
            if(object.value("mesg_type").toString() == "valve_test")
            {
                qDebug()<<"++++++++++++++++++++++气阀测试";
                QJsonValue jsonBit = obj.value("valve_state");
                if(!jsonBit.isArray()) return;
                QJsonArray array = jsonBit.toArray();

                QBitArray m_bitValveStates;
                m_bitValveStates.resize(96);
                m_bitValveStates.fill(false);
                for (int i = 0; i < array.size(); ++i) {
                    m_bitValveStates.setBit(array.at(i).toInt(),true);
                }

                for(int i=0;i<8;i++)
                {
                    for(int j=0; j<12; ++j)
                    {
                        ui->m_tabAirValve->setItem(j,i,new QTableWidgetItem(QIcon(":/image/out.png"),(QString::number(i+j*8))));
                        ui->m_tabAirValve->item(j,i)->setIcon(m_bitValveStates.at(i+j*8) ? QIcon(":/image/in.png"):QIcon(":/image/out.png"));
                    }
                }
            }
            //处理选针器测试界面相关的数据
            else if(object.value("mesg_type").toString() == "actuator_test")
            {
                qDebug()<<"++++++++++++++++++++++选针器测试";
                QJsonValue jsonBit = obj.value("actuator_state");
                if(!jsonBit.isArray()) return;
                QJsonArray array = jsonBit.toArray();
                QBitArray m_bitValveStates;
                m_bitValveStates.resize(12);
                m_bitValveStates.fill(false);
                for (int i = 0; i < array.size(); ++i) {
                    m_bitValveStates.setBit(array.at(i).toInt(),true);
                }

                for(int i=0;i<12;++i)
                {
                    ui->m_tabActuator->item(i,0)->setIcon(m_bitValveStates.at(i) ? QIcon(":/image/in.png"):QIcon(":/image/out.png"));
                }
            }
        }
    }
}
