#include "Frm_chainProcessingClass.h"
#include "ui_Frm_chainProcessingClass.h"
#include <QFileSystemModel>
#include <QDirModel>
Frm_chainProcessingClass::Frm_chainProcessingClass(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_chainProcessingClass),
    m_treeFileModel(NULL)
{
    ui->setupUi(this);

    /*增加编辑位置的二级菜单*/
    m_lstFirstNodeName <<tr("开始") <<tr("包口") <<tr("橡筋") <<tr("上统") <<tr("袜跟上") <<tr("袜跟下") <<tr("下统")
                      <<tr("过桥") <<tr("袜头上") <<tr("袜头下") <<tr("缝头线") <<tr("机头线") <<tr("结束");
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
    default:
        break;
    }
}

/*处理串口数据-page1-链条文件管理*/
void Frm_chainProcessingClass::dealPg1(int key)
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
            initChainManageTable();
            QUIMessageBox::Instance()->hide();
            break;
        }
        case Key_Esc:
            QUIMessageBox::Instance()->hide();
            break;
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
            initChainManageTable();
            myHelper::showMessageBoxInfo(tr("从U盘拷贝到本地成功！"), 1);
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
            myHelper::showMessageBoxQuestion(tr("U盘有同名文件，是否覆盖？"));
        }
        else {
            QFile::copy(m_sourceFile, m_destFile);
            myHelper::showMessageBoxInfo(tr("从本地拷贝到U盘成功！"), 1);
        }
        break;
    }
    case Key_F3:
    {
        QString fileName;
        if( ui->m_tabChainManage->hasFocus())
        {
            m_delFile = ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),2)->text();
            fileName = ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),3)->text();

            /*如果是正在运行的链条，不可以删除*/
            if(fileName == ui->m_labWorkChain->text())
            {
                myHelper::showMessageBoxInfo(tr("不可以删除当前正在工作的链条！"), 1);
                return;
            }
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
    case Key_F4:
    {
        initChainTree();
        ui->stackedWidget->setCurrentIndex(1);
        ui->m_chainTitle->setText(QString(tr("[链条编辑] %1")).arg(ui->m_labWorkChain->text()));
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("保存")<<tr("下一菜单\n[1/2]"));
        break;
    }
    case Key_F5:
    {
        QString fileName=ui->m_tabChainManage->item(ui->m_tabChainManage->currentRow(),3)->text();

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

        myHelper::showMessageBoxInfo(tr("运行链条设置成功!"), 1);

        writeToXddp();
        break;
    }

    case Key_Set:
    {
        if(ui->m_tabChainManage->hasFocus()) return;

        QModelIndex index = ui->m_treeFile->currentIndex();

        ui->m_treeFile->isExpanded(index) ? ui->m_treeFile->collapse(index):ui->m_treeFile->expand(index);
        break;
    }

    case Key_Up:
    {
        ui->m_tabChainManage->hasFocus() ? QCoreApplication::sendEvent(ui->m_tabChainManage, key_up):QCoreApplication::sendEvent(ui->m_treeFile, key_up);
        break;
    }
    case Key_Down:
    {
        ui->m_tabChainManage->hasFocus() ? QCoreApplication::sendEvent(ui->m_tabChainManage, key_down):QCoreApplication::sendEvent(ui->m_treeFile, key_down);
        break;
    }
    case Key_Left:
    {
        ui->m_tabChainManage->setFocus();
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

/*处理串口数据-page2-单个链条管理*/
void Frm_chainProcessingClass::dealPg2(int key)
{
    if(QUIMessageBox::Instance()->isVisible())
    {
        switch (key) {
        case Key_Set:
        {
            QTreeWidgetItem* item = ui->m_chainTree->currentItem();
            delete item;
            item =NULL;
            secendLevelNodeSort();
            QUIMessageBox::Instance()->hide();
            break;
        }
        case Key_Esc:
            QUIMessageBox::Instance()->hide();
        default:
            break;
        }
    }

    //命令编辑
    if(ui->m_stackCmdEdit->isVisible())
    {
        dealAddCmd(key);
        return;
    }

    if(g_lstRightButton.at(5)->text() == tr("下一菜单\n[1/2]"))
    {
        dealPg2_1(key);
    }
    else if(g_lstRightButton.at(5)->text() == tr("上一菜单\n[2/2]"))
    {
        dealPg2_2(key);
    }

    switch (key) {
    case Key_Left:
    case Key_Right:
    {
        int step=(key==Key_Left ? -1:1);

        QTreeWidgetItem *current = ui->m_chainTree->currentItem();
        QTreeWidgetItem *parent = current->parent();
        QTreeWidgetItem *nextSibling;
        if(parent)
        {
            nextSibling =parent->child(parent->indexOfChild(current)+step);
        }
        else {
            nextSibling = ui->m_chainTree->topLevelItem(ui->m_chainTree->indexOfTopLevelItem(current)+step);
        }
        ui->m_chainTree->setCurrentItem(NULL==nextSibling ? current:nextSibling);
        break;
    }
        break;
    default:
        break;
    }
}

/*处理串口数据-page2-1*/
void Frm_chainProcessingClass::dealPg2_1(int key)
{
    switch (key) {
    case Key_F0:
        ui->stackedWidget->setCurrentIndex(0);
        ui->m_tabChainManage->setFocus();
        ui->m_chainTitle->setText(tr("[链条管理]"));
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("从U盘\n输入")<<tr("输出到\nU盘")<<tr("删除")<<tr("编辑\n工作链条")<<tr("工作链条\n设定"));
        break;
    case Key_F1:
    {
        if(ui->m_chainTree->currentItem()->type()==FIRST_LEVEL_NODE)
        {
            myHelper::showMessageBoxInfo(tr("请选择具体需要新增的步骤！"), 1);
            return;
        }
        ui->m_stackCmdEdit->setCurrentIndex(0);
        ui->m_stackCmdEdit->show();
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("添加")<<tr("Tab")<<tr("确认\n参数值")<<tr("")<<tr(""));
        ui->m_wgtCmdName->setFocus();

        break;
    }
    case Key_F2:
    {
        myHelper::showMessageBoxQuestion(tr("确认要删除当前步骤及步骤内的所有内容吗？"));
        break;
    }
    case Key_F3:
    {
        if(THIRD_LEVEL_NODE != getItemType(ui->m_chainTree->currentItem()))
            return;

        QTreeWidgetItem *item = ui->m_chainTree->currentItem();
        ui->m_stackCmdEdit->setCurrentIndex(1);

        //设置title的内容
        ui->m_title->setText(ComConfigClass::GetInstance()->getCmdProByIndex(item->text(1).toInt()).name.at(ComConfigClass::m_iLanguage));
        QString strCmdType = ComConfigClass::GetInstance()->getCmdProByIndex(item->text(1).toInt()).cmdType;

        if(strCmdType == "valve")
        {
            ui->m_state->setCurrentIndex(item->text(2).toInt());
            ui->m_needle->setText(item->text(3));
            ui->m_model->setCurrentIndex(item->text(4).toInt());

            ui->m_stackCmdParaSet->setCurrentIndex(0);
            ui->m_state->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_state<<ui->m_needle<<ui->m_model;
        }
        else if(strCmdType == "speed"){
            ui->m_speed->setText(item->text(2));
            ui->m_circle->setText(item->text(3));

            ui->m_stackCmdParaSet->setCurrentIndex(1);
            ui->m_speed->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_speed<<ui->m_circle;
        }
        else if(strCmdType == "macro"){
            ui->m_lstMarco->setCurrentText(item->text(2));
            ui->m_needle_2->setText(item->text(3));
            ui->m_speed_2->setCurrentIndex(item->text(4).toInt());

            ui->m_stackCmdParaSet->setCurrentIndex(2);
            ui->m_lstMarco->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_lstMarco<<ui->m_needle_2<<ui->m_speed_2;
        }
        else if(strCmdType == "turning")
        {
            ui->m_motoDirect->setCurrentIndex(item->text(2)=="0" ? 1:0);
            ui->m_motoMode->setCurrentIndex(item->text(3).toInt());
            ui->m_motoNeedlValve->setText(item->text(4));

            ui->m_stackCmdParaSet->setCurrentIndex(3);
            ui->m_motoDirect->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_motoDirect<<ui->m_motoMode<<ui->m_motoNeedlValve;
        }
        else if(strCmdType == "lowSpeed")
        {
            ui->m_state_3->setCurrentIndex(item->text(2).toInt());
            ui->m_needle_3->setText(item->text(3));

            ui->m_stackCmdParaSet->setCurrentIndex(4);
            ui->m_state_3->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_state_3<<ui->m_needle_3;
        }
        else if(strCmdType == "selection")
        {
            ui->m_needleNum->setCurrentText(item->text(2));
            ui->m_state_5->setCurrentIndex(item->text(3)=="1" ? 0:1);
            ui->m_state_4->setCurrentIndex(item->text(4).toInt());

            ui->m_stackCmdParaSet->setCurrentIndex(5);
            ui->m_needleNum->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_needleNum<<ui->m_state_4<<ui->m_state_5;
        }
        else if(strCmdType == "footIncreaseEcrease")
        {
            ui->m_left->setText(item->text(2));
            ui->m_right->setText(item->text(3));

            ui->m_stackCmdParaSet->setCurrentIndex(6);
            ui->m_left->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_left<<ui->m_right;
        }
        else if(strCmdType == "circle")
        {
            ui->m_edtCircle_1->setText(item->text(2));
            ui->m_edtCircle_2->setText(item->text(3));
            ui->m_edtCircle_3->setText(item->text(4));
            ui->m_edtCircle_4->setText(item->text(5));
            ui->m_edtCircle_5->setText(item->text(6));
            ui->m_edtCircle_6->setText(item->text(7));
            ui->m_edtCircle_7->setText(item->text(8));
            ui->m_edtCircle_8->setText(item->text(9));

            ui->m_stackCmdParaSet->setCurrentIndex(8);
            ui->m_edtCircle_1->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_edtCircle_1<<ui->m_edtCircle_2<<ui->m_edtCircle_3<<ui->m_edtCircle_4<<ui->m_edtCircle_5
                      <<ui->m_edtCircle_6<<ui->m_edtCircle_7<<ui->m_edtCircle_8;
        }
        else if(strCmdType == "patternUse")
        {
            QStringList lst=getPatternList();
            ui->m_patternLink->clear();
            for(int i=0; i<lst.count();++i)
                ui->m_patternLink->addItem(lst.at(i));

            ui->m_patternLink->setCurrentText(item->text(2));

            ui->m_stackCmdParaSet->setCurrentIndex(7);
            ui->m_patternLink->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_patternLink;
        }
        else {
            return;
        }
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("添加")<<tr("Tab")<<tr("确认\n参数值")<<tr("")<<tr(""));
        ui->m_stackCmdEdit->show();
        break;
    }
    case Key_F4:
        writeChainTree();
        writeToXddp();
        myHelper::showMessageBoxInfo(tr("文件保存成功!"), 1);
        break;
    case Key_F5:
    {
        freshRightButtonContent(QStringList()<<tr("复制")<<tr("粘贴")<<tr("")<<tr("")<<tr("")<<tr("上一菜单\n[2/2]"));
        break;
    }
    case Key_Up:
    {
        QCoreApplication::sendEvent(ui->m_chainTree, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(ui->m_chainTree, key_down);
        break;
    }
    case Key_Set:
    {
        QTreeWidgetItem * item = ui->m_chainTree->currentItem();
        if(item!=NULL)
            item->setExpanded(!item->isExpanded());
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
            myHelper::showMessageBoxInfo(tr("复制成功！"),1);
        }
        else {
            myHelper::showMessageBoxInfo(tr("只能复制步骤下面的具体动作！"), 1);
        }
        break;
    }
    case Key_F1:
    {
        if(!m_cpItem)
        {
            myHelper::showMessageBoxInfo(tr("当前没有复制操作!"), 1);
        }
        else if(ui->m_chainTree->currentItem()->type()==FIRST_LEVEL_NODE) {
            myHelper::showMessageBoxInfo(tr("请选择具体需要粘贴的步骤！"), 1);
        }
        else {
            QTreeWidgetItem* itemLv2 = (ui->m_chainTree->currentItem()->type()==SECEND_LEVEL_NODE ?ui->m_chainTree->currentItem():ui->m_chainTree->currentItem()->parent());
            QTreeWidgetItem* itemLv3  = new QTreeWidgetItem(itemLv2,QStringList()<<m_cpItem->text(0),THIRD_LEVEL_NODE);
            //            itemLv3->setIcon(0, QIcon(ComConfigClass::GetInstance()->getCmdIcoPathByName(itemLv3->text(0).split(" [").at(0))));
            myHelper::showMessageBoxInfo(tr("粘贴成功！"),1);
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
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("保存")<<tr("下一菜单\n[1/2]"));
        break;
    }
    case Key_Up:
    {
        QCoreApplication::sendEvent(ui->m_chainTree, key_up);
        break;
    }
    case Key_Down:
    {
        QCoreApplication::sendEvent(ui->m_chainTree, key_down);
        break;
    }
    default:
        break;
    }
}

