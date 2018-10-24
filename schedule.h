#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include <mainWindow.h>
#include "frm/Frm_chainProcessingClass.h"
#include "frm/Frm_settingMenu.h"
#include "frm/Frm_testingMenu.h"
#include "frm/Frm_patternManage.h"
#include "frm/Frm_timingTable.h"
#include "baseClassWgt.h"

#include <QDebug>
class schedule : public QWidget
{
    Q_OBJECT

public:
    explicit schedule(QWidget *parent = 0);
    ~schedule();

public:
    mainWindow * m_pMainWindow;
    Frm_settingMenu *m_frmSettingMenu;
    Frm_chainProcessingClass* m_frmChain;
    Frm_testingMenu  *m_frmTestMenu;
    Frm_patternManage  *m_frmPatternManage;
    Frm_timingTable*  m_frmTimings;
    QList<QWidget*> lstWgtFrm;
private:
    void lightUpOrOffScreen();

    bool bIsLock;   /*屏幕是否息屏*/
signals:
    void toSerialData(QByteArray data);
    void toXddpData(QByteArray data);
public slots:
    void handleSerialData(QByteArray);
    void handleXddpData(QByteArray);
    void writeToSerial(QByteArray data);
    void writeToXddp(QByteArray data);
};

#endif // SCHEDULE_H
