#include "hprcClock.h"

#include "Windows/mainwindow.h"

#include "stylepainters.h"

#include <QApplication>

hprcClock::hprcClock(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    // subscribe to any data that needs to cause an update
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_Clock;
}

void HPRCStyle::drawHPRCClock(QPainter* p, const hprcDisplayWidget* w) {
    m_widgetLarge.setPointSize(w->height() / 5);
    p->setFont(m_widgetLarge);
    p->setPen(QPen(m_textBrush, 3));
    if (w->getDataType() == hprcDisplayWidget::HPRC_MET) {
        double t = m_latest->groundTime / 1000.0;

        int wholeNumSecs = t;
        int hours = wholeNumSecs / 60 / 60;
        int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
        double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

        int rseconds = seconds * 100;
        seconds = rseconds / 100.0;

        std::string hourString(std::to_string(hours));
        std::string minuteString(std::to_string(minutes));
        std::string secondString(std::to_string(seconds));

        if (hourString.length() == 1)
            hourString.insert(0, "0");
        if (minuteString.length() == 1)
            minuteString.insert(0, "0");
        if (secondString.at(1) == *".")
            secondString.insert(0, "0");
        secondString = secondString.substr(0, 5);
        std::string timeString = "Ground Time: T + " + hourString + ":" + minuteString + ":" + secondString;
        p->drawText(w->rect().adjusted(0, 0, 0, w->rect().height() / -2.0), QString::fromStdString(timeString));

        // rocket internal time //

        t = m_latest->rocketTimeSinceLaunch / 1000.0;

        wholeNumSecs = t;
        hours = wholeNumSecs / 60 / 60;
        minutes = (wholeNumSecs - hours * 60 * 60) / 60;
        seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

        rseconds = seconds * 100;
        seconds = rseconds / 100.0;

        std::string hourString2(std::to_string(hours));
        std::string minuteString2(std::to_string(minutes));
        std::string secondString2(std::to_string(seconds));

        if (hourString2.length() == 1)
            hourString2.insert(0, "0");
        if (minuteString2.length() == 1)
            minuteString2.insert(0, "0");
        if (secondString2.at(1) == *".")
            secondString2.insert(0, "0");
        secondString2 = secondString2.substr(0, 5);
        std::string timeString2 = "Rocket Time: T + " + hourString2 + ":" + minuteString2 + ":" + secondString2;
        p->drawText(w->rect().adjusted(0, w->rect().height() / 2.0, 0, 0), QString::fromStdString(timeString2));
    }
    else if (w->getDataType() == hprcDisplayWidget::HPRC_UTC) {


        QDateTime currentUTC = QDateTime::currentDateTimeUtc();
        p->drawText(w->rect(), currentUTC.toString("hh:mm:ss dddd, MMMM dd yyyy"));


    }


}