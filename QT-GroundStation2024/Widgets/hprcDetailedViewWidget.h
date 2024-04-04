#include "hprcdisplaywidget.h"
#include "hprcgraphicswidget.h"

#include <QPainter>

class hprcDetailedViewWidget : public hprcDisplayWidget {

public:
	hprcDisplayWidget* widget;

	hprcDetailedViewWidget(hprcDisplayWidget* widget, QWidget* parent = nullptr) : hprcDisplayWidget(widget) {
		m_widgetType = HPRC_DETAILED_VIEW_WIDGET;

		this->widget = widget;
	};
};

class hprcGraphicsDetailedViewWidget : public hprcGraphicsWidget {
public:
	hprcGraphicsWidget* widget;

	hprcGraphicsDetailedViewWidget(hprcGraphicsWidget* widget, QWidget* parent = nullptr) : hprcGraphicsWidget(widget, parent) {
		m_widgetType = HPRC_GRAPHICS_DETAILED_VIEW_WIDGET;

		this->widget = widget;

		widget->graphicsScene = graphicsScene;
		widget->graphicsView = graphicsView;
	}
};