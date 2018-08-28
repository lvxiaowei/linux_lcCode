#include "Frm_patternManage.h"
#include "ui_Frm_patternManage.h"

Frm_patternManage::Frm_patternManage(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_patternManage)
{
    g_mapIndexToWgt[PAGE_PATTERNMANAGE]= this;
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
    default:
        break;
    }
}

/*处理串口数据-page1*/
void Frm_patternManage::dealPg1(int key)
{
    switch (key) {
    case Key_F9:
    {
        setNextDealWgt(PAGE_SETTING);
        break;
    }
    case Key_F8:
    {
        setWorkingPat(PAT_SET_PAT1);
        break;
    }
    case Key_9:
    {
        setWorkingPat(PAT_SET_PAT2);
        break;
    }
    case Key_8:
    {
        setWorkingPat(PAT_SET_PAT3);
        break;
    }
    case Key_7:
        break;
    case Key_PageUp:
    {
        initPatternProcesPage();
        break;
    }


    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabPatManage, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabPatManage, &keyPress);
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
        dealPg2_selectColor(key);
        break;
    }
    case 3:
    {
        dealPg2_patTimingSet(key);
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
    case Key_F9:
    {
        ui->m_stackPat->setCurrentIndex(0);
        ui->m_title->setText(tr("[花型管理]"));
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("设置\n花型１")<<tr("设置\n花型2")<<tr("设置\n花型3")<<tr("")<<tr("编辑"));
        delete m_pattrenTable;
        delete m_YFTable;
        m_pattrenTable = NULL;
        m_YFTable = NULL;
        break;
    }
    case Key_F8:
    {
        ui->m_frmYFSet->show();
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴选择")<<tr("起始行")<<tr("结束行")<<tr("操作")<<tr("设置"));
        break;
    }
    case Key_9:
    {
        ui->m_stackPatOper->setCurrentIndex(1);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("")<<tr("")<<tr("")<<tr("")<<tr(""));
        break;
    }
    case Key_8:
    {
        ui->m_stackPatOper->setCurrentIndex(2);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("")<<tr("")<<tr("")<<tr("")<<tr(""));
        break;

        break;
    }
    case Key_7:
    {
        ui->m_stackPatOper->setCurrentIndex(3);
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("")<<tr("")<<tr("")<<tr("")<<tr(""));
        break;
    }
    case Key_PageUp:
        break;
    default:
        break;
    }
}

/*处理串口数据-page2-纱嘴设置界面处理*/
void Frm_patternManage::dealPg2_YFSet(int key)
{
    switch (key) {
    case Key_F9:
    {
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("选针器\n配色设置")<<tr("花纹纱嘴\n时序设置")<<tr("保存"));
        ui->m_frmYFSet->hide();
        break;
    }
    case Key_F8:
    {
        int i = m_lstYF.indexOf(ui->m_labYfName->text());
        i++;
        ui->m_labYfName->setText( i>=m_lstYF.count() ? m_lstYF.at(0):m_lstYF.at(i));
        break;
    }
    case Key_9:
    {
        ui->m_edtRowStar->setFocus();
        break;
    }
    case Key_8:
    {
        ui->m_edtRowEnd->setFocus();
        break;
    }
    case Key_7:
    {
        ui->m_labOperType->setText(ui->m_labOperType->text()==tr("选择") ? tr("取消"):tr("选择"));
        break;
    }
    case Key_PageUp:
    {
        bool ok;
        m_YFTable->setYF(m_lstYF.indexOf(ui->m_labYfName->text())+1,ui->m_edtRowStar->text().toInt(&ok,10),ui->m_edtRowEnd->text().toInt(&ok,10),(ui->m_labOperType->text()==tr("选择")));
        break;

    }

    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    {
        if(!m_mapNoKeyToValue.contains(key) || !(ui->m_edtRowStar->hasFocus() ||ui->m_edtRowEnd->hasFocus()) ) break;

        QLineEdit *edt = (ui->m_edtRowStar->hasFocus() ? ui->m_edtRowStar:ui->m_edtRowEnd);
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = edt->text() + strInputValue;
        edt->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_Del:
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
    case Key_F9:
    {
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("选针器\n配色设置")<<tr("花纹纱嘴\n时序设置")<<tr("保存"));
        ui->m_stackPatOper->setCurrentIndex(0);
        break;
    }


    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabLoop, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabLoop, &keyPress);
        break;
    }
    case Key_Left:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabLoop, &keyPress);
        break;
    }
    case Key_Right:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabLoop, &keyPress);
        break;
    }


    case Key_0:
    case Key_1:
    case Key_2:
    case Key_3:
    case Key_4:
    case Key_5:
    case Key_6:
    {
        if(ui->m_tabLoop->currentItem()->column()==0) break;

        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = ui->m_tabLoop->currentItem()->text() + strInputValue;
        ui->m_tabLoop->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_Del:
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

/*处理串口数据-page2-选针器配色设置*/
void Frm_patternManage::dealPg2_selectColor(int key)
{
    switch (key) {
    case Key_F9:
    {
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("选针器\n配色设置")<<tr("花纹纱嘴\n时序设置")<<tr("保存"));
        ui->m_stackPatOper->setCurrentIndex(0);
        break;
    }


    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_needlSelectColor, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_needlSelectColor, &keyPress);
        break;
    }
    case Key_Set:
    {
        if(ui->m_needlSelectColor->currentItem()->type() != SECEND_LEVEL_NODE) return;

        Qt::CheckState stat =  ui->m_needlSelectColor->currentItem()->checkState(0);
        ui->m_needlSelectColor->currentItem()->setCheckState(0,stat==Qt::Checked ? Qt::Unchecked:Qt::Checked);

        break;
    }
    default:
        break;
    }
}

