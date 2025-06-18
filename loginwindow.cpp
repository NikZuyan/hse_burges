#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include "registerwindow.h"
#include "mainwindowadmin.h"
#include "mainwindowuser.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlDatabase>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::connectToDatabase()
{
    if (QSqlDatabase::contains("my_connection")) {
        QSqlDatabase db = QSqlDatabase::database("my_connection");
        if (db.isOpen())
            return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "my_connection");
    db.setHostName("localhost");        // адрес сервера базы данных
    db.setPort(5432);                   // порт PostgreSQL
    db.setDatabaseName("HSE_burges"); // имя вашей базы
    db.setUserName("postgres");          // имя пользователя
    db.setPassword("qazwsx11009922");      // пароль

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось подключиться к базе данных:\n" + db.lastError().text());
        return false;
    }
    return true;
}

void LoginWindow::on_btnRegister_clicked()
{
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();
    this->hide();
}


void LoginWindow::on_btnLogin_clicked()
{
    QString login = ui->txtLogin->text().trimmed();
    QString password = ui->txtPassword->text();

    // Проверка пустых полей
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля!");
        return;
    }

    if (!connectToDatabase())
        return;

    // Хэширование пароля
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    // Получение соединения с базой
    QSqlDatabase db = QSqlDatabase::database("my_connection");
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Соединение с базой данных не установлено!");
        return;
    }

    // Подготовка запроса
    QSqlQuery query(db);
    query.prepare("SELECT id, role FROM users WHERE username = :username AND password_hash = :password_hash");
    query.bindValue(":username", login);
    query.bindValue(":password_hash", passwordHash);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка", "Ошибка при выполнении запроса:\n" + query.lastError().text());
        return;
    }

    // Проверка результата
    if (query.next()) {

        int userId = query.value("id").toInt();
        QString role = query.value("role").toString();

        if(role=="admin"){
            MainWindowAdmin *mainWindowAdmin = new MainWindowAdmin();
            mainWindowAdmin->show();
            this->hide();
        }
        else
        {
            MainWindowUser *mainWindowUser = new MainWindowUser();
            mainWindowUser->show();
            this->hide();
        }



        QMessageBox::information(this, "Успех", "Вход выполнен! Роль пользователя: " + role);

    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль!");
    }
}


