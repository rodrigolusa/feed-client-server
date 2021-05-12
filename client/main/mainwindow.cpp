#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QScrollBar>
#include <QByteArray>
#include <QCloseEvent>
#include <QMessageBox>

#define MAX_TEXT_LENGTH 128

MainWindow::MainWindow(QWidget *parent, std::string userName) :
    QMainWindow( parent ),
    title( QString::fromUtf8( userName.data(), userName.size() ) ),
    comm( new ClientComms() ),
    ui( new Ui::MainWindow )
{
    this->setWindowTitle( title );
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

void MainWindow::setUserName(std::string userName)
{
    title = QString::fromUtf8( userName.data(), userName.size() );
    this->setWindowTitle( title );
}

void MainWindow::setComm(ClientComms* new_comm)
{
    comm = new_comm;
}

void MainWindow::news()
{
    while( true ) {
        packet* message = comm->readMessage();

        if (message != NULL && message->type == CMDS::SEND_NAME) {
            packet* message2 = comm->readMessage();
            if (message != NULL && message2->type == CMDS::SEND_DATA) {
              QListWidgetItem *item = new QListWidgetItem(ui->mensagens_recebidas);
              std::string mensagem = "_________________________________________________\n\n";
              mensagem += "[";
              mensagem += getDate(message->timestamp);
              mensagem += "][";
              mensagem += " ";
              mensagem += message->_payload;
              mensagem += "]";
              mensagem += " diz:";
              mensagem += "\n\n";
              mensagem += message2->_payload;
              mensagem += "\n";
              item->setText(QString::fromUtf8(mensagem.data(), mensagem.size()));
              QScrollBar *scroll = ui->mensagens_recebidas->verticalScrollBar();
              scroll->setSliderPosition(scroll->maximum());
            }
        }
    }
}

void MainWindow::on_send_message_clicked()
{
    QByteArray message = ui->area_mensagem->toPlainText().toLocal8Bit();

    if (!message.trimmed().isEmpty()) {
        int recive = comm->sendMessage(CMDS::SEND_UNNAMED, message.data());

        if (recive == 0) {
            ui->area_mensagem->clear();
            ui->area_mensagem->setFocus();
        } else {
            QMessageBox::warning( this, title, "Erro ao tentar dar tweetar.");
        }
    }
}

void MainWindow::on_follow_clicked()
{
    QByteArray message = ui->perfil->toPlainText().toLocal8Bit();

    if (!message.trimmed().isEmpty() && QString::compare(message, title, Qt::CaseInsensitive) != 0) {
        int recive = comm->sendMessage(CMDS::FOLLOW, message.data());

        if (recive == 0) {
            ui->perfil->clear();
            ui->perfil->setFocus();
        } else {
            QMessageBox::warning( this, title, "Erro ao tentar dar follow.");
        }
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
    QString str = ui->perfil->toPlainText();
    if(str.length() > MAX_TEXT_LENGTH)
    {
        int diff = ui->perfil->toPlainText().length() - MAX_TEXT_LENGTH;
        str.chop(diff);
        ui->perfil->setText(str);
        QTextCursor cursor(ui->perfil->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->perfil->setTextCursor(cursor);
    }

    if (str.indexOf("\n", 0) != -1) {
        str.chop(1);
        ui->perfil->setText(str);
        QTextCursor cursor(ui->perfil->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->perfil->setTextCursor(cursor);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, title,
                                                                QString("Tem certeza que deseja sair?"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        comm->connectionInterrupted();
        event->accept();
    }
}