/*处理串口数据-page2-花纹纱嘴时序设置*/
void Frm_patternManage::dealPg2_patTimingSet(int key)
{
    switch (key) {
    case Key_F9:
    {
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("选针器\n配色设置")<<tr("花纹纱嘴\n时序设置")<<tr("保存"));
        ui->m_stackPatOper->setCurrentIndex(0);
        break;
    }


    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabTimings, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabTimings, &keyPress);
        break;
    }
    case Key_Left:
    {
        if(ui->m_tabTimings->currentItem()->column()==1) break;
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabTimings, &keyPress);
        break;
    }
    case Key_Right:
    {
        if(ui->m_tabTimings->currentItem()->column()==2) break;
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(ui->m_tabTimings, &keyPress);
        break;
    }


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
        QString strCurentValue = ui->m_tabTimings->currentItem()->text() + strInputValue;
        ui->m_tabTimings->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_Del:
    {
        bool ok;
        QString strCurentValue = ui->m_tabTimings->currentItem()->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        ui->m_tabTimings->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        break;
    }
    default:
        break;
    }
}

/*数据初始化*/
void Frm_patternManage::initShowFrmConfig()
{
    ui->m_stackPat->setCurrentIndex(0);
    ui->m_title->setText(tr("[花型管理]"));
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("设置\n花型１")<<tr("设置\n花型2")<<tr("设置\n花型3")<<tr("")<<tr("编辑"));
    initPatManageTabl();
    ui->m_frmYFSet->hide();
}

