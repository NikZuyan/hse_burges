#include "loginwindow.h"
#include <QtWidgets/QApplication>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(":/hse_burges/Nunito-Regular.ttf");
    if (fontId == -1) {
        qWarning() << "No loading Nunito-Regular.ttf";
    }
    else 
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont nunitoFont(family);
        app.setFont(nunitoFont);
    }

    LoginWindow window;
    window.show();
    return app.exec();
}
