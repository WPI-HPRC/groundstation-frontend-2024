//
// Created by William Scheirey on 2/23/24.
//

#include "connectionIndicator.h"
#include "../styles.h"

connectionIndicator::connectionIndicator(QWidget *parent) : hprcGraphicsWidget(parent) {
    indicator = new QGraphicsEllipseItem();
    connectionLabel = new BetterQGraphicsTextItem(parent->geometry(), Qt::AlignVCenter | Qt::AlignLeft,
                                                  "Not Connected");

    graphicsScene->addItem(indicator);
    graphicsScene->addItem(connectionLabel);

    m_widgetType = hprcDisplayWidget::HPRC_serverConnection;

            foreach (QWidget *w, qApp->topLevelWidgets()) {
            if (MainWindow *mainWin = qobject_cast<MainWindow *>(w)) {
                connect(mainWin, SIGNAL(webSocketStateChanged(QAbstractSocket::SocketState)), this,
                        SLOT(socketStateChanged(QAbstractSocket::SocketState)));
            }
        }
}

void connectionIndicator::socketStateChanged(QAbstractSocket::SocketState state) {
    this->socketState = state;
    this->repaint();
}

void HPRCStyle::drawConnectionIndicator(QPainter *p, const connectionIndicator *w, QString label)
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

    switch (w->socketState) {
        case QAbstractSocket::ConnectedState:
            color = Qt::green;
            break;
        case QAbstractSocket::ConnectingState:
            color = Qt::yellow;
            break;
        default:
            color = Qt::red;
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
    w->connectionLabel->setPlainText(label);

    w->graphicsView->viewport()->update();
}
