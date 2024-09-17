#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "Dialog/dialogsign.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chat = new Chat();
    connect(chat, &Chat::signalHandleWrite, this, &MainWindow::handleWrite);

//    connectToServer("192.168.90.81", 8000);
    connectToServer("192.168.90.81", 11001);
    if (socket->waitForConnected(3000)){

        qDebug("Connected!");
    }else{
        qDebug("Превышено время ожидания сервера!");

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnected()
{
    qDebug() << "Connected to server";
    DialogSign dialog;
    dialog.exec();
    if (static_cast<QDialog::DialogCode>(dialog.result()) == QDialog::DialogCode::Accepted){
        /*else{
                            QMessageBox msgBox;
                            msgBox.setText(j_obj["message"].toString());
                            msgBox.exec();
                            if (socket->isOpen()){
                                onConnected();
                            }
                        }*/
        QJsonObject obj;
        switch (dialog.getType()) {
        case TypeAuth::SIGN:
            obj["type"] = "signIn";
            obj["login"] = dialog.getLogin();
            obj["password"] = dialog.getPass();
            break;
        case TypeAuth::AUTH:
            obj["type"] = "logIn";
            obj["login"] = dialog.getLogin();
            obj["name"] = dialog.getName();
            obj["password"] = dialog.getPass();
            break;

        }

        handleWrite(std::move(obj));
        chat->setName(dialog.getLogin());
    }else{
        ///Закрыть приложение
    }
}

void MainWindow::onReadyRead()
{
    QByteArray msg = socket->readAll();
    QByteArray msg_2 = msg;
    QByteArray res;

    auto start = msg_2.indexOf("!start");

    while (start != -1){

        auto end = msg_2.indexOf("!end");

        if (end == -1 or start > end) return;

        res = msg_2.mid(start + 6, end-8);

        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(res, &error);

        if (error.error != QJsonParseError::ParseError::NoError) return;

        QJsonObject j_obj = json.object();

        if (j_obj.contains("type")){

            if (j_obj["type"].toString() == "sign"){

                if(j_obj["status"].toBool()){
                    setCentralWidget(chat);
                }else{
                    QMessageBox msgBox;
                    msgBox.setText(j_obj["message"].toString());
                    msgBox.exec();
                    if (socket->isOpen()){
                        onConnected();
                    }
                }
            }else{
                chat->handleRead(j_obj);
            }
        }


        msg_2.replace(0, end + 4, "");

        start = msg_2.indexOf("!start");

    }
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected from server";
}

void MainWindow::handleWrite(QJsonObject obj)
{
    QByteArray answer{"!start"};

    answer += QJsonDocument(obj).toJson(QJsonDocument::Compact);

    answer.push_back(0x1);
    answer.push_back(0x1);
    answer.push_back("!end");

    socket->write(answer);
}

void MainWindow::connectToServer(const QString &hostName, quint16 port)
{
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(hostName), port);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
}

