#include "Frm_parameterSettings.h"
#include "ui_Frm_parameterSettings.h"

Frm_parameterSettings::Frm_parameterSettings(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_parameterSettings)
{
    g_mapIndexToWgt[PAGE_PARASETTING]= this;
    ui->setupUi(this);
    w = new QDialog(this);
    ui_pop.setupUi(w);

    setTabOrder(ui->m_edtZeroing,ui->m_edtNeedl);
    setTabOrder(ui->m_edtNeedl, ui->m_edtOilMode);
    setTabOrder(ui->m_edtOilMode, ui->m_edtOilEnable);
    setTabOrder(ui->m_edtOilEnable,ui->m_edtOilFre);
    setTabOrder(ui->m_edtOilFre,ui->m_edtOilKeep);
    setTabOrder(ui->m_edtOilKeep,ui->m_edtLanguage);
    setTabOrder(ui->m_edtLanguage,ui->m_edtSysTime);
    setTabOrder(ui->m_edtSysTime,ui->m_edtJog);
    setTabOrder(ui->m_edtJog,ui->m_edtMaxSpeed);
    setTabOrder(ui->m_edtMaxSpeed,ui->m_edtSize);
    setTabOrder(ui->m_edtSize,ui->m_edtOutput);

    /*新建定时器*/
    m_timer = new QTimer(this);

    /*设置系统时间控件*/
    initSystemTime();
}

Frm_parameterSettings::~Frm_parameterSettings()
{
    delete ui;
}

/*处理串口数据*/
void Frm_parameterSettings::keyPressEvent(int key)
{
    /*处理设置的弹出框信息*/
    if(w->isVisible())
    {
        keyPressEventPopSet(key);
        return;
    }

    qDebug()<<"当前正在设置界面,key="<<key;
    switch (key) {
    case Key_F0:
        setNextDealWgt(PAGE_SETTING);
        break;
    case Key_F1:
        saveParaToFile();
        break;
    case Key_F2:
        break;
    case Key_F3:
        break;
    case Key_F4:
        break;
    case Key_F5:
        break;

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
        /*对有下拉框的QComboBox控件做差异处理*/
        QLineEdit *pCurrentFocusWgt =dynamic_cast<QLineEdit*>(focusWidget());
        if(pCurrentFocusWgt!=NULL)
        {
            bool ok;
            QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
            QString strCurentValue = pCurrentFocusWgt->text() + strInputValue;
            pCurrentFocusWgt->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        }
    }
        break;
    case Key_minus:
    {
        /*对有下拉框的QComboBox控件做差异处理*/
        QLineEdit *pCurrentFocusWgt =dynamic_cast<QLineEdit*>(focusWidget());
        if(pCurrentFocusWgt!=NULL)
        {
            bool ok;
            QString strCurentValue = pCurrentFocusWgt->text();
            strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
            pCurrentFocusWgt->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        }
        break;
    }

    case Key_Up:
    case Key_Down:
    {
        /*对有下拉框的QComboBox控件做差异处理*/
        QComboBox *pCurrentFocusWgt =dynamic_cast<QComboBox*>(focusWidget());
        if(pCurrentFocusWgt!=NULL && pCurrentFocusWgt->view()->isActiveWindow())
        {
            int current_index=pCurrentFocusWgt->currentIndex();
            if((current_index<pCurrentFocusWgt->count()-1) && key == Key_Down)
            {
                ++current_index;
            }
            else if ((current_index >0) && key == Key_Up)
            {
                --current_index;
            }
            pCurrentFocusWgt->setCurrentIndex(current_index);
            QModelIndex itemIndex = pCurrentFocusWgt->view()->model()->index(current_index,0);
            pCurrentFocusWgt->view()->selectionModel()->setCurrentIndex(itemIndex,QItemSelectionModel::SelectCurrent);
        }
        else {
            key == Key_Up ? focusPreviousChild():focusNextChild();
        }
        break;
    }
    case Key_Left:
    {
        ui->m_edtZeroing->setFocus();
        break;
    }
    case Key_Right:
    {
        ui->m_edtLanguage->setFocus();
        break;
    }
    case Key_Set:
    {
        dealPressKeyEvent();
        break;
    }
    default:
        break;
    }
}

