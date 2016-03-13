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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayTracerClass
{
public:
    QAction *actionLoad_Scene;
    QAction *actionGeneral_RayTracing;
    QAction *actionMC_PathTracing;
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
    QCheckBox *drawShadowCheck;
    QCheckBox *drawReflectCheck;
    QLineEdit *depthEdit;
    QLabel *label_6;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRender_Type;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RayTracerClass)
    {
        if (RayTracerClass->objectName().isEmpty())
            RayTracerClass->setObjectName(QStringLiteral("RayTracerClass"));
        RayTracerClass->resize(1026, 703);
        QFont font;
        font.setPointSize(10);
        RayTracerClass->setFont(font);
        actionLoad_Scene = new QAction(RayTracerClass);
        actionLoad_Scene->setObjectName(QStringLiteral("actionLoad_Scene"));
        actionGeneral_RayTracing = new QAction(RayTracerClass);
        actionGeneral_RayTracing->setObjectName(QStringLiteral("actionGeneral_RayTracing"));
        actionGeneral_RayTracing->setCheckable(true);
        actionGeneral_RayTracing->setChecked(true);
        actionGeneral_RayTracing->setEnabled(true);
        actionMC_PathTracing = new QAction(RayTracerClass);
        actionMC_PathTracing->setObjectName(QStringLiteral("actionMC_PathTracing"));
        actionMC_PathTracing->setCheckable(true);
        centralWidget = new QWidget(RayTracerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(840, 20, 91, 16));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        RenderButton = new QPushButton(centralWidget);
        RenderButton->setObjectName(QStringLiteral("RenderButton"));
        RenderButton->setGeometry(QRect(880, 190, 81, 31));
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
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(1000, 50, 16, 16));
        label_3->setFont(font2);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(900, 50, 16, 20));
        label_4->setFont(font2);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(950, 50, 16, 20));
        label_5->setFont(font2);
        render_label = new QLabel(centralWidget);
        render_label->setObjectName(QStringLiteral("render_label"));
        render_label->setGeometry(QRect(20, 10, 54, 12));
        drawShadowCheck = new QCheckBox(centralWidget);
        drawShadowCheck->setObjectName(QStringLiteral("drawShadowCheck"));
        drawShadowCheck->setGeometry(QRect(850, 120, 131, 16));
        drawShadowCheck->setFont(font1);
        drawShadowCheck->setChecked(true);
        drawReflectCheck = new QCheckBox(centralWidget);
        drawReflectCheck->setObjectName(QStringLiteral("drawReflectCheck"));
        drawReflectCheck->setGeometry(QRect(850, 150, 161, 16));
        drawReflectCheck->setFont(font1);
        drawReflectCheck->setChecked(true);
        depthEdit = new QLineEdit(centralWidget);
        depthEdit->setObjectName(QStringLiteral("depthEdit"));
        depthEdit->setGeometry(QRect(900, 90, 31, 20));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(850, 90, 41, 16));
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        label_6->setFont(font3);
        RayTracerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RayTracerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1026, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRender_Type = new QMenu(menuBar);
        menuRender_Type->setObjectName(QStringLiteral("menuRender_Type"));
        RayTracerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RayTracerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RayTracerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RayTracerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RayTracerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRender_Type->menuAction());
        menuFile->addAction(actionLoad_Scene);
        menuRender_Type->addAction(actionGeneral_RayTracing);
        menuRender_Type->addAction(actionMC_PathTracing);

        retranslateUi(RayTracerClass);

        QMetaObject::connectSlotsByName(RayTracerClass);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracerClass)
    {
        RayTracerClass->setWindowTitle(QApplication::translate("RayTracerClass", "RayTracer", 0));
        actionLoad_Scene->setText(QApplication::translate("RayTracerClass", "Load Scene", 0));
        actionGeneral_RayTracing->setText(QApplication::translate("RayTracerClass", "General RayTracing", 0));
        actionMC_PathTracing->setText(QApplication::translate("RayTracerClass", "MC PathTracing", 0));
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
        drawShadowCheck->setText(QApplication::translate("RayTracerClass", "Draw Shadow", 0));
        drawReflectCheck->setText(QApplication::translate("RayTracerClass", "Draw Reflection", 0));
        depthEdit->setText(QApplication::translate("RayTracerClass", "2", 0));
        label_6->setText(QApplication::translate("RayTracerClass", "Depth", 0));
        menuFile->setTitle(QApplication::translate("RayTracerClass", "File", 0));
        menuRender_Type->setTitle(QApplication::translate("RayTracerClass", "Render Type", 0));
    } // retranslateUi

};

namespace Ui {
    class RayTracerClass: public Ui_RayTracerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACER_H
