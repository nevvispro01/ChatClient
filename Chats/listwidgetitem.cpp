#include "listwidgetitem.h"

ListWidgetItem::ListWidgetItem(const QString &text, int id_, QListWidget *parent) : QListWidgetItem(text, parent),
    id(id_)
{

}