/*二级弹出框处理，包括密码设置等*/
void Frm_parameterSettings::keyPressEventPopSet(int key)
{

    switch (ui_pop.stackedWidget->currentIndex()) {
    case 0:
        keyPressEventPopSet_paswd(key);
        break;
    case 1:
        keyPressEventPopSet_zeroingSet(key);
        break;
    case 2:
    {
        keyPressEventPopSet_systimeSet(key);
        break;
    }
    default:
        break;
    }
}

/*密码输入界面设置*/
void Frm_parameterSettings::keyPressEventPopSet_paswd(int key)
{
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
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = ui_pop.m_edtPasWd->text() + strInputValue;
        ui_pop.m_edtPasWd->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_minus:
    {
        QString strCurentValue = ui_pop.m_edtPasWd->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        ui_pop.m_edtPasWd->setText(strCurentValue);
        break;
    }
    case Key_F0:
    case Key_Esc:
    {
        w->hide();
        ui->m_edtZeroing->setFocus();
        break;
    }
    case Key_Set:
    {
        if(ui_pop.m_edtPasWd->text() == "123456")
        {
            ui_pop.stackedWidget->setCurrentIndex(1);
            myHelper::showMessageBoxInfo(tr("登录成功!"), 1);
            writeToXddp("in");
            connect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
        }
        else {
            myHelper::showMessageBoxInfo(tr("密码错误!"), 1);
        }
        break;
    }
    default:
        break;
    }
}

/*零位设置界面*/
void Frm_parameterSettings::keyPressEventPopSet_zeroingSet(int key)
{
    switch (key) {
    case Key_F0:
    case Key_Esc:
    {
        w->hide();
        ui->m_edtZeroing->setFocus();
        disconnect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
        writeToXddp("out");
        break;
    }
    case Key_Set:
    {
        writeToXddp("set");
        myHelper::showMessageBoxInfo(tr("零位设置成功！"), 1);
        break;
    }
    default:
        break;
    }
}

/*系统时间设置*/
void Frm_parameterSettings::keyPressEventPopSet_systimeSet(int key)
{
    switch (key) {
    case Key_Up:
    {
        QCoreApplication::sendEvent(ui_pop.dateTimeEdit, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(ui_pop.dateTimeEdit, key_down);
        break;
    }
    case Key_Left:
    {
        QCoreApplication::sendEvent(ui_pop.dateTimeEdit, key_left);
        break;
    }
    case Key_Right:
    {
        QCoreApplication::sendEvent(ui_pop.dateTimeEdit, key_right);
        break;
    }

    case Key_Esc:
    {
        w->hide();
        ui->m_edtSysTime->setFocus();
        break;
    }
    case Key_Set:
    {
        QString str="date "+ui_pop.dateTimeEdit->dateTime().toString("MMddhhmmyyyy");

        QProcess::execute(str); //用进程调用linux的命令:date
        QProcess::execute("hwclock -w");        //-w:将系统时钟同步到硬件时钟；-s:将硬件时钟同步到系统时钟；
        QProcess::execute("sync");              //sync命令：可用来强制将内存缓冲区中的数据写入磁盘中；
        myHelper::showMessageBoxInfo(tr("系统时间修改成功！"), 1);
        break;
    }
    default:
        break;
    }
}

/*数据初始化*/
void Frm_parameterSettings::initShowFrmConfig()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("保存设置")<<tr("")<<tr("")<<tr("")<<tr(""));

    ui->m_edtZeroing->setFocus();
    readParaFromFile();
}

/*处理set键相关的事件*/
void Frm_parameterSettings::dealPressKeyEvent()
{
    /*针对不同的控件，做不同的处理*/
    if(focusWidget()->inherits("QComboBox") )
    {
        QComboBox *pComboxWgt =dynamic_cast<QComboBox*>(focusWidget());
        if(pComboxWgt == NULL)
            return;
        pComboxWgt->view()->isActiveWindow() ? pComboxWgt->hidePopup():pComboxWgt->showPopup();
    }
    else if(focusWidget() == ui->m_edtZeroing)
    {
        ui_pop.stackedWidget->setCurrentIndex(0);
        ui_pop.m_edtPasWd->setFocus();
        w->show();
    }
    else if(focusWidget() == ui->m_edtSysTime)
    {
        ui_pop.stackedWidget->setCurrentIndex(2);
        ui_pop.dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui_pop.dateTimeEdit->setFocus();
        w->show();
    }
}

