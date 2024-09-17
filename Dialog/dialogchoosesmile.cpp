#include "dialogchoosesmile.h"
#include "ui_dialogchoosesmile.h"
#include <QLabel>

DialogChooseSmile::DialogChooseSmile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseSmile)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(2);

    for (auto row{0}; row<2; ++row){

        for (auto column{0}; column<4; ++column){

            auto item = new QTableWidgetItem(QIcon(smile[row*4 + column]),"");
            ui->tableWidget->setItem(row, column, item);
        }
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->tableWidget, &QTableWidget::itemClicked, [this](QTableWidgetItem *item){

        smileId = (item->row()*4 + item->column());
        accept();
    });
}

DialogChooseSmile::~DialogChooseSmile()
{
    delete ui;
}
