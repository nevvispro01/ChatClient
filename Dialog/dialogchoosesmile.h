#ifndef DIALOGCHOOSESMILE_H
#define DIALOGCHOOSESMILE_H

#include <QDialog>
#include <QMap>

namespace Ui {
class DialogChooseSmile;
}

class DialogChooseSmile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseSmile(QWidget *parent = nullptr);
    ~DialogChooseSmile();

    int & getSmileId(){ return smileId; }
    QString & getImage(int id){ return smile[id];}

private:
    Ui::DialogChooseSmile *ui;
    int smileId;
    QMap<int, QString> smile{{0, ":/images/smile1.png"},
                             {1, ":/images/smile2.png"},
                             {2, ":/images/smile3.png"},
                             {3, ":/images/smile4.png"},
                             {4, ":/images/smile5.png"},
                             {5, ":/images/smile6.png"},
                             {6, ":/images/smile7.png"},
                             {7, ":/images/smile8.png"}};
};

#endif // DIALOGCHOOSESMILE_H
