#include "Frm_patternManage.h"
#include "ui_Frm_patternManage.h"

Frm_patternManage::Frm_patternManage(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_patternManage),
    m_treeFileModel(NULL)
{
    ui->setupUi(this);
}

Frm_patternManage::~Frm_patternManage()
{
    delete ui;
}

/*处理串口数据*/
void Frm_patternManage::keyPressEvent(int key)
{
    qDebug()<<"当前正在花型管理界面,key="<<key;
    switch (ui->m_stackPat->currentIndex()) {
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
void Frm_patternManage::dealPg1(int key)
{
    if(myHelper::isMessageBoxShow())
    {
        switch (key) {
        case Key_Set:
        {
            if(myHelper::getMessageInfo().contains(QString(tr("确定要删除"))))
            {
                QFile file(m_delFile);
                file.remove();
            }
            else {
                QFile file(m_destFile);
                file.remove();
                QFile::copy(m_sourceFile, m_destFile);
            }
            initPatManageTabl();
            QUIMessageBox::Instance()->hide();
            break;
        }
        case Key_Esc:
            QUIMessageBox::Instance()->hide();
        default:
            break;
        }
    }

    switch (key) {
    case Key_F0:
    {
        setNextDealWgt(PAGE_SETTING);
        break;
    }
    case Key_F1:
    {
        initTimingsTable();
        break;
    }
    case Key_F2:
    {
        if(!ui->m_treeFile->currentIndex().isValid())
            return;

        m_sourceFile = m_treeFileModel->filePath(ui->m_treeFile->currentIndex());
        m_destFile =  QString("%1/%2").arg(PATH_CHAIN_FILE_LOCAL).arg(m_treeFileModel->fileName(ui->m_treeFile->currentIndex()));
        /*判断本地是否有同名文件*/
        if(isFileExist(m_destFile))
        {
            myHelper::showMessageBoxQuestion(tr("本地有同名文件，是否覆盖？"));
        }
        else {
            QFile::copy(m_sourceFile, m_destFile);
            initPatManageTabl();
            myHelper::showMessageBoxInfo(tr("从U盘拷贝到本地成功！"), 1);
        }
        break;
    }
    case Key_F3:
    {
        m_sourceFile = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),2)->text();
        m_destFile = QString("%1/%2").arg(USB_PATH).arg(ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),3)->text());
        /*判断本地是否有同名文件*/
        if(isFileExist(m_destFile))
        {
            myHelper::showMessageBoxQuestion(tr("U盘有同名文件，是否覆盖？"));
        }
        else {
            QFile::copy(m_sourceFile, m_destFile);
            myHelper::showMessageBoxInfo(tr("从本地拷贝到U盘成功！"), 1);
        }
        break;
    }
    case Key_F4:
    {
        QString fileName;
        if( ui->m_tabPatManage->hasFocus())
        {
            m_delFile = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),2)->text();
            fileName = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),3)->text();
        }
        else {
            if(!ui->m_treeFile->currentIndex().isValid())
                return;


            m_delFile = m_treeFileModel->filePath(ui->m_treeFile->currentIndex());
            fileName = m_treeFileModel->fileName(ui->m_treeFile->currentIndex());
        }
        myHelper::showMessageBoxQuestion(QString(tr("确定要删除%1文件%2吗？")).arg(ui->m_labCurentOperForder->text()).arg(fileName));
        break;
    }
    case Key_F5:
    {
        initPatternProcesPage();
        break;
    }

    case Key_Up:
    {
        ui->m_tabPatManage->hasFocus() ? QCoreApplication::sendEvent(ui->m_tabPatManage, key_up):QCoreApplication::sendEvent(ui->m_treeFile, key_up);
        break;
    }
    case Key_Down:
    {
        ui->m_tabPatManage->hasFocus() ? QCoreApplication::sendEvent(ui->m_tabPatManage, key_down):QCoreApplication::sendEvent(ui->m_treeFile, key_down);
        break;
    }
    case Key_Left:
    {
        ui->m_tabPatManage->setFocus();
        ui->m_labCurentOperForder->setText(tr("本地"));
        break;
    }
    case Key_Right:
    {
        ui->m_treeFile->setFocus();
        ui->m_labCurentOperForder->setText(tr("U盘"));
        break;
    }

    default:
        break;
    }
}