/*初始化花型编辑界面*/
void Frm_patternManage::initPatternProcesPage()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("纱嘴设置")<<tr("循环设置")<<tr("选针器\n配色设置")<<tr("花纹纱嘴\n时序设置")<<tr("保存"));
    ui->m_stackPat->setCurrentIndex(1);

    QString filePath = ui->m_tabPatManage->item(ui->m_tabPatManage->currentRow(),3)->text();
    ui->m_title->setText(QString(tr("[花型编辑] %1")).arg(filePath));
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        myMessageBox::getInstance()->setMessage(tr("文件打开失败!"), BoxInfo);
        return;
    }
    QByteArray bt = file.readAll();

    file.close();

    m_iScale = 10;
    /*******************************初始化图案表格**************************************/
    m_pattrenTable = new patternTableWgt(this, 168, "hsp", m_iScale, bt);
    ui->m_grdPat->addWidget(m_pattrenTable);
    m_pattrenTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    /**************************************初始化刷子填色表格**************************************/
    QMap<QString, int>  m_mapYF;
    m_mapYF.clear();
    m_mapYF[QString("%1").arg(tr("1路纱嘴"))] = 6;
    m_lstYF.clear();
    m_lstYF<< tr("1路1号纱嘴") << tr("1路2号纱嘴")<< tr("1路3号纱嘴")<< tr("1路4号纱嘴")<< tr("1路5号纱嘴")<< tr("1路6号纱嘴");
    m_YFTable = new YFTableWgt(m_mapYF, this, m_iScale,  m_pattrenTable->rowCount());
    ui->m_layoutYf->addWidget(m_YFTable);
    m_YFTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    /**************************************初始化循环设置**************************************/
    ui->m_tabLoop->clearContents();
    ui->m_tabLoop->setRowCount(10);
    ui->m_tabLoop->setColumnCount(4); //设置总列数；
    ui->m_tabLoop->setColumnWidth(0,40);
    ui->m_tabLoop->setColumnWidth(1,50);
    ui->m_tabLoop->setColumnWidth(2,50);
    ui->m_tabLoop->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("起始")<<tr("结束")<<tr("编织行数"));
    QFont font=ui->m_tabLoop->horizontalHeader()->font();  //设置表头的字体为粗体；
    font.setBold(true);
    font.setPixelSize(20);
    ui->m_tabLoop->horizontalHeader()->setFont(font);

    ui->m_tabLoop->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui->m_tabLoop->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui->m_tabLoop->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));
    ui->m_tabLoop->horizontalHeaderItem(3)->setTextColor(QColor(0,85,0));
    for(int i=0; i<10; ++i)
    {
        ui->m_tabLoop->setItem(i,0,new QTableWidgetItem(QString("%1").arg(i)));
        ui->m_tabLoop->setItem(i,1,new QTableWidgetItem(QString("0")));
        ui->m_tabLoop->setItem(i,2,new QTableWidgetItem(QString("0")));
        ui->m_tabLoop->setItem(i,3,new QTableWidgetItem(QString("0")));
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
    /**************************************花纹纱嘴时需设置**************************************/
    QTableWidgetItem* item0, *item3;
    ui->m_tabTimings->verticalHeader()->setVisible(false);
    ui->m_tabTimings->setRowCount(6);
    ui->m_tabTimings->setColumnCount(4);
    ui->m_tabTimings->setColumnWidth(0,70);
    ui->m_tabTimings->setColumnWidth(1,45);
    ui->m_tabTimings->setColumnWidth(2,45);
    ui->m_tabTimings->setColumnWidth(3,50);

    ui->m_tabTimings->setHorizontalHeaderLabels(QStringList()<<tr("名称")<<tr("IN")<<tr("OUT")<<tr("类型"));

    QFont fontTimings=ui->m_tabTimings->horizontalHeader()->font();  //设置表头的字体为粗体；
    fontTimings.setBold(true);
    fontTimings.setPixelSize(20);
    ui->m_tabTimings->horizontalHeader()->setFont(fontTimings);

    ui->m_tabTimings->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui->m_tabTimings->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui->m_tabTimings->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));
    ui->m_tabTimings->horizontalHeaderItem(3)->setTextColor(QColor(0,85,0));
    for(int i=0; i<6; ++i)
    {
        item0 = new QTableWidgetItem(QString(tr("%1号纱嘴")).arg(i+1));
        item0->setTextAlignment(Qt::AlignCenter);
        item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
        ui->m_tabTimings->setItem(i, 0, item0);

        ui->m_tabTimings->setItem(i,1,new QTableWidgetItem(QString("0")));
        ui->m_tabTimings->setItem(i,2,new QTableWidgetItem(QString("0")));
        ui->m_tabTimings->item(i,1)->setTextAlignment(Qt::AlignCenter);
        ui->m_tabTimings->item(i,2)->setTextAlignment(Qt::AlignCenter);

        item3 = new QTableWidgetItem(QString("%1").arg(i==5 ? tr("橡筋"):tr("标准")));
        item3->setTextAlignment(Qt::AlignCenter);
        item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
        ui->m_tabTimings->setItem(i, 3, item3);
    }
    ui->m_tabTimings->setCurrentCell(0,1);
}

