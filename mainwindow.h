#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

extern "C"
{
    #include "lua536/include/lua.h"
    #include "lua536/include/lauxlib.h"
    #include "lua536/include/lua.hpp"
    #include "lua536/include/luaconf.h"
    #include "lua536/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "lua536/liblua53.a")
#endif

namespace Ui {
class MainWindow;
}

struct Player {
    QString name;
    QString unit_class;
    int level;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_fileOne_clicked();

    void on_pushButton_PlayerfileLoad1_clicked();

    void on_pushButton_PlayerfileLoad2_clicked();

    void on_pushButton_PlayerfileLoad3_clicked();

    void on_pushButton_function1_clicked();

    void on_pushButton_PlayerfileLoad4_clicked();

    void on_pushButton_function2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
