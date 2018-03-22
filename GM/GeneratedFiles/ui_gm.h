/********************************************************************************
** Form generated from reading UI file 'gm.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GM_H
#define UI_GM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GMClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GMClass)
    {
        if (GMClass->objectName().isEmpty())
            GMClass->setObjectName(QStringLiteral("GMClass"));
        GMClass->resize(600, 400);
        menuBar = new QMenuBar(GMClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        GMClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GMClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GMClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GMClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GMClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GMClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GMClass->setStatusBar(statusBar);

        retranslateUi(GMClass);

        QMetaObject::connectSlotsByName(GMClass);
    } // setupUi

    void retranslateUi(QMainWindow *GMClass)
    {
        GMClass->setWindowTitle(QApplication::translate("GMClass", "GM", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GMClass: public Ui_GMClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GM_H
