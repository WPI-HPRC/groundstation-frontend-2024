#include "hprcgraph.h"
#include "../stylepainters.h"

hprcGraph::hprcGraph(QWidget *parent) :
    hprcGraphicsWidget(parent, true)
{

    graphicsView->setObjectName("Graphs");

    this->altSubGraph = new HPRCSubGraph("Alt (m)", graphicsScene);
    this->velSubGraph = new HPRCSubGraph("VEL (m/s)", graphicsScene);
    this->accelSubGraph = new HPRCSubGraph("ACCEL (m/s²)", graphicsScene);

        bgRect = new QGraphicsRectItem();
    outlineRect = new QGraphicsRectItem();

    graphicsScene->addItem(bgRect);
    graphicsScene->addItem(outlineRect);

    setMouseTracking(true);
    m_widgetType = HPRC_Graph;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }
}

void HPRCStyle::drawHPRCGraph(QPainter *p, hprcGraph *w)
{
    int width= w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1-scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = w->layout()->geometry().adjusted(margin, margin, -margin, -margin);

    w->graphicsView->setSceneRect(w->layout()->geometry());

    // label padding = 7.5%
    int lMargin = drawBox.height() * 0.075;
    drawBox.adjust(lMargin, 0, 0, -lMargin);

    QPointF topLeftTop = drawBox.topLeft();
    QPointF bottomRightBottom = drawBox.bottomRight();

    int row2Top = drawBox.y() + drawBox.height()/3;
    int row3Top = drawBox.y() + 2 * drawBox.height()/3;

    QPointF bottomRightTop(drawBox.right(), row2Top);
    QPointF topLeftMiddle(drawBox.left(), row2Top);
    QPointF bottomRightMiddle(drawBox.right(), row3Top);
    QPointF topLeftBottom(drawBox.left(), row3Top);

    QRectF top(topLeftTop, bottomRightTop);
    QRectF middle(topLeftMiddle, bottomRightMiddle);
    QRectF bottom(topLeftBottom, bottomRightBottom);

    double start = graphPointCircularBufferGetValueAtIndex(m_latest->altData, 0)->time;
    double range = graphPointCircularBufferGetValueAtIndex(m_latest->altData, m_latest->altData->length - 1)->time - start;


    bool drawT = false;
    if(drawBox.contains(w->m_mousePos))
    {
        drawT = true;

        w->m_mousePos.setX(fmin(fmax(w->m_mousePos.x(), drawBox.x() + TOOLTIP_WIDTH_HALF + 2), drawBox.right() - TOOLTIP_WIDTH_HALF - 2));
    }

    // <---- draw ----> //

    w->graphicsScene->setBackgroundBrush(m_transparentBrush);

    // Do a little adjusting to help with tooltip rendering
    w->bgRect->setRect(drawBox.adjusted(0, 0, 0, 2));
    w->bgRect->setPen(QPen(m_backgroundBrush, 6));
    w->bgRect->setBrush(m_backgroundBrush);
    w->bgRect->setZValue(-1);

    // Do a little adjusting to help with tooltip rendering
    drawHPRCSubGraph(p, top, m_highlightBrush.color(), m_latest->accData, GRAPH_Acceleration, range, start, w, w->graphicsScene, drawT);
    drawHPRCSubGraph(p, middle.adjusted(0, 1, 0, 1), QColor("#2c4985"), m_latest->velData, GRAPH_Velocity, range, start, w, w->graphicsScene, drawT);
    drawHPRCSubGraph(p, bottom.adjusted(0, 2, 0, 2), QColor("#471d57"), m_latest->altData, GRAPH_Altitude, range, start, w, w->graphicsScene, drawT);

    // Draw an outline to clean up weird border rendering
    w->outlineRect->setRect(drawBox.adjusted(0, 0, 0, 3));
    w->outlineRect->setBrush(m_transparentBrush);
    w->outlineRect->setPen(QPen(m_backgroundBrush, 4));
    w->outlineRect->setZValue(100);

    w->graphicsView->viewport()->update();
}