/*初始化花型管理表格*/
void Frm_patternManage::initPatManageTabl()
{
    /*初始化表格数据*/
    ui->m_tabPatManage->clearContents();
    ui->m_tabPatManage->setRowCount(0);
    ui->m_tabPatManage->setColumnCount(5); //设置总列数；
    ui->m_tabPatManage->setColumnWidth(0,50);
    ui->m_tabPatManage->setColumnWidth(1,90);
    ui->m_tabPatManage->setColumnWidth(2,90);
    ui->m_tabPatManage->setColumnWidth(3,0);

    ui->m_tabPatManage->setHorizontalHeaderLabels(QStringList()<<tr("序号")<<tr("工作设定")<<tr("文件大小")<<tr("文件路径")<<tr("花型文件")); //设置表头名称；

    QFont font=ui->m_tabPatManage->horizontalHeader()->font();  //设置表头的字体为粗体；
    font.setBold(true);
    font.setPixelSize(20);
    ui->m_tabPatManage->horizontalHeader()->setFont(font);

    ui->m_tabPatManage->horizontalHeaderItem(0)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(1)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(2)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(3)->setTextColor(QColor(0,85,0));
    ui->m_tabPatManage->horizontalHeaderItem(4)->setTextColor(QColor(0,85,0));

    ui->m_tabPatManage ->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->m_tabPatManage->horizontalHeader()->setFixedHeight(43);                  //设置表头的高度；

    //获取xml配置文件中花型设置的内容
    QDomDocument document;
    QString pat1,pat2,pat3;
    if(getXmlConfig(document))
    {
        QDomNodeList nodePat1 = document.elementsByTagName("pat1");
        if(nodePat1.count()!=0) pat1=nodePat1.at(0).toElement().attribute("name");
        QDomNodeList nodePat2 = document.elementsByTagName("pat2");
        if(nodePat2.count()!=0) pat2=nodePat2.at(0).toElement().attribute("name");
        QDomNodeList nodePat3 = document.elementsByTagName("pat3");
        if(nodePat3.count()!=0) pat3=nodePat3.at(0).toElement().attribute("name");
    }


    QDir dir(PATTERN_FILE_LOCAL_PATH);
    if(!dir.exists())
    {
        return;
    }
    else
    {
        QStringList filters;     //定义过滤变量；
        filters<<QString("*.hsp");
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

            //根据配置文件中花型信息显示工作设定值
            if(pat1.compare(fileName_str,Qt::CaseInsensitive)==0)
            {
                ui->m_tabPatManage->setItem(rows,1,new QTableWidgetItem("Pat1"));
            }
            else if(pat2.compare(fileName_str,Qt::CaseInsensitive)==0)
            {
                ui->m_tabPatManage->setItem(rows,1,new QTableWidgetItem("Pat2"));
            }
            else if(pat3.compare(fileName_str,Qt::CaseInsensitive)==0)
            {
                ui->m_tabPatManage->setItem(rows,1,new QTableWidgetItem("Pat3"));
            }
            else {
                ui->m_tabPatManage->setItem(rows,1,new QTableWidgetItem(" "));
            }

            ui->m_tabPatManage->setItem(rows,2,new QTableWidgetItem(fileSize_str));
            ui->m_tabPatManage->setItem(rows,3,new QTableWidgetItem(file_info.filePath()));
            ui->m_tabPatManage->setItem(rows,4,new QTableWidgetItem(fileName_str));
            ui->m_tabPatManage->item(rows,0)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabPatManage->item(rows,1)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabPatManage->item(rows,2)->setTextAlignment(Qt::AlignCenter);
            ui->m_tabPatManage->item(rows,4)->setTextAlignment(Qt::AlignCenter);
        }
    }

    if(ui->m_tabPatManage->rowCount()>0)
        ui->m_tabPatManage->setCurrentCell(0,0);
}

/*设置工作花型*/
void Frm_patternManage::setWorkingPat(int pat)
{
    for(int i=0; i<ui->m_tabPatManage->rowCount(); ++i)
    {
        if(ui->m_tabPatManage->item(i,1)->text() == QString("Pat%1").arg(pat))
        {
            ui->m_tabPatManage->item(i,1)->setText(" ");
            break;
        }
    }
    int irow=ui->m_tabPatManage->currentRow();
    ui->m_tabPatManage->item(irow,1)->setText(QString("Pat%1").arg(pat));

    //设置花型写入到配置xml
    QDomDocument document;
    if(!getXmlConfig(document))
        return;
    QString nodeName=QString("pat%1").arg(pat);
    QDomNodeList nodePat = document.elementsByTagName(nodeName);
    if(nodePat.count()!=0)
    {
        nodePat.at(0).toElement().setAttribute("name", ui->m_tabPatManage->item(irow,4)->text());
        QFile file(CONFIG_FILE_XML_PATH);
        file.open(QIODevice::WriteOnly|QFile::Truncate);
        QTextStream stream(&file);
        document.save(stream, 4);
        file.close();
    }
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
    item->setBackground(QBrush(QColor(255, 195, 0)));
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
    if(fileType.compare("wgr",Qt::CaseInsensitive)==0)
    {
        initWgrFilePattern(bt);
    }
    else if(fileType.compare("hsp",Qt::CaseInsensitive)==0)
    {
        initHspFilePattern(bt);
    }
    else if(fileType.compare("dat",Qt::CaseInsensitive)==0)
    {
        initDatFilePattern(bt);
    }
}

