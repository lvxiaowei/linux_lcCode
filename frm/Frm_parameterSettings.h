#ifndef FRM_PARAMETERSETTINGS_H
#define FRM_PARAMETERSETTINGS_H

#include "baseClassWgt.h"
#include "ui_Frm_parameterSettingsPop.h"

namespace Ui {
class Frm_parameterSettings;
}

class Frm_parameterSettings : public baseClassWgt
{
    Q_OBJECT

public:
    explicit Frm_parameterSettings(QWidget *parent = 0);
    ~Frm_parameterSettings();

    void keyPressEvent(int key);
    void initShowFrmConfig();
public slots:
    void writeToXddp();
    void timerUpDate();
private:
    Ui::Frm_parameterSettings *ui;
    Ui::Frm_parameterSettingsPop ui_pop;    /*弹出界面对应的窗口*/
    QDialog *w;                             /*弹出界面对应的窗口*/

    QTimer *m_timer;

    void initSystemTime();

    void dealPressKeyEvent();
    void keyPressEventPopSet(int key);
    void keyPressEventPopSet_paswd(int key);
    void keyPressEventPopSet_zeroingSet(int key);
    void keyPressEventPopSet_systimeSet(int key);
};

#endif // FRM_PARAMETERSETTINGS_H
