#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

class Category {
public:
    int id; // Идентификатор категории
    QString name; // Название категории

    Category(int id, const QString& name) : id(id), name(name) {}
};

#endif // CATEGORY_H
