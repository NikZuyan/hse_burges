#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QByteArray>
#include <QList>

struct Product {
    int id;
    QString name;
    QString price;
    QByteArray imageData;

    Product() : id(0), name(), price(), imageData() {}

    Product(int id, QString name,  QString price, QByteArray imageData) : id(id), name(name), price(price), imageData(imageData) {}
};

struct Category {
    int id;
    QString name;
    QList<Product> products;

    Category(int id, QString name,  QList<Product> products) : id(id), name(name), products(products) {}
};

#endif // MODELS_H
