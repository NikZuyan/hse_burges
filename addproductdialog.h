#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

class AddProductDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddProductDialog(QWidget *parent = nullptr);

    QString getProductName() const;
    double getProductPrice() const;
    QString getImagePath() const;

private slots:
    void chooseImage();
    void addProduct();

private:
    QLineEdit *nameEdit;
    QDoubleSpinBox *priceEdit;
    QLabel *imagePathLabel;
};

#endif // ADDPRODUCTDIALOG_H
