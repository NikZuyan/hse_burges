#ifndef MAINWINDOWADMIN_H
#define MAINWINDOWADMIN_H

#include "model.h"

#include <QMainWindow>
#include <QList>
#include <QVBoxLayout>

namespace Ui {
class MainWindowAdmin;
}

class MainWindowAdmin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowAdmin(QWidget *parent = nullptr);
    ~MainWindowAdmin();

private slots:
    void on_btnAddCat_clicked();
     void deleteCategory(int categoryId);
    void deleteProduct(int productId);
     void editProduct(Product product);
      void addProduct(int categoryId);



private:
    Ui::MainWindowAdmin *ui;
    void initializeCategories();
    QWidget* categoriesContainer;
    public:
    QList<Category> getCategories();
        void openAddProductDialog(int categoryId);
};

#endif // MAINWINDOWADMIN_H
