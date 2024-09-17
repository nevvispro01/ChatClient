#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QJsonObject>
#include "QListWidget"
#include <Dialog/dialogsettingchat.h>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    void setName(QString name);

    void updateRoom(QJsonObject obj);
    void updateUserRoom(QJsonObject obj);

    void handleMessage(QJsonObject obj);

    void handleRead(QJsonObject j_obj);

protected:

     void	keyPressEvent(QKeyEvent *event) override;

signals:
    void signalHandleWrite(QJsonObject obj);

private slots:
    void sendMessage();
    void sendSmile();
    void changeChat(QListWidgetItem *current);
    void settingsChat();

private:

    Ui::Chat *ui;
    QListWidget * listWidget;
    DialogSettingChat *dialogSetting;
};

#endif // CHAT_H
