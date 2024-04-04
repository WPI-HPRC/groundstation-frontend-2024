#include "hprcdisplaywidget.h"
#include "../Windows/mainwindow.h"
#include "qapplication.h"

#include <QStyle>
#include <QMouseEvent>
#include <QStyleOption>

#include "Widgets/hprcDetailedViewWidget.h"

hprcDisplayWidget::hprcDisplayWidget(QWidget *parent)
    : QWidget{parent}
{
    m_widgetType = HPRC_Hidden;
    m_filledPercent = 0;
    m_label = QString("TESTING");

#if RUN_SPEED_TESTS
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(speedTick(int)), this, SLOT(doSpeedTick(int)));
        }
#endif
}

void hprcDisplayWidget::makeDetailedWidget(hprcDisplayWidget *baseWidget)
{
    if(baseWidget->detailedViewEnabled) {
        // Create a new window
        QMainWindow* window = new QMainWindow();
        window->setPalette(QApplication::style()->standardPalette());
        window->setBackgroundRole(QPalette::Window);

        // Set the widget to use the entire new window
        hprcDisplayWidget* detailedViewWidget;
        if (QString(typeid(*baseWidget).name()).remove("class ") == "hprcGraph") { //If it is a widget that uses graphics view
            detailedViewWidget = new hprcGraphicsDetailedViewWidget((hprcGraphicsWidget*) baseWidget);
        }
        else {
            detailedViewWidget = new hprcDetailedViewWidget(baseWidget);
        }
        window->setWindowTitle(QString(typeid(*baseWidget).name()).remove("class "));
        window->setCentralWidget((QWidget*)detailedViewWidget);
        window->resize(detailedViewWindowWidth, detailedViewWindowHeight);
        window->show();
    }
}

void hprcDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        hprcDisplayWidget::makeDetailedWidget(this);
    }
}

void hprcDisplayWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    const QStyleOption opt(QStyleOption::Version, QStyleOption::SO_Default);
    QApplication::style()->drawPrimitive(QStyle::PE_CustomBase, &opt, &p, this);
}

void hprcDisplayWidget::updateFilled(float input)
{

    QPropertyAnimation *animation = new QPropertyAnimation(this, "filledPercent");
    animation->setDuration(400);
    animation->setEndValue((int) input);
    animation->start();
}

void hprcDisplayWidget::updateFilled(int input)
{

    QPropertyAnimation *animation = new QPropertyAnimation(this, "filledPercent");
    animation->setDuration(400);
    animation->setEndValue((int) input);
    animation->start();
}

void hprcDisplayWidget::doSpeedTick(int input)
{
    repaint();
//    updateFilled(input);
}

int hprcDisplayWidget::getFilled()
{

    return m_filledPercent;
}
void hprcDisplayWidget::setFilled(int input)
{
    m_filledPercent = input;
    update();
}
int hprcDisplayWidget::fillChanged()
{

    return m_filledPercent;
}