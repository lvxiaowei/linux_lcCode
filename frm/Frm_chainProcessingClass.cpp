#include "Frm_chainProcessingClass.h"
#include "ui_Frm_chainProcessingClass.h"

Frm_chainProcessingClass::Frm_chainProcessingClass(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_chainProcessingClass)
{
    g_mapIndexToWgt[PAGE_CHAINPROCESSING]= this;
    ui->setupUi(this);

    //几种不同命令类型的正则匹配
    rx1=QRegExp(tr("(.*)[\[]状态：(.*)---针位：(.*)---模式：(.*)[]]"));             /*增加命令参数设置页面*/
    rx2=QRegExp(tr("(.*)[\[]转速：(.*)---加速圈数：(.*)[]]"));                      /*增加速度设置页面*/
    rx3=QRegExp(tr("(.*)[\[]宏：(.*)---动作针位：(.*)---模式：(.*)[]]"));            /*增加宏链接设置页面*/
    rx4_1=QRegExp(tr("(.*)[\[]电机转向：(.*)---模式：(.*)---针位：(.*)[]]"));        /*增加电机转向设置页面-1*/
    rx4_2=QRegExp(tr("(.*)[\[]电机转向：(.*)---模式：(.*)[]]"));                    /*增加电机转向设置页面-2*/
    rx5=QRegExp(tr("(.*)[\[]动作状态：(.*)---针位：(.*)[]]"));                      /*增加低速度设置页面*/
    rx6=QRegExp(tr("(.*)[\[]选针编号：(.*)---动作方式：(.*)---选针模式：(.*)[]]"));   /*增加选针模式设置页面*/
    rx7=QRegExp(tr("(.*)[\[]向左移：(.*)---向右移：(.*)[]]"));                      /*增加脚底花纹宽度页面*/
    rx8=QRegExp(tr("(.*)[\[]花纹名称：(.*)[]]"));                                  /*增加花纹调用页面*/
    rx9=QRegExp(tr("(.*)[\[](.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)[]]"));       /*循环页面*/
    rx10=QRegExp(tr("(.*)[\[]动 作状态：(.*)[]]"));                                /*禁止归零页面*/
}

Frm_chainProcessingClass::~Frm_chainProcessingClass()
{
    delete ui;
}


/*处理串口数据*/
void Frm_chainProcessingClass::keyPressEvent(int key)
{
    qDebug()<<"当前正在链条编辑界面,key="<<key;

    if(myMessageBox::getInstance()->isVisible())
    {
        switch (key) {
        case Key_plus:
        {
            QTreeWidgetItem* item = ui->m_chainTree->currentItem();
            delete item;
            item =NULL;
            secendLevelNodeSort();
            myMessageBox::getInstance()->hide();
            break;
        }
        case Key_minus:
            myMessageBox::getInstance()->hide();
        default:
            break;
        }
    }

    if(g_lstRightButton.at(5)->text() == tr("下一菜单\n[1/2]"))
    {
        dealPg1(key);
    }
    else if(g_lstRightButton.at(5)->text() == tr("上一菜单\n[2/2]"))
    {
        dealPg2(key);
    }
    else if(g_lstRightButton.at(5)->text() == tr("[新增命令]"))
    {
        dealPg3(key);
    }
}

/*处理串口数据-page1*/
void Frm_chainProcessingClass::dealPg1(int key)
{
    switch (key) {
    case Key_F9:
        setNextDealWgt(PAGE_SETTING);
        break;
    case Key_F8:
    {
        if(ui->m_chainTree->currentItem()->type()==FIRST_LEVEL_NODE)
        {
            myMessageBox::getInstance()->setMessage(tr("请选择具体需要新增的步骤！"), BoxInfo);
            return;
        }
        ui->stackedWidget->setCurrentIndex(1);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("确认添加")<<tr("")<<tr("")<<tr("")<<tr("[新增命令]"));
        break;
    }
    case Key_9:
    {
        myMessageBox::getInstance()->setMessage(tr("确认要删除当前步骤及步骤内的所有内容吗？"), BoxQuesion);
        break;
    }
    case Key_8:
    {
        break;
    }
    case Key_7:

        break;
    case Key_PageUp:
    {
        freshRightButtonContent(QStringList()<<tr("复制")<<tr("粘贴")<<tr("")<<tr("")<<tr("")<<tr("上一菜单\n[2/2]"));
        break;
    }
    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_chainTree, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_chainTree, &keyPress);
        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page2*/
