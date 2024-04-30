//
// Created by William Scheirey on 2/23/24.
//

#include "ConnectionIndicator.h"
#include "../styles.h"

ConnectionIndicator::ConnectionIndicator(QWidget *parent) : hprcGraphicsWidget(parent) {
    indicator = new QGraphicsEllipseItem();
    connectionLabel = new BetterQGraphicsTextItem(parent->geometry(), Qt::AlignVCenter | Qt::AlignLeft,
                                                  "");

    graphicsScene->addItem(indicator);
    graphicsScene->addItem(connectionLabel);
}

void HPRCStyle::drawConnectionIndicator(QPainter *p, const ConnectionIndicator *w)
{
    int width = w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1 - scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = w->layout()->geometry().adjusted(margin, margin, -margin, -margin);

    float indicatorSize = drawBox.width() / 15;

    m_widgetMedium.setPointSize(drawBox.height() * 1 / 4);

    w->graphicsView->setSceneRect(w->layout()->geometry());

    QColor color;

    switch (w->state) {
        case ConnectionIndicator::GREEN:
            color = Qt::green;
            break;
        case ConnectionIndicator::YELLOW:
            color = Qt::yellow;
            break;
        case ConnectionIndicator::RED:
            color = Qt::red;
            break;
        default:
            // Should never happen
            color = Qt::white;
            break;
    }

    w->indicator->setBrush(QBrush(color));
    w->indicator->setPen(QPen(color));
    w->indicator->setRect(drawBox.x(), round(drawBox.y() + drawBox.height() * 1 / 8 - indicatorSize / 2 - 2),
                          indicatorSize, indicatorSize);

    w->connectionLabel->setDefaultTextColor(m_textBrush.color());
    w->connectionLabel->geometry = QRect(drawBox.x() + indicatorSize * 2, drawBox.y(), drawBox.width(),
                                         drawBox.height() * 1 / 4);
    w->connectionLabel->setFont(m_widgetMedium);
    w->connectionLabel->setPlainText(w->label);

    w->graphicsView->viewport()->update();
}
