#include "EditProductDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

EditProductDialog::EditProductDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Редактировать продукт");

    // Создаем виджеты
    QLabel *nameLabel = new QLabel("Название:");
    nameEdit = new QLineEdit(this);

    QLabel *priceLabel = new QLabel("Цена:");
    priceEdit = new QDoubleSpinBox(this);
    priceEdit->setRange(0.0, 10000.0);
    priceEdit->setDecimals(2);
    priceEdit->setPrefix("");
    priceEdit->setSuffix(" руб.");

    // Кнопки
    QPushButton *saveButton = new QPushButton("Сохранить", this);
    QPushButton *cancelButton = new QPushButton("Отмена", this);

    connect(saveButton, &QPushButton::clicked, this, &EditProductDialog::saveProduct);
    connect(cancelButton, &QPushButton::clicked, this, &EditProductDialog::reject);

    // Макеты
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(priceLabel);
    mainLayout->addWidget(priceEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

QString EditProductDialog::getProductName() const {
    return nameEdit->text();
}

double EditProductDialog::getProductPrice() const {
    return priceEdit->value();
}

void EditProductDialog::setProductName(const QString &name) {
    nameEdit->setText(name);
}

void EditProductDialog::setProductPrice(double price) {
    priceEdit->setValue(price);
}

void EditProductDialog::saveProduct() {
    if (getProductName().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите название продукта.");
        return;
    }


    accept();
}
