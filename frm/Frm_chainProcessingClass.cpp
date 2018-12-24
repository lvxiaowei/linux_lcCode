#include "Frm_chainProcessingClass.h"
#include "ui_Frm_chainProcessingClass.h"
#include <QFileSystemModel>
#include <QDirModel>
Frm_chainProcessingClass::Frm_chainProcessingClass(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_chainProcessingClass)
{
    g_mapIndexToWgt[PAGE_CHAINPROCESSING]= this;
    ui->setupUi(this);
}

Frm_chainProcessingClass::~Frm_chainProcessingClass()
{
    delete ui;
}


/*处理串口数据*/
void Frm_chainProcessingClass::keyPressEvent(int key)
{
    qDebug()<<"当前正在链条编辑界面,key="<<key;

    switch (ui->stackedWidget->currentIndex()) {
    case 0:
        dealPg1(key);
        break;
    case 1:
        dealPg2(key);
        break;
    case 2:
        dealPg3(key);
        break;
    default:
        break;
    }
}

/*处理串口数据-page1*/
void Frm_chainProcessingClass::dealPg1(int key)
{
    if(myMessageBox::getInstance()->isVisible())
    {
        switch (key) {
        case Key_Set:
        {
            if(myMessageBox::getInstance()->getMessage().contains(QString(tr("确定要删除"))))
            {
                QFile file(m_delFile);
                file.remove();
            }
            else {
                QFile file(m_destFile);
                file.remove();
                QFile::copy(m_sourceFile, m_destFile);
            }
            initChainManageTable();
            myMessageBox::getInstance()->hide();
            break;
        }
        case Key_Esc:
            myMessageBox::getInstance()->hide();
        default:
            break;
        }
    }

    switch (key) {
    case Key_F0:
        setNextDealWgt(PAGE_SETTING);
        break;
    case Key_F1:
    {
        m_sourceFile = m_treeFileModel->filePath(ui->m_treeFile->currentIndex());
        m_destFile =  QString("%1/%2").arg(PATH_CHAIN_FILE_LOCAL).arg(m_treeFileModel->fileName(ui->m_treeFile->currentIndex()));
        /*判断本地是否有同名文件*/
        if(isFileExist(m_destFile))
        {
            myMessageBox::getInstance()->setMessage(tr("本地有同名文件，是否覆盖？"), BoxQuesion);
        }
        else {
            QFile::copy(m_sourceFile, m_destFile);
            initChainManageTable();
            myMessageBox::getInstance()->setMessage(tr("从U盘拷贝到本地成功！"), BoxInfo);
        }
        break;
    }
    case Key_F2:
    {
        m_sourceFile = ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),2)->text();
        m_destFile = QString("%1/%2").arg(USB_PATH).arg(ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),3)->text());
        /*判断本地是否有同名文件*/
        if(isFileExist(m_destFile))
        {
            myMessageBox::getInstance()->setMessage(tr("U盘有同名文件，是否覆盖？"), BoxQuesion);
        }
        else {
            QFile::copy(m_sourceFile, m_destFile);
            myMessageBox::getInstance()->setMessage(tr("从本地拷贝到U盘成功！"), BoxInfo);
        }
        break;
    }
    case Key_F3:
    {
        QString fileName;
        if( ui->m_labCurentOperForder->text()==tr("本地"))
        {
            m_delFile = ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),2)->text();
            fileName = ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),3)->text();

            /*如果是正在运行的链条，不可以删除*/
            if(fileName == ui->m_labWorkChain->text())
            {
                myMessageBox::getInstance()->setMessage(tr("不可以删除当前正在工作的链条！"), BoxInfo);
                return;
            }
        }
        else {
            m_delFile = m_treeFileModel->filePath(ui->m_treeFile->currentIndex());
            fileName = m_treeFileModel->fileName(ui->m_treeFile->currentIndex());
        }
        myMessageBox::getInstance()->setMessage(QString(tr("确定要删除%1文件%2吗？")).arg(ui->m_labCurentOperForder->text()).arg(fileName), BoxQuesion);
        break;
    }
    case Key_F4:
    {
        initChainTree();
        ui->stackedWidget->setCurrentIndex(1);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("")<<tr("下一菜单\n[1/2]"));
        break;
    }
    case Key_F5:
    {
        QString fileName=ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),3)->text();

        if(fileName == ui->m_labWorkChain->text()) return;

        ui->m_labWorkChain->setText(fileName);

        QDomDocument document;
        if(!getXmlConfig(document))
            return;
        QDomNodeList nodeTimTab = document.elementsByTagName("activeFile");
        if(nodeTimTab.count()!=0)
        {
            nodeTimTab.at(0).toElement().setAttribute("sta", ui->m_labWorkChain->text());
        }
        QFile file(CONFIG_FILE_XML_PATH);
        file.open(QIODevice::WriteOnly|QFile::Truncate);
        QTextStream stream(&file);
        document.save(stream, 4);
        file.close();

        myMessageBox::getInstance()->setMessage(tr("运行链条设置成功!"), BoxInfo);
        break;
    }

    case Key_Set:
    {
        if(ui->m_labCurentOperForder->text()==tr("本地")) return;

        QModelIndex index = ui->m_treeFile->currentIndex();

        ui->m_treeFile->isExpanded(index) ? ui->m_treeFile->collapse(index):ui->m_treeFile->expand(index);
        break;
    }

    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        ui->m_labCurentOperForder->text()==tr("本地") ? QCoreApplication::sendEvent(ui->m_tabChainManage, &keyPress):QCoreApplication::sendEvent(ui->m_treeFile, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        ui->m_labCurentOperForder->text()==tr("本地") ? QCoreApplication::sendEvent(ui->m_tabChainManage, &keyPress):QCoreApplication::sendEvent(ui->m_treeFile, &keyPress);
        break;
    }
    case Key_Left:
    {
        ui->m_labCurentOperForder->setText(tr("本地"));
        break;
    }
    case Key_Right:
    {
        ui->m_labCurentOperForder->setText(tr("u盘"));
        break;
    }

    default:
        break;
    }
}

