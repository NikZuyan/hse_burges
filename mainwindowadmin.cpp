#include "addproductdialog.h"
#include "editproductdialog.h"
#include "flowlayout.h"
#include "loginwindow.h"
#include "mainwindowadmin.h"
#include "ui_mainwindowadmin.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QBuffer>


MainWindowAdmin::MainWindowAdmin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowAdmin)
{
    ui->setupUi(this);


    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    categoriesContainer = ui->categoriesContainer;
    QVBoxLayout* containerLayout = new QVBoxLayout(categoriesContainer);
    categoriesContainer->setLayout(containerLayout);

    scrollArea->setWidget(categoriesContainer);


    if (ui->centralwidget->layout())
        ui->centralwidget->layout()->addWidget(scrollArea);
    else {
        QVBoxLayout* layout = new QVBoxLayout(ui->centralwidget);
        layout->setContentsMargins(0, 85, 0, 0);
        layout->addWidget(scrollArea);
    }
    initializeCategories();
}

MainWindowAdmin::~MainWindowAdmin()
{
    delete ui;
}

void MainWindowAdmin::initializeCategories() {
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(categoriesContainer->layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout(categoriesContainer);
        categoriesContainer->setLayout(mainLayout);
    }


    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        if (item->widget())
            delete item->widget();
        delete item;
    }

    for (const Category& category : MainWindowAdmin::getCategories()) {

        QWidget* labelContainer = new QWidget(categoriesContainer);
        QHBoxLayout* labelLayout = new QHBoxLayout(labelContainer);
        labelLayout->setContentsMargins(0, 25, 0, 10);
        labelLayout->setSpacing(10);

        QLabel* categoryLabel = new QLabel(category.name, labelContainer);
        QFont font = categoryLabel->font();
        font.setBold(true);
        font.setPointSize(14);
        categoryLabel->setFont(font);


        QPushButton* deleteButton = new QPushButton("Удалить", labelContainer);
deleteButton->setStyleSheet(
          "QPushButton {"
          "  background-color: #e4002b;"
          "  color: white;"
          "  border: none;"
          "  border-radius: 4px;"
          "  font-size: 8pt;"
          "}"
          "QPushButton:hover {"
          "  background-color: #e6002b;"
          "}"
          );
        deleteButton->setFixedSize(80, 30);
        connect(deleteButton, &QPushButton::clicked, [this, category]() {
            deleteCategory(category.id);
        });


        labelLayout->addWidget(categoryLabel);
        labelLayout->addStretch();
        labelLayout->addWidget(deleteButton);

        mainLayout->addWidget(labelContainer);


        QWidget* productsWidget = new QWidget(categoriesContainer);
        FlowLayout* productsLayout = new FlowLayout(productsWidget, 0, 10, 10);
        productsLayout->setSpacing(10);
        productsLayout->setContentsMargins(0, 0, 0, 0);

          for (const Product& product : category.products) {
              QWidget* productWidget = new QWidget(productsWidget);
              productWidget->setFixedSize(235, 235);
              productWidget->setStyleSheet(
                  "background-color: white;"
                  "border: 1px solid #ccc;"
                  "border-radius: 8px;"
                  "padding: 8px;"
                  );

              QVBoxLayout* productLayout = new QVBoxLayout(productWidget);
              productLayout->setSpacing(5);
              productLayout->setContentsMargins(5, 5, 5, 5);

              QLabel* imageLabel = new QLabel(productWidget);
              imageLabel->setStyleSheet("border: none;");
              QPixmap pixmap;
              if (pixmap.loadFromData(product.imageData)) {
                  imageLabel->setPixmap(pixmap.scaled(150, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
              } else {
                  imageLabel->setText("No Image");
              }
              imageLabel->setAlignment(Qt::AlignCenter);

              productLayout->addWidget(imageLabel);

              QHBoxLayout* namePriceLayout = new QHBoxLayout();
              QLabel* nameLabel = new QLabel(product.name, productWidget);
              nameLabel->setStyleSheet("border: none;");
              nameLabel->setAlignment(Qt::AlignLeft);
              nameLabel->setWordWrap(true);
              QFont nameFont = nameLabel->font();
              nameFont.setBold(false);
              nameLabel->setFont(nameFont);

              QLabel* priceLabel = new QLabel(product.price, productWidget);
              priceLabel->setStyleSheet("border: none;");
              priceLabel->setAlignment(Qt::AlignRight);
              QFont priceFont = priceLabel->font();
              priceFont.setPointSize(10);
              priceFont.setItalic(true);
              priceFont.setBold(true);
              priceLabel->setFont(priceFont);

              namePriceLayout->addWidget(nameLabel);
              namePriceLayout->addWidget(priceLabel);
              productLayout->addLayout(namePriceLayout);

              QHBoxLayout* buttonLayout = new QHBoxLayout();
              QPushButton* editButton = new QPushButton("Редактировать", productWidget);
              connect(editButton, &QPushButton::clicked, [this, product]() {
                  editProduct(product);
              });
              editButton->setFixedHeight(24);
              editButton->setStyleSheet(
                  "QPushButton {"
                  "  background-color: #0078d7;"
                  "  color: white;"
                  "  border: none;"
                  "  border-radius: 4px;"
                  "  font-size: 8pt;"
                  "}"
                  "QPushButton:hover {"
                  "  background-color: #005a9e;"
                  "}"
                  );

              QPushButton* removeButton = new QPushButton("Удалить", productWidget);
              connect(removeButton, &QPushButton::clicked, [this, product]() {
                  deleteProduct(product.id);
              });
              removeButton->setFixedHeight(24);
              removeButton->setStyleSheet(
                  "QPushButton {"
                  "  background-color: #e4002b;"
                  "  color: white;"
                  "  border: none;"
                  "  border-radius: 4px;"
                  "  font-size: 8pt;"
                  "}"
                  "QPushButton:hover {"
                  "  background-color: #e6002b;"
                  "}"
                  );

              buttonLayout->addWidget(editButton);
              buttonLayout->addWidget(removeButton);
              productLayout->addLayout(buttonLayout);

              productsLayout->addWidget(productWidget);
          }

        QPushButton* addButton = new QPushButton("+", productsWidget);
          connect(addButton, &QPushButton::clicked, [this, category]() {
              addProduct(category.id);
          });
        addButton->setFixedSize(40, 40);
        addButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #28a745;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 20px;"
            "  font-size: 24pt;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  background-color: #218838;"
            "}"
            );

        productsLayout->addWidget(addButton);
        mainLayout->addWidget(productsWidget);
    }
}

