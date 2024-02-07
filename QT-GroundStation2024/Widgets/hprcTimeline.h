#ifndef HPRCTIMELINE_H
#define HPRCTIMELINE_H

#include "hprcdisplaywidget.h"
#include "qapplication.h"

class hprcTimeline : public hprcDisplayWidget
{
    Q_OBJECT
public:
    explicit hprcTimeline(QWidget *parent)
        : hprcDisplayWidget{parent}
    {
        m_widgetType = HPRC_Timeline;
        foreach (QWidget *w, qApp->topLevelWidgets())
            if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
            {
                connect(mainWin, SIGNAL(stateUpdated(int)), this, SLOT(updateFilled(int)));
            }
    }

signals:
};

#endif // HPRCTIMELINE_H
