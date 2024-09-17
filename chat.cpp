#include "chat.h"
#include "ui_chat.h"
#include <QJsonArray>
#include <QInputDialog>
#include "Chats/listwidgetitem.h"
#include <QKeyEvent>
#include "Dialog/dialogchoosesmile.h"
#include <iostream>

#ifdef BUILD_WINDOWS
#include "/home/dmitry/dev/Libs/bass24-win/c/bass.h"
#endif
#ifndef BUILD_WINDOWS
//#include "/home/dmitry/dev/Libs/bass24-linux/bass.h"
#endif

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    ui->windowChat->setVisible(false);

    connect(ui->newChat, &QPushButton::clicked, [this]{
        bool ok;
        auto name = QInputDialog::getText(this, "Создания чата", "Введите название чата", QLineEdit::EchoMode::Normal, "", &ok);

        if (ok and not name.isEmpty()){
            QJsonObject obj;
            obj["type"] = "createRoom";
            obj["nameRoom"] = name;
            emit signalHandleWrite(obj);
        }
    });
    connect(ui->send, &QPushButton::clicked, this, &Chat::sendMessage);
    connect(ui->listChats, &QListWidget::itemClicked, this, &Chat::changeChat);
    connect(ui->settingsChat, &QPushButton::clicked, this, &Chat::settingsChat);
    connect(ui->lineEdit, &QLineEdit::editingFinished, this, &Chat::sendMessage);
    connect(ui->smile, &QPushButton::clicked, this, &Chat::sendSmile);

    ui->listChats->setIconSize(QSize(10,10));
    ui->listUserChat->setIconSize(QSize(10,10));
}

Chat::~Chat()
{
    delete ui;
}

void Chat::setName(QString name)
{
    ui->name->setText(name);
}

void Chat::updateRoom(QJsonObject obj)
{
    if (ui->listChats->findItems(obj["nameRoom"].toString(), Qt::MatchCaseSensitive).size() == 0){

        auto item = new ListWidgetItem(obj["nameRoom"].toString(), obj["idRoom"].toInt());
        ui->listChats->addItem(item);
        if (obj["changeCurrentRoom"].toBool()) ui->listChats->setCurrentItem(item);
    }else{
        bool exist{false};
        for (auto & item : ui->listChats->findItems(obj["nameRoom"].toString(), Qt::MatchCaseSensitive)){
            if (static_cast<ListWidgetItem *>(item)->getId() == obj["idRoom"].toInt()){

                if (obj["changeCurrentRoom"].toBool()){

                    item->setIcon(QIcon());
                }
                exist = true;
                break;
            }
        }
        if (not exist){
            auto item = new ListWidgetItem(obj["nameRoom"].toString(), obj["idRoom"].toInt());
            ui->listChats->addItem(item);
            if (obj["changeCurrentRoom"].toBool()) ui->listChats->setCurrentItem(item);
        }
    }

    if (obj["changeCurrentRoom"].toBool()){

        ui->windowChat->setVisible(true);
        ui->noSelectedChat->setVisible(false);

        ui->nameCurrentChat->setText(obj["nameRoom"].toString());

        ui->listMessage->clear();
        ui->listUserChat->clear();

        std::cout << ui->listUserChat->count() << std::endl;

        for (auto message : obj["messages"].toArray()){
            auto mess = message.toObject();

            if (mess["typeMessage"].toInt() == 0){
                ui->listMessage->setFontUnderline(true);
                ui->listMessage->append("(" + mess["nameUser"].toString() + ")");
                if (mess["nameUser"].toString() == ui->name->text()){
                    ui->listMessage->setAlignment(Qt::AlignRight);
                }else{
                    ui->listMessage->setAlignment(Qt::AlignLeft);
                }
                ui->listMessage->setFontUnderline(false);
                ui->listMessage->insertPlainText(" " + mess["mess"].toString());
            }else if (mess["typeMessage"].toInt() == 1){
                ui->listMessage->append("(" + mess["nameUser"].toString() + ")");
                if (mess["nameUser"].toString() == ui->name->text()){
                    ui->listMessage->setAlignment(Qt::AlignRight);
                }else{
                    ui->listMessage->setAlignment(Qt::AlignLeft);
                }
                DialogChooseSmile dialog;
                QString str{QString("<img src=\"")+dialog.getImage(mess["mess"].toString().toInt())+QString("\" />")};
                ui->listMessage->insertHtml(" " + str);
            }
        }

        for (auto user : obj["usersRoom"].toArray()){
            auto user_obj = user.toObject();

            auto item = new QListWidgetItem(user_obj["nameUser"].toString());
            ui->listUserChat->addItem(item);

            if (user_obj["statusUser"].toInt() == 0){
                item->setIcon(QIcon(":/images/Green.svg"));
            }else{
                item->setIcon(QIcon(":/images/Red.svg"));
            }
        }

    }
}

