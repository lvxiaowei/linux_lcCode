#ifndef COMCONFIGCLASS_H
#define COMCONFIGCLASS_H

#include <QString>
#include <QDir>
#include <QtXml>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTableWidget>
#include <QToolBar>
#include <QHeaderView>
#include <QApplication>
#include <QString>
#include <QStringList>
#include "publicData.h"
#include <QDebug>
#include "api/myMessageBox.h"
/*start*****************************************************************************公共配置解析类******************************************************************************/
class ComConfigClass
{
public:
    /*创建单实例对象*/
    static ComConfigClass* GetInstance()
    {
        if(ComConfigClass::m_comCfgClass == NULL)
        {
            ComConfigClass::m_comCfgClass = new ComConfigClass();
        }
        return ComConfigClass::m_comCfgClass;
    }

    static int m_iLanguage; /*配置文件中的语言索引*/
private:
    explicit ComConfigClass();
    ~ComConfigClass();

    static ComConfigClass *m_comCfgClass;

public:
    QStringList getCmdModelTypes();
    QList<cmdSettingPro> getSpecificModuleType(int index);
    QMap<QString, QList<QList<proNameAndIco>>> getMacroModel(){return m_macroModel;}
    QStringList getMacList(){return m_lstMac;}
    QStringList getMachineTypes(){return m_machineTypeModel.keys();}
    machineTypePro getMachineTypePro(QString machineType){return m_machineTypeModel[machineType];}
    QMap<QString, QList<motorPro>> getMotoPro(){return m_mapMotorPro;}
    int getCmdModelIndex(const QString & str){return m_mapModuleNumberTOVal[str];}
    QString getModuleVal(const int i){return m_mapModuleNumberTOVal.key(i);}
    QString getCmdIcoPathByName(const int & str){return ":/image/"+m_mapCmdContentToCmdType[str].ico;}
    cmdSettingPro getCmdProByIndex(const int & index){return m_mapCmdContentToCmdType[index];}
    int getCmdTypeByIndex(const int & index){return m_mapModuleNumberTOVal[m_mapCmdContentToCmdType[index].cmdType];}
private:
    QMap<int, QStringList> m_cmdModelType;                      /*命令模块数据保存（多语言）--命令名字，如所有、剪刀、吹气、归零的多语言保存*/
    QMap<int, QList<cmdSettingPro>> m_cmdModel;             /*命令模块数据保存*/
    QMap<QString, QList<QList<proNameAndIco>>> m_macroModel;    /*宏文件数据保存*/
    QStringList m_lstMac;                                       /*保存宏名字*/
    QMap<QString, machineTypePro> m_machineTypeModel;           /*机器类型数据保存*/
    QMap<QString, QList<motorPro>> m_mapMotorPro;               /*密度设置点击属性*/
    QMap<QString, int> m_mapModuleNumberTOVal;                  /*数据格式映射表*/
    QMap<int, cmdSettingPro> m_mapCmdContentToCmdType;      /*命令模块name和属性的映射关系*/
    void parseCfgFile();
    void initMachineDataMapping();
};
/*end*****************************************************************************公共配置解析类******************************************************************************/

/*此类用来处理选中的时候去除虚线框的作用*/
class NoFocusDelegate:public QStyledItemDelegate
{
public:
    NoFocusDelegate();
    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};


/*用来画刻度尺的类*/
class paintScallBar : public QWidget
{
    Q_OBJECT

public:
    explicit paintScallBar(Qt::Orientation direction = Qt::Horizontal, QWidget *parent = 0, int len=0);
    void setPos(int i){m_pos = i;}
private:
    void paintEvent(QPaintEvent *event);
    void drawPos();
    Qt::Orientation   m_direction;
    int m_pos;
    int m_len;
};

/*start*********************************************************选针花型和图形等处理的公共类*******************************************************************************************/
class cMyTableWIdget : public QTableWidget
{
    Q_OBJECT
public:
    explicit cMyTableWIdget(QWidget *parent = 0);

    void setShowZoom(int i);        /*设置显示的比例大小*/

    int  m_scale;                   /*放大缩小的比例系数*/
    bool m_showGrid;
    int  m_row;
    int  m_column;
    int m_xMiniPixel;               /*x的最小像素点*/
    int m_yMiniPixel;               /*y的最小像素点*/
    QList<int> m_lstRowfixed;       /*当图像进行放大缩小的时候固定大小的行*/
    QList<int> m_lstColumfixed;     /*当图像进行放大缩小的时候固定大小的列*/
signals:
    void zoomChanged(int);          /*表格大小发生变化*/
public slots:
    void zoomOut();
    void zoomIn();
    void setGrid();
};
/*end*********************************************************选针花型和图形等处理的公共类*******************************************************************************************/

#endif // COMCONFIGCLASS_H