/*根据传入的数据刷新图案---WGR格式*/
void patternTableWgt::initWgrFilePattern(QByteArray& bt)
{
    //Demio花型文件按byte解析(16进制)： //只是目前的分析进展，已经满足大部分花型的解析
    //4D：表示花型文件的针数
    //51 52：表示花型文件的行数 52高位 51低位
    //55：表示当前花型文件配色种类(不一定在花型中使用)
    //56开始：每5个字节代表一种配色，前3位字节分别代表rgb的三个数字，后2位字节补0作为扩充
    //配色全部解析完之后空2个字节：1、每个字节代表一个格子的颜色，其数值代表的上面配色的顺序编号
    //                         2、花型是从最下面一行开始解析的一直到第一行
    //                         3、针数不为4的倍数的话，一行的长度会补齐为4的倍数

    /*以下是按照demio的数据进行文件解析的*/

    /*获取文件的行数*/
    int patternLine = (quint8)bt.at(81)+(quint8)bt.at(82)*256;
    m_row = patternLine+1;

    /*从bt中获取配色信息*/
    int colorNum = (quint8)bt.at(85);
    int r,g,b;
    m_lstColor.clear();
    for(int i =0; i<colorNum; ++i)
    {
        r = (quint8)bt.at(86+0+i*5);
        g = (quint8)bt.at(86+1+i*5);
        b = (quint8)bt.at(86+2+i*5);
        m_lstColor <<QColor(r,g,b);
    }

    /*表格初始化*/
    initPatternTab();

    int lineNum = ((quint8)bt.at(56))%4 == 0 ? (quint8)bt.at(56) : (((quint8)bt.at(56))/4+1)*4;
    int num = 88+ ((quint8)bt.at(85))*5;

    m_mapSelectedColor.clear();
    for(int i=patternLine-1; i>=0; i--)
        for(int j=0;j<lineNum;j++)
        {
            colorNum = (quint8)bt.at(num);
            m_image.setPixel(j,i,m_lstColor.at(colorNum).rgba());
            m_mapSelectedColor[colorNum] = m_lstColor.at(colorNum);
            num++;
        }
    m_palettBoard = new paletteBoard(m_image,m_scale);
    setCellWidget(1,1,m_palettBoard);
}

/*根据传入的数据刷新图案---HSP格式*/
void patternTableWgt::initHspFilePattern(QByteArray& bt)
{
    //HSP文件:
    //30：链条针数
    //32: 颜色数目
    //34 35：代表链条最高行数 34低位 35高位
    //60~B9：文件保存时候的反复设置项目，每6个字节一组，共15组；每一组三个数据(开始行 结束行 转动)，每个数据占2个字节，低位在前高位在后
    //150:开始画图  按使用颜色进行分区 每个区域中 一行32字节(无论多少针都是32字节) 32字节转换成2进制从高位起对应行的最左边起，1填色 0为默认的黑色

    /*获取文件的行数*/
    m_row = (quint8)bt.at(52)+(quint8)bt.at(53)*256 +1;
    qDebug()<<"当前花型的行数"<<m_row;
    /*初始化配色信息*/
    m_lstColor.clear();
    m_lstColor <<QColor(0,0,0) <<QColor(62,184,184) <<QColor(255, 75,0);

    m_mapSelectedColor[0] = QColor(0,0,0);
    m_mapSelectedColor[1] = QColor(62,184,184);
    m_mapSelectedColor[2] = QColor(255, 75,0);

    int colorNo = (quint8)bt.at(50);
    /*表格初始化*/
    initPatternTab();
    int beginPosition = 336;
    int circlTimes = (m_column-1)%8==0 ? (m_column-1)/8 : (m_column-1)/8+1; /*每个字节代表了8个格子，这块确认下需要多少字节*/
    int value;
    for(int i=0; i<colorNo; ++i)  /*颜色的循环*/
    {
        for(int j=0; j<m_row-1; ++j)    /*行数的循环*/
        {
            for(int z=0; z<circlTimes; ++z)  /*字节的循环*/
            {
                value = (quint8)bt.at(beginPosition+z);

                for(int m= 7; m>=0; --m)  /*一个字节8个格子的循环*/
                {
                    int x=z*8+m;
                    if((value & 0x1)==1)
                        m_image.setPixel(x,j,m_lstColor.at(i+1).rgba());
                    value >>= 1;
                }
            }

            beginPosition += 32;
        }
    }
    m_palettBoard = new paletteBoard(m_image,m_scale);
    setCellWidget(1,1,m_palettBoard);
}