/*处理串口数据-page2*/
void Frm_patternManage::dealPg2(int key)
{

    switch (ui->m_stackPatOper->currentIndex()) {
    case 0:
    {
        ui->m_frmYFSet->isHidden() ? dealPg2_menu(key):dealPg2_YFSet(key);
        break;
    }
    case 1:
    {
        dealPg2_loop(key);
        break;
    }
    case 2:
    {
        break;
    }
    case 3:
    {
        break;
    }
    default:
        break;
    }

    switch (key) {
    case Key_plus:
    {
        m_pattrenTable->zoomOut();
        m_YFTable->zoomOut();
        break;
    }
    case Key_minus:
    {
        m_pattrenTable->zoomIn();
        m_YFTable->zoomIn();
        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page2-menu*/
void Frm_patternManage::dealPg2_menu(int key)
{
    switch (key) {
    case Key_F0:
    {
        ui->m_stackPat->setCurrentIndex(0);
        ui->m_title->setText(tr("[花型管理]"));
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("时序设置")<<tr("从U盘\n输入")<<tr("输出到\nU盘")<<tr("删除")<<tr("编辑"));
        delete m_pattrenTable;
        delete m_YFTable;
        m_pattrenTable = NULL;
        m_YFTable = NULL;
        break;
    }
    case Key_F1:
    {
        ui->m_frmYFSet->show();
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴选择")<<tr("起始行")<<tr("结束行")<<tr("操作")<<tr("设置"));
        break;
    }
    case Key_F2:
    {
        ui->m_stackPatOper->setCurrentIndex(1);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("")<<tr("")<<tr("")<<tr("")<<tr(""));
        break;
    }
    case Key_F5:
    {
        savePatternDataToFile();
        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page2-纱嘴设置界面处理*/
void Frm_patternManage::dealPg2_YFSet(int key)
{
    switch (key) {
    case Key_F0:
    {
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("")<<tr("")<<tr("保存"));
        ui->m_frmYFSet->hide();
        break;
    }
    case Key_F1:
    {
        int i = m_lstYF.indexOf(ui->m_labYfName->text());
        i++;
        ui->m_labYfName->setText( i>=m_lstYF.count() ? m_lstYF.at(0):m_lstYF.at(i));
        break;
    }
    case Key_F2:
    {
        ui->m_edtRowStar->setFocus();
        break;
    }
    case Key_F3:
    {
        ui->m_edtRowEnd->setFocus();
        break;
    }
    case Key_F4:
    {
        ui->m_labOperType->setText(ui->m_labOperType->text()==tr("选择") ? tr("取消"):tr("选择"));
        break;
    }
    case Key_F5:
    {
        bool ok;

        //这里的colunm得根据去除表格中的空白行换算出来的
        QString strYFName=ui->m_labYfName->text();
        int column(1);
        column = m_lstYF.indexOf(ui->m_labYfName->text()) + strYFName.left(1).toInt()*2 - 1;

        m_YFTable->setYF(column,ui->m_edtRowStar->text().toInt(&ok,10),ui->m_edtRowEnd->text().toInt(&ok,10),(ui->m_labOperType->text()==tr("选择")));
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
        if(!m_mapNoKeyToValue.contains(key) || !(ui->m_edtRowStar->hasFocus() ||ui->m_edtRowEnd->hasFocus()) ) break;

        QLineEdit *edt = (ui->m_edtRowStar->hasFocus() ? ui->m_edtRowStar:ui->m_edtRowEnd);
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = edt->text() + strInputValue;
        edt->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_minus:
    {
        if(!ui->m_edtRowStar->hasFocus() && !ui->m_edtRowEnd->hasFocus()) break;

        QLineEdit *edt = (ui->m_edtRowStar->hasFocus() ? ui->m_edtRowStar:ui->m_edtRowEnd);

        bool ok;
        QString strCurentValue = edt->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        edt->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        break;
    }

    default:
        break;
    }
}

/*处理串口数据-page2-循环设置界面处理*/
void Frm_patternManage::dealPg2_loop(int key)
{
    switch (key) {
    case Key_F0:
    {
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("")<<tr("")<<tr("保存"));
        ui->m_stackPatOper->setCurrentIndex(0);
        break;
    }


    case Key_Up:
    {
        QCoreApplication::sendEvent(ui->m_tabLoop, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(ui->m_tabLoop, key_down);
        break;
    }
    case Key_Left:
    {
        QCoreApplication::sendEvent(ui->m_tabLoop, key_left);
        break;
    }
    case Key_Right:
    {
        QCoreApplication::sendEvent(ui->m_tabLoop, key_right);
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
        if(ui->m_tabLoop->currentItem()->column()==0) break;

        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = ui->m_tabLoop->currentItem()->text() + strInputValue;
        ui->m_tabLoop->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_minus:
    {
        if(ui->m_tabLoop->currentItem()->column()==0) break;
        bool ok;
        QString strCurentValue = ui->m_tabLoop->currentItem()->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        ui->m_tabLoop->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        break;
    }

    default:
        break;
    }
}

/*处理串口数据-page3*/
void Frm_patternManage::dealPg3(int key)
{
    QTableWidget *wgtTable = dynamic_cast<QTableWidget*>(ui->m_tabYF->currentWidget());
    if(wgtTable==NULL) return;

    switch (key) {
    case Key_F0:
    {
        setNextDealWgt(PAGE_PATTERNMANAGE);
        break;
    }
    case Key_F1:
    {
        break;
    }
    case Key_F2:
    {
        break;
    }
    case Key_F3:
    {

        break;
    }
    case Key_F4:
    {

        break;
    }
    case Key_F5:
    {
        saveTimingsDataToConfigFile();
        break;
    }

    case Key_Up:
    {
        if((wgtTable->currentRow()==1)) return;
        QCoreApplication::sendEvent(wgtTable, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(wgtTable, key_down);
        break;
    }
    case Key_Left:
    {
        if((wgtTable->currentColumn()==1)) return;

        QCoreApplication::sendEvent(wgtTable, key_left);
        break;
    }
    case Key_Right:
    {
        QCoreApplication::sendEvent(wgtTable, key_right);
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
        if((wgtTable->currentColumn()==3)) return;
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = wgtTable->currentItem()->text() + strInputValue;
        wgtTable->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_minus:
    {
        if((wgtTable->currentColumn()==3)) return;
        bool ok;
        QString strCurentValue = wgtTable->currentItem()->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        wgtTable->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        break;
    }
    default:
        break;
    }
}

/*向XDDP发送数据*/
void Frm_patternManage::writeToXddp()
{
    QJsonObject json;
    json.insert("mesg_type", "parameter_set");
    json.insert("mesg_dir", "req");
    QJsonObject jsContent;

    jsContent.insert("index", oper_ChainChange);
    json.insert("content", jsContent);

    // 构建 JSON 文档
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Indented);
    emit xddpDataToScheduler(byteArray);
}

/*数据初始化*/
void Frm_patternManage::initShowFrmConfig()
{
    ui->m_stackPat->setCurrentIndex(0);
    ui->m_title->setText(tr("[花型管理]"));
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("时序设置")<<tr("从U盘\n输入")<<tr("输出到\nU盘")<<tr("删除")<<tr("编辑"));
    initPatManageTabl();
    ui->m_frmYFSet->hide();

    /*初始化usb窗口*/
    QDir dir(USB_PATH);
    bIsUExit = dir.exists();
    if(bIsUExit)
    {
        if(m_treeFileModel!=NULL)
        {
            delete m_treeFileModel;
        }
        m_treeFileModel = new QFileSystemModel();
        m_treeFileModel->setRootPath(USB_PATH);

        QStringList nameFilter;
        nameFilter << "*.dis";
        m_treeFileModel->setNameFilterDisables(false);
        m_treeFileModel->setNameFilters(nameFilter);
        ui->m_treeFile->setModel(m_treeFileModel);
        ui->m_treeFile->setRootIndex(m_treeFileModel->index(USB_PATH));
        ui->m_treeFile->setColumnHidden(1,true);
        ui->m_treeFile->setColumnHidden(2,true);
        ui->m_treeFile->setColumnHidden(3,true);

        ui->m_treeFile->setCurrentIndex(m_treeFileModel->index(0,0));
    }

    myHelper::sleep(10);
    QCoreApplication::sendEvent(ui->m_treeFile, key_up);
}

bool Frm_patternManage::isFileExist(QString fileFullName)
{
    QFileInfo fileInfo(fileFullName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}

/*初始化花型编辑界面*/
void Frm_patternManage::initPatternProcesPage()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("")<<tr("")<<tr("保存"));
    ui->m_stackPat->setCurrentIndex(1);

    QString filePath = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),2)->text();
    QString fileName = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),3)->text();
    ui->m_title->setText(QString(tr("[花型编辑] %1")).arg(fileName));

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        myHelper::showMessageBoxInfo(tr("文件打开失败!"), 1);
        return;
    }
    QByteArray bt = file.readAll();

    file.close();

    PatternFile_Head *fileHead = (PatternFile_Head*)bt.data();

    m_iScale = 10;
    /*******************************初始化图案表格**************************************/

    m_pattrenTable = new patternTableWgt(this, fileHead->needles, fileName.split(".").last(), m_iScale, bt);
    ui->m_grdPat->addWidget(m_pattrenTable);
    m_pattrenTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    /**************************************初始化刷子填色表格**************************************/
    QMap<QString, int>  m_mapYF;
    m_mapYF.clear();
    m_lstYF.clear();
    for(int i=0; i<fileHead->machineRoads; ++i)
    {
        m_mapYF[QString(tr("%1路纱嘴")).arg(i+1)] = fileHead->YF_Amount[i];
        for(int j=0; j<fileHead->YF_Amount[i]; ++j)
        {
            m_lstYF<< QString(tr("%1路%2号纱嘴")).arg(i+1).arg(j+1);
        }
    }

    m_YFTable = new YFTableWgt(m_mapYF, this, m_iScale,  m_pattrenTable->rowCount(), bt);
    ui->m_layoutYf->addWidget(m_YFTable);
    m_YFTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    /**************************************初始化循环设置**************************************/
    ui->m_tabLoop->clearContents();
    ui->m_tabLoop->setRowCount(20);
    ui->m_tabLoop->setColumnCount(4); //设置总列数；
    ui->m_tabLoop->setColumnWidth(0,40);
    ui->m_tabLoop->setColumnWidth(1,60);
    ui->m_tabLoop->setColumnWidth(2,60);
    ui->m_tabLoop->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->m_tabLoop->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("起始")<<tr("结束")<<tr("行数"));
    QFont font=ui->m_tabLoop->horizontalHeader()->font();  //设置表头的字体为粗体；
    font.setBold(true);
    font.setPixelSize(20);
    ui->m_tabLoop->horizontalHeader()->setFont(font);

    ui->m_tabLoop->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui->m_tabLoop->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui->m_tabLoop->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));
    ui->m_tabLoop->horizontalHeaderItem(3)->setTextColor(QColor(0,85,0));
    int m,n,j;
    for(int i=0; i<20; ++i)
    {
        m = (quint8)bt.at(fileHead->REP_InfoPos+i*6)+(quint8)bt.at(fileHead->REP_InfoPos+i*6+1)*256;
        n = (quint8)bt.at(fileHead->REP_InfoPos+i*6+2)+(quint8)bt.at(fileHead->REP_InfoPos+i*6+2+1)*256;
        j = (quint8)bt.at(fileHead->REP_InfoPos+i*6+4)+(quint8)bt.at(fileHead->REP_InfoPos+i*6+4+1)*256;

        ui->m_tabLoop->setItem(i,0,new QTableWidgetItem(QString("%1").arg(i)));
        ui->m_tabLoop->setItem(i,1,new QTableWidgetItem(QString("%1").arg(m)));
        ui->m_tabLoop->setItem(i,2,new QTableWidgetItem(QString("%1").arg(n)));
        ui->m_tabLoop->setItem(i,3,new QTableWidgetItem(QString("%1").arg(j)));
        ui->m_tabLoop->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->m_tabLoop->item(i,1)->setTextAlignment(Qt::AlignCenter);
        ui->m_tabLoop->item(i,2)->setTextAlignment(Qt::AlignCenter);
        ui->m_tabLoop->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }
    ui->m_tabLoop->setCurrentCell(0,1);
    /**************************************初始化选针器配色设置**************************************/
    QMap<int, QColor> selectedColor = m_pattrenTable->getSelectedColor();
    QList<int> lstNum= selectedColor.keys();
    QTreeWidgetItem* item,*itemChild;
    ui->m_needlSelectColor->clear();
    ui->m_needlSelectColor->setColumnCount(2);
    ui->m_needlSelectColor->setColumnWidth(0,80);
    ui->m_needlSelectColor ->setSelectionBehavior(QAbstractItemView::SelectItems);
    foreach (QString str, m_mapYF.keys()) {
        item = new QTreeWidgetItem(ui->m_needlSelectColor, QStringList()<<str.replace(tr("纱嘴"), tr("选针器")),FIRST_LEVEL_NODE);
        foreach (int i, lstNum) {
            itemChild = new QTreeWidgetItem(item, QStringList()<<QString("%1").arg(i)<<QString(" "),SECEND_LEVEL_NODE);
            itemChild->setCheckState(0,Qt::Unchecked);
            itemChild->setBackgroundColor(1,selectedColor[i]);
        }
    }
    ui->m_needlSelectColor->setCurrentItem(ui->m_needlSelectColor->topLevelItem(0));
    ui->m_needlSelectColor->expandAll();
}