void MainWindowAdmin::addProduct(int categoryId) {
    openAddProductDialog(categoryId);
}


void MainWindowAdmin::deleteCategory(int categoryId) {

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Подтверждение удаления",
        "Вы уверены, что хотите удалить эту категорию?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) {
        return;
    }

    if (!LoginWindow::connectToDatabase())
        return;

    QSqlDatabase db = QSqlDatabase::database("my_connection");
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM categories WHERE id = :id");
    deleteQuery.bindValue(":id", categoryId);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить категорию\n" + deleteQuery.lastError().text());
        return;
    }

    initializeCategories();

    QMessageBox::information(this, "Успех", "Категория успешно удалена!");
}


void MainWindowAdmin::deleteProduct(int productId) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Подтверждение удаления",
        "Вы уверены, что хотите удалить этот продукт?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) {
        return;
    }

    if (!LoginWindow::connectToDatabase())
        return;

    QSqlDatabase db = QSqlDatabase::database("my_connection");
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM menu_items WHERE id = :id");
    deleteQuery.bindValue(":id", productId);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить продукт\n" + deleteQuery.lastError().text());
        return;
    }

    initializeCategories();

    QMessageBox::information(this, "Успех", "Продукт успешно удален!");
}

void MainWindowAdmin::editProduct(Product product) {
    EditProductDialog dialog(this);

    double price = 0.0;
    bool ok = false;

    price = product.price.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Некорректное значение цены продукта");
        return;
    }

    dialog.setProductName(product.name);
    dialog.setProductPrice(price);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getProductName();
        double newPrice = dialog.getProductPrice();

        if (!LoginWindow::connectToDatabase())
            return;

        QSqlDatabase db = QSqlDatabase::database("my_connection");
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Ошибка", "База данных не открыта");
            return;
        }

        QSqlQuery updateQuery(db);
        updateQuery.prepare("UPDATE menu_items SET name = :name, price = :price WHERE id = :id");
        updateQuery.bindValue(":name", newName);
        updateQuery.bindValue(":price", newPrice);
        updateQuery.bindValue(":id", product.id);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Ошибка", "Не удалось обновить информацию\n" + updateQuery.lastError().text());
            return;
        }

        initializeCategories();

        QMessageBox::information(this, "Успех", "Информация обновлена!");
    }
}




