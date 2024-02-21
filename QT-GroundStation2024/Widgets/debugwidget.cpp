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

        "Accel X"
        "Accel Y"
        "Accel Z"

        "Gyro X"
        "Gyro Y"
        "Gyro Z"

        "Mag X"
        "Mag Y"
        "Mag Z"

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
        textItems->insert(name + " Label", new BetterQGraphicsTextItem(this->geometry(), Qt::AlignLeft | Qt::AlignVCenter, name + ": "));

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

void debugWidget::updateSize()
{
//    setGeometry(parent->geometry());
//    this->setGeometry(parent->geometry());
}

void HPRCStyle::drawDebugWidget(QPainter *p, debugWidget *w)
{
    w->graphicsView->setSceneRect(w->layout()->geometry());
    w->graphicsView->viewport()->update();

    m_widgetMedium.setPointSize(w->width() / 40);

    float padding = fminf(w->geometry().width(), w->geometry().height()) * 0.05;

    QRectF drawBox = w->geometry().adjusted(padding, padding, -padding, -padding);

    float textItemWidth = drawBox.width() / 5;
    float textItemHeight = drawBox.height() / 10;
    float itemSpacing = fminf(w->geometry().width(), w->geometry().height()) * 0.02;

    for(BetterQGraphicsTextItem *textItem : w->textItems->values())
    {
        textItem->setDefaultTextColor(m_textBrush.color());
        textItem->setFont(m_widgetMedium);
        textItem->hide();
    }


    w->textItems->value("Epoch Time")->setPlainText(QString::asprintf("%d", m_latest->epochTime));
    w->textItems->value("Epoch Time")->geometry = QRect(drawBox.left() + textItemWidth + itemSpacing, drawBox.top(), textItemWidth, textItemHeight);
    w->textItems->value("Epoch Time Label")->geometry = QRect(drawBox.left(), drawBox.top(), textItemWidth, textItemHeight);
    w->textItems->value("Epoch Time Label")->show();
    w->textItems->value("Epoch Time")->show();

    w->textItems->value("Timestamp")->setPlainText(QString::asprintf("%0.0f", m_latest->rocketTime));
    w->textItems->value("Timestamp")->geometry = QRect(drawBox.left() + textItemWidth + itemSpacing, drawBox.top() + textItemHeight + itemSpacing, textItemWidth, textItemHeight);
    w->textItems->value("Timestamp Label")->geometry = QRect(drawBox.left(), drawBox.top() + textItemHeight + itemSpacing, textItemWidth, textItemHeight);
    w->textItems->value("Timestamp")->show();
    w->textItems->value("Timestamp Label")->show();

    w->textItems->value("GPS Lock")->setPlainText(m_latest -> gpsLock ? "YES" : "NO");
    w->textItems->value("GPS Lock")->geometry = QRect(drawBox.left() + 3 * (textItemWidth + itemSpacing), drawBox.top() + 0 * (textItemHeight + itemSpacing), textItemWidth, textItemHeight);
    w->textItems->value("GPS Lock Label")->geometry = QRect(drawBox.left() + 2 * (textItemWidth + itemSpacing), drawBox.top() + 0 * (textItemHeight + itemSpacing), textItemWidth, textItemHeight);
    w->textItems->value("GPS Lock")->show();
    w->textItems->value("GPS Lock Label")->show();


}