/*初始化花型管理表格*/
void Frm_patternManage::initPatManageTabl()
{
    /*初始化表格数据*/
    ui->m_tabPatManage->clearContents();
    ui->m_tabPatManage->setRowCount(0);
    ui->m_tabPatManage->setColumnCount(4); //设置总列数；
    ui->m_tabPatManage->setColumnWidth(0,50);
    ui->m_tabPatManage->setColumnWidth(1,120);
    ui->m_tabPatManage->setColumnWidth(2,0);

    ui->m_tabPatManage->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("文件大小")<<tr("文件路径")<<tr("花型文件")); //设置表头名称；

    QFont font=ui->m_tabPatManage->horizontalHeader()->font();  //设置表头的字体为粗体；
    font.setBold(true);
    font.setPixelSize(20);
    ui->m_tabPatManage->horizontalHeader()->setFont(font);

    ui->m_tabPatManage->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(3)->setTextColor(QColor(0,85,0));

    ui->m_tabPatManage ->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_tabPatManage->horizontalHeader()->setFixedHeight(43);                  //设置表头的高度；

    QDir dir(PATTERN_FILE_LOCAL_PATH);
    if(!dir.exists())
    {
        return;
    }
    else
    {
        QStringList filters;     //定义过滤变量；
        filters<< QString("*.dis");
        QDirIterator dir_iterator(PATTERN_FILE_LOCAL_PATH,filters,QDir::Files | QDir::NoSymLinks,QDirIterator::Subdirectories);//定义迭代器并设置过滤器；
        QString fileName_str,fileSize_str; //定义文件名称，文件的大小；
        while(dir_iterator.hasNext())
        {
            dir_iterator.next();
            QFileInfo file_info=dir_iterator.fileInfo();

            fileName_str=file_info.fileName();
            fileSize_str=QString::number(file_info.size())+tr(" 字节");
            int rows=ui->m_tabPatManage->rowCount();
            ui->m_tabPatManage->insertRow(rows);
            ui->m_tabPatManage->setItem(rows,0,new QTableWidgetItem(QString("%1").arg(rows+1)));

            ui->m_tabPatManage->setItem(rows,1,new QTableWidgetItem(fileSize_str));
            ui->m_tabPatManage->setItem(rows,2,new QTableWidgetItem(file_info.filePath()));
            ui->m_tabPatManage->setItem(rows,3,new QTableWidgetItem(fileName_str));
            ui->m_tabPatManage->item(rows,0)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabPatManage->item(rows,1)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabPatManage->item(rows,3)->setTextAlignment(Qt::AlignCenter);
        }
    }

    if(ui->m_tabPatManage->rowCount()>0)
        ui->m_tabPatManage->setCurrentCell(0,0);
}

