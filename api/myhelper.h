#ifndef MYHELPER_H
#define MYHELPER_H

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QApplication>

class myHelper: public QObject
{

public:
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

};

#endif // MYHELPER_H
