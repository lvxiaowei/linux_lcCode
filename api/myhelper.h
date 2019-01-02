#ifndef MYHELPER_H
#define MYHELPER_H

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDialog>
#include <QMessageBox>

//弹出信息框类
class QUIMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit QUIMessageBox(QWidget *parent = 0);
    ~QUIMessageBox();

    static QUIMessageBox *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new QUIMessageBox;
            }
        }

        return self;
    }

protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    static QUIMessageBox *self;

    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout3;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout4;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout4;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIcoMain;
    QSpacerItem *horizontalSpacer1;
    QLabel *labInfo;
    QHBoxLayout *horizontalLayout2;
    QSpacerItem *horizontalSpacer2;
    QPushButton *btnOk;
    QPushButton *btnCancel;

public:
    QString getMessageInfo(){return this->labInfo->text();}
private:
    int closeSec;                   //总显示时间
    int currentSec;                 //当前已显示时间

private slots:
    void initControl();             //初始化控件
    void initForm();                //初始化窗体
    void checkSec();                //校验倒计时

private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

public Q_SLOTS:
    void setIconMain(QChar str, quint32 size = 9);
    void setMessage(const QString &msg, int type, int closeSec = 0);
};

//弹出输入框类
class QUIInputBox : public QDialog
{
    Q_OBJECT

public:
    explicit QUIInputBox(QWidget *parent = 0);
    ~QUIInputBox();

    static QUIInputBox *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new QUIInputBox;
            }
        }

        return self;
    }

protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    static QUIInputBox *self;

    QVBoxLayout *verticalLayout1;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout1;
    QLabel *labIco;
    QLabel *labTitle;
    QLabel *labTime;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout2;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QVBoxLayout *verticalLayout2;
    QFrame *frame;
    QVBoxLayout *verticalLayout3;
    QLabel *labInfo;
    QComboBox *cboxValue;
    QHBoxLayout *lay;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

private:
    int closeSec;                   //总显示时间
    int currentSec;                 //当前已显示时间
    QString value;                  //当前值

private slots:
    void initControl();             //初始化控件
    void initForm();                //初始化窗体
    void checkSec();                //校验倒计时

private slots:
    void on_btnOk_clicked();
    void on_btnMenu_Close_clicked();

public:
    QString getValue()const;
    QLineEdit *txtValue;
public Q_SLOTS:
    void setIconMain(QChar str, quint32 size = 9);
    void setParameter(const QString &title, int type = 0, int closeSec = 0,
                      QString defaultValue = QString(), bool pwd = false);

};

//图形字体处理类
class IconHelper : public QObject
{
    Q_OBJECT

public:
    explicit IconHelper(QObject *parent = 0);
    static IconHelper *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new IconHelper;
            }
        }

        return self;
    }

    void setIcon(QLabel *lab, QChar c, quint32 size = 9);
    void setIcon(QAbstractButton *btn, QChar c, quint32 size = 9);
    QPixmap getPixmap(const QString &color, QChar c, quint32 size = 9,
                      quint32 pixWidth = 10, quint32 pixHeight = 10);

    //根据按钮获取该按钮对应的图标
    QPixmap getPixmap(QToolButton *btn, bool normal);

private:
    static IconHelper *self;    //对象自身
    QFont iconFont;             //图形字体
    QList<QToolButton *> btns;  //按钮队列
    QList<QPixmap> pixNormal;   //正常图片队列
    QList<QPixmap> pixDark;     //加深图片队列
};

class myHelper: public QObject
{

public:
    //全局图标
    static QChar IconMain;          //标题栏左上角图标
    static QChar IconMenu;          //下拉菜单图标
    static QChar IconMin;           //最小化图标
    static QChar IconMax;           //最大化图标
    static QChar IconNormal;        //还原图标
    static QChar IconClose;         //关闭图标
    static QString FontName;        //全局字体名称
    static int FontSize;            //全局字体大小
    //设置编码为UTF8
    static void setTextCode( const char *name="zh_CN.UTF-8") {
        QTextCodec *codec = QTextCodec::codecForName(name);
        QTextCodec::setCodecForLocale(codec);
    }
    //设置字体
    static void setFont(const QString name ="WenQuanYi Micro Hei")
    {
        /*规定程序所用到的字体*/
        QFont font;
        font.setPointSize(15);
        font.setFamily(name);
        font.setBold(false);
        QApplication::setFont(font);
    }

    //设置指定样式
    static void setStyle(const QString &qssFile) {
        QFile file(qssFile);
        if (file.open(QFile::ReadOnly)) {
            QString qss = QLatin1String(file.readAll());
            qApp->setStyleSheet(qss);
            QString PaletteColor = qss.mid(20, 7);
            qApp->setPalette(QPalette(QColor(PaletteColor)));
            file.close();
        }
    }

    //加载中文字符
    static void setChinese() {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(":/image/qt_zh_CN.qm");
        qApp->installTranslator(translator);
    }

    //延时
    static void sleep(int ms) {
        QTime dieTime = QTime::currentTime().addMSecs(ms);
        while ( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    //弹出消息框
    static void showMessageBoxInfo(const QString &info, int closeSec = 0);
    //弹出错误框
    static void showMessageBoxError(const QString &info, int closeSec = 0);
    //弹出询问框
    static void showMessageBoxQuestion(const QString &info);
    static QString getMessageInfo(){return  QUIMessageBox::Instance()->getMessageInfo();}
    static bool isMessageBoxShow(){return QUIMessageBox::Instance()->isVisible();}
    static void messageBoxHide(){QUIMessageBox::Instance()->hide();}
    //弹出输入框
    static void showInputBox(const QString &title, int type = 0, int closeSec = 0,
                                QString defaultValue = QString(), bool pwd = false);
    static bool isInputBoxShow(){return QUIInputBox::Instance()->isVisible();}
    static void inputBoxHIde(){QUIInputBox::Instance()->hide();}
    static QLineEdit* getInputPoint(){return QUIInputBox::Instance()->txtValue;}

};

#endif // MYHELPER_H