void MainWindowAdmin::on_btnAddCat_clicked()
{
    QString categorie = ui->txtCat->text().trimmed();

    if (categorie.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните поле!");
        return;
    }

    if (!LoginWindow::connectToDatabase())
        return;

     QSqlDatabase db = QSqlDatabase::database("my_connection");
     QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO categories (name) VALUES (:name)");
    insertQuery.bindValue(":name", categorie);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить новую категорию\n" + insertQuery.lastError().text());
        return;
    }

    ui->txtCat->clear();

     initializeCategories();

    QMessageBox::information(this, "Успех", "Новая категория успешно добавлена!");


}

QList<Category> MainWindowAdmin::getCategories() {
    QList<Category> categories;

    if (!LoginWindow::connectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных.");
        return categories;
    }

    QSqlDatabase db = QSqlDatabase::database("my_connection");
    QSqlQuery categoryQuery(db);

    categoryQuery.prepare("SELECT id, name FROM categories");

    if (!categoryQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось получить категории\n" + categoryQuery.lastError().text());
        return categories;
    }

    while (categoryQuery.next()) {
        int categoryId = categoryQuery.value(0).toInt();
        QString categoryName = categoryQuery.value(1).toString();


        QList<Product> products;

        QSqlQuery productQuery(db);
        productQuery.prepare("SELECT id, name, price, image_data FROM menu_items WHERE category_id = :categoryId");
        productQuery.bindValue(":categoryId", categoryId);

        if (productQuery.exec()) {
            while (productQuery.next()) {
                int productId = productQuery.value(0).toInt();
                QString productName = productQuery.value(1).toString();
                QString productPrice = productQuery.value(2).toString();
                QByteArray imageData = productQuery.value(3).toByteArray();

                products.append(Product{productId, productName, productPrice, imageData});
            }
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось получить продукты\n" + productQuery.lastError().text());
        }

        categories.append(Category(categoryId, categoryName, products));
    }

    return categories;
}

void MainWindowAdmin::openAddProductDialog(int categoryId) {
    AddProductDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString productName = dialog.getProductName();
        double productPrice = dialog.getProductPrice();

        if (!LoginWindow::connectToDatabase())
            return;

        QString imagePath = dialog.getImagePath();
        QImage image(imagePath);

        QByteArray imageData;
        if (image.isNull()) {
            qWarning() << "Не удалось загрузить изображение из пути:" << imagePath;
        } else {
            QBuffer buffer(&imageData);
            if (buffer.open(QIODevice::WriteOnly)) {
                image.save(&buffer, "PNG");
                buffer.close();
            } else {
                qWarning() << "Не удалось открыть буфер для записи изображения";
            }
        }

        QSqlDatabase db = QSqlDatabase::database("my_connection");
        if (!db.isOpen()) {
            qWarning() << "База данных не открыта";
            return;
        }

        QSqlQuery insertQuery(db);
        insertQuery.prepare("INSERT INTO menu_items (image_data, name, category_id, price) VALUES (:image_data, :name, :category_id, :price)");
        insertQuery.bindValue(":image_data", imageData);
        insertQuery.bindValue(":name", productName);
        insertQuery.bindValue(":category_id", categoryId);
        insertQuery.bindValue(":price", productPrice);

        if (!insertQuery.exec()) {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить новый товар\n" + insertQuery.lastError().text());
            return;
        }

        initializeCategories();

        QMessageBox::information(this, "Успех",
                                 QString("Продукт '%1' добавлен.").arg(productName));
    }
}