/*初始化花型进出时序表格*/
void Frm_patternManage::initTimingsTable()
{
    ui->m_stackPat->setCurrentIndex(2);
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("")<<tr("")<<tr("")<<tr("")<<tr("保存修改"));
    //移除所有的tab
    for(int i=0; i<ui->m_tabYF->count(); ++i)
    {
        ui->m_tabYF->removeTab(0);
    }

    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    QDomNodeList nodeTimTab = document.elementsByTagName("YFTimingsTable");
    if(nodeTimTab.count()==0) return;

    //沙嘴时序初始化
    for(int i=0; i<nodeTimTab.at(0).childNodes().count(); ++i)
    {
        QDomElement tableElement = nodeTimTab.at(0).childNodes().at(i).toElement();
        QString  tableName = QString(tr("%1路纱嘴")).arg(tableElement.attribute("name").remove("F"));
        int iCount=tableElement.attribute("count").toInt();
        QStringList lstVal = tableElement.attribute("val").split(",");
        QStringList lstTypes = tableElement.attribute("types").split("/");

        QTableWidget *tableWidget = new QTableWidget(iCount+1, 4);
        /*设置表头内容*/
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //均分行
        tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); //均分行
        tableWidget->setStyleSheet("gridline-color: rgb(0,0,0)");
        tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
        /*设置表格内容*/
        QStringList header;
        header<<tr("名称")<<tr("针位[IN]")<<tr("针位[OUT]")<<tr("类型");
        for(int i=0; i<4; ++i)
        {
            tableWidget->setItem(0,i, new QTableWidgetItem(header.at(i)));
            tableWidget->item(0,i)->setBackgroundColor(QColor("skyblue"));
        }
        for(int i=1; i<tableWidget->rowCount(); ++i)
        {
            tableWidget->setItem(i,0, new QTableWidgetItem(QString("%1号纱嘴").arg(i)));
            tableWidget->item(i,0)->setBackgroundColor(QColor("skyblue"));
        }

        for(int i=1; i<tableWidget->rowCount(); ++i)
            for(int j=1; j<tableWidget->columnCount(); ++j)
            {
                if(j==3) //类型那一列
                {
                    tableWidget->setItem(i,j, new QTableWidgetItem(lstTypes.at(i-1)=="0" ? tr("标准"):tr("橡筋")));
                }
                else {
                    tableWidget->setItem(i,j, new QTableWidgetItem(lstVal.at((i-1)*2+(j-1))));
                }

            }
        //设置表格数据单元格居中
        for(int i=0; i< tableWidget->rowCount(); ++i)
            for(int j=0; j<tableWidget->columnCount(); ++j)
            {
                QTableWidgetItem* item=tableWidget->item(i,j);
                if(item != NULL)
                {
                    item->setTextAlignment(Qt::AlignCenter);
                }
            }
        tableWidget->setCurrentCell(1,1);

        ui->m_tabYF->addTab(tableWidget,tableName);

    }
}