/*处理串口数据-page2*/
void Frm_chainProcessingClass::dealPg2(int key)
{
    if(myMessageBox::getInstance()->isVisible())
    {
        switch (key) {
        case Key_Set:
        {
            QTreeWidgetItem* item = ui->m_chainTree->currentItem();
            delete item;
            item =NULL;
            secendLevelNodeSort();
            myMessageBox::getInstance()->hide();
            break;
        }
        case Key_Esc:
            myMessageBox::getInstance()->hide();
        default:
            break;
        }
    }

    if(g_lstRightButton.at(5)->text() == tr("下一菜单\n[1/2]"))
    {
        dealPg2_1(key);
    }
    else if(g_lstRightButton.at(5)->text() == tr("上一菜单\n[2/2]"))
    {
        dealPg2_2(key);
    }
}

/*处理串口数据-page2-1*/
void Frm_chainProcessingClass::dealPg2_1(int key)
{
    switch (key) {
    case Key_F0:
        ui->stackedWidget->setCurrentIndex(0);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("从U盘\n输入")<<tr("输出到\nU盘")<<tr("删除")<<tr("编辑\n工作链条")<<tr("工作链条\n设定"));
        break;
    case Key_F1:
    {
        if(ui->m_chainTree->currentItem()->type()==FIRST_LEVEL_NODE)
        {
            myMessageBox::getInstance()->setMessage(tr("请选择具体需要新增的步骤！"), BoxInfo);
            return;
        }
        initCmdEdit();
        ui->cmd_val1->setFocus();
        ui->stackedWidget->setCurrentIndex(2);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("确认添加")<<tr("")<<tr("")<<tr("")<<tr("[新增命令]"));
        break;
    }
    case Key_F2:
    {
        myMessageBox::getInstance()->setMessage(tr("确认要删除当前步骤及步骤内的所有内容吗？"), BoxQuesion);
        break;
    }
    case Key_F3:
    {
        break;
    }
    case Key_F4:

        break;
    case Key_F5:
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

