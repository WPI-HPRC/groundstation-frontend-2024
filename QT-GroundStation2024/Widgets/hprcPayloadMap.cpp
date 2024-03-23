#include "hprcPayloadMap.h"

#include <QVBoxLayout>
#include <QTimer>

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