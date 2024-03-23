#include <QObject>
#include "Widgets/hprcdisplaywidget.h"

#include <QWebEngineView>
#include <QWebChannel>

class JsInterface : public QObject
{
    Q_OBJECT
public slots:
    void log(const QString& str);
    void payloadPoint(double lat, double lng);
    void targetPoint(double lat, double lng);

signals:
    void updatePayloadPoint(double lat, double lng);
    void updateTargetPoint(double lat, double lng);
};

class hprcPayloadMap : public hprcDisplayWidget
{
public:
    explicit hprcPayloadMap(QWidget* parent = nullptr);

    QWebEngineView* m_view;
    JsInterface* m_interface;
    QWebChannel* m_channel;
};