void Chat::updateUserRoom(QJsonObject obj)
{
    for (auto & chat : ui->listChats->findItems(obj["nameRoom"].toString(), Qt::MatchCaseSensitive)){
        if (static_cast<ListWidgetItem *>(chat)->getId() == obj["idRoom"].toInt()){
            if (ui->listChats->currentItem() == chat){
                auto users = ui->listUserChat->findItems(obj["nameUser"].toString(), Qt::MatchCaseSensitive);
                if (users.size() == 0){

                    auto item = new QListWidgetItem(obj["nameUser"].toString());
                    ui->listUserChat->addItem(item);
                }
                users = ui->listUserChat->findItems(obj["nameUser"].toString(), Qt::MatchCaseSensitive);
                if (obj["statusUser"].toInt() == 0){
                    users.at(0)->setIcon(QIcon(":/images/Green.svg"));
                }else{
                    users.at(0)->setIcon(QIcon(":/images/Red.svg"));
                }
        }
        }
    }
}

void Chat::handleMessage(QJsonObject obj)
{
    for (auto & chat : ui->listChats->findItems(obj["nameRoom"].toString(), Qt::MatchCaseSensitive)){
        if (static_cast<ListWidgetItem *>(chat)->getId() == obj["idRoom"].toInt()){
            if (ui->listChats->currentItem() == chat){

                if (obj["typeMessage"].toInt() == 0){

                    ui->listMessage->setFontUnderline(true);
                    ui->listMessage->append("(" + obj["nameUser"].toString() + ")");
                    if (obj["nameUser"].toString() == ui->name->text()){
                        ui->listMessage->setAlignment(Qt::AlignRight);
                    }else{
                        ui->listMessage->setAlignment(Qt::AlignLeft);
                    }
                    ui->listMessage->setFontUnderline(false);
                    ui->listMessage->insertPlainText(" " + obj["message"].toString());
                }else if (obj["typeMessage"].toInt() == 1){
                    ui->listMessage->append("(" + obj["nameUser"].toString() + ")");
                    if (obj["nameUser"].toString() == ui->name->text()){
                        ui->listMessage->setAlignment(Qt::AlignRight);
                    }else{
                        ui->listMessage->setAlignment(Qt::AlignLeft);
                    }
                    DialogChooseSmile dialog;
                    QString str{QString("<img src=\"")+dialog.getImage(obj["message"].toString().toInt())+QString("\" />")};
                    ui->listMessage->insertHtml(" " + str);
                }
            }else{
//                chat->setBackground(Qt::darkGreen);
                chat->setIcon(QIcon(":/images/Green.svg"));
//                BASS_Init(-1, 44100, 0, 0, NULL);
//                BASS_SetVolume(1);
//                HSAMPLE sample = BASS_SampleLoad(false, "/home/dmitry/dev/Qt/Chat/ChatClient/images/zvuk-dostavlennogo-soobscheniya.wav", 0, 0, 1, BASS_SAMPLE_MONO);
//                HCHANNEL channel=BASS_SampleGetChannel(sample, FALSE);
//                BASS_ChannelPlay(channel, FALSE);
            }
        }
    }

}