/*处理串口数据-page2-2*/
void Frm_chainProcessingClass::dealPg2_2(int key)
{
    switch (key) {
    case Key_F0:
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
    case Key_F1:
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
    case Key_F2:
        break;
    case Key_F3:
        break;
    case Key_F4:
        break;
    case Key_F5:
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
    case Key_F0:
    {
        ui->stackedWidget->setCurrentIndex(1);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("")<<tr("下一菜单\n[1/2]"));
        break;
    }
    case Key_F1:
    {

        break;
    }
    case Key_F2:
        break;
    case Key_F3:
        break;
    case Key_F4:
        break;
    case Key_F5:
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
    ui->m_labCurentOperForder->setText(tr("本地"));
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("从U盘\n输入")<<tr("输出到\nU盘")<<tr("删除")<<tr("编辑\n工作链条")<<tr("工作链条\n设定"));

    /*初始化链条管理窗口*/
    initChainManageTable();

    /*获取当前正在运行的链条名字*/
    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    QDomNodeList nodeTimTab = document.elementsByTagName("activeFile");
    if(nodeTimTab.count()!=0)
    {
        ui->m_labWorkChain->setText(nodeTimTab.at(0).toElement().attribute("sta"));
    }

    /*初始化usb窗口*/
    m_treeFileModel = new QFileSystemModel();
    m_treeFileModel->setRootPath(USB_PATH);

    QStringList nameFilter;
    nameFilter << "*.sta" << "*.dis";
    m_treeFileModel->setNameFilterDisables(false);
    m_treeFileModel->setNameFilters(nameFilter);
    ui->m_treeFile->setModel(m_treeFileModel);
    ui->m_treeFile->setRootIndex(m_treeFileModel->index(USB_PATH));
    ui->m_treeFile->setColumnHidden(1,true);
    ui->m_treeFile->setColumnHidden(2,true);
    ui->m_treeFile->setColumnHidden(3,true);

    ui->m_treeFile->setCurrentIndex(m_treeFileModel->index(0,0));

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
    myHelper::sleep(10);
    QCoreApplication::sendEvent(ui->m_treeFile, &keyPress);
}

//初始化链条命令树
void Frm_chainProcessingClass::initChainTree()
{
    ui->m_chainTree->clear();

    DBProcessingClass::GetInstance()->openDB(PATH_CHAIN_ACTIVE_FILE);

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

/*初始化链条管理表格*/
void Frm_chainProcessingClass::initChainManageTable()
{
    /*start*******************************************初始化链条管理窗口******************************************************/
    /*初始化表格数据*/
    ui->m_tabChainManage->clearContents();
    ui->m_tabChainManage->setRowCount(0);
    ui->m_tabChainManage->setColumnCount(4); //设置总列数；
    ui->m_tabChainManage->setColumnWidth(0,50);
    ui->m_tabChainManage->setColumnWidth(1,90);
    ui->m_tabChainManage->setColumnWidth(2,0);

    ui->m_tabChainManage->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("文件大小")<<tr("文件路径")<<tr("链条文件")); //设置表头名称；

    QFont font=ui->m_tabChainManage->horizontalHeader()->font();  //设置表头的字体为粗体；
    font.setBold(true);
    font.setPixelSize(20);
    ui->m_tabChainManage->horizontalHeader()->setFont(font);

    ui->m_tabChainManage->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui->m_tabChainManage->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui->m_tabChainManage->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));
    ui->m_tabChainManage->horizontalHeaderItem(3)->setTextColor(QColor(0,85,0));

    ui->m_tabChainManage ->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_tabChainManage->horizontalHeader()->setFixedHeight(43);                  //设置表头的高度；

    QDir dir(PATH_CHAIN_FILE_LOCAL);
    if(!dir.exists())
    {
        return;
    }
    else
    {
        QStringList filters;     //定义过滤变量；
        filters<< QString("*.sta")<< QString("*.dis");
        QDirIterator dir_iterator(PATH_CHAIN_FILE_LOCAL,filters,QDir::Files | QDir::NoSymLinks,QDirIterator::Subdirectories);//定义迭代器并设置过滤器；
        QString fileName_str,fileSize_str; //定义文件名称，文件的大小；
        while(dir_iterator.hasNext())
        {
            dir_iterator.next();
            QFileInfo file_info=dir_iterator.fileInfo();

            fileName_str=file_info.fileName();
            fileSize_str=QString::number(file_info.size())+tr(" 字节");
            int rows=ui->m_tabChainManage->rowCount();
            ui->m_tabChainManage->insertRow(rows);
            ui->m_tabChainManage->setItem(rows,0,new QTableWidgetItem(QString("%1").arg(rows+1)));

            ui->m_tabChainManage->setItem(rows,1,new QTableWidgetItem(fileSize_str));
            ui->m_tabChainManage->setItem(rows,2,new QTableWidgetItem(file_info.filePath()));
            ui->m_tabChainManage->setItem(rows,3,new QTableWidgetItem(fileName_str));
            ui->m_tabChainManage->item(rows,0)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabChainManage->item(rows,1)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabChainManage->item(rows,3)->setTextAlignment(Qt::AlignCenter);
        }
    }

    if(ui->m_tabChainManage->rowCount()>0)
        ui->m_tabChainManage->setCurrentCell(0,0);

    /*end*******************************************初始化链条管理窗口******************************************************/
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

bool Frm_chainProcessingClass::isFileExist(QString fileFullName)
{
    QFileInfo fileInfo(fileFullName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}
