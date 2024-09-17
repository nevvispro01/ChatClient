#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "chat.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void onConnected();

    void onReadyRead();

    void onDisconnected();

private:

    void handleWrite(QJsonObject obj);
    void connectToServer(const QString &hostName, quint16 port);

    Ui::MainWindow *ui;
    QTcpSocket *socket;
    Chat * chat;
};
#endif // MAINWINDOW_H