//保存花型数据到文件
void Frm_patternManage::savePatternDataToFile()
{
    QString filePath = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),2)->text();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        myHelper::showMessageBoxInfo(tr("文件保存失败!"), 1);
        return;
    }
    QByteArray bt = file.readAll();
    file.close();
    qDebug()<<bt;
    PatternFile_Head *fileHead = (PatternFile_Head*)bt.data();

    /*保存循环设置值*/
    for(int i=0; i<20; ++i)
    {
        bt[fileHead->REP_InfoPos+i*6]  =(ui->m_tabLoop->item(i,1)->text().toInt())%256;
        bt[fileHead->REP_InfoPos+i*6+1]=(ui->m_tabLoop->item(i,1)->text().toInt())/256;

        bt[fileHead->REP_InfoPos+i*6+2]=(ui->m_tabLoop->item(i,2)->text().toInt())%256;
        bt[fileHead->REP_InfoPos+i*6+3]=(ui->m_tabLoop->item(i,2)->text().toInt())/256;

        bt[fileHead->REP_InfoPos+i*6+4]=(ui->m_tabLoop->item(i,3)->text().toInt())%256;
        bt[fileHead->REP_InfoPos+i*6+5]=(ui->m_tabLoop->item(i,3)->text().toInt())/256;
    }

    m_YFTable->saveYFData(bt);

    QFile file_save(filePath);
    file_save.open(QIODevice::WriteOnly);
    file_save.write(bt);
    file_save.close();

    writeToXddp();
    myHelper::showMessageBoxInfo(tr("文件保存成功!"), 1);
}

