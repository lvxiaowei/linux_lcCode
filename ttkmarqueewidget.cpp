#include "ttkmarqueewidget.h"

#include <QPainter>

void TTKMarqueeWidget::setText(const QString &newText)
{
    m_myText = newText;
}

void TTKMarqueeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    QFont f(font());
    f.setBold(true);
    painter.setFont(f);

    int textWidth = fontMetrics().width(m_myText);
    if(textWidth < 1)
    {
        return;
    }

    static int i=0;
    i=i%width()+2;

    painter.drawText(i, 0, textWidth, height(),
                     Qt::AlignLeft | Qt::AlignVCenter, m_myText);

    if(i+textWidth>width())
    {
        painter.drawText(-(width()-i), 0, textWidth, height(),
                         Qt::AlignLeft | Qt::AlignVCenter, m_myText);
    }
    painter.end();
}


