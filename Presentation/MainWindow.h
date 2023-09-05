//
// Created by Andrei on 26.08.2023.
//

#ifndef APP1_MAINWINDOW_H
#define APP1_MAINWINDOW_H

#include <QMainWindow>
#include "AuthWidgets/LoginWidget.h"
#include "../Observer/ObserverAuth.h"
#include <QSystemTrayIcon>
#include "AuthWidgets/RegisterWidget.h"
#include "MainWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public ObserverAuth{
Q_OBJECT

private:
    Ui::MainWindow *ui;
    LoginWidget* loginWidget;
    RegisterWidget* registerWidget;
    MainWidget* mainWidget;
    void load_window();
    void load_login_widget();

public:
    explicit MainWindow(LoginWidget *loginWidget1, RegisterWidget *registerWidget1, MainWidget *mainWidget1,
                        QWidget *parent= nullptr);
    void update(const std::string &option, const std::string &option2) override;
    ~MainWindow() override;

};


#endif //APP1_MAINWINDOW_H
