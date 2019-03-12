#include "comConfigClass.h"
#include <QDebug>
/*start********************************************************ComConfigClass*********************************************/
ComConfigClass* ComConfigClass::m_comCfgClass = NULL;
int ComConfigClass::m_iLanguage=0;

ComConfigClass::ComConfigClass()
{
    QString path;
    QDir dir;
    path=dir.currentPath();

    parseCfgFile();

    initMachineDataMapping();
}

/*解析配置文件*/
void ComConfigClass::parseCfgFile()
{
    m_cmdModel.clear();

    QFile file(COMMCONFIG_FILE_PATH);
    if (!file.open(QIODevice::ReadOnly))
    {
        myHelper::showMessageBoxError(QObject::tr("配置文件打开失败,影响当前功能使用，请联系工程师解决!"), 2);
        return;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        QMessageBox::information(NULL, QString(QObject::tr("提示")), QString(QObject::tr("配置文件在行%1和列%2处解析文件失败")).arg(QString::number(row, 10)).arg(QString::number(column, 10)));
        return;
    }

    if(document.isNull())
    {
        QMessageBox::information(NULL, QString(QObject::tr("提示")), QString(QObject::tr("配置文件内容为空！")));
        return;
    }

    /********************************语言类型映射数据解析******************************************/
    QDomNodeList lstLanguage = document.elementsByTagName("language");
    QStringList lstLanguageType;
    if(lstLanguage.count()==0)
    {
        //如果配置文件中没有语言相关选项，默认是中文处理
        m_iLanguage=0;
        lstLanguageType<<"CN";
    }
    else {
        m_iLanguage = lstLanguage.at(0).toElement().attribute("index").toInt();
        lstLanguageType = lstLanguage.at(0).toElement().attribute("content").split(";");
    }

    /********************************命令模块数据解析******************************************/
    QDomNodeList lstCmdModel = document.elementsByTagName("cmdModel");
    if(!lstCmdModel.isEmpty())
    {
        QDomNodeList lstNode = lstCmdModel.at(0).childNodes();
        QString strName;
        QDomNode node;
        QDomNodeList lstNodeChild;
        QDomElement nodeChildElt;
        cmdSettingPro cmdPro;
        QList<cmdSettingPro> lstCmdPro;
        int index;
        bool ok;
        QStringList lstCmdModelLge;
        for(int i=0; i<lstNode.count(); ++i)
        {
            node = lstNode.item(i);
            strName = node.toElement().attributeNode("index").value();

            if(strName.isEmpty()) continue;
            index=strName.toInt(&ok,16);

            //保存parameter的多语言map
            lstCmdModelLge.clear();
            for(int z=0; z<lstLanguageType.count(); ++z)
            {
                lstCmdModelLge <<node.toElement().attributeNode(lstLanguageType.at(z)).value();
            }
            m_cmdModelType[index] = lstCmdModelLge;

            lstNodeChild = node.childNodes();
            for(int j=0; j<lstNodeChild.count(); ++j)
            {
                nodeChildElt= lstNodeChild.at(j).toElement();
                cmdPro.NO = nodeChildElt.attributeNode("NO").value();
                cmdPro.ico = nodeChildElt.attributeNode("ico").value();
                cmdPro.cmdType = nodeChildElt.attributeNode("cmdType").value();
                cmdPro.index = nodeChildElt.attributeNode("index").value().toInt(&ok,16);

                cmdPro.name.clear();
                for(int z=0; z<lstLanguageType.count(); ++z)
                {
                    cmdPro.name <<nodeChildElt.attributeNode(lstLanguageType.at(z)).value();
                }
                lstCmdPro.append(cmdPro);
                m_mapCmdContentToCmdType[cmdPro.index] = cmdPro;
            }
            m_cmdModel[index] = lstCmdPro;
            lstCmdPro.clear();
        }
    }

    /********************************宏文件数据解析******************************************/
    QDomNodeList lstMacroModel = document.elementsByTagName("macro"),lstStep,lstStepItem;
    QString stepVal;
    QDomNode macroModel,step;
    QDomElement stepElement;
    proNameAndIco pro;
    QList<proNameAndIco> lstPro;
    QList<QList<proNameAndIco>> lstLstPro;
    for(int i=0; i<lstMacroModel.count(); ++i)
    {
        macroModel = lstMacroModel.at(i);
        stepVal = macroModel.toElement().attributeNode("value").value();
        if(stepVal.isEmpty()) continue;

        if(m_macroModel.contains(stepVal))
        {
            QMessageBox::information(NULL, QString(QObject::tr("提示")), QString(QObject::tr("配置文件中宏名字[%1]定义重复").arg(stepVal)));
        }
        lstStep = macroModel.childNodes();

        lstLstPro.clear();
        for(int i=0; i<lstStep.count(); ++i)
        {
            step = lstStep.at(i);
            lstStepItem = step.childNodes();
            lstPro.clear();
            for(int j=0; j<lstStepItem.count(); ++j)
            {
                stepElement = lstStepItem.at(j).toElement();
                pro.name = stepElement.attributeNode("value").value();
                pro.ico  = stepElement.attributeNode("ico").value();
                lstPro.append(pro);
            }
            lstLstPro.append(lstPro);
        }
        m_macroModel[stepVal] = lstLstPro;
        m_lstMac.append(stepVal);
    }
    /********************************机器类型数据解析******************************************/
    QDomNodeList  lstMachinType = document.elementsByTagName("machineType"),lstYarnfinger;
    QDomNode machineType;
    QDomElement machineTypeElmt,yarnfinger;
    yarnfingerPro typePro;
    QList<yarnfingerPro> lstTypePro;
    machineTypePro mchineTypePro;
    QString machineName,strLKS;
    for(int i=0; i<lstMachinType.count(); ++i)
    {
        machineType = lstMachinType.at(i);
        machineTypeElmt = machineType.toElement();
        machineName = machineTypeElmt.attributeNode("name").value();
        strLKS = machineTypeElmt.attributeNode("LKS").value();
        lstYarnfinger = machineType.childNodes();

        lstTypePro.clear();
        for(int j=0; j<lstYarnfinger.count(); ++j)
        {
            typePro.count=0;
            typePro.name.clear();
            typePro.types.clear();

            yarnfinger     =  lstYarnfinger.at(j).toElement();
            typePro.name   =  yarnfinger.attributeNode("name").value();
            typePro.count  =  yarnfinger.attributeNode("count").value().toInt();
            typePro.types  << yarnfinger.attributeNode("types").value().split("/");
            lstTypePro.append(typePro);
        }
        mchineTypePro.bNeedLKS = (strLKS == "true" ? true:false);
        mchineTypePro.lstYarnFingerPro =lstTypePro;
        m_machineTypeModel[machineName] = mchineTypePro;
    }
    /********************************密度设置数据解析******************************************/
    QDomNodeList  lstMotors = document.elementsByTagName("motor");
    motorPro motoPro;
    QList<motorPro> lstMotoPro;
    for(int i=0; i<lstMotors.count(); ++i)
    {
        QString motoName = lstMotors.at(i).toElement().attributeNode("name").value();
        QDomNodeList lstMotorContent = lstMotors.at(i).toElement().childNodes();
        lstMotoPro.clear();
        for(int j=0; j<lstMotorContent.count(); ++j)
        {
            QDomElement motoProElt = lstMotorContent.at(j).toElement();
            motoPro.block      = motoProElt.attributeNode("block").value();
            motoPro.startStep  = motoProElt.attributeNode("startStep").value();
            motoPro.endStep    = motoProElt.attributeNode("endStep").value();
            motoPro.startValue = motoProElt.attributeNode("startValue").value();
            motoPro.endValue   = motoProElt.attributeNode("endValue").value();
            motoPro.steps      = motoProElt.attributeNode("steps").value();
            lstMotoPro.append(motoPro);
        }
        m_mapMotorPro[motoName] = lstMotoPro;
    }
}

