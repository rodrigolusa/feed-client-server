#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../clientcomms.hpp"
#include <QMainWindow>
#include <QString>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString title;
    ClientComms* comm;
public:
    explicit MainWindow(QWidget *parent = 0, std::string title = "MainWindow");
    ~MainWindow();
    void news();
    void setUserName(std::string user_name);
    void setComm(ClientComms* new_comm);

private slots:
    void on_send_message_clicked();
    void on_follow_clicked();
    void on_mensagem_textChanged();
    void on_user_textChanged();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
