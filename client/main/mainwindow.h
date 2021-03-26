#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../clientcomms.hpp"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::string title;
    ClientComms* comm;
public:
    explicit MainWindow(QWidget *parent = 0, std::string title = "MainWindow");
    ~MainWindow();
    void news();
    void setUserName(std::string user_name);

private slots:
    void on_send_message_clicked();
    void on_follow_clicked();
    void on_mensagem_textChanged();
    void on_user_textChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
