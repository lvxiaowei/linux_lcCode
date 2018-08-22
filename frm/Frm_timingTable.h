#ifndef FRM_TIMINGTABLE_H
#define FRM_TIMINGTABLE_H

#include "baseClassWgt.h"

namespace Ui {
class Frm_timingTable;
}

class Frm_timingTable : public baseClassWgt
{
    Q_OBJECT

public:
    explicit Frm_timingTable(QWidget *parent = 0);
    ~Frm_timingTable();

    void keyPressEvent(int key);
    void initShowFrmConfig();
    void handleXddpData(QByteArray data);
private:
    void initTimingsTable();
    void saveConfigFile();
    QTableWidget *m_curentTable;

    Ui::Frm_timingTable *ui;
};

#endif // FRM_TIMINGTABLE_H
