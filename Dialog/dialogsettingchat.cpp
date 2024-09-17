#include "dialogsettingchat.h"
#include "ui_dialogsettingchat.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>

DialogSettingChat::DialogSettingChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingChat)
{
    ui->setupUi(this);
    connect(ui->addUserChat, &QPushButton::clicked, this, &DialogSettingChat::addUserChat);
}

DialogSettingChat::~DialogSettingChat()
{
    delete ui;
}

void DialogSettingChat::setInfo(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void DialogSettingChat::loadUsers(QStringList & users)
{
    ui->comboBox->clear();
    ui->comboBox->addItems(users);
}

void DialogSettingChat::addUserChat()
{
    emit signalAddUser(ui->comboBox->currentText());
}
