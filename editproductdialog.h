#ifndef EDITPRODUCTDIALOG_H
#define EDITPRODUCTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>

class EditProductDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditProductDialog(QWidget *parent = nullptr);

    QString getProductName() const;
    double getProductPrice() const;

    void setProductName(const QString &name);
    void setProductPrice(double price);

private slots:
    void saveProduct();

private:
    QLineEdit *nameEdit; // Поле для ввода названия продукта
    QDoubleSpinBox *priceEdit; // Поле для ввода цены продукта
};

#endif // EDITPRODUCTDIALOG_H
