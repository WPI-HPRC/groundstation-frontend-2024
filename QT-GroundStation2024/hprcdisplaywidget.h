#ifndef HPRCDISPLAYWIDGET_H
#define HPRCDISPLAYWIDGET_H

#include <QObject>
#include <QWidget>

class hprcDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit hprcDisplayWidget(QWidget *parent = nullptr);

    enum hprcWidgetType
    {
        HPRC_Timeline,
        HPRC_Hidden,
    };

    hprcWidgetType getType() const { return m_widgetType; };

    void paintEvent(QPaintEvent *e);

protected:

    hprcWidgetType m_widgetType;

signals:

};

class hprcTimeline : public hprcDisplayWidget
{
    Q_OBJECT
public:
    explicit hprcTimeline(QWidget *parent = nullptr);

signals:
};

#endif // HPRCDISPLAYWIDGET_H
