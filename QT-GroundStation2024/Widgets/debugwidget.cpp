#include "debugwidget.h"
#include "stylepainters.h"

debugWidget::debugWidget(QWidget *parent): hprcGraphicsWidget(parent, false)
{
    qDebug("AYYYYOOOOO");
    textItems = new QMap<QString, BetterQGraphicsTextItem *>();

    this->setGeometry(parent->geometry());
    this->graphicsView->setGeometry(parent->geometry());

    dataNames = {
        "Epoch Time",
        "Timestamp",
        "State",

        "GPS Lock",
        "Satellites",
        "GPS Lat",
        "GPS Long",

        "GPS Alt MSL",
        "GPS Alt AGL",

        "Pressure",
        "Altitude",

        "Accel X",
        "Accel Y",
        "Accel Z",

        "Gyro X",
        "Gyro Y",
        "Gyro Z",

        "Mag X",
        "Mag Y",
        "Mag Z",

        "i",
        "j",
        "k",
        "w",

        "Pos X",
        "Pos Y",
        "Pos Z",

        "Vel X",
        "Vel Y",
        "Vel Z",
    };

    for (QString& name: dataNames)
    {
        textItems->insert(name, new BetterQGraphicsTextItem(this->geometry(), Qt::AlignLeft | Qt::AlignVCenter, "0"));
        textItems->insert(name + " Label", new BetterQGraphicsTextItem(this->geometry(), Qt::AlignRight | Qt::AlignVCenter, name + ": "));

        textItems->value(name + " Label")->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        graphicsScene->addItem(textItems->value(name));
        graphicsScene->addItem(textItems->value(name + " Label"));
    }

    m_widgetType = HPRC_DebugWidget;

    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }
}

void debugWidget::drawData(QString dataName, QString dataString, int xIndex, int yIndex, QRectF drawBox, float itemSpacing, float textItemWidth, float textItemHeight)
{
    textItems->value(dataName)->setPlainText(dataString);
    textItems->value(dataName)->geometry = QRect(drawBox.left() + (xIndex + 1) * (textItemWidth + itemSpacing), drawBox.top() + yIndex * (textItemHeight + itemSpacing), textItemWidth, textItemHeight);
    textItems->value(dataName + " Label")->geometry = QRect(drawBox.left() + xIndex * (textItemWidth + itemSpacing), drawBox.top() + yIndex * (textItemHeight + itemSpacing), textItemWidth, textItemHeight);
    textItems->value(dataName + " Label")->show();
    textItems->value(dataName)->show();
}

void HPRCStyle::drawDebugWidget(QPainter *p, debugWidget *w)
{
    w->graphicsView->setSceneRect(w->layout()->geometry());
    w->graphicsView->viewport()->update();

    m_widgetMedium.setPointSize(w->width() / 70);

    float padding = fminf(w->geometry().width(), w->geometry().height()) * 0.05;

    QRectF drawBox = w->geometry().adjusted(padding, padding, -padding, -padding);

    float textItemWidth = drawBox.width() / 7;
    float textItemHeight = drawBox.height() / 20;
    float itemSpacing = fminf(w->geometry().width(), w->geometry().height()) * 0.02;

    BetterQGraphicsTextItem *textItem;
    for (auto [key, value] : w->textItems->asKeyValueRange())
    {
        value->setDefaultTextColor(m_textBrush.color());
        value->setFont(m_widgetMedium);
        value->hide();
    }

    int yIndex = 0;
    int xIndex = 0;

    w->drawData("Epoch Time", QString::asprintf("%d", m_latest->epochTime), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Timestamp", QString::asprintf("%0.0f", m_latest->rocketTime), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("State", QString::asprintf("%d", m_latest->state), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("GPS Lock", m_latest -> gpsLock ? "YES" : "NO", xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Satellites", QString::asprintf("%d", m_latest->numSatellites), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Lat", QString::asprintf("%0.4f", m_latest->p_gpsLat), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Long", QString::asprintf("%0.4f", m_latest->p_gpsLong), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Alt MSL", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Alt AGL", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Pressure", QString::asprintf("%0.4f", m_latest->pressure), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Altitude", QString::asprintf("%0.4f", m_latest->altitude), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex = 0;
    xIndex = 2;

    w->drawData("Accel X", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Accel Y", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Accel Z", QString::asprintf("%0.4f", m_latest->acceleration), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Gyro X", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Gyro Y", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Gyro Z", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Mag X", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Mag Y", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Mag Z", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex = 0;
    xIndex = 4;
    w->drawData("i", QString::asprintf("%0.4f", m_latest->i), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("j", QString::asprintf("%0.4f", m_latest->j), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("k", QString::asprintf("%0.4f", m_latest->k), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("w", QString::asprintf("%0.4f", m_latest->w), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Pos X", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Pos Y", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Pos Z", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Vel X", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Vel Y", QString::asprintf("NOT IMPLEMENTED"), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Vel Z", QString::asprintf("%0.4f", m_latest->velocity), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);



}
