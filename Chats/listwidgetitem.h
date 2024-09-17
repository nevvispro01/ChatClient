#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <QListWidgetItem>

class ListWidgetItem : public QListWidgetItem
{
public:
    ListWidgetItem(const QString &text, int id_, QListWidget *parent = nullptr);

    int getId(){return id;}

private:
    int id;
};

#endif // LISTWIDGETITEM_H
