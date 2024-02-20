#include "hprcgpslockindicator.h"
#include "stylepainters.h"

hprcGpsLockIndicator::hprcGpsLockIndicator(QWidget *parent): hprcGraphicsWidget(parent, false)
{
    m_widgetType = HPRC_GpsLock;

    lockIndicator = new QGraphicsEllipseItem();
    lockIndicator->setRect(0, 0, 10, 10);
    lockIndicator->setBrush(QBrush(Qt::green));

    graphicsScene->addItem(lockIndicator);

//    out

    foreach (QWidget *w, qApp->topLevelWidgets())
    {
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            qDebug("Connecting!");
            connect(mainWin, SIGNAL(gpsLockUpdated()), this, SLOT(repaint()));
            qDebug("Connected!");
        }
    }
}

void HPRCStyle::drawHprcGpsLockIndicator(QPainter *p, const hprcGpsLockIndicator *w)
{
    w->graphicsView->setSceneRect(w->layout()->geometry());

    w->lockIndicator->setBrush(QBrush(m_latest->gpsLock ? Qt::green : Qt::red));

    w->graphicsView->viewport()->update();
}

