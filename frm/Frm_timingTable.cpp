#include "Frm_timingTable.h"
#include "ui_Frm_timingTable.h"

Frm_timingTable::Frm_timingTable(QWidget *parent) :
    baseClassWgt(parent),
    ui(new Ui::Frm_timingTable)
{
    g_mapIndexToWgt[PAGE_TIMINGS]= this;
    ui->setupUi(this);
}

Frm_timingTable::~Frm_timingTable()
{
    delete ui;
}

/*处理串口数据*/
void Frm_timingTable::keyPressEvent(int key)
{
    qDebug()<<"当前正在时序设置界面界面,key="<<key;
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
        saveConfigFile();
        break;


    case Key_Up:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, QString());
        if(m_curentTable->currentRow()==3) return;
        QCoreApplication::sendEvent(m_curentTable, &keyPress);
        break;
    }
    case Key_Down:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(m_curentTable, &keyPress);
        break;
    }
    case Key_Left:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(m_curentTable, &keyPress);
        break;
    }
    case Key_Right:
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, QString());
        QCoreApplication::sendEvent(m_curentTable, &keyPress);
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
        if(m_curentTable->currentItem()->text()=="-") return;
        bool ok;
        QString strInputValue = QString("%1").arg(m_mapNoKeyToValue[key]);
        QString strCurentValue = m_curentTable->currentItem()->text() + strInputValue;
        m_curentTable->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));

    }
        break;
    case Key_Del:
    {
        if(m_curentTable->currentItem()->text()=="-") return;
        bool ok;
        QString strCurentValue = m_curentTable->currentItem()->text();
        strCurentValue = strCurentValue.left(strCurentValue.length() - 1);
        m_curentTable->currentItem()->setText(QString::number((strCurentValue.trimmed()).toInt(&ok,10)));
        break;
    }

    default:
        break;
    }
}

/*数据初始化*/
void Frm_timingTable::initShowFrmConfig()
{
    freshRightButtonContent(QStringList()<<tr("返回")<<tr("切换")<<tr("")<<tr("")<<tr("")<<tr("保存修改"));
    initTimingsTable();
    ui->tabWidget->setCurrentIndex(0);
    m_curentTable = dynamic_cast<QTableWidget*>(ui->tabWidget->currentWidget());
}

/*处理XDDP数据*/
void Frm_timingTable::handleXddpData(QByteArray data)
{
    Q_UNUSED(data);
}

