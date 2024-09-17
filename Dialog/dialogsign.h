#ifndef DIALOGSIGN_H
#define DIALOGSIGN_H

#include <QDialog>

namespace Ui {
class DialogSign;
}

enum class TypeAuth{
    SIGN,
    AUTH
};

class DialogSign : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSign(QWidget *parent = nullptr);
    ~DialogSign();

    QString getLogin();
    TypeAuth getType();
    QString getName();
    QString getPass();

private:
    Ui::DialogSign *ui;
};

#endif // DIALOGSIGN_H
