#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_btnRegister_clicked();

    void on_btnLogin_clicked();

private:
    Ui::LoginWindow *ui;

public:
    static bool connectToDatabase();
};

#endif // LOGINWINDOW_H