/*时序表初始化*/
void Frm_timingTable::initTimingsTable()
{
    //移除所有的tab
    for(int i=0; i<ui->tabWidget->count(); ++i)
    {
        ui->tabWidget->removeTab(0);
    }

    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    QDomNodeList nodeTimTab = document.elementsByTagName("timingsTable");
    if(nodeTimTab.count()==0) return;

    for(int i=0; i<nodeTimTab.at(0).childNodes().count(); ++i)
    {
        QDomNode tableNode = nodeTimTab.at(0).childNodes().at(i);
        QString  tableName = tableNode.toElement().attribute("name");

        QTableWidget *tableWidget = new QTableWidget(10, 4*(tableNode.childNodes().count()));  //时序表一共10行固定的，列的话根据硕子的数目来决定，一把硕子4列
        /*设置表头内容*/
        QStringList header;
        header<<tr("名称")<<tr("状态(in/out)")<<tr("动作方式(in/out)")<<tr("沙嘴辅助")<<tr("沙嘴半位/全位")<<tr("剪刀")<<tr("进线吹气")<<tr("剪刀吹气")<<tr("换线吹气")<<tr("护针吹气");
        tableWidget->setVerticalHeaderLabels(header);
        tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); //均分行
        tableWidget->setStyleSheet("gridline-color: rgb(0,0,0)");
        //设置单元格大小
        for(int i=0; i<tableWidget->columnCount(); ++i)
        {
            tableWidget->setColumnWidth(i,60);
        }

        ui->tabWidget->addTab(tableWidget,tableName);

        for(int j=0; j<tableNode.childNodes().count(); ++j)
        {
            QDomElement itemNode = tableNode.childNodes().at(j).toElement();
            QString     itemName = itemNode.attribute("name");
            QStringList lstVal   = itemNode.attribute("val").split(",");

            //合并单元格
            tableWidget->setSpan(0, j*4,   1, 4);
            tableWidget->setSpan(1, j*4,   1, 2);
            tableWidget->setSpan(1, j*4+2, 1, 2);
            //初始化数据表头，前3行
            tableWidget->setItem(0, j*4,   new QTableWidgetItem(itemName));
            tableWidget->item(0,j*4)->setBackgroundColor(QColor("skyblue"));
            tableWidget->setItem(1, j*4,   new QTableWidgetItem(tr("进")));
            tableWidget->item(1,j*4)->setBackgroundColor(QColor("skyblue"));
            tableWidget->setItem(1, j*4+2, new QTableWidgetItem(tr("出")));
            tableWidget->item(1,j*4+2)->setBackgroundColor(QColor("skyblue"));
            tableWidget->setItem(2, j*4,   new QTableWidgetItem(tr("I")));
            tableWidget->item(2,j*4)->setBackgroundColor(QColor("skyblue"));
            tableWidget->setItem(2, j*4+1, new QTableWidgetItem(tr("O")));
            tableWidget->item(2,j*4+1)->setBackgroundColor(QColor("skyblue"));
            tableWidget->setItem(2, j*4+2, new QTableWidgetItem(tr("I")));
            tableWidget->item(2,j*4+2)->setBackgroundColor(QColor("skyblue"));
            tableWidget->setItem(2, j*4+3, new QTableWidgetItem(tr("O")));
            tableWidget->item(2,j*4+3)->setBackgroundColor(QColor("skyblue"));
            //初始化针位数据
            for(int m=0; m<4; ++m)
                for(int n=0; n<7; ++n)
                {
                    QString val = lstVal.at(m*7+n);
                    tableWidget->setItem(n+3, j*4+m, new QTableWidgetItem(val));
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
        tableWidget->setCurrentCell(3,0);
    }
}

//保存数据到配置文件
void Frm_timingTable::saveConfigFile()
{
    QDomDocument document;
    if(!getXmlConfig(document))
        return;

    QDomNodeList nodeTimTab = document.elementsByTagName("timingsTable");
    if(nodeTimTab.count()==0) return;

    for(int i=0; i<ui->tabWidget->count(); ++i)
    {
        QTableWidget *wgtTable = dynamic_cast<QTableWidget*>(ui->tabWidget->widget(i));

        QString tabName= ui->tabWidget->tabText(i);

        if(wgtTable==NULL) continue;

        for(int j=0; j<wgtTable->columnCount()/4; ++j)
        {
            //itemName为YF1 YF2等梭子的名字
            QString itemName=wgtTable->item(0,j*4)->text();
            //根据表明和YF1 YF2等获取的节点 tableNode为table节点，itemNode为item节点
            QDomNode tableNode, itemNode;
            for(int n=0;n<nodeTimTab.at(0).childNodes().count(); ++n)
            {
                if(nodeTimTab.at(0).childNodes().at(n).toElement().attribute("name")==tabName)
                {
                    tableNode = nodeTimTab.at(0).childNodes().at(n);
                    break;
                }
            }
            for(int n=0; n<tableNode.childNodes().count(); ++n)
            {
                if(tableNode.childNodes().at(n).toElement().attribute("name")==itemName)
                {
                    itemNode = tableNode.childNodes().at(n);
                    break;
                }
            }
            //获取xml里面item节点的val需要的值
            //初始化针位数据
            QStringList lstVal;
            for(int m=0; m<4; ++m)
                for(int n=0; n<7; ++n)
                {
                    lstVal.append(wgtTable->item(n+3, j*4+m)->text());
                }

            itemNode.toElement().setAttribute("val",lstVal.join(","));

        }
    }

    QFile file(CONFIG_FILE_XML_PATH);
    file.open(QIODevice::WriteOnly|QFile::Truncate);
    QTextStream stream(&file);
    document.save(stream, 4);
    file.close();

     myMessageBox::getInstance()->setMessage(tr("保存修改成功！"), BoxInfo);
}