//保存数据到配置文件
void Frm_patternManage::saveTimingsDataToConfigFile()
{
    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    //沙嘴时序
    QDomNodeList lstNodeTimTab = document.elementsByTagName("YFTimingsTable");
    if(lstNodeTimTab.count()==0) return;
    QDomNode nodeTimTab = lstNodeTimTab.at(0);

    QDomNode itemNode;
    for(int i=0; i<ui->m_tabYF->count(); ++i)
    {
        QTableWidget *wgtTable = dynamic_cast<QTableWidget*>(ui->m_tabYF->widget(i));

        if(wgtTable==NULL) continue;

        for(int n=0; n<nodeTimTab.childNodes().count(); ++n)
        {
            if(nodeTimTab.childNodes().at(n).toElement().attribute("name")==QString("%1F").arg(i+1))
            {
                itemNode = nodeTimTab.childNodes().at(n);
                break;
            }
        }
        //获取xml里面item节点的val需要的值
        //初始化针位数据
        QStringList lstVal;
        for(int i=1; i<wgtTable->rowCount(); ++i)
            for(int j=1; j<=2; ++j)
            {
                lstVal.append(wgtTable->item(i, j)->text());
            }
        itemNode.toElement().setAttribute("val",lstVal.join(","));

    }

    QFile file(CONFIG_FILE_XML_PATH);
    file.open(QIODevice::WriteOnly|QFile::Truncate);
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();

    myHelper::showMessageBoxInfo(tr("保存修改成功！"), 1);
}

/*START*****************************************************paletteBoard******************************************************************************/
paletteBoard::paletteBoard(const QImage &image, int scale)
{
    m_scale = scale;

    m_image = image.convertToFormat(QImage::Format_ARGB32);

    m_showGrid = true;
    update();
}

void paletteBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (m_scale >= 3) {
        painter.setPen(QColor(84, 84, 84));
        for (int i = 1; i <= m_image.width(); ++i)
            painter.drawLine(m_scale * i-1, 0,
                             m_scale * i-1, m_scale * m_image.height());
        for (int j = 1; j <= m_image.height(); ++j)
            painter.drawLine(0, m_scale * j-1,
                             m_scale * m_image.width(), m_scale * j-1);
    }

    for (int i = 0; i < m_image.width(); ++i) {
        for (int j = 0; j < m_image.height(); ++j) {
            QRect rect = pixelRect(i, j);
            if (!event->region().intersected(rect).isEmpty()) {
                QColor color = QColor::fromRgba(m_image.pixel(i, j));
                painter.fillRect(rect, color);
            }
        }
    }
}

QRect paletteBoard::pixelRect(int i, int j) const
{
    if (m_scale >= 3 && m_showGrid) {
        return QRect(m_scale * i, m_scale * j, m_scale - 1, m_scale - 1);
    } else {
        return QRect(m_scale * i, m_scale * j, m_scale, m_scale);
    }
}

/*END*****************************************************paletteBoard******************************************************************************/

/*START******************************************************patternTableWgt*********************************************/
patternTableWgt::patternTableWgt(QWidget *parent, int colum, QString fileType, int scale, QByteArray bt) :
    cMyTableWIdget(parent)
{
    m_column = colum+1;
    m_fileType = fileType;

    m_scale = scale;  /*初始化放大缩小的比例系数*/
    /*这边bt为0的话说明没能读取到文件流，直接初始化表格就可以了*/
    if(bt.count()<=0)
    {
        m_row = 201;   /*当bt解析失败的时候，写个默认的图案的行数200+1，其中的1为刻度尺占的格子*/
        initPatternTab();
    }
    else
    {
        freshPattern(fileType, bt);
    }

    /*刻度尺初始化*/
    m_scallBarHor   = new paintScallBar(Qt::Horizontal, this, m_column-1);
    m_scallBarVer   = new paintScallBar(Qt::Vertical,   this, m_row-1);
    setCellWidget(0,1,m_scallBarHor);
    setCellWidget(1,0,m_scallBarVer);

    /*设置左上角0,0格子颜色和刻度尺保持一致*/
    QTableWidgetItem *item =new QTableWidgetItem();
    item->setBackground(QBrush(QColor(0, 0, 0)));
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    setItem(0,0,item);
}

