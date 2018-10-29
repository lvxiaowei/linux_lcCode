#include "baseClassWgt.h"
//#include "ui_baseClassWgt.h"

QList<QPushButton*> baseClassWgt::g_lstRightButton;
QStackedWidget* baseClassWgt::g_pStackedWgt;
QMap<int, baseClassWgt*> baseClassWgt::g_mapIndexToWgt;
baseClassWgt* baseClassWgt::g_pCurentDealWgt;

baseClassWgt::baseClassWgt(QWidget *parent) :
    QWidget(parent)
{
    /*键盘数字0到9的枚举和数字0到9的映射*/
    m_mapNoKeyToValue.clear();
    m_mapNoKeyToValue[Key_0]=0;
    m_mapNoKeyToValue[Key_1]=1;
    m_mapNoKeyToValue[Key_2]=2;
    m_mapNoKeyToValue[Key_3]=3;
    m_mapNoKeyToValue[Key_4]=4;
    m_mapNoKeyToValue[Key_5]=5;
    m_mapNoKeyToValue[Key_6]=6;
    m_mapNoKeyToValue[Key_7]=7;
    m_mapNoKeyToValue[Key_8]=8;
    m_mapNoKeyToValue[Key_9]=9;
}

baseClassWgt::~baseClassWgt()
{

}

//刷新右侧快捷键的显示信息
void baseClassWgt::freshRightButtonContent(QStringList lstStr)
{
    if(lstStr.count() !=  g_lstRightButton.count())
    {
        return;
    }
    int i=0;
    foreach (QPushButton* p, g_lstRightButton) {
        p->setText(lstStr.at(i));
        ++i;
    }
}

//设置下一个处理的窗口
void baseClassWgt::setNextDealWgt(int index)
{
    g_pCurentDealWgt = g_mapIndexToWgt[index];

    g_pCurentDealWgt->initShowFrmConfig();

    if(g_pStackedWgt->isAncestorOf(g_pCurentDealWgt))
    {
        g_pStackedWgt->setCurrentWidget(g_pCurentDealWgt);
    }
}

//获取xml配置文件信息
bool baseClassWgt::getXmlConfig(QDomDocument& document)
{
    QFile file(CONFIG_FILE_XML_PATH);
    if (!file.open(QIODevice::ReadOnly))
    {
        myMessageBox::getInstance()->setMessage(tr("配置文件打开失败！"), BoxInfo);
        file.close();
        return false;
    }

    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        QMessageBox::information(NULL, QString(QObject::tr("提示")), QString(QObject::tr("配置文件在行%1和列%2处解析文件失败")).arg(QString::number(row, 10)).arg(QString::number(column, 10)));
        file.close();
        return false;
    }
    file.close();
    if(document.isNull())
    {
        QMessageBox::information(NULL, QString(QObject::tr("提示")), QString(QObject::tr("配置文件内容为空！")));
        return false;
    }
    return true;
}

/*设置并刷新控件的属性*/
void baseClassWgt::setObjProperty(QWidget* pwgt, const char *name, const QVariant &value)
{
    if(!pwgt) return;

    pwgt->setProperty(name, value);
    pwgt->style()->unpolish(pwgt);
    pwgt->style()->polish(pwgt);
    pwgt->update();
}
