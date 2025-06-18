#ifndef MAINWINDOWUSER_H
#define MAINWINDOWUSER_H

#include <QMainWindow>

namespace Ui {
class MainWindowUser;
}


class MainWindowUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowUser(QWidget *parent = nullptr);
    ~MainWindowUser();

private slots:

private:
    Ui::MainWindowUser *ui;

};

#endif // MAINWINDOWUSER_H