/*从配置文件读取配置*/
void Frm_parameterSettings::readParaFromFile()
{
    /*获取当前正在运行的链条名字*/
    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    /*针筒总针数*/
    ui->m_edtNeedl->setText(document.elementsByTagName("needle").at(0).toElement().attribute("val"));
    /*加油模式*/
    ui->m_edtOilMode->setCurrentIndex(document.elementsByTagName("oil").at(0).toElement().attribute("mode").toInt());
    /*自动加油*/
    ui->m_edtOilEnable->setCurrentIndex(document.elementsByTagName("oil").at(0).toElement().attribute("enable").toInt());
    /*加油间隔*/
    ui->m_edtOilFre->setText(document.elementsByTagName("oil").at(0).toElement().attribute("frequency"));
    /*加油保持(秒)*/
    ui->m_edtOilKeep->setText(document.elementsByTagName("oil").at(0).toElement().attribute("keep"));
    /*语言设置*/
    ui->m_edtLanguage->setCurrentIndex(document.elementsByTagName("language").at(0).toElement().attribute("val").toInt());
    /*点动速度*/
    ui->m_edtJog->setText(document.elementsByTagName("speed").at(0).toElement().attribute("jog"));
    /*最大速度*/
    ui->m_edtJog->setText(document.elementsByTagName("speed").at(0).toElement().attribute("max"));
    /*尺寸设置*/
    ui->m_edtSize->setCurrentIndex(document.elementsByTagName("size").at(0).toElement().attribute("val").toInt()-1);
    /*目标产量*/
    ui->m_edtOutput->setText(document.elementsByTagName("targetYield").at(0).toElement().attribute("val"));
}

/*保存配置到配置文件*/
void Frm_parameterSettings::saveParaToFile()
{
    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    /*针筒总针数*/
    document.elementsByTagName("needle").at(0).toElement().setAttribute("val", ui->m_edtNeedl->text());
    /*加油模式*/
    document.elementsByTagName("oil").at(0).toElement().setAttribute("mode", ui->m_edtOilMode->currentIndex());
    /*自动加油*/
    document.elementsByTagName("oil").at(0).toElement().setAttribute("enable", ui->m_edtOilEnable->currentIndex());
    /*加油间隔*/
    document.elementsByTagName("oil").at(0).toElement().setAttribute("frequency", ui->m_edtOilFre->text());
    /*加油保持(秒)*/
    document.elementsByTagName("oil").at(0).toElement().setAttribute("keep", ui->m_edtOilKeep->text());
    /*语言设置*/
    document.elementsByTagName("language").at(0).toElement().setAttribute("val", ui->m_edtLanguage->currentIndex());
    /*点动速度*/
    document.elementsByTagName("speed").at(0).toElement().setAttribute("jog", ui->m_edtJog->text());
    /*最大速度*/
    document.elementsByTagName("speed").at(0).toElement().setAttribute("max", ui->m_edtJog->text());
    /*尺寸设置*/
    document.elementsByTagName("size").at(0).toElement().setAttribute("val", ui->m_edtSize->currentIndex()+1);
    /*目标产量*/
    document.elementsByTagName("targetYield").at(0).toElement().setAttribute("val", ui->m_edtOutput->text());

    QFile file(CONFIG_FILE_XML_PATH);
    file.open(QIODevice::WriteOnly|QFile::Truncate);
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();

    myHelper::showMessageBoxInfo(tr("保存成功！"), 2);
}

/*系统时间控件定时刷新*/
void Frm_parameterSettings::initSystemTime()
{
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    m_timer->start(100);
}

/*系统时间控件定时刷新*/
void Frm_parameterSettings::timerUpDate()
{
    QDateTime QDateCurentTime= QDateTime::currentDateTime();//获取系统现在的时间
    QString strCurentTime = QDateCurentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    ui->m_edtSysTime->setText(strCurentTime);//在标签上显示时间
}

/*向XDDP发送数据*/
void Frm_parameterSettings::writeToXddp(QString operMode)
{
    QJsonObject json;
    json.insert("mesg_dir", "req");
    QJsonObject jsContent;
    if(!operMode.isEmpty())
        jsContent.insert("operMode", operMode);

    json.insert("mesg_type", "zeroing");
    json.insert("content", jsContent);

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Indented);

    emit xddpDataToScheduler(byteArray);
}

/*处理XDDP数据*/
void Frm_parameterSettings::handleXddpData(QByteArray data)
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

            if(object.value("mesg_type").toString() == "zeroing")
            {
                ui_pop.m_labPulse->setText(QString("%1").arg(obj.value("needle_no").toInt()));
            }
        }
    }

}
