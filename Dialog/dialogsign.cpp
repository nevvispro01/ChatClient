#include "dialogsign.h"
#include "ui_dialogsign.h"

DialogSign::DialogSign(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSign)
{
    ui->setupUi(this);
    connect(ui->linkAuth, &QCommandLinkButton::clicked, [this]{
       ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->linkSign, &QCommandLinkButton::clicked, [this]{
       ui->stackedWidget->setCurrentIndex(1);
    });
}

DialogSign::~DialogSign()
{
    delete ui;
}

QString DialogSign::getLogin()
{
    switch (getType()) {
    case TypeAuth::SIGN:
        return ui->loginSign->text();
    case TypeAuth::AUTH:
        return ui->loginAuth->text();
    }
}

TypeAuth DialogSign::getType()
{
    if (ui->stackedWidget->currentIndex() == 0){
        return TypeAuth::SIGN;
    }else{
        return TypeAuth::AUTH;
    }
}

QString DialogSign::getName()
{
    return ui->name->text();
}

QString DialogSign::getPass()
{
    switch (getType()) {
    case TypeAuth::SIGN:
        return ui->passSign->text();
    case TypeAuth::AUTH:
        return ui->passAuth->text();
    }
}
