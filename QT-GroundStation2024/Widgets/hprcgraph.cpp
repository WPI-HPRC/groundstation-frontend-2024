#include "hprcgraph.h"
#include "stylepainters.h"
#include "hprcPayloadGraph.h"

hprcGraph::hprcGraph(QWidget *parent) :
    hprcGraphicsWidget(parent, true)
{
    detailedViewEnabled = true;

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

void HPRCStyle::drawHPRCGraph(QPainter* p, hprcGraph* w)
{
    int width = w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1 - scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = w->layout()->geometry().adjusted(margin, margin, -margin, -margin);

    w->graphicsView->setSceneRect(w->layout()->geometry());

    // label padding = 7.5%
    int lMargin = drawBox.height() * 0.075;
    drawBox.adjust(lMargin, 0, 0, -lMargin);

    QPointF topLeftTop = drawBox.topLeft();
    QPointF bottomRightBottom = drawBox.bottomRight();

    int row2Top = drawBox.y() + drawBox.height() / 3;
    int row3Top = drawBox.y() + 2 * drawBox.height() / 3;

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
    if (drawBox.contains(w->m_mousePos))
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

HPRCStyle::Range
HPRCStyle::drawHPRCSubGraph(QPainter* p, QRectF rect, QColor bg, QList<MainWindow::graphPoint>* data, double range,
    double start, const hprcDisplayWidget* w, bool drawTooltip, double scaleMin,
    double scaleMax, bool enableEndZeroPoints, bool enablePolygonTransformationRendering,
    int* startIndex, QPolygonF* polygon) {
    QBrush lightHighlighterBrush(QColor(255, 255, 255, 64));

    QPointF bottomPt = rect.bottomLeft();
    //    bottomPt.setY(rect.bottom() + rect.height()/3);
    QLinearGradient gradient(bottomPt, rect.topLeft());
    //    gradient.setColorAt(0, m_transparentBrush.color());
    //    gradient.setColorAt(1, bg);
    gradient.setColorAt(0, bg);
    gradient.setColorAt(1, m_transparentBrush.color());

    QList<QPointF> pointsToDraw;
    double max = 9999;

    double scale = fmax(1.0, scaleMax - scaleMin);

    QRectF ptHighlight(-100, 0, 0, 0);
    QPointF highlighted(-100, 0);
    QString ptLabel("");

    //Draw zero line
    float zeroY = -((0 - scaleMin) / scale) * (rect.height() * 0.97) + rect.bottom();
    p->setPen(QPen(QBrush(QColor(128, 128, 128)), 2));
    p->drawLine(QLineF(rect.left(), zeroY, rect.right(), zeroY));

    //Draw min / max label text
    p->setPen(QPen(m_textBrush, 3));
    int fontSize = rect.height() / 12;
    m_widgetLarge.setPointSize(fontSize);
    p->setFont(m_widgetLarge);
    QString minString = QString::number(scaleMin, 'f', 2);
    //Make sure there are no trailing zeros
    while (minString.length() > 0 && minString.back() == '0') {
        minString.remove(minString.length() - 1, 1);
    }
    if (minString.back() == '.') {
        minString.remove(minString.length() - 1, 1);
    }
    QString maxString = QString::number(scaleMax, 'f', 2);
    //Make sure there are no trailing zeros
    while (maxString.length() > 0 && maxString.back() == '0') {
        maxString.remove(maxString.length() - 1, 1);
    }
    if (maxString.back() == '.') {
        maxString.remove(maxString.length() - 1, 1);
        p->drawText(QRect(rect.right() - 105, rect.bottom() - fontSize, 100, 100), Qt::AlignRight, minString);
        p->drawText(QRect(rect.right() - 105, rect.top() - fontSize, 100, 100), Qt::AlignRight, maxString);
    }

    //    p->setPen(QPen(m_transparentBrush, 3));
    p->setPen(QPen(bg, 3));

    //New things start here (polygon scaling idea)
    if (enablePolygonTransformationRendering) {
        //Remove points that are off of the graph
        while (0 < polygon->size() && (*polygon)[0].x() < start) {
            polygon->removeFirst();
        }
        while (0 < data->size() && (*data)[0].time < start) {
            data->removeFirst(); //This is removing data from the data array. This has a possiblity to affect something else if it is also using this data (but for now nothing else should be using the data)
        }

        for (int i = *startIndex; i < data->size(); i++) {
            MainWindow::graphPoint* g = &(*data)[i];

            double valX = 0;
            double valY = 0;
            if (valY < max)
                max = valY;
            valX = g->time;
            valY = g->value;

            //NOTE: SOMETHING NEW WILL HAVE TO BE put in for the tooltip now

            (*polygon) << QPointF(valX, valY);
        }
        (*startIndex) = data->size();

        //Set transformations for the polygon. Transformations need to be done seperately so they can build upon one another.
        QPolygonF transformedPolygon;
        ((hprcPayloadGraph*)w)->transform.reset();
        ((hprcPayloadGraph*)w)->transform.translate(-start, -scaleMin);
        transformedPolygon = ((hprcPayloadGraph*)w)->transform.map(*polygon);
        ((hprcPayloadGraph*)w)->transform.reset();
        ((hprcPayloadGraph*)w)->transform.scale(-rect.width() / range, -(rect.height() * 0.97) / scale);
        transformedPolygon = ((hprcPayloadGraph*)w)->transform.map(transformedPolygon);
        ((hprcPayloadGraph*)w)->transform.reset();
        ((hprcPayloadGraph*)w)->transform.translate(rect.right(), rect.bottom());
        transformedPolygon = ((hprcPayloadGraph*)w)->transform.map(transformedPolygon);

        gradient.setFinalStop(rect.topLeft());
        if (enableEndZeroPoints) {
            transformedPolygon << rect.bottomLeft();
            transformedPolygon << rect.bottomRight();
        }
        else if (data->length() > 0) {
            transformedPolygon
                << QPointF(-((*data)[data->length() - 1].time - start) / (range) * (rect.width()) + rect.right(),
                    rect.bottomRight().y());
            transformedPolygon << QPointF(-((*data)[0].time - start) / (range) * (rect.width()) + rect.right(),
                rect.bottomLeft().y());
        }

        p->setBrush(QBrush(gradient));
        //    p->setBrush(m_transparentBrush);
        p->drawPolygon(transformedPolygon);
        p->setPen(QPen(m_highlightBrush, 3));
        p->setBrush(lightHighlighterBrush);
    }
    else {
        //Will render all data points in the array normally if length of data is less than max points, otherwise will render max number of points evenly districuted throughout array
        float dataInterval = std::max((float)data->length() / hprcPayloadGraph::MAX_RENDERED_POINTS, (float)1);
        for (int i = 0; i <
            (data->length() > hprcPayloadGraph::MAX_RENDERED_POINTS ? hprcPayloadGraph::MAX_RENDERED_POINTS
                : data->length()); i++) {
            MainWindow::graphPoint* g = &(*data)[floor(i * dataInterval)];

            //Skip drawing data points until they are within the range of the graph
            if (g->time < start) {
                continue;
            }

            double valX = 0;
            double valY = 0;
            if (valY < max)
                max = valY;
            valX = -(g->time - start) / (range) * (rect.width()) + rect.right();
            valY = -((g->value - scaleMin) / scale) * (rect.height() * 0.97) + rect.bottom();

            if (valX - w->m_mousePos.x() > (rect.width() / 2.0 / data->size()) &&
                valX - w->m_mousePos.x() < -(rect.width() / 2.0 / data->size())) {
                ptHighlight = QRectF(valX - 25, rect.top(), 50, rect.height());
                ptLabel = QString::number(g->value);
                highlighted = QPointF(valX, valY);
            }
            else {

            }

            pointsToDraw.append(QPointF(valX, valY));
        }

        gradient.setFinalStop(rect.topLeft());
        if (enableEndZeroPoints) {
            pointsToDraw.append(rect.bottomLeft());
            pointsToDraw.append(rect.bottomRight());
        }
        else if (data->length() > 0) {
            pointsToDraw.append(
                QPointF(-((*data)[data->length() - 1].time - start) / (range) * (rect.width()) + rect.right(),
                    rect.bottomRight().y()));
            pointsToDraw.append(QPointF(-((*data)[0].time - start) / (range) * (rect.width()) + rect.right(),
                rect.bottomLeft().y()));
        }

        p->setBrush(QBrush(gradient));
        //    p->setBrush(m_transparentBrush);
        p->drawPolygon(pointsToDraw);
        p->setPen(QPen(m_highlightBrush, 3));
        p->setBrush(lightHighlighterBrush);

        if (drawTooltip) {
            p->drawRect(ptHighlight);
            p->drawLine(highlighted.x(), rect.top(), highlighted.x(), rect.bottom());

            p->setBrush(m_highlightBrush);
            p->drawEllipse(highlighted, 5, 5);

            p->setPen(QPen(m_textBrush, 3));
            p->setFont(m_widgetMedium);
            p->drawText(ptHighlight, ptLabel);
            p->setPen(QPen(m_highlightBrush, 3));
        }
    }

    return Range{ scaleMin, scaleMax };
}