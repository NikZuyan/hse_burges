#include "loginwindow.h"
#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlDatabase>


RegisterWindow::RegisterWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_btnLogin_clicked()
{
    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->show();
    this->hide();
}

void RegisterWindow::on_btnRegister_clicked()
{
    if (!LoginWindow::connectToDatabase())
        return;

    QString login = ui->txtLogin->text().trimmed();
    QString password = ui->txtPassword->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля!");
        return;
    }

    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    QSqlDatabase db = QSqlDatabase::database("my_connection");

    // Проверяем, что логин уникален
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", login);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Ошибка запроса:\n" + checkQuery.lastError().text());
        return;
    }
    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким логином уже существует!");
        return;
    }

    // Вставляем пользователя
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO users (username, password_hash, role) VALUES (:username, :password_hash, 'user')");
    insertQuery.bindValue(":username", login);
    insertQuery.bindValue(":password_hash", passwordHash);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось зарегистрировать пользователя:\n" + insertQuery.lastError().text());
        return;
    }

    ui->txtLogin->clear();
    ui->txtPassword->clear();

    LoginWindow *loginWindow = new LoginWindow();
    loginWindow->show();
    this->hide();

    QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");

}

