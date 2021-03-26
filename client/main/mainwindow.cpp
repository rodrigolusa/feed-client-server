#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QScrollBar>
#include <QByteArray>

#define MAX_TEXT_LENGTH 128

MainWindow::MainWindow(QWidget *parent, std::string title) :
    QMainWindow( parent ),
    title( title ),
    comm( new ClientComms() ),
    ui( new Ui::MainWindow )
{
    this->setWindowTitle( QString::fromUtf8( title.data(), title.size() ) );
    ui->setupUi(this);
    QListWidgetItem* item = new QListWidgetItem(ui->mensagens_recebidas);
    item->setText("Mensagens Recebidas");
    ui->mensagens_recebidas->viewport()->setAttribute( Qt::WA_TransparentForMouseEvents );

    connect( ui->enviar, SIGNAL( clicked() ), this, SLOT( on_send_message_clicked() ) );
    connect( ui->follow_button, SIGNAL( clicked() ), this, SLOT( on_follow_clicked() ) );
    connect( ui->area_mensagem, SIGNAL( textChanged() ), this, SLOT( on_mensagem_textChanged() ) );
    connect( ui->perfil, SIGNAL( textChanged() ), this, SLOT( on_user_textChanged()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUserName(std::string user_name)
{
    title = user_name;
}

void MainWindow::news()
{
    while( true ) {
        packet* message = comm->readMessage();

        if (message != NULL && message->type == CMDS::SEND) { // TODO arrumar verificação de tipo
            QListWidgetItem *item = new QListWidgetItem(ui->mensagens_recebidas);
            std::string mensagem = "_________________________________________________\n\nANONYMOUS\n\n";
            mensagem += message->_payload;
            mensagem += "\n";
            item->setText(QString::fromUtf8(mensagem.data(), mensagem.size()));

            QScrollBar *scroll = ui->mensagens_recebidas->verticalScrollBar();
            scroll->setSliderPosition(scroll->maximum());
        }
    }
}

void MainWindow::on_send_message_clicked()
{
    QByteArray message = ui->area_mensagem->toPlainText().toLocal8Bit();

    int recive = comm->sendMessage(CMDS::SEND, message.data());

    if (recive == 0) {
        ui->area_mensagem->clear();
        ui->area_mensagem->setFocus();
    } else {
        // TODO dialog de erro
    }
}

void MainWindow::on_follow_clicked()
{
    QByteArray message = ui->perfil->toPlainText().toLocal8Bit();

    int recive = comm->sendMessage(CMDS::FOLLOW, message.data());

    if (recive == 0) {
        ui->perfil->clear();
        ui->perfil->setFocus();
    } else {
        // TODO dialog de erro
    }
}

void MainWindow::on_mensagem_textChanged()
{
    if(ui->area_mensagem->toPlainText().length() > MAX_TEXT_LENGTH)
    {
        int diff = ui->area_mensagem->toPlainText().length() - MAX_TEXT_LENGTH;
        QString newStr = ui->area_mensagem->toPlainText();
        newStr.chop(diff);
        ui->area_mensagem->setText(newStr);
        QTextCursor cursor(ui->area_mensagem->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->area_mensagem->setTextCursor(cursor);
    }
}

void MainWindow::on_user_textChanged()
{
    if(ui->perfil->toPlainText().length() > MAX_TEXT_LENGTH)
    {
        int diff = ui->perfil->toPlainText().length() - MAX_TEXT_LENGTH;
        QString newStr = ui->perfil->toPlainText();
        newStr.chop(diff);
        ui->perfil->setText(newStr);
        QTextCursor cursor(ui->perfil->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->perfil->setTextCursor(cursor);
    }
}
