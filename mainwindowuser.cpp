#include "mainwindowuser.h"
#include "ui_mainwindowuser.h"
#include "flowlayout.h"
#include "loginwindow.h"


MainWindowUser::MainWindowUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowUser)
{
    ui->setupUi(this);

   
}

MainWindowUser::~MainWindowUser()
{
    delete ui;
}