#include "hprcdisplaywidget.h"

#include <QPainter>

class hprcDetailedViewWidget : public hprcDisplayWidget {

public:
	hprcDisplayWidget* widget;

	hprcDetailedViewWidget(hprcDisplayWidget* widget, QWidget* parent = nullptr) : hprcDisplayWidget(widget) {
		m_widgetType = HPRC_DETAILED_VIEW_WIDGET;

		this->widget = widget;
	};
};