/*根据传入的数据刷新图案---Dat格式*/
void patternTableWgt::initDatFilePattern(QByteArray& bt)
{
    //Dat文件：
    //9：链条针数
    //4C 4D或者4E 4F代表链条最高行数 4C 4E低位 4D 4F高位
    //50开始：表示的是保存文件界面的反复设置框相关的参数，每组6个字节，每2个字节代表一个数据----这块数据和我们无关
    //150：开始画图

    /*获取文件的行数*/
    m_row = (quint8)bt.at(76)+(quint8)bt.at(77)*256 +1;

    /*初始化配色信息*/
    m_lstColor.clear();
    m_lstColor <<QColor(0,0,0) <<QColor(62,184,184) <<QColor(255, 75,0);

    m_mapSelectedColor[0] = QColor(0,0,0);
    m_mapSelectedColor[1] = QColor(62,184,184);
    m_mapSelectedColor[2] = QColor(255, 75,0);

    //int colorNo = (quint8)bt.at(50);
    /*表格初始化*/
    initPatternTab();

    int beginPosition = 336;
    int circlTimes = (m_column-1)%8==0 ? (m_column-1)/8 : (m_column-1)/8+1; /*每个字节代表了8个格子，这块确认下需要多少字节*/
    //    for(int i=0; i<colorNo; ++i)  /*颜色的循环*/     当前没能分析出dat文件中的选色情况
    for(int j=0; j<m_row-1; ++j)    /*行数的循环*/
    {
        for(int z=0; z<circlTimes; ++z)  /*字节的循环*/
        {
            int value = (quint8)bt.at(beginPosition+z);

            for(int m= 7; m>=0; --m)  /*一个字节8个格子的循环*/
            {
                int x=z*8+m;
                if((value & 0x1)==1)
                    m_image.setPixel(x,j,m_lstColor.at(1).rgba());
                value >>= 1;
            }
        }

        beginPosition += 32;
    }

    m_palettBoard = new paletteBoard(m_image,m_scale);
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
YFTableWgt::YFTableWgt(QMap<QString, int> m_mapYF, QWidget *parent, int scale, int row) :
    cMyTableWIdget(parent)
{
    /*算一下需要的列数，包含间隔的空格*/
    int count =(m_mapYF.count())*2;
    foreach (int i, m_mapYF.values()) {
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
    QBrush brush(QColor(144, 214, 106));
    brush.setStyle(Qt::SolidPattern);
    for(int i=0; i<m_mapYF.count(); ++i)
    {
        m_lstColumfixed<<num;
        setSpan(0,num,1,m_mapYF.values().at(i)+2);
        QTableWidgetItem *item =new QTableWidgetItem();
        item->setText(m_mapYF.keys().at(i));
        item->setTextAlignment(Qt::AlignCenter);
        item->setBackground(brush);
        item->setFlags(Qt::ItemIsEnabled);
        setItem(0,num,item);
        num += m_mapYF.values().at(i)+2;
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
}

/*设置纱嘴的选择情况*/
bool YFTableWgt::setYF(int column, int star, int end, bool set)
{
    if(column>6 || column<1)
    {
        myMessageBox::getInstance()->setMessage(tr("纱嘴设置范围不正确，请重新设置！"), BoxInfo);
        return false;
    }
    if(star>end || end>rowCount())
    {
        myMessageBox::getInstance()->setMessage(tr("起始行/结束行设置的值不正确！"), BoxInfo);
        return false;
    }
    QColor color = (set ? QColor(255, 0, 0) : QColor(255,255,255));

    for(int i=star; i<=end; ++i)
    {
        item(i, column)->setBackground(QBrush(color));
    }
    return true;
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
