/********************************************************************************
** Form generated from reading UI file 'raytracer.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAYTRACER_H
#define UI_RAYTRACER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayTracerClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *RenderButton;
    QLineEdit *posXEdit;
    QLineEdit *posYEdit;
    QLineEdit *posZEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *render_label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RayTracerClass)
    {
        if (RayTracerClass->objectName().isEmpty())
            RayTracerClass->setObjectName(QStringLiteral("RayTracerClass"));
        RayTracerClass->resize(1026, 703);
        centralWidget = new QWidget(RayTracerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(840, 10, 91, 16));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        RenderButton = new QPushButton(centralWidget);
        RenderButton->setObjectName(QStringLiteral("RenderButton"));
        RenderButton->setGeometry(QRect(880, 100, 75, 23));
        posXEdit = new QLineEdit(centralWidget);
        posXEdit->setObjectName(QStringLiteral("posXEdit"));
        posXEdit->setGeometry(QRect(860, 50, 31, 20));
        posYEdit = new QLineEdit(centralWidget);
        posYEdit->setObjectName(QStringLiteral("posYEdit"));
        posYEdit->setGeometry(QRect(910, 50, 31, 20));
        posZEdit = new QLineEdit(centralWidget);
        posZEdit->setObjectName(QStringLiteral("posZEdit"));
        posZEdit->setGeometry(QRect(960, 50, 31, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(840, 50, 16, 16));
        QFont font1;
        font1.setPointSize(12);
        label_2->setFont(font1);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(1000, 50, 16, 16));
        label_3->setFont(font1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(900, 50, 16, 20));
        label_4->setFont(font1);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(950, 50, 16, 20));
        label_5->setFont(font1);
        render_label = new QLabel(centralWidget);
        render_label->setObjectName(QStringLiteral("render_label"));
        render_label->setGeometry(QRect(20, 10, 54, 12));
        RayTracerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RayTracerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1026, 23));
        RayTracerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RayTracerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RayTracerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RayTracerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RayTracerClass->setStatusBar(statusBar);

        retranslateUi(RayTracerClass);

        QMetaObject::connectSlotsByName(RayTracerClass);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracerClass)
    {
        RayTracerClass->setWindowTitle(QApplication::translate("RayTracerClass", "RayTracer", 0));
        label->setText(QApplication::translate("RayTracerClass", "Camera Pos", 0));
        RenderButton->setText(QApplication::translate("RayTracerClass", "Render", 0));
        posXEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        posYEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        posZEdit->setText(QApplication::translate("RayTracerClass", "10", 0));
        label_2->setText(QApplication::translate("RayTracerClass", "(", 0));
        label_3->setText(QApplication::translate("RayTracerClass", ")", 0));
        label_4->setText(QApplication::translate("RayTracerClass", ",", 0));
        label_5->setText(QApplication::translate("RayTracerClass", ",", 0));
        render_label->setText(QApplication::translate("RayTracerClass", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class RayTracerClass: public Ui_RayTracerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACER_H
