/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QListWidget *mensagens_recebidas;
    QFrame *frame;
    QGridLayout *gridLayout;
    QTextEdit *area_mensagem;
    QPushButton *enviar;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QFrame *follow;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QTextEdit *perfil;
    QPushButton *follow_button;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(393, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(393, 600));
        MainWindow->setMaximumSize(QSize(393, 16777215));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8(".QWidget {\n"
"	padding: 0px;\n"
"}"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mensagens_recebidas = new QListWidget(centralWidget);
        mensagens_recebidas->setObjectName(QString::fromUtf8("mensagens_recebidas"));
        mensagens_recebidas->setFrameShape(QFrame::NoFrame);
        mensagens_recebidas->setFrameShadow(QFrame::Plain);
        mensagens_recebidas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        mensagens_recebidas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mensagens_recebidas->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        mensagens_recebidas->setAutoScroll(true);
        mensagens_recebidas->setAutoScrollMargin(0);
        mensagens_recebidas->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mensagens_recebidas->setProperty("showDropIndicator", QVariant(false));
        mensagens_recebidas->setDragDropMode(QAbstractItemView::DragOnly);
        mensagens_recebidas->setDefaultDropAction(Qt::IgnoreAction);
        mensagens_recebidas->setSelectionMode(QAbstractItemView::NoSelection);
        mensagens_recebidas->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        mensagens_recebidas->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        mensagens_recebidas->setMovement(QListView::Free);
        mensagens_recebidas->setSpacing(1);

        verticalLayout->addWidget(mensagens_recebidas);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(16777215, 186));
        frame->setStyleSheet(QString::fromUtf8(".QFrame {\n"
"	width: 100%;\n"
"	border-top-left-radius: 20px;\n"
"	border-top-right-radius: 20px;\n"
"	background: #B0DDEB;\n"
"	padding: 3px;\n"
"	max-height:	180px;\n"
"}\n"
""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(9, -1, -1, -1);
        area_mensagem = new QTextEdit(frame);
        area_mensagem->setObjectName(QString::fromUtf8("area_mensagem"));
        area_mensagem->setStyleSheet(QString::fromUtf8(".QTextEdit {\n"
"	border-radius: 8px;\n"
"	background: #FFF;\n"
"	padding: 5px;\n"
"	margin-top: 6px;\n"
"}\n"
""));

        gridLayout->addWidget(area_mensagem, 4, 0, 1, 3);

        enviar = new QPushButton(frame);
        enviar->setObjectName(QString::fromUtf8("enviar"));
        enviar->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #FFF;\n"
"	border-radius: 12px;\n"
"	background: #3182EB;\n"
"	padding: 5px 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: #79ACEE;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background: #163B6B;\n"
"}"));

        gridLayout->addWidget(enviar, 2, 2, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);


        verticalLayout->addWidget(frame);

        follow = new QFrame(centralWidget);
        follow->setObjectName(QString::fromUtf8("follow"));
        follow->setMinimumSize(QSize(0, 50));
        follow->setMaximumSize(QSize(16777215, 60));
        follow->setStyleSheet(QString::fromUtf8(".QFrame {\n"
"	background: #B0DDEB;\n"
"	padding: 0.5px;\n"
"}\n"
""));
        follow->setFrameShape(QFrame::NoFrame);
        follow->setFrameShadow(QFrame::Plain);
        horizontalLayout = new QHBoxLayout(follow);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(follow);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        perfil = new QTextEdit(follow);
        perfil->setObjectName(QString::fromUtf8("perfil"));
        perfil->setMaximumSize(QSize(16777215, 40));
        perfil->setStyleSheet(QString::fromUtf8(".QTextEdit {\n"
"	border-radius: 8px;\n"
"	background: #FFF;\n"
"	padding: 5px;\n"
"	margin-top: 6px;\n"
"}\n"
""));
        perfil->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        horizontalLayout->addWidget(perfil);

        follow_button = new QPushButton(follow);
        follow_button->setObjectName(QString::fromUtf8("follow_button"));
        follow_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: #FFF;\n"
"	border-radius: 12px;\n"
"	background: #3182EB;\n"
"	padding: 5px 20px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background: #79ACEE;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background: #163B6B;\n"
"}"));

        horizontalLayout->addWidget(follow_button);


        verticalLayout->addWidget(follow);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        enviar->setText(QApplication::translate("MainWindow", "send", nullptr));
        label->setText(QApplication::translate("MainWindow", "Nova mensagem", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "<h3>@</h3>", nullptr));
        follow_button->setText(QApplication::translate("MainWindow", "follow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
