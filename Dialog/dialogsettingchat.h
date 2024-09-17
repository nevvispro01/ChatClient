#ifndef DIALOGSETTINGCHAT_H
#define DIALOGSETTINGCHAT_H

#include <QDialog>

namespace Ui {
class DialogSettingChat;
}

class DialogSettingChat : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettingChat(QWidget *parent = nullptr);
    ~DialogSettingChat();

    void setInfo(QString message);

    void loadUsers(QStringList & users);

signals:
    void signalAddUser(QString name);

private slots:
    void addUserChat();

private:
    Ui::DialogSettingChat *ui;
};

#endif // DIALOGSETTINGCHAT_H