/*初始化上机文件数据格式映射表*/
void ComConfigClass::initMachineDataMapping()
{
    /*数据格式映射表--模块类型定义*/
    m_mapModuleNumberTOVal.clear();
    m_mapModuleNumberTOVal["valve"]               = OPER_VALVE;/*气阀*/
    m_mapModuleNumberTOVal["macro"]               = OPER_MACRO;/*宏*/
    m_mapModuleNumberTOVal["speed"]               = OPER_SPEED;/*速度*/
    m_mapModuleNumberTOVal["elasticStart"]        = OPER_ELASTIC_START;/*橡筋马达开始*/
    m_mapModuleNumberTOVal["elasticEnd"]          = OPER_ELASTIC_END;/*橡筋马达结束*/
    m_mapModuleNumberTOVal["selection"]           = OPER_SELECTION;/*固定选针*/
    m_mapModuleNumberTOVal["patternUse"]          = OPER_PATTERN_USE;/*花型调用*/
    m_mapModuleNumberTOVal["patternContinue"]     = OPER_PATTERN_CONTINUE;/*花型继续*/
    m_mapModuleNumberTOVal["patternQuit"]         = OPER_PATTERN_QUIT;/*花型退出*/
    //    m_mapModuleNumberTOVal["patternUse"]          = OPER_FOOTPATTERN_USE;/*脚底花型调用*/
    m_mapModuleNumberTOVal["footPatternQuit"]     = OPER_FOOTPATTERN_QUIT;/*脚底花型退出*/
    m_mapModuleNumberTOVal["stepCircle"]          = OPER_STEP_CIRCLE;/*步循环*/
    m_mapModuleNumberTOVal["circle"]              = OPER_CIRCLE;/*循环*/
    m_mapModuleNumberTOVal["circleEnd"]           = OPER_CIRCLE_END;/*循环结束*/
    m_mapModuleNumberTOVal["chainPause"]          = OPER_CHAIN_PAUSE;/*链条暂停*/
    m_mapModuleNumberTOVal["miniCircleEnable"]    = OPER_MINICIRCLE_ENABLE;/*最小循环使能*/
    m_mapModuleNumberTOVal["miniCircleDisable"]   = OPER_MINICIRCLE_DISABLE;/*最小循环除能*/
    m_mapModuleNumberTOVal["oil"]                 = OPER_OIL;/*加油*/
    m_mapModuleNumberTOVal["footIncreaseEcrease"] = OPER_FOOT_INCREASE_DCREASE;/*脚底增减*/
    m_mapModuleNumberTOVal["turning"]             = OPER_TURNING;/*转向指令*/
    m_mapModuleNumberTOVal["lowSpeed"]            = OPER_LOWSPEED_CMD;/*低速命令*/
    m_mapModuleNumberTOVal["upMoto"]              = OPER_UPMOTO;/*上针筒马达密度归零*/
    m_mapModuleNumberTOVal["downMoto"]            = OPER_DOWNMOTO;/*下针筒马达密度归零*/
    m_mapModuleNumberTOVal["elasticMoto"]         =  OPER_ELASTICMOTO;/*橡筋马达密度归零*/
    m_mapModuleNumberTOVal["zeroing"]             = OPER_ZEROING;/*归零程序调用*/
    m_mapModuleNumberTOVal["stepStop"]            = OPER_STEPSTOP;/*步段停机*/
    m_mapModuleNumberTOVal["chainEnd"]            = OPER_CHAIN_END;/*链条结束*/

}