/*初始化表格*/
void patternTableWgt::initPatternTab()
{
    setColumnCount(m_column);
    setRowCount(m_row);

    /*当图像进行放大缩小的时候固定大小的行和列*/
    m_lstRowfixed << 0;
    m_lstColumfixed<<0;

    setSelectionMode(QAbstractItemView::NoSelection);
    setStyleSheet("gridline-color: rgb(84, 84, 84)");

    for(int i=1;i<m_row;++i)
    {
        setRowHeight(i,m_scale);
    }

    for(int j=1;j<m_column;++j)
    {
        setColumnWidth(j,m_scale);
    }

    /*合并单元格*/
    setSpan(0,1,1,m_column-1); /*paintScallBar使用的单元格*/
    setSpan(1,0,m_row-1,1);
    setSpan(1, 1, m_row-1, m_column-1); /*paletteBoard使用的单元格*/

    setRowHeight(0,18);
    setColumnWidth(0,18);

    m_image = QImage(m_column-1, m_row-1, QImage::Format_ARGB32);
    m_image.fill(qRgba(0,0,0, 255));
}
/*根据传入的数据刷新图案*/
void patternTableWgt::freshPattern(QString fileType, QByteArray& bt)
{
    if(fileType.compare("dis",Qt::CaseInsensitive)==0)
    {
        initDisFilePattern(bt);
    }
}

/*根据传入的数据刷新图案---Dis格式*/
void patternTableWgt::initDisFilePattern(QByteArray& bt)
{
    PatternFile_Head *fileHead = (PatternFile_Head*)bt.data();

    /*获取文件的行数*/
    m_row = fileHead->rows +1;

    m_column = fileHead->needles+1;
    /*初始化配色信息*/
    int colorCount = fileHead->colorAmount;

    m_lstColor.clear();
    for(int i=0; i<colorCount; ++i)
    {
        QColor rgb = QColor((quint8)bt.at(fileHead->palettePos+1+i*4),(quint8)bt.at(fileHead->palettePos+2+i*4),(quint8)bt.at(fileHead->palettePos+3+i*4));
        m_lstColor <<rgb;
        m_mapSelectedColor[(quint8)bt.at(fileHead->palettePos+i*4)] = rgb;
    }

    /*表格初始化*/
    initPatternTab();

    for(int i=0; i<fileHead->rows; ++i)
        for(int j=0; j<fileHead->needles; j++)
        {

            int num = (quint8)bt.at(fileHead->patternPos+i*(fileHead->needles)+j);
            m_image.setPixel(j,i, (m_mapSelectedColor.keys().contains(num) ? m_mapSelectedColor[num].rgba():QColor(0,0,0).rgba()));
        }


    m_palettBoard = new paletteBoard(m_image,10);
    setCellWidget(1,1,m_palettBoard);
}

void patternTableWgt::drawPos(int i)
{
    m_scallBarVer->setPos(i);
    m_scallBarVer->update();
}

/*放大*/
void patternTableWgt::zoomOut()
{
    m_scale = m_scale +1;
    setShowZoom(m_scale);
    m_palettBoard->setScale(m_scale);
}

/*缩小*/
void patternTableWgt::zoomIn()
{
    if(m_scale <=1) return;
    m_scale = m_scale -1;
    setShowZoom(m_scale);

    m_palettBoard->setScale(m_scale);
}
/*END******************************************************patternTableWgt*********************************************/

