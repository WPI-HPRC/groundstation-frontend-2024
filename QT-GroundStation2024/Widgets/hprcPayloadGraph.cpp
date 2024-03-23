#include "hprcPayloadGraph.h"

#include <QMouseEvent>
#include <QApplication>

hprcPayloadGraph::hprcPayloadGraph(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    setMouseTracking(true);
    m_widgetType = HPRC_PAYLOAD_GRAPH;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            mainWindow = mainWin;
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(accUpdated(int)), this, SLOT(updateVerticalSpeed()));
        }
}

void hprcPayloadGraph::updateVerticalSpeed() {
    if (mainWindow->m_currentData.altData1.length() > 1) {
        verticalSpeedData.append(MainWindow::graphPoint{ (mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 1].value - mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 2].value) / ((mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 1].time - mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 2].time) / 1000), mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 1].time });
    }
}

void hprcPayloadGraph::mouseMoveEvent(QMouseEvent* e)
{
    m_mousePos = e->pos();

}