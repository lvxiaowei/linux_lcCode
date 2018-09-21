#ifndef FRM_PATTERNMANAGE_H
#define FRM_PATTERNMANAGE_H

#include <QRadioButton>
#include <QSpinBox>
#include "baseClassWgt.h"
#include "api/myMessageBox.h"

enum PAT_SET{
    PAT_SET_PAT1 = 1,
    PAT_SET_PAT2,
    PAT_SET_PAT3
};

/*用来画patternTableWgt中的图案的的类*/
class paletteBoard : public QWidget
{
    Q_OBJECT

public:
    explicit paletteBoard(const QImage &image, int scale = 10);
    void setScale(const int i){m_scale = i; update();}
    void setShowGrid(const bool i){m_showGrid = i; update();}

protected:
    void paintEvent(QPaintEvent *event);

private:
    QRect pixelRect(int i, int j) const;

    QImage m_image;
    int    m_scale;
    bool   m_showGrid;
};

/*用来画图案的类--包含刻度尺，和图案的显示*/
class patternTableWgt : public cMyTableWIdget
{
    Q_OBJECT

public:
    explicit patternTableWgt(QWidget *parent = 0, int colum = 168, QString fileType="", int scale = 5, QByteArray bt=0);

    void freshPattern(QString fileType, QByteArray &bt);
    void initPatternTab();
    void initDisFilePattern(QByteArray& bt);
    QMap<int, QColor> getSelectedColor(){return m_mapSelectedColor;}

    QImage            m_image;
private:
    paintScallBar*    m_scallBarHor;
    paintScallBar*    m_scallBarVer;
    QList<QColor>     m_lstColor;
    QMap<int, QColor> m_mapSelectedColor;    /*图案中使用到的颜色*/
    QString           m_fileType;            /*文件类型*/
    paletteBoard *    m_palettBoard;
public slots:
    void drawPos(int i);
    void zoomOut();
    void zoomIn();
};

/*用来画选针和设置重复的类*/
class YFTableWgt : public cMyTableWIdget
{
    Q_OBJECT

public:
    explicit YFTableWgt(QMap<QString, int> m_mapYF, QWidget *parent = 0, int scale = 5, int row = 200, QByteArray bt=0);

    bool setYF(int column, int star, int end, bool set);
};

/*用来画刷子和重复选择表格中间的空白*/
class spacingBar : public QWidget
{
    Q_OBJECT

public:
    explicit spacingBar( QWidget *parent = 0){Q_UNUSED(parent);}
private:
    void paintEvent(QPaintEvent *event);
};

namespace Ui {
class Frm_patternManage;
}

class Frm_patternManage : public baseClassWgt
{
    Q_OBJECT

public:
    explicit Frm_patternManage(QWidget *parent = 0);
    ~Frm_patternManage();

    void keyPressEvent(int key);
    void initShowFrmConfig();
signals:

private:
    void dealPg1(int key);
    void dealPg2(int key);
    void dealPg2_menu(int key);
    void dealPg2_YFSet(int key);
    void dealPg2_loop(int key);
    void dealPg3(int key);
    void initPatternProcesPage();
    void initPatManageTabl();
    void initTimingsTable();
    void saveTimingsDataToConfigFile();
    patternTableWgt *  m_pattrenTable;
    YFTableWgt *       m_YFTable;
    int m_iScale;    //图案放大系数
    QStringList m_lstYF;
    Ui::Frm_patternManage *ui;
};
#endif // FRM_PATTERNMANAGE_H
