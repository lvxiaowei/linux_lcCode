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
    setTabOrder(ui->m_edtNeedl, ui->m_edtOilEnable);
    setTabOrder(ui->m_edtOilEnable,ui->m_edtOilFre);
    setTabOrder(ui->m_edtOilFre,ui->m_edtOilKeep);
    setTabOrder(ui->m_edtOilKeep,ui->m_edtScreen);
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
    case Key_F9:
        setNextDealWgt(PAGE_SETTING);
        break;
    case Key_F8:
        break;
    case Key_9:
        break;
    case Key_8:
        break;
    case Key_7:
        break;
    case Key_PageUp:
        break;


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
    {
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = ui_pop.m_edtPasWd->text() + strInputValue;
        ui_pop.m_edtPasWd->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_Del:
    {
        QString strCurentValue = ui_pop.m_edtPasWd->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        ui_pop.m_edtPasWd->setText(strCurentValue);
        break;
    }
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
            myMessageBox::getInstance()->setMessage(tr("登录成功!"), BoxInfo);
            connect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
        }
        else {
            myMessageBox::getInstance()->setMessage(tr("密码错误!"), BoxInfo);
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
    case Key_Esc:
    {
        w->hide();
        ui->m_edtZeroing->setFocus();
        disconnect(m_timer,SIGNAL(timeout()),this,SLOT(writeToXddp()));
        break;
    }
    case Key_Set:
    {
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
    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    {
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = ui_pop.m_edtTime->text() + strInputValue;
        ui_pop.m_edtTime->setText(strCurentValue.trimmed());

    }
        break;
    case Key_Del:
    {
        QString strCurentValue = ui_pop.m_edtTime->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        ui_pop.m_edtTime->setText(strCurentValue);
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
        QString strValue = ui_pop.m_edtTime->text();
        if(strValue.length()!=12 || ((strValue.mid(0,2).toInt()>12)||(strValue.mid(2,2).toInt()>31)||(strValue.mid(4,2).toInt()>24)
                                     ||(strValue.mid(6,2).toInt()>59)||(strValue.mid(8,2).toInt()>20)||(strValue.mid(10,2).toInt()>37)))
        {
            myMessageBox::getInstance()->setMessage(tr("时间格式不正确！"), BoxInfo);
        }
        else {
            QString str="date "+strValue;
            QProcess::execute(str); //用进程调用linux的命令:date
            QProcess::execute("hwclock -w");        //-w:将系统时钟同步到硬件时钟；-s:将硬件时钟同步到系统时钟；
            QProcess::execute("sync");              //sync命令：可用来强制将内存缓冲区中的数据写入磁盘中；
            myMessageBox::getInstance()->setMessage(tr("系统时间修改成功！"), BoxInfo);
        }
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
        ui_pop.m_edtTime->setFocus();
        w->show();
    }
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
void Frm_parameterSettings::writeToXddp()
{
    qDebug()<<"------------------"<<"获取实时脉冲";
}
