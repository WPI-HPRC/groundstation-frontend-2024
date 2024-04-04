#include "hprcPayloadMap.h"

#include <QVBoxLayout>
#include <QTimer>

#include "stylepainters.h"

void JsInterface::payloadPoint(double lat, double lng) {
    emit updatePayloadPoint(lat, lng);
}

void JsInterface::log(const QString& str) {
    qDebug() << "Log from JS: " << str;
}

void JsInterface::targetPoint(double lat, double lng) {
    emit updateTargetPoint(lat, lng);
}

hprcPayloadMap::hprcPayloadMap(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_PayloadMap;

    // Create a web engine view, and display an offline leaflet map webpage inside it
    m_view = new QWebEngineView(this);
    m_view->load(QUrl("qrc:/Maps/index.html"));
    m_channel = new QWebChannel(m_view->page());
    m_view->page()->setWebChannel(m_channel);

    // Create an interface for sending information to the leaflet map
    m_interface = new JsInterface();
    m_channel->registerObject(QString("qtLeaflet"), m_interface);

    // Create a layout inside this widget to resize the webpage automatically
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_view);
    setLayout(layout);

    // Connect each instance of the widget to the payload update signal
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(2000);
}

void HPRCStyle::drawHPRCPayloadMap(QPainter* p, const hprcDisplayWidget* w) {
    if (w->getType() == hprcDisplayWidget::HPRC_PayloadMap) {
        const hprcPayloadMap* mapWidget = dynamic_cast<const hprcPayloadMap*>(w);

        if (mapWidget && m_latest->p_gpsLock) {
            // Send updated positions to the map frontend
            mapWidget->m_interface->payloadPoint(m_latest->p_gpsLat, m_latest->p_gpsLong);
            mapWidget->m_interface->targetPoint(m_latest->p_targetGpsLat, m_latest->p_targetGpsLong);
            // Uncomment to test random points on the map
            // double rand1 = QRandomGenerator::global()->bounded(0.04);
            // double rand2 = QRandomGenerator::global()->bounded(0.04);
            // mapWidget->m_interface->payloadPoint(32.99020169835385 + rand1, -106.97596734602624 + rand2);
        }
    }
}