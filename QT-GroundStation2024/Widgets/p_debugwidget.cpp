#include "p_debugwidget.h"
#include "stylepainters.h"

p_debugWidget::p_debugWidget(QWidget *parent): hprcGraphicsWidget(parent, false)
{
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

    m_widgetType = HPRC_p_DebugWidget;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(500);
}

void p_debugWidget::drawData(QString dataName, QString dataString, int xIndex, int yIndex, QRectF drawBox, float itemSpacing, float textItemWidth, float textItemHeight)
{
    textItems->value(dataName)->setPlainText(dataString);
    textItems->value(dataName)->geometry = QRect(drawBox.left() + (xIndex + 1) * (textItemWidth + itemSpacing), drawBox.top() + yIndex * (textItemHeight + itemSpacing), textItemWidth, textItemHeight);
    textItems->value(dataName + " Label")->geometry = QRect(drawBox.left() + xIndex * (textItemWidth + itemSpacing), drawBox.top() + yIndex * (textItemHeight + itemSpacing), textItemWidth, textItemHeight);
    textItems->value(dataName + " Label")->show();
    textItems->value(dataName)->show();
}

void HPRCStyle::drawp_DebugWidget(QPainter *p, p_debugWidget *w)
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

    w->drawData("Epoch Time", QString::asprintf("%d", m_latest->p_epochTime), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Timestamp", QString::asprintf("%0.0f", m_latest->p_rocketTime), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("State", QString::asprintf("%d", m_latest->p_state), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("GPS Lock", m_latest -> p_gpsLock ? "YES" : "NO", xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Satellites", QString::asprintf("%d", m_latest->p_numSatellites), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Lat", QString::asprintf("%0.4f", m_latest->p_gpsLat), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Long", QString::asprintf("%0.4f", m_latest->p_gpsLong), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Alt MSL", QString::asprintf("%0.4f", m_latest->p_gpsAltMSL), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("GPS Alt AGL", QString::asprintf("%0.4f", m_latest->p_gpsAltAGL), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Pressure", QString::asprintf("%0.4f", m_latest->p_pressure), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Altitude", QString::asprintf("%0.4f", m_latest->p_altitude), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex = 0;
    xIndex = 2;

    w->drawData("Accel X", QString::asprintf("%0.4f", m_latest->p_accelX), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Accel Y", QString::asprintf("%0.4f", m_latest->p_accelY), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Accel Z", QString::asprintf("%0.4f", m_latest->p_accelZ), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Gyro X", QString::asprintf("%0.4f", m_latest->p_gyroX), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Gyro Y", QString::asprintf("%0.4f", m_latest->p_gyroY), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Gyro Z", QString::asprintf("%0.4f", m_latest->p_gyroZ), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Mag X", QString::asprintf("%0.4f", m_latest->p_magX), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Mag Y", QString::asprintf("%0.4f", m_latest->p_magY), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Mag Z", QString::asprintf("%0.4f", m_latest->p_magZ), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex = 0;
    xIndex = 4;
    w->drawData("i", QString::asprintf("%0.4f", m_latest->p_i), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("j", QString::asprintf("%0.4f", m_latest->p_j), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("k", QString::asprintf("%0.4f", m_latest->p_k), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("w", QString::asprintf("%0.4f", m_latest->p_w), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Pos X", QString::asprintf("%0.4f", m_latest->p_posX), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Pos Y", QString::asprintf("%0.4f", m_latest->p_posY), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Pos Z", QString::asprintf("%0.4f", m_latest->p_posZ), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);

    yIndex++;

    w->drawData("Vel X", QString::asprintf("%0.4f", m_latest->p_velX), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Vel Y", QString::asprintf("%0.4f", m_latest->p_velY), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
    w->drawData("Vel Z", QString::asprintf("%0.4f", m_latest->p_velZ), xIndex, yIndex++, drawBox, itemSpacing, textItemWidth, textItemHeight);
}
