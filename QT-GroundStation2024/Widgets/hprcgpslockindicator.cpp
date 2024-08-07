#include "hprcgpslockindicator.h"
#include "stylepainters.h"

hprcGpsLockIndicator::hprcGpsLockIndicator(QWidget *parent): hprcGraphicsWidget(parent, false)
{
    m_widgetType = HPRC_GpsLock;

    lockIndicator = new QGraphicsEllipseItem();
    lockIndicator->setRect(0, 0, 10, 10);
    lockIndicator->setBrush(QBrush(Qt::green));
    lockIndicator->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    lockLabel = new BetterQGraphicsTextItem(this->geometry(), Qt::AlignVCenter, "GPS Lock");
    lockLabel->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    satellitesLabel = new BetterQGraphicsTextItem(this->geometry(), Qt::AlignVCenter, "No Satellites");
    satellitesLabel->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    graphicsScene->addItem(lockIndicator);
    graphicsScene->addItem(lockLabel);
    graphicsScene->addItem(satellitesLabel);

    foreach (QWidget *w, qApp->topLevelWidgets())
    {
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(gpsLockUpdated()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(numSatellitesUpdated()), this, SLOT(repaint()));
        }
    }
}

void HPRCStyle::drawHprcGpsLockIndicator(QPainter *p, const hprcGpsLockIndicator *w)
{
    int width= w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1-scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = w->layout()->geometry().adjusted(margin, margin, -margin, -margin);

    float indicatorSize = drawBox.width() / 15;

    m_widgetMedium.setPointSize(drawBox.height() * 1/4);

    w->graphicsView->setSceneRect(w->layout()->geometry());

    w->lockIndicator->setBrush(QBrush(m_latest->gpsLock ? Qt::green : Qt::red));
    w->lockIndicator->setPen(QPen(m_latest->gpsLock ? Qt::green : Qt::red, 1));
    w->lockIndicator->setRect(drawBox.x(), round(drawBox.y() + drawBox.height() * 1/8 - indicatorSize/2 - 2), indicatorSize, indicatorSize);

    w->lockLabel->setDefaultTextColor(m_textBrush.color());
    w->lockLabel->geometry = QRect(drawBox.x() + indicatorSize * 2, drawBox.y(), drawBox.width(), drawBox.height() * 1/4);
    w->lockLabel->setFont(m_widgetMedium);

    w->satellitesLabel->setDefaultTextColor(m_textBrush.color());
    w->satellitesLabel->geometry = QRect(drawBox.x() + indicatorSize * 2, drawBox.y() + drawBox.height() * 1/3, drawBox.width(), drawBox.height() * 1/4);
    w->satellitesLabel->setFont(m_widgetMedium);
    w->satellitesLabel->setPlainText(QString::asprintf("%d satellite%s", m_latest->numSatellites, m_latest->numSatellites == 1 ? "" : "s"));


    w->graphicsView->viewport()->update();
}