void Frm_chainProcessingClass::dealPg2(int key)
{
    switch (key) {
    case Key_F9:
    {
        if(ui->m_chainTree->currentItem()->type()==THIRD_LEVEL_NODE){
            m_cpItem = ui->m_chainTree->currentItem()->clone();
            myMessageBox::getInstance()->setMessage(tr("复制成功！"), BoxInfo);
        }
        else {
            myMessageBox::getInstance()->setMessage(tr("只能复制步骤下面的具体动作！"), BoxInfo);
        }
        break;
    }
    case Key_F8:
    {
        if(!m_cpItem)
        {
            myMessageBox::getInstance()->setMessage(tr("当前没有复制操作!"), BoxInfo);
        }
        else if(ui->m_chainTree->currentItem()->type()==FIRST_LEVEL_NODE) {
            myMessageBox::getInstance()->setMessage(tr("请选择具体需要粘贴的步骤！"), BoxInfo);
        }
        else {
            QTreeWidgetItem* itemLv2 = (ui->m_chainTree->currentItem()->type()==SECEND_LEVEL_NODE ?ui->m_chainTree->currentItem():ui->m_chainTree->currentItem()->parent());
            QTreeWidgetItem* itemLv3  = new QTreeWidgetItem(itemLv2,QStringList()<<m_cpItem->text(0),THIRD_LEVEL_NODE);
            itemLv3->setIcon(0, QIcon(ComConfigClass::GetInstance()->getCmdIcoPathByName(itemLv3->text(0).split(" [").at(0))));
            myMessageBox::getInstance()->setMessage(tr("粘贴成功！"), BoxInfo);
        }
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
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("")<<tr("下一菜单\n[1/2]"));
        break;
    }
    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_chainTree, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_chainTree, &keyPress);
        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page3*/
void Frm_chainProcessingClass::dealPg3(int key)
{
    switch (key) {
    case Key_F9:
    {
        ui->stackedWidget->setCurrentIndex(0);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("")<<tr("下一菜单\n[1/2]"));
        break;
    }
    case Key_F8:
    {

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
        break;
    }
    case Key_Up:
    case Key_Down:
    {
        QKeyEvent key_up(QEvent::KeyPress, m_iCmdPagePos==2 ? Qt::Key_Back:Qt::Key_Up, Qt::NoModifier, QString());
        QKeyEvent key_down(QEvent::KeyPress, m_iCmdPagePos==2 ? Qt::Key_Tab:Qt::Key_Down, Qt::NoModifier, QString());
        if(m_iCmdPagePos==0)
        {
            QCoreApplication::sendEvent(ui->m_wgtCmdName, key==Key_Up ? &key_up:&key_down);
        }
        else if(m_iCmdPagePos==1)
        {
            QCoreApplication::sendEvent(ui->m_wgtCmdContent, key==Key_Up ? &key_up:&key_down);
        }
        else if(m_iCmdPagePos==2){
            if((key == Key_Up && ui->cmd_val1->hasFocus())||(key == Key_Down && ui->cmd_val8->hasFocus()))
                return;
            key==Key_Up  ? focusPreviousChild():focusNextChild();
        }

        break;
    }
    case Key_Left:
    {
        if(m_iCmdPagePos>0)
        {
            m_iCmdPagePos--;

        }
        break;
    }
    case Key_Right:
    {
        if(m_iCmdPagePos<2)
        {
            m_iCmdPagePos++;

        }
        break;
    }
    default:
        break;
    }
}

/*数据初始化*/
void Frm_chainProcessingClass::initShowFrmConfig()
{
    m_cpItem =NULL;
    m_iCmdPagePos=0;
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("")<<tr("下一菜单\n[1/2]"));
    initChainTree();

    initCmdEdit();
    ui->cmd_val1->setFocus();
}

//初始化链条命令树
void Frm_chainProcessingClass::initChainTree()
{
    ui->m_chainTree->clear();

    DBProcessingClass::GetInstance()->openDB(CHAIN_ACTIVE_FILE_PATH);

    //初始化主链条一级节点
    QString select_sql = "select distinct weavingPosition from mainChainTable order by step";
    QTreeWidgetItem* itemLv1,* itemLv2,* itemLv3;
    QSqlQuery sql_query;
    sql_query.prepare(select_sql);
    if(sql_query.exec())
    {
        while(sql_query.next())
        {
            itemLv1  = new QTreeWidgetItem(ui->m_chainTree,QStringList()<<sql_query.value(0).toString(),FIRST_LEVEL_NODE);
            itemLv1->setIcon(0, QIcon(":/image/socks.png"));
        }
    }
    //初始化主链条二级节点
    for(int i=0; i<ui->m_chainTree->topLevelItemCount(); ++i)
    {
        QString strLve1 = ui->m_chainTree->topLevelItem(i)->text(0);
        select_sql = QString("select distinct step from mainChainTable where weavingPosition = \"%1\" order by step").arg(strLve1);
        sql_query.prepare(select_sql);
        if(sql_query.exec())
        {
            while(sql_query.next())
            {
                itemLv2  = new QTreeWidgetItem(ui->m_chainTree->topLevelItem(i),QStringList()<<sql_query.value(0).toString(),SECEND_LEVEL_NODE);
                itemLv2->setIcon(0, QIcon(":/image/right_arrow.png"));
            }
        }
    }
    //初始化主链条三级节点
    for(int i=0; i<ui->m_chainTree->topLevelItemCount(); ++i)
    {
        itemLv1 = ui->m_chainTree->topLevelItem(i);
        for(int j=0; j<itemLv1->childCount(); ++j)
        {
            itemLv2 = itemLv1->child(j);
            select_sql = QString("select  cmdContent from mainChainTable where weavingPosition = \"%1\" and step = %2").arg(itemLv1->text(0)).arg(itemLv2->text(0).toInt());
            sql_query.prepare(select_sql);
            if(sql_query.exec())
            {
                while(sql_query.next())
                {
                    itemLv3  = new QTreeWidgetItem(itemLv2,QStringList()<<sql_query.value(0).toString(),THIRD_LEVEL_NODE);
                    itemLv3->setIcon(0, QIcon(ComConfigClass::GetInstance()->getCmdIcoPathByName(itemLv3->text(0).split(" [").at(0))));

                }
            }
        }
    }
    ui->m_chainTree->expandAll();
    if(ui->m_chainTree->columnCount()>0)
    {
        ui->m_chainTree->setCurrentItem(ui->m_chainTree->topLevelItem(0));
    }
    secendLevelNodeSort();

    DBProcessingClass::GetInstance()->closeDB();
}

