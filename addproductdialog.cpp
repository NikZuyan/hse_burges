#include "AddProductDialog.h"

AddProductDialog::AddProductDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Добавить продукт");

    // Создаем виджеты
    QLabel *nameLabel = new QLabel("Название:");
    nameEdit = new QLineEdit(this);

    QLabel *priceLabel = new QLabel("Цена:");
    priceEdit = new QDoubleSpinBox(this);
    priceEdit->setPrefix("");
    priceEdit->setRange(0.0, 10000.0); // Устанавливаем диапазон цен

    QPushButton *chooseImageButton = new QPushButton("Выбрать фото", this);
    connect(chooseImageButton, &QPushButton::clicked, this, &AddProductDialog::chooseImage);

    QLabel *imageLabel = new QLabel("Фото:");
    imagePathLabel = new QLabel(this);

    QPushButton *addButton = new QPushButton("Добавить", this);
    connect(addButton, &QPushButton::clicked, this, &AddProductDialog::addProduct);

    QPushButton *cancelButton = new QPushButton("Отмена", this);
    connect(cancelButton, &QPushButton::clicked, this, &AddProductDialog::reject);

    // Создаем макеты
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(priceLabel);
    mainLayout->addWidget(priceEdit);
    mainLayout->addWidget(chooseImageButton);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(imagePathLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

QString AddProductDialog::getProductName() const {
    return nameEdit->text();
}

double AddProductDialog::getProductPrice() const {
    return priceEdit->value();
}

QString AddProductDialog::getImagePath() const {
    return imagePathLabel->text();
}

void AddProductDialog::chooseImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите изображение", "", "Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (!fileName.isEmpty()) {
        imagePathLabel->setText(fileName);
    }
}

void AddProductDialog::addProduct() {
    if (getProductName().isEmpty() || getImagePath().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }
    accept();
}
