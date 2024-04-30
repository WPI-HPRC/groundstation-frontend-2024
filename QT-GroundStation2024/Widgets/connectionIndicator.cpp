//
// Created by William Scheirey on 2/23/24.
//

#include "ConnectionIndicator.h"
#include "../styles.h"

ConnectionIndicator::ConnectionIndicator(QWidget *parent) : hprcGraphicsWidget(parent) {
    indicator = new QGraphicsEllipseItem();
    connectionLabel = new BetterQGraphicsTextItem(parent->geometry(), Qt::AlignVCenter | Qt::AlignLeft,
                                                  "");
    bg = new QGraphicsRectItem();
    bg->setPen(QPen(Qt::darkGreen));
    bg->setBrush(QBrush(Qt::darkGreen));

//    graphicsScene->addItem(bg);
    graphicsScene->addItem(indicator);
    graphicsScene->addItem(connectionLabel);

    m_widgetType = HPRC_connectionIndicator;
}

void HPRCStyle::drawConnectionIndicator(QPainter *p, ConnectionIndicator *w)
{
    w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    int width = w->layout()->sizeHint().width();
    int height = w->layout()->sizeHint().height();

    double scaleF = 1;
    double paddingRatio = (1 - scaleF) / 2.0;

    int margin = paddingRatio * width;

    QRectF drawBox = QRectF(w->layout()->geometry().topLeft(), QSizeF(w->layout()->geometry().width(), w->layout()->sizeHint().height())).adjusted(width/4, 0, 0, -height/1.5);
    w->setFixedHeight(drawBox.height());

    w->graphicsView->setGeometry(drawBox.toRect());
    w->graphicsView->setSceneRect(drawBox);

    w->bg->setRect(w->graphicsView->sceneRect());

    float indicatorSize = drawBox.height() / 2.5;

    m_widgetMedium.setPointSize(drawBox.width() / 10);

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
    w->indicator->setRect(drawBox.x() + 2, drawBox.y() + (drawBox.height() - indicatorSize) / 2,
                          indicatorSize, indicatorSize);

    w->connectionLabel->setDefaultTextColor(m_textBrush.color());
    w->connectionLabel->geometry = drawBox.toRect().adjusted(indicatorSize + 15, 2, 0, 0);
    w->connectionLabel->setFont(m_widgetMedium);
    w->connectionLabel->setPlainText(w->label);

    w->graphicsView->viewport()->update();
}