/*链条命令树二级节点排序*/
void Frm_chainProcessingClass::secendLevelNodeSort()
{
    int num = 0;
    for(int i=0; i<ui->m_chainTree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->m_chainTree->topLevelItem(i);
        if((NULL == item) || item->childCount() == 0) continue;
        for(int j=0; j<item->childCount(); ++j)
        {
            QTreeWidgetItem* itemChild = item->child(j);
            if(NULL == itemChild) continue;
            if(itemChild->childCount()==0)
            {
                delete itemChild;
                itemChild=NULL;
                continue;
            }
            itemChild->setText(0,QString(tr("步骤  %1")).arg(num));
            num++;
        }
    }
}

/*初始话命令模块新增窗口*/
void Frm_chainProcessingClass::initCmdEdit()
{
    QStringList lstCmdTYpes = ComConfigClass::GetInstance()->getCmdModelTypes();
    ui->m_wgtCmdName->clearContents();

    ui->m_wgtCmdName->clearContents();
    ui->m_wgtCmdName->setRowCount(lstCmdTYpes.count());
    ui->m_wgtCmdName->setColumnWidth(0,158);
    for(int i=0;i<ui->m_wgtCmdName->rowCount(); ++i)
    {
        ui->m_wgtCmdName->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(lstCmdTYpes.at(i))));
    }
    ui->m_wgtCmdName->setCurrentCell(0,0);
    freshCmdFormData(0);

    connect(ui->m_wgtCmdName,SIGNAL(itemSelectionChanged()), this, SLOT(freshCmdContent()));

    setTabOrder(ui->cmd_val1, ui->cmd_val2);
    setTabOrder(ui->cmd_val2, ui->cmd_val3);
    setTabOrder(ui->cmd_val3, ui->cmd_val4);
    setTabOrder(ui->cmd_val4, ui->cmd_val5);
    setTabOrder(ui->cmd_val5, ui->cmd_val6);
    setTabOrder(ui->cmd_val6, ui->cmd_val7);
    setTabOrder(ui->cmd_val7, ui->cmd_val8);

    m_iCmdPagePos=0;
}

/*刷新命令模块表格数据,可以根据关键字搜索，如果关键字为空，则根据索引搜索*/
void Frm_chainProcessingClass::freshCmdFormData(int index)
{
    QList<cmdSettingPro> lstSelectedModels;

    lstSelectedModels = ComConfigClass::GetInstance()->getSpecificModuleType(index);


    if(lstSelectedModels.count()<=0) return;

    ui->m_wgtCmdContent->clearContents();
    ui->m_wgtCmdContent->setRowCount(lstSelectedModels.count());
    ui->m_wgtCmdContent->setColumnWidth(0,393);

    for(int i=0;i<ui->m_wgtCmdContent->rowCount(); ++i)
    {
        cmdSettingPro cmdPro = lstSelectedModels.at(i);

        ui->m_wgtCmdContent->setItem(i, 0, new QTableWidgetItem(QIcon(QString(":/image/%1").arg(cmdPro.ico)), QString("   %1").arg(cmdPro.name)));
    }
    ui->m_wgtCmdContent->setCurrentCell(0,0);
    ui->m_wgtCmdContent->setIconSize(QSize(30,30));
}

void Frm_chainProcessingClass::freshCmdContent()
{
    int i = ui->m_wgtCmdName->currentRow();
    qDebug()<<"-------------------"<<i;
    freshCmdFormData(i);
}
