#ifndef FRM_TESTINGMENU_H
#define FRM_TESTINGMENU_H

#include "baseClassWgt.h"
#include "api/myMessageBox.h"
#include <QRadioButton>

enum TEST_TYPE{
    AIRVALVETEST = 0,
    SELECTIONTEST,
    MOTOTEST,
    YFTEST
};

namespace Ui {
class Frm_testingMenu;
}

class Frm_testingMenu : public baseClassWgt
{
    Q_OBJECT

public:
    explicit Frm_testingMenu(QWidget *parent = 0);
    ~Frm_testingMenu();

    void keyPressEvent(int key);
    void initShowFrmConfig();
    void handleXddpData(QByteArray data);
public slots:
    void focusAirValveChanged();
private:
    void dataInit();
    void dealPg1(int key);
    void dealPg2(int key);
    void dealPg3(int key);
    void dealPg4(int key);
    void dealPg5(int key);
    bool readAirValveConfig();
    void initAirValve();
    void initAirValvePage();
    void initMotoPage();
    void initActuatorPage();
    void initYFPage();


    void writeToXddp(const int testType, QString operMode="");  /*向XDDP发送数据*/

    QList<QLineEdit*>    m_lstMotoVal;
    QList<QRadioButton*> m_lstMotoRadio;
    QStringList m_lstAirValve;              //配置文件的气阀名称；
    int m_iPos;     //当前处于哪个马达测试的标志位
    bool m_bIsTesting;
    QString m_strTestMode;
    Ui::Frm_testingMenu *ui;
};

#endif // FRM_TESTINGMENU_H
