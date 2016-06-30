/********************************************************************************
** Form generated from reading UI file 'raytracer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
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
    QAction *actionNormal_Rendering;
    QAction *actionMC_PathTracing;
    QAction *actionScene1;
    QAction *actionScene2;
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
    QLineEdit *depthEdit;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *sampleEdit;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lightIntenEdit;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *viewYEdit;
    QLineEdit *viewZEdit;
    QLabel *label_12;
    QLineEdit *viewXEdit;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *upYEdit;
    QLabel *label_16;
    QLineEdit *upXEdit;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLineEdit *upZEdit;
    QLabel *label_20;
    QLabel *label_21;
    QLineEdit *envGEdit;
    QLabel *label_22;
    QLabel *label_23;
    QLineEdit *envREdit;
    QLineEdit *envBEdit;
    QLabel *label_24;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRender_Type;
    QMenu *menuScene;
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
        actionNormal_Rendering = new QAction(RayTracerClass);
        actionNormal_Rendering->setObjectName(QStringLiteral("actionNormal_Rendering"));
        actionNormal_Rendering->setCheckable(true);
        actionNormal_Rendering->setChecked(false);
        actionNormal_Rendering->setEnabled(true);
        actionMC_PathTracing = new QAction(RayTracerClass);
        actionMC_PathTracing->setObjectName(QStringLiteral("actionMC_PathTracing"));
        actionMC_PathTracing->setCheckable(true);
        actionMC_PathTracing->setChecked(true);
        actionScene1 = new QAction(RayTracerClass);
        actionScene1->setObjectName(QStringLiteral("actionScene1"));
        actionScene1->setCheckable(true);
        actionScene1->setChecked(true);
        actionScene2 = new QAction(RayTracerClass);
        actionScene2->setObjectName(QStringLiteral("actionScene2"));
        actionScene2->setCheckable(true);
        centralWidget = new QWidget(RayTracerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(840, 20, 101, 16));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        RenderButton = new QPushButton(centralWidget);
        RenderButton->setObjectName(QStringLiteral("RenderButton"));
        RenderButton->setGeometry(QRect(880, 430, 81, 31));
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
        depthEdit = new QLineEdit(centralWidget);
        depthEdit->setObjectName(QStringLiteral("depthEdit"));
        depthEdit->setGeometry(QRect(930, 300, 31, 20));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(860, 300, 51, 16));
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        label_6->setFont(font3);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(860, 340, 61, 21));
        label_7->setFont(font3);
        sampleEdit = new QLineEdit(centralWidget);
        sampleEdit->setObjectName(QStringLiteral("sampleEdit"));
        sampleEdit->setGeometry(QRect(930, 340, 31, 20));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(860, 370, 51, 21));
        label_8->setFont(font3);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(860, 390, 91, 21));
        label_9->setFont(font3);
        lightIntenEdit = new QLineEdit(centralWidget);
        lightIntenEdit->setObjectName(QStringLiteral("lightIntenEdit"));
        lightIntenEdit->setGeometry(QRect(930, 370, 31, 21));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(840, 80, 101, 16));
        label_10->setFont(font1);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(950, 110, 16, 20));
        label_11->setFont(font2);
        viewYEdit = new QLineEdit(centralWidget);
        viewYEdit->setObjectName(QStringLiteral("viewYEdit"));
        viewYEdit->setGeometry(QRect(910, 110, 31, 20));
        viewZEdit = new QLineEdit(centralWidget);
        viewZEdit->setObjectName(QStringLiteral("viewZEdit"));
        viewZEdit->setGeometry(QRect(960, 110, 31, 20));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(840, 110, 16, 16));
        label_12->setFont(font2);
        viewXEdit = new QLineEdit(centralWidget);
        viewXEdit->setObjectName(QStringLiteral("viewXEdit"));
        viewXEdit->setGeometry(QRect(860, 110, 31, 20));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(900, 110, 16, 20));
        label_13->setFont(font2);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(1000, 110, 16, 16));
        label_14->setFont(font2);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(900, 180, 16, 20));
        label_15->setFont(font2);
        upYEdit = new QLineEdit(centralWidget);
        upYEdit->setObjectName(QStringLiteral("upYEdit"));
        upYEdit->setGeometry(QRect(910, 180, 31, 20));
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(840, 140, 101, 21));
        label_16->setFont(font1);
        upXEdit = new QLineEdit(centralWidget);
        upXEdit->setObjectName(QStringLiteral("upXEdit"));
        upXEdit->setGeometry(QRect(860, 180, 31, 20));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(840, 180, 16, 16));
        label_17->setFont(font2);
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(950, 180, 16, 20));
        label_18->setFont(font2);
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(1000, 180, 16, 16));
        label_19->setFont(font2);
        upZEdit = new QLineEdit(centralWidget);
        upZEdit->setObjectName(QStringLiteral("upZEdit"));
        upZEdit->setGeometry(QRect(960, 180, 31, 20));
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(900, 250, 16, 20));
        label_20->setFont(font2);
        label_21 = new QLabel(centralWidget);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(840, 210, 141, 21));
        label_21->setFont(font1);
        envGEdit = new QLineEdit(centralWidget);
        envGEdit->setObjectName(QStringLiteral("envGEdit"));
        envGEdit->setGeometry(QRect(910, 250, 31, 20));
        label_22 = new QLabel(centralWidget);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(950, 250, 16, 20));
        label_22->setFont(font2);
        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(1000, 250, 16, 16));
        label_23->setFont(font2);
        envREdit = new QLineEdit(centralWidget);
        envREdit->setObjectName(QStringLiteral("envREdit"));
        envREdit->setGeometry(QRect(860, 250, 31, 20));
        envBEdit = new QLineEdit(centralWidget);
        envBEdit->setObjectName(QStringLiteral("envBEdit"));
        envBEdit->setGeometry(QRect(960, 250, 31, 20));
        label_24 = new QLabel(centralWidget);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(840, 250, 16, 16));
        label_24->setFont(font2);
        RayTracerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RayTracerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1026, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRender_Type = new QMenu(menuBar);
        menuRender_Type->setObjectName(QStringLiteral("menuRender_Type"));
        menuScene = new QMenu(menuBar);
        menuScene->setObjectName(QStringLiteral("menuScene"));
        RayTracerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RayTracerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        RayTracerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RayTracerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        RayTracerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuRender_Type->menuAction());
        menuBar->addAction(menuScene->menuAction());
        menuFile->addAction(actionLoad_Scene);
        menuRender_Type->addAction(actionNormal_Rendering);
        menuRender_Type->addAction(actionMC_PathTracing);
        menuScene->addAction(actionScene1);
        menuScene->addAction(actionScene2);

        retranslateUi(RayTracerClass);

        QMetaObject::connectSlotsByName(RayTracerClass);
    } // setupUi

    void retranslateUi(QMainWindow *RayTracerClass)
    {
        RayTracerClass->setWindowTitle(QApplication::translate("RayTracerClass", "RayTracer", 0));
        actionLoad_Scene->setText(QApplication::translate("RayTracerClass", "Load Scene", 0));
        actionNormal_Rendering->setText(QApplication::translate("RayTracerClass", "Normal Rendering", 0));
        actionMC_PathTracing->setText(QApplication::translate("RayTracerClass", "MC PathTracing", 0));
        actionScene1->setText(QApplication::translate("RayTracerClass", "scene1", 0));
        actionScene2->setText(QApplication::translate("RayTracerClass", "scene2", 0));
        label->setText(QApplication::translate("RayTracerClass", "Camera Pos", 0));
        RenderButton->setText(QApplication::translate("RayTracerClass", "Render", 0));
        posXEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        posYEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        posZEdit->setText(QApplication::translate("RayTracerClass", "30", 0));
        label_2->setText(QApplication::translate("RayTracerClass", "(", 0));
        label_3->setText(QApplication::translate("RayTracerClass", ")", 0));
        label_4->setText(QApplication::translate("RayTracerClass", ",", 0));
        label_5->setText(QApplication::translate("RayTracerClass", ",", 0));
        render_label->setText(QApplication::translate("RayTracerClass", "TextLabel", 0));
        depthEdit->setText(QApplication::translate("RayTracerClass", "7", 0));
        label_6->setText(QApplication::translate("RayTracerClass", "Depth", 0));
        label_7->setText(QApplication::translate("RayTracerClass", "Sample", 0));
        sampleEdit->setText(QApplication::translate("RayTracerClass", "1024", 0));
        label_8->setText(QApplication::translate("RayTracerClass", "Light", 0));
        label_9->setText(QApplication::translate("RayTracerClass", "Intensity", 0));
        lightIntenEdit->setText(QApplication::translate("RayTracerClass", "25", 0));
        label_10->setText(QApplication::translate("RayTracerClass", "View Dir", 0));
        label_11->setText(QApplication::translate("RayTracerClass", ",", 0));
        viewYEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        viewZEdit->setText(QApplication::translate("RayTracerClass", "-1", 0));
        label_12->setText(QApplication::translate("RayTracerClass", "(", 0));
        viewXEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        label_13->setText(QApplication::translate("RayTracerClass", ",", 0));
        label_14->setText(QApplication::translate("RayTracerClass", ")", 0));
        label_15->setText(QApplication::translate("RayTracerClass", ",", 0));
        upYEdit->setText(QApplication::translate("RayTracerClass", "1", 0));
        label_16->setText(QApplication::translate("RayTracerClass", "Up Vector", 0));
        upXEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        label_17->setText(QApplication::translate("RayTracerClass", "(", 0));
        label_18->setText(QApplication::translate("RayTracerClass", ",", 0));
        label_19->setText(QApplication::translate("RayTracerClass", ")", 0));
        upZEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        label_20->setText(QApplication::translate("RayTracerClass", ",", 0));
        label_21->setText(QApplication::translate("RayTracerClass", "Environment Color", 0));
        envGEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        label_22->setText(QApplication::translate("RayTracerClass", ",", 0));
        label_23->setText(QApplication::translate("RayTracerClass", ")", 0));
        envREdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        envBEdit->setText(QApplication::translate("RayTracerClass", "0", 0));
        label_24->setText(QApplication::translate("RayTracerClass", "(", 0));
        menuFile->setTitle(QApplication::translate("RayTracerClass", "File", 0));
        menuRender_Type->setTitle(QApplication::translate("RayTracerClass", "Render Type", 0));
        menuScene->setTitle(QApplication::translate("RayTracerClass", "Scene", 0));
    } // retranslateUi

};

namespace Ui {
    class RayTracerClass: public Ui_RayTracerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYTRACER_H