/*处理链条管理界面新增或者编辑单条命令*/
void Frm_chainProcessingClass::dealAddCmd(int key)
{
    switch (key) {
    case Key_Esc:
    case Key_F0:
        ui->m_stackCmdEdit->hide();
        freshRightButtonContent(QStringList()<<tr("返回")<<tr("新增")<<tr("删除")<<tr("编辑")<<tr("保存")<<tr("下一菜单\n[1/2]"));
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
        /*对有下拉框的QLineEdit控件做差异处理*/
        QLineEdit *pCurrentFocusWgt =dynamic_cast<QLineEdit*>(focusWidget());
        if(pCurrentFocusWgt!=NULL)
        {
            bool ok;
            QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
            QString strCurentValue = pCurrentFocusWgt->text() + strInputValue;
            pCurrentFocusWgt->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        }
        break;
    }
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
    case Key_Set:
    {
        /*针对不同的控件，做不同的处理*/
        if(focusWidget()->inherits("QComboBox") )
        {
            QComboBox *pComboxWgt =dynamic_cast<QComboBox*>(focusWidget());
            if(pComboxWgt == NULL)
                return;
            pComboxWgt->view()->isActiveWindow() ? pComboxWgt->hidePopup():pComboxWgt->showPopup();
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
            QCoreApplication::sendEvent(ui->m_wgtCmdName->hasFocus()? ui->m_wgtCmdName:ui->m_wgtCmdContent, key==Key_Up ? key_up:key_down);
        }
        break;
    }
    case Key_Left:
    {
        if(ui->m_stackCmdEdit->currentIndex()==0)
            ui->m_wgtCmdName->setFocus();
        break;
    }
    case Key_Right:
    {
        if(ui->m_stackCmdEdit->currentIndex()==0)
            ui->m_wgtCmdContent->setFocus();
        break;
    }
    case Key_F1:
    {
        if(ui->m_stackCmdEdit->currentIndex()!=0)
            return;

        QTreeWidgetItem* parentItem = (getItemType(ui->m_chainTree->currentItem())==SECEND_LEVEL_NODE ?
                                           ui->m_chainTree->currentItem(): ui->m_chainTree->currentItem()->parent());

        QTableWidgetItem* itemTable = ui->m_wgtCmdContent->selectedItems().at(0);
        ui->m_title->setText(itemTable->text());
        int iType = ComConfigClass::GetInstance()->getCmdTypeByIndex(itemTable->type());
        QStringList lstPara;
        QString strContex;
        switch (iType) {
        case OPER_VALVE:
        {
            strContex =QString(tr("%1 [状态：%2---针位：%3---模式：%4]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_state->currentText()).arg(ui->m_needle->text()).arg(ui->m_model->currentText());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<QString("%1").arg(ui->m_state->currentIndex())<<ui->m_needle->text()<<QString("%1").arg(ui->m_model->currentIndex());

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(0);
            ui->m_state->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_state<<ui->m_needle<<ui->m_model;
            break;
        }
        case OPER_SPEED:
        {
            strContex =QString(tr("%1 [转速：%2---加速圈数：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_speed->text().toInt()).arg(ui->m_circle->text().toInt());

            lstPara<<strContex<<ui->m_speed->text()<<ui->m_circle->text();

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(1);
            ui->m_speed->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_speed<<ui->m_circle;
            break;
        }
        case OPER_MACRO:
        {
            strContex =QString(tr("%1 [宏：%2---动作针位：%3---模式：%4]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_lstMarco->currentText()).arg(ui->m_needle_2->text()).arg(ui->m_speed_2->currentText());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<ui->m_lstMarco->currentText()<<ui->m_needle_2->text()<<QString("%1").arg(ui->m_speed_2->currentIndex());

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(2);
            ui->m_lstMarco->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_lstMarco<<ui->m_needle_2<<ui->m_speed_2;
            break;
        }
        case OPER_TURNING:
        {
            QString strContex1 =QString(tr("%1 [电机转向：%2---模式：%3---针位：%4]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_motoDirect->currentIndex()==0 ? tr("正转"):tr("反转")).arg(ui->m_motoMode->currentText()).arg(ui->m_motoNeedlValve->text());
            QString strContex2 =QString(tr("%1 [电机转向：%2---模式：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_motoDirect->currentIndex()==0 ? tr("正转"):tr("反转")).arg(ui->m_motoMode->currentText());
            strContex = ui->m_motoMode->currentText() ==tr("无") || ui->m_motoMode->currentText() ==tr("到零") ? strContex2:strContex1;

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<QString("%1").arg(ui->m_motoDirect->currentIndex()==1 ? 0:1)<<QString("%1").arg(ui->m_motoMode->currentIndex())<<ui->m_motoNeedlValve->text();

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(3);
            ui->m_motoDirect->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_motoDirect<<ui->m_motoMode<<ui->m_motoNeedlValve;
            break;
        }
        case OPER_LOWSPEED_CMD:
        {
            strContex =QString(tr("%1 [动作状态：%2---针位：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_state_3->currentText()).arg(ui->m_needle_3->text());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<QString("%1").arg(ui->m_state_3->currentIndex())<<ui->m_needle_3->text();

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(4);
            ui->m_state_3->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_state_3<<ui->m_needle_3;
            break;
        }
        case OPER_SELECTION:
        {
            strContex =QString(tr("%1 [选针编号：%2---动作方式：%3---选针模式：%4]")).arg(ui->m_title->text().trimmed()).
                    arg(ui->m_needleNum->currentText()).arg(ui->m_state_5->currentIndex()==0 ? tr("正常"):tr("辅助")).arg(ui->m_state_4->currentText());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<ui->m_needleNum->currentText()<<QString("%1").arg(ui->m_state_5->currentIndex()==0 ? 1:0)
                  <<QString("%1").arg(ui->m_state_4->currentIndex());

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(5);
            ui->m_needleNum->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_needleNum<<ui->m_state_4<<ui->m_state_5;
            break;
        }
        case OPER_FOOT_INCREASE_DCREASE:
        {
            strContex =QString(tr("%1 [向左移：%2---向右移：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_left->text()).arg(ui->m_right->text());
            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<ui->m_left->text()<<ui->m_right->text();

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(6);
            ui->m_left->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_left<<ui->m_right;
            break;
        }
        case OPER_PATTERN_USE:
        {
            QStringList lst=getPatternList();
            qDebug()<<lst;
            ui->m_patternLink->clear();
            for(int i=0; i<lst.count();++i)
                ui->m_patternLink->addItem(lst.at(i));

            strContex =QString(tr("%1 [花纹名称：%2]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_patternLink->currentText());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<ui->m_patternLink->currentText();

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(7);
            ui->m_patternLink->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_patternLink;
            break;
        }
        case OPER_CIRCLE:
        {
            strContex =QString(tr("%1 [%2,%3,%4,%5,%6,%7,%8,%9]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_edtCircle_1->text())
                    .arg(ui->m_edtCircle_2->text())
                    .arg(ui->m_edtCircle_3->text())
                    .arg(ui->m_edtCircle_4->text())
                    .arg(ui->m_edtCircle_5->text())
                    .arg(ui->m_edtCircle_6->text())
                    .arg(ui->m_edtCircle_7->text())
                    .arg(ui->m_edtCircle_8->text());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<ui->m_edtCircle_1->text()<<ui->m_edtCircle_2->text()<<ui->m_edtCircle_3->text()
                  <<ui->m_edtCircle_4->text()<<ui->m_edtCircle_5->text()<<ui->m_edtCircle_6->text()<<ui->m_edtCircle_7->text()<<ui->m_edtCircle_8->text();

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(8);
            ui->m_edtCircle_1->setFocus();
            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_edtCircle_1<<ui->m_edtCircle_2<<ui->m_edtCircle_3<<ui->m_edtCircle_4<<ui->m_edtCircle_5
                      <<ui->m_edtCircle_6<<ui->m_edtCircle_7<<ui->m_edtCircle_8;
            break;
        }
        case OPER_ZEROING:
        {
            strContex =QString(tr("%1 [动作状态：%2]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_state_6->currentText());

            lstPara<<strContex<<QString("%1").arg(itemTable->type())<<QString("%1").arg(ui->m_state_6->currentIndex());

            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, lstPara, THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);

            ui->m_stackCmdEdit->setCurrentIndex(1);
            ui->m_stackCmdParaSet->setCurrentIndex(9);
            ui->m_state_6->setFocus();

            m_lstTabWgt.clear();
            m_lstTabWgt<<ui->m_state_6;
            break;
        }
        default:
        {
            QTreeWidgetItem* itemNew = new QTreeWidgetItem(parentItem, QStringList()<<itemTable->text().trimmed()<<QString("%1").arg(itemTable->type()), THIRD_LEVEL_NODE);
            itemNew->setIcon(0, itemTable->icon());
            ui->m_chainTree->setCurrentItem(itemNew);
            break;
        }
        }
        break;
    }
    case Key_F2:
    {
        if(!m_lstTabWgt.contains(focusWidget()))
            return;

        int index = m_lstTabWgt.indexOf(focusWidget());
        m_lstTabWgt.at(index==(m_lstTabWgt.count()-1) ? 0: index+1)->setFocus();
        break;
    }
    case Key_F3:
    {
        if(ui->m_stackCmdEdit->currentIndex()==0)
            return;

        int index = ui->m_stackCmdParaSet->currentIndex();
        qDebug()<<"========================index="<<index;
        QStringList lstPara;
        QString strContex;
        switch (index) {
        case 0:
        {
            strContex =QString(tr("%1 [状态：%2---针位：%3---模式：%4]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_state->currentText()).arg(ui->m_needle->text()).arg(ui->m_model->currentText());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<QString("%1").arg(ui->m_state->currentIndex())<<ui->m_needle->text()<<QString("%1").arg(ui->m_model->currentIndex());
            break;
        }
        case 1:
        {
            strContex =QString(tr("%1 [转速：%2---加速圈数：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_speed->text().toInt()).arg(ui->m_circle->text().toInt());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<ui->m_speed->text()<<ui->m_circle->text();
            break;
        }
        case 2:
        {
            strContex =QString(tr("%1 [宏：%2---动作针位：%3---模式：%4]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_lstMarco->currentText()).arg(ui->m_needle_2->text()).arg(ui->m_speed_2->currentText());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<ui->m_lstMarco->currentText()<<ui->m_needle_2->text()<<QString("%1").arg(ui->m_speed_2->currentIndex());
            break;
        }
        case 3:
        {
            QString strContex1 =QString(tr("%1 [电机转向：%2---模式：%3---针位：%4]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_motoDirect->currentIndex()==0 ? tr("正转"):tr("反转")).arg(ui->m_motoMode->currentText()).arg(ui->m_motoNeedlValve->text());
            QString strContex2 =QString(tr("%1 [电机转向：%2---模式：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_motoDirect->currentIndex()==0 ? tr("正转"):tr("反转")).arg(ui->m_motoMode->currentText());
            strContex = ui->m_motoMode->currentText() ==tr("无") || ui->m_motoMode->currentText() ==tr("到零") ? strContex2:strContex1;

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<QString("%1").arg(ui->m_motoDirect->currentIndex()==1 ? 0:1)<<QString("%1").arg(ui->m_motoMode->currentIndex())<<ui->m_motoNeedlValve->text();
        }
        case 4:
        {
            strContex =QString(tr("%1 [动作状态：%2---针位：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_state_3->currentText()).arg(ui->m_needle_3->text());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<QString("%1").arg(ui->m_state_3->currentIndex())<<ui->m_needle_3->text();
            break;
        }
        case 5:
        {
            strContex =QString(tr("%1 [选针编号：%2---动作方式：%3---选针模式：%4]")).arg(ui->m_title->text().trimmed()).
                    arg(ui->m_needleNum->currentText()).arg(ui->m_state_5->currentIndex()==0 ? tr("正常"):tr("辅助")).arg(ui->m_state_4->currentText());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<ui->m_needleNum->currentText()<<QString("%1").arg(ui->m_state_5->currentIndex()==0 ? 1:0)
                  <<QString("%1").arg(ui->m_state_4->currentIndex());
            break;
        }
        case 6:
        {
            strContex =QString(tr("%1 [向左移：%2---向右移：%3]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_left->text()).arg(ui->m_right->text());
            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<ui->m_left->text()<<ui->m_right->text();
            break;
        }
        case 7:
        {
            strContex =QString(tr("%1 [花纹名称：%2]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_patternLink->currentText());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<ui->m_patternLink->currentText();
            break;
        }
        case 8:
        {
            strContex =QString(tr("%1 [%2,%3,%4,%5,%6,%7,%8,%9]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_edtCircle_1->text())
                    .arg(ui->m_edtCircle_2->text())
                    .arg(ui->m_edtCircle_3->text())
                    .arg(ui->m_edtCircle_4->text())
                    .arg(ui->m_edtCircle_5->text())
                    .arg(ui->m_edtCircle_6->text())
                    .arg(ui->m_edtCircle_7->text())
                    .arg(ui->m_edtCircle_8->text());

            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<ui->m_edtCircle_1->text()<<ui->m_edtCircle_2->text()<<ui->m_edtCircle_3->text()
                  <<ui->m_edtCircle_4->text()<<ui->m_edtCircle_5->text()<<ui->m_edtCircle_6->text()<<ui->m_edtCircle_7->text()<<ui->m_edtCircle_8->text();
            break;
        }
        case 9:
        {
            strContex =QString(tr("%1 [动作状态：%2]")).arg(ui->m_title->text().trimmed())
                    .arg(ui->m_state_6->currentText());
            lstPara<<strContex<<ui->m_chainTree->currentItem()->text(1)<<QString("%1").arg(ui->m_state_6->currentIndex());
            break;
        }
        default:
            break;
        }

        for(int i=0; i<lstPara.count(); ++i)
        {
            ui->m_chainTree->currentItem()->setText(i,lstPara.at(i));
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
    ui->stackedWidget->setCurrentIndex(0);
    ui->m_stackCmdEdit->hide();
    ui->m_chainTitle->setText(tr("[链条管理]"));

    freshRightButtonContent(QStringList()<<tr("返回")<<tr("从U盘\n输入")<<tr("输出到\nU盘")<<tr("删除")<<tr("编辑\n工作链条")<<tr("工作链条\n设定"));

    /*初始化链条管理窗口*/
    initChainManageTable();

    /*初始化命令选择模块*/
    initCmdEdit();

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
        nameFilter << "*.sta";
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

//初始化链条命令树
void Frm_chainProcessingClass::initChainTree()
{
    ui->m_chainTree->clear();

    QString filePath = QString("%1/%2").arg(PATH_CHAIN_FILE_LOCAL).arg(ui->m_labWorkChain->text());
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDomDocument document;
    document.setContent(&file);
    file.close();

    QTreeWidgetItem* itemLv1,* itemLv2,* itemLv3;
    QDomNodeList lstCmdLv1 = document.elementsByTagName("phase");
    if(!lstCmdLv1.isEmpty())
    {
        for(int i=0; i<lstCmdLv1.count(); ++i)
        {
            itemLv1  = new QTreeWidgetItem(ui->m_chainTree,QStringList()<<m_lstFirstNodeName[lstCmdLv1.at(i).toElement().attribute("val").toInt()] ,FIRST_LEVEL_NODE);
            itemLv1->setIcon(0, QIcon(":/image/socks.png"));

            QDomNodeList lstCmdLv2 = lstCmdLv1.at(i).childNodes();
            for(int j=0;j<lstCmdLv2.count(); ++j)
            {
                itemLv2  = new QTreeWidgetItem(itemLv1,QStringList()<<lstCmdLv2.at(j).toElement().attribute("number") ,SECEND_LEVEL_NODE);
                itemLv2->setIcon(0, QIcon(":/image/right_arrow.png"));


                QDomNodeList lstCmdLv3 = lstCmdLv2.at(j).childNodes();
                for(int z=0;z<lstCmdLv3.count(); ++z)
                {
                    itemLv3  = new QTreeWidgetItem(itemLv2,QString("%1").arg(lstCmdLv3.at(z).toElement().attribute("content")).split(";"),THIRD_LEVEL_NODE);
                    itemLv3->setIcon(0, QIcon(ComConfigClass::GetInstance()->getCmdIcoPathByName(itemLv3->text(1).toInt())));
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

}

/*写入主链条xml节点数据*/
void Frm_chainProcessingClass::writeChainTree()
{
    /*读取文件内容*/
    QString filePath = QString("%1/%2").arg(PATH_CHAIN_FILE_LOCAL).arg(ui->m_labWorkChain->text());
    QFile fileRed(filePath);
    fileRed.open(QIODevice::ReadOnly);
    QDomDocument doc;
    doc.setContent(&fileRed);
    fileRed.close();

    QDomNodeList lstCmdModel = doc.elementsByTagName("mainChain");
    if(lstCmdModel.count()==0) return;
    QDomElement mainChainBefor = lstCmdModel.at(0).toElement();

    QDomElement mainChainAfter = doc.createElement("mainChain");;

    QTreeWidgetItem *itemLv1, *itemL2, *itemL3;
    QDomElement domLv1,domLv2,domLv3;
    for(int i=0; i<ui->m_chainTree->topLevelItemCount(); ++i)
    {
        itemLv1 = ui->m_chainTree->topLevelItem(i);
        //写入一级节点
        domLv1 = doc.createElement("phase");
        domLv1.setAttribute("val", m_lstFirstNodeName.indexOf(itemLv1->text(0)));

        mainChainAfter.appendChild(domLv1);
        for(int j=0; j<itemLv1->childCount(); ++j)
        {
            itemL2 = itemLv1->child(j);
            //写入二级节点
            domLv2 = doc.createElement("step");
            domLv2.setAttribute("number", itemL2->text(0).split(" ").last());
            domLv1.appendChild(domLv2);
            for(int z=0; z<itemL2->childCount(); ++z)
            {
                itemL3 = itemL2->child(z);

                //写入三级节点
                domLv3 = doc.createElement("cmd");
                domLv3.setAttribute("content", itemL3->text(0));
                domLv2.appendChild(domLv3);
                //写入命令类型
                domLv3.setAttribute("type", QString("%1").arg(ComConfigClass::GetInstance()->getCmdTypeByIndex(itemL3->text(1).toInt())));
                //写入阀号NO val0
                domLv3.setAttribute("val0", QString("%1").arg(ComConfigClass::GetInstance()->getCmdProByIndex(itemL3->text(1).toInt()).NO));
                //写入命令属性值val1往后,由于lstAtt前2个值分别是显示的内容和index,从低三个值开始才是对应的属性值，所以n从2开始
                QStringList lstAtt=getCmdAtt(itemL3);
                for(int n=2; n<lstAtt.count(); ++n)
                {
                    domLv3.setAttribute(QString("val%1").arg(n-1), lstAtt.at(n));
                }
                //写入content
                domLv3.setAttribute("content", lstAtt.join(";"));
            }
        }
    }
    doc.elementsByTagName("body").at(0).replaceChild(mainChainAfter, mainChainBefor);

    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QFile::Truncate);
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
}

/*获取命令树三级节点的属性*/
QStringList Frm_chainProcessingClass::getCmdAtt(QTreeWidgetItem *item)
{
    QStringList lst;
    lst.clear();
    for(int i=0;i<item->columnCount(); ++i)
    {
        lst<<item->text(i);
    }
    return lst;
}

/*向XDDP发送数据*/
void Frm_chainProcessingClass::writeToXddp()
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

/*初始化链条管理表格*/
void Frm_chainProcessingClass::initChainManageTable()
{
    /*start*******************************************初始化链条管理窗口******************************************************/
    /*初始化表格数据*/
    ui->m_tabChainManage->clearContents();
    ui->m_tabChainManage->setRowCount(0);
    ui->m_tabChainManage->setColumnCount(4); //设置总列数；
    ui->m_tabChainManage->setColumnWidth(0,50);
    ui->m_tabChainManage->setColumnWidth(1,120);
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
        filters<< QString("*.sta");
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

/*获取设备中的花型列表*/
QStringList Frm_chainProcessingClass::getPatternList()
{
    qDebug()<<"-------------------";
    QStringList filters;     //定义过滤变量；
    filters<< QString("*.dis");
    QDirIterator dir_iterator(PATH_CHAIN_FILE_LOCAL,filters,QDir::Files | QDir::NoSymLinks,QDirIterator::Subdirectories);//定义迭代器并设置过滤器；
    QStringList lstName;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info=dir_iterator.fileInfo();

        lstName<<file_info.baseName();
    }
    qDebug()<<lstName;
    return lstName;
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

    ui->splitter->setStretchFactor(0,5);
    ui->splitter->setStretchFactor(1,2);

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

        ui->m_wgtCmdContent->setItem(i, 0, new QTableWidgetItem(QIcon(QString(":/image/%1").arg(cmdPro.ico)), QString("%1").arg(cmdPro.name.at(ComConfigClass::m_iLanguage)),cmdPro.index));
    }
    ui->m_wgtCmdContent->setCurrentCell(0,0);
    ui->m_wgtCmdContent->setIconSize(QSize(30,30));
}

/*获取节点的类型*/
int Frm_chainProcessingClass::getItemType(QTreeWidgetItem *item)
{
    if(!item->parent())
    {
        return FIRST_LEVEL_NODE;
    }
    else if(!item->parent()->parent())
    {
        return SECEND_LEVEL_NODE;
    }
    else if(!item->parent()->parent()->parent())
    {
        return THIRD_LEVEL_NODE;
    }
    return DEFALUT_LEVE_NODE;
}

void Frm_chainProcessingClass::freshCmdContent()
{
    int i = ui->m_wgtCmdName->currentRow();
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
