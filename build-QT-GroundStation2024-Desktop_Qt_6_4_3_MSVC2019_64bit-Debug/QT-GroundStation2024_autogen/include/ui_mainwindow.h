/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "hprcdisplaywidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QFrame *uTime;
    QFrame *mTime;
    QFrame *gaugesPanel;
    QFrame *timelinePanel;
    hprcTimeline *timeline;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1920, 1080);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        uTime = new QFrame(centralwidget);
        uTime->setObjectName("uTime");
        uTime->setFrameShape(QFrame::StyledPanel);
        uTime->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(uTime);

        mTime = new QFrame(centralwidget);
        mTime->setObjectName("mTime");
        mTime->setFrameShape(QFrame::StyledPanel);
        mTime->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(mTime);


        verticalLayout_2->addLayout(horizontalLayout_2);

        gaugesPanel = new QFrame(centralwidget);
        gaugesPanel->setObjectName("gaugesPanel");
        gaugesPanel->setFrameShape(QFrame::StyledPanel);
        gaugesPanel->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(gaugesPanel);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 6);

        horizontalLayout->addLayout(verticalLayout_2);

        timelinePanel = new QFrame(centralwidget);
        timelinePanel->setObjectName("timelinePanel");
        timelinePanel->setFrameShape(QFrame::StyledPanel);
        timelinePanel->setFrameShadow(QFrame::Raised);
        timeline = new hprcTimeline(timelinePanel);
        timeline->setObjectName("timeline");
        timeline->setGeometry(QRect(0, 0, 381, 1061));

        horizontalLayout->addWidget(timelinePanel);

        horizontalLayout->setStretch(0, 4);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