void Chat::handleRead(QJsonObject j_obj)
{
    if(j_obj["type"].toString() == "room"){

        if (j_obj["typeRoom"].toString() == "create"){
            updateRoom(j_obj);
        }else if (j_obj["typeRoom"].toString() == "message"){
            handleMessage(j_obj);
        }else if (j_obj["typeRoom"].toString() == "openRoom"){
            updateRoom(j_obj);
        }else if (j_obj["typeRoom"].toString() == "addUser"){
            updateUserRoom(j_obj);
        }else if (j_obj["typeRoom"].toString() == "updateUser"){
            updateUserRoom(j_obj);
        }
    }else if(j_obj["type"].toString() == "info"){
        if (j_obj["typeInfo"].toString() == "addUser"){
            if (dialogSetting != nullptr){
                dialogSetting->setInfo(j_obj["message"].toString());
            }
        }else if (j_obj["typeInfo"].toString() == "allUsers"){
            QStringList list;
            for (auto usr : j_obj["users"].toArray()){
                auto usrObj = usr.toObject();
                if (ui->listUserChat->findItems(usrObj["name"].toString(), Qt::MatchCaseSensitive).size() == 0){

                    list.append(usrObj["name"].toString());
                }
            }
            if (dialogSetting != nullptr){
                dialogSetting->loadUsers(list);
            }
        }
    }
}

void Chat::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter and ui->windowChat->isVisible() == true){
        sendMessage();
    }
}

void Chat::sendMessage()
{
    if (not ui->lineEdit->text().isEmpty()){
        auto text = ui->lineEdit->text();
        auto nameRoom = ui->listChats->currentItem()->text();
        auto idRoom = static_cast<ListWidgetItem *>(ui->listChats->currentItem())->getId();

        QJsonObject obj;
        obj["type"] = "room";
        obj["typeRoom"] = "message";
        obj["nameRoom"] = nameRoom;
        obj["idRoom"] = idRoom;
        obj["text"] = text;
        obj["typeMessage"] = 0;
        emit signalHandleWrite(obj);

        ui->lineEdit->clear();
    }
}

void Chat::sendSmile()
{
    DialogChooseSmile dialog;

    dialog.exec();

    if (static_cast<QDialog::DialogCode>(dialog.result()) == QDialog::DialogCode::Accepted){

        auto nameRoom = ui->listChats->currentItem()->text();
        auto idRoom = static_cast<ListWidgetItem *>(ui->listChats->currentItem())->getId();

        QJsonObject obj;
        obj["type"] = "room";
        obj["typeRoom"] = "message";
        obj["nameRoom"] = nameRoom;
        obj["idRoom"] = idRoom;
        obj["text"] = QString::number(dialog.getSmileId());
        obj["typeMessage"] = 1;
        emit signalHandleWrite(obj);
    }

}

void Chat::changeChat(QListWidgetItem *current)
{
    QJsonObject obj;
    obj["type"] = "room";
    obj["typeRoom"] = "changeRoom";
    obj["nameRoom"] = current->text();
    obj["idRoom"] = static_cast<ListWidgetItem *>(current)->getId();
    emit signalHandleWrite(obj);
}

void Chat::settingsChat()
{
    dialogSetting = new DialogSettingChat;

    QJsonObject obj;
    obj["type"] = "info";
    obj["typeInfo"] = "allUsers";
    emit signalHandleWrite(obj);

    connect(dialogSetting, &DialogSettingChat::signalAddUser, [this](QString nameUser){
        auto idRoom = static_cast<ListWidgetItem *>(ui->listChats->currentItem())->getId();
        QJsonObject obj;
        obj["type"] = "room";
        obj["typeRoom"] = "addUser";
        obj["nameRoom"] = ui->nameCurrentChat->text();
        obj["idRoom"] = idRoom;
        obj["nameUser"] = nameUser;
        emit signalHandleWrite(obj);
    });
    dialogSetting->show();
}