/*根据具体的命令模块，返回具体的选项*/
QList<cmdSettingPro> ComConfigClass::getSpecificModuleType(int index)
{
    QList<cmdSettingPro> lstPro;
    if(0==index)
    {
        foreach (QList<cmdSettingPro> var, m_cmdModel.values()) {
            lstPro.append(var);
        }
        return lstPro;
    }
    else {
        return m_cmdModel[m_cmdModel.keys().at(index-1)];
    }
}

/*返回命令模块的命令类型lst*/
QStringList ComConfigClass::getCmdModelTypes()
{
    QStringList lstCmdModelTypes;
    lstCmdModelTypes<<QObject::tr("所有");
    for(int i=0; i<m_cmdModelType.count(); ++i)
    {
        lstCmdModelTypes << m_cmdModelType.values().at(i).at(m_iLanguage);
    }
    return lstCmdModelTypes;
}

/*end********************************************************ComConfigClass*********************************************/

/*start******************************************************NoFocusDelegate*********************************************/
NoFocusDelegate::NoFocusDelegate()
{
}

void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
    {
        itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
}
/*end******************************************************NoFocusDelegate*********************************************/

/*start******************************************************paintScallBar*********************************************/
paintScallBar::paintScallBar(Qt::Orientation direction, QWidget *parent, int len)
    :QWidget(parent)
{
    m_direction   = direction;
    m_len         = len;
}

void paintScallBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter1(this);
    QRect rulerRect = rect();
    painter1.fillRect(rulerRect,QColor(0, 0, 0));

    QPainter painter_size10(this);
    QFont font10;
    font10.setPixelSize(10);
    painter_size10.setFont(font10);

    QPainter painter_size12(this);
    QFont font12;
    font12.setPixelSize(10);
    painter_size12.setFont(font12);
    if(m_direction ==Qt::Horizontal)
    {
        /*添加wgt下边框的线*/
        painter_size10.drawLine(rulerRect.bottomLeft(),rulerRect.bottomRight());
        /*获取每一列的长度*/
        float width = rulerRect.width()/float(m_len);

        for(int i=0; i<m_len; ++i)
        {
            if(i%5 ==0 && i%10 !=0)
            {
                painter_size10.drawLine(i*width, rulerRect.height(), i*width, rulerRect.height()/float(2));
                painter_size10.drawText(i*width+2, rulerRect.height()/float(2)+2,QString("%1").arg(i));
            }
            else if(i%5 ==0 && i%10 ==0)
            {
                painter_size12.drawLine(i*width, rulerRect.height(), i*width, 0);
                painter_size12.drawText(i*width+2, rulerRect.height()/float(2)+2,QString("%1").arg(i));
            }
            else
            {
                painter_size10.drawLine(i*width, rulerRect.height(), i*width, rulerRect.height()/float(4)*3);
            }
        }
    }
    else
    {
        /*添加wgt下边框的线*/
        painter_size10.drawLine(rulerRect.topRight(),rulerRect.bottomRight());

        /*获取每一列的长度*/
        float height = rulerRect.height()/float(m_len);

        for(int i=0; i<=m_len; ++i)
        {
            if(i%5 ==0 && i%10 !=0)
            {
                painter_size10.drawLine(rulerRect.width()/float(2), i*height, rulerRect.width(), i*height);
                painter_size10.rotate(180);
                painter_size10.drawText(0, i*height-2, QString("%1").arg(i));
            }
            else if(i%5 ==0 && i%10 ==0)
            {
                painter_size12.drawLine(0, i*height, rulerRect.width(), i*height);
                painter_size12.drawText(0, i*height-2, QString("%1").arg(i));
            }
            else
            {
                painter_size10.drawLine(rulerRect.width()/float(4)*3, i*height, rulerRect.width(), i*height );
            }
        }
    }

    drawPos();
}

void paintScallBar::drawPos()
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    QRect rulerRect = rect();

    if(m_direction ==Qt::Vertical)
    {
        /*获取每一列的长度*/
        float height = rulerRect.height()/float(m_len);

        painter.drawLine(0, height*(m_pos-0.5), rulerRect.width(), height*(m_pos-0.5));
    }
}
/*end******************************************************paintScallBar*********************************************/

/*start******************************************************cMyTableWIdget*********************************************/
cMyTableWIdget::cMyTableWIdget(QWidget *parent) :
    QTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setHighlightSections(false);
    setItemDelegate(new NoFocusDelegate());  /*设置表格点击没有虚框显示*/
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    m_lstRowfixed.clear();
    m_lstColumfixed.clear();
    m_showGrid       = true;
    m_xMiniPixel = m_yMiniPixel = 1;
}

/*放大*/
void cMyTableWIdget::zoomOut()
{
    m_scale = m_scale +1;
    setShowZoom(m_scale);
}

/*缩小*/
void cMyTableWIdget::zoomIn()
{
    if(m_scale <=1) return;
    m_scale = m_scale -1;
    setShowZoom(m_scale);
}

/*显示网格*/
void cMyTableWIdget::setGrid()
{
    m_showGrid = showGrid() ? false:true;
    setShowGrid(m_showGrid);
}

void cMyTableWIdget::setShowZoom(int i)
{
    if(i<1) return;
    m_scale = i;
    /*当显示的比例为1:1的时候去网格*/
    m_scale == 1 ? setShowGrid(false) : setShowGrid(m_showGrid);

    for(int i=0;i<m_row;i++)
    {
        if(m_lstRowfixed.contains(i)) continue;
        setRowHeight(i,m_yMiniPixel*m_scale);
    }
    for(int j=0;j<m_column;j++)
    {
        if(m_lstColumfixed.contains(j)) continue;
        setColumnWidth(j,m_xMiniPixel*m_scale);
    }
    emit zoomChanged(m_scale);
}
/*end******************************************************cMyTableWIdget*********************************************/
