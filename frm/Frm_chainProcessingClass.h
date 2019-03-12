#ifndef FRM_CHAINPROCESSINGCLASS_H
#define FRM_CHAINPROCESSINGCLASS_H

#include <baseClassWgt.h>
#include <QTreeWidgetItem>
#include <QFileSystemModel>
//#include "api/myMessageBox.h"

namespace Ui {
class Frm_chainProcessingClass;
}

class Frm_chainProcessingClass : public baseClassWgt
{
    Q_OBJECT

public:
    explicit Frm_chainProcessingClass(QWidget *parent = 0);
    ~Frm_chainProcessingClass();

    void keyPressEvent(int key);
    void initShowFrmConfig();
public slots:
    void freshCmdContent();
private:
    void writeToXddp();
    void initChainTree();
    void writeChainTree();
    void initChainManageTable();
    void secendLevelNodeSort();
    QStringList getPatternList();
    QStringList getCmdAtt(QTreeWidgetItem *item);
    void initCmdEdit();
    void freshCmdFormData(int index);
    bool isFileExist(QString fileFullName);
    int getItemType(QTreeWidgetItem *item);
    void dealPg1(int key);
    void dealPg2(int key);
    void dealPg2_1(int key);
    void dealPg2_2(int key);
    void dealAddCmd(int key);
    QStringList m_lstFirstNodeName;
    int m_iCmdPagePos;    //用来记录新增命令窗口页左右键操作的时候当前处于那一列控件
    bool bIsUExit; //U盘是否插入
    QTreeWidgetItem* m_cpItem;
    QFileSystemModel *m_treeFileModel;
    QList<QWidget*> m_lstTabWgt;  //用来临时保存命令行配置界面切换的几个widget
    Ui::Frm_chainProcessingClass *ui;
    QString m_sourceFile,m_destFile,m_delFile; //进行文件拷贝时候的源文件和目的文件
};

#endif // FRM_CHAINPROCESSINGCLASS_H