/*start*****************************************************YFTableWgt******************************************************************************/
YFTableWgt::YFTableWgt(QMap<QString, int> mapYF, QWidget *parent, int scale, int row, QByteArray bt) :
    cMyTableWIdget(parent),
    m_mapYF(mapYF)
{
    /*算一下需要的列数，包含间隔的空格*/
    int count =(mapYF.count())*2;
    foreach (int i, mapYF.values()) {
        count +=i;
    }

    m_scale = scale;
    m_row = row;
    m_column = count;
    m_lstRowfixed << 0;
    m_lstColumfixed<<0;

    setColumnCount(count);
    setRowCount(m_row);

    setCursor(Qt::CrossCursor); /*设置鼠标样式*/
    setStyleSheet("gridline-color: rgb(84, 84, 84)");
    setMouseTracking(true);
    /*设置第一行表头*/
    int num=0;
    QBrush brush(QColor(0, 0, 0));
    brush.setStyle(Qt::SolidPattern);
    for(int i=0; i<mapYF.count(); ++i)
    {
        m_lstColumfixed<<num;
        setSpan(0,num,1,mapYF.values().at(i)+2);
        QTableWidgetItem *item =new QTableWidgetItem();
        item->setText(mapYF.keys().at(i));
        item->setTextAlignment(Qt::AlignCenter);
        item->setBackground(brush);
        item->setFlags(Qt::ItemIsEnabled);
        setItem(0,num,item);
        num += mapYF.values().at(i)+2;
        m_lstColumfixed<<num-1;
    }

    for(int i=1;i<m_row;i++)
        for(int j=0;j<count;j++)
        {
            if(m_lstColumfixed.contains(j)) continue;
            QTableWidgetItem *item =new QTableWidgetItem();
            item->setBackground(QBrush(QColor(255,255,255)));
            setItem(i,j,item);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        }

    /*将空格行屏蔽掉,用白色wgt填充*/
    foreach (int x, m_lstColumfixed) {
        spacingBar * spaceItem = new spacingBar(this);
        spaceItem->setEnabled(false);
        setSpan(1,x,rowCount()-1,1);
        setCellWidget(1,x,spaceItem);
    }

    for(int i=0;i<m_row;++i)
    {
        setRowHeight(i,m_scale);
    }

    for(int j=0;j<count;++j)
    {
        setColumnWidth(j,m_scale);
    }

    setRowHeight(0,18);

    //初始化沙嘴数据
    PatternFile_Head *fileHead = (PatternFile_Head*)bt.data();
    //获取总列数
    int iCount(0);
    for(int i=0; i<mapYF.count(); ++i)
    {
        iCount+=mapYF.values().at(i);
    }
    QStringList lstArry;
    QString arry;
    for(int i=0; i<iCount; ++i)
    {
        arry.clear();
        for(int j=0; j<fileHead->rows; ++j)
        {
            arry.append(QString("%1").arg((quint8)bt.at(fileHead->YF_SelectInfoPos+i*fileHead->rows+j)));
        }
        lstArry<<arry;
    }

    int iColum=1;
    num=0;
    for(int i=0; i<mapYF.count(); ++i)
    {
        for(int j=0; j<mapYF.values().at(i); ++j)
        {
            for(int z=0; z<fileHead->rows; ++z)
            {
                item(z+1,iColum)->setBackground(lstArry.at(num).mid(z,1)=="0" ? QBrush(QColor(255,255,255)):QColor(255, 0, 0));
            }
            num++;
            iColum++;
        }
        iColum+=2;
    }

}

/*设置纱嘴的选择情况*/
bool YFTableWgt::setYF(int column, int star, int end, bool set)
{
    if(column>columnCount() || column<1)
    {
        myHelper::showMessageBoxInfo(tr("纱嘴设置范围不正确，请重新设置！"), 1);
        return false;
    }
    if(star>end || end>rowCount())
    {
        myHelper::showMessageBoxInfo(tr("起始行/结束行设置的值不正确！"), 1);
        return false;
    }
    QColor color = (set ? QColor(255, 0, 0) : QColor(255,255,255));

    for(int i=star; i<=end; ++i)
    {
        item(i, column)->setBackground(QBrush(color));
    }
    return true;
}

void YFTableWgt::saveYFData(QByteArray &bt)
{
    PatternFile_Head *fileHead = (PatternFile_Head*)bt.data();

    int iColum=1,num=0;

    QStringList lstArry;
    QString arry;
    for(int i=0; i<m_mapYF.count(); ++i)
    {
        for(int j=0; j<m_mapYF.values().at(i); ++j)
        {
            arry.clear();
            for(int z=0; z<fileHead->rows; ++z)
            {
                arry.append(QString("%1").arg(item(z+1, iColum)->backgroundColor()==QColor(255, 0, 0) ? 1:0));
            }
            lstArry<<arry;
            num++;
            iColum++;
        }
        iColum+=2;
    }

    //获取总列数
    int iCount(0);
    for(int i=0; i<m_mapYF.count(); ++i)
    {
        iCount+=m_mapYF.values().at(i);
    }

    for(int i=0; i<iCount; ++i)
    {
        arry.clear();
        for(int j=0; j<fileHead->rows; ++j)
        {
            bt[fileHead->YF_SelectInfoPos+i*fileHead->rows+j]=lstArry.at(i).mid(j,1).toInt();
            arry.append(QString("%1").arg((quint8)bt.at(fileHead->YF_SelectInfoPos+i*fileHead->rows+j)));
        }
    }
}

/*end*****************************************************YFTableWgt******************************************************************************/

/*start*****************************************************spacingBar******************************************************************************/
void spacingBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QRect rulerRect = rect();
    painter.fillRect(rulerRect,QColor(0xFF, 0xFF, 0xFF));
}
/*end*****************************************************spacingBar******************************************************************************/
