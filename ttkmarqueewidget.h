#ifndef TTKMARQUEEWIDGET_H
#define TTKMARQUEEWIDGET_H

#include <QWidget>

class  TTKMarqueeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TTKMarqueeWidget(QWidget *parent = 0):QWidget(parent){}

    void setText(const QString &newText);
    inline QString text() const { return m_myText.trimmed(); }


protected:
    virtual void paintEvent(QPaintEvent *event) override;


private:
    QString m_myText;
};

#endif // TTKMARQUEEWIDGET_H
