#ifndef FRM_CHAINPROCESSINGCLASS_H
#define FRM_CHAINPROCESSINGCLASS_H

#include <baseClassWgt.h>
#include <QTreeWidgetItem>
#include "api/myMessageBox.h"

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
    void initChainTree();
    void secendLevelNodeSort();
    void initCmdEdit();
    void freshCmdFormData(int index);
    void dealPg1(int key);
    void dealPg2(int key);
    void dealPg3(int key);
    int m_iCmdPagePos;    //用来记录新增命令窗口页左右键操作的时候当前处于那一列控件
    QTreeWidgetItem* m_cpItem;
    QRegExp rx1, rx2, rx3, rx4_1, rx4_2, rx5, rx6, rx7, rx8, rx9, rx10;

    Ui::Frm_chainProcessingClass *ui;
};

#endif // FRM_CHAINPROCESSINGCLASS_H
