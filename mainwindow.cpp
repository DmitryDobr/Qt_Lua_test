#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_clearLineEdit1, &QPushButton::clicked, ui->textEdit_playerFile_one, &QTextEdit::clear);
    connect(ui->pushButton_clearLineEdit2, &QPushButton::clicked, ui->textEdit_playerFile_two, &QTextEdit::clear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() // выполнение команды в строке
{
    lua_State *L = luaL_newstate(); // lua машина
    luaL_openlibs(L); // открытие библиотек lua

    string cmd = "a = 7 + 11 + math.sin(23.7)";

    int r = luaL_dostring(L, cmd.c_str()); // выполнение команды, прописанной в строке

    if (r == LUA_OK) // строка выполнилась без ошибок
    {
        lua_getglobal(L, "a"); // глобальная переменная скриптового файла грузится в lua стек

        if (lua_isnumber(L, -1)) // проверка на типизацию данных в стеке по индексу -1
        {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            ui->label->setText(QString::number(a_in_cpp));
        }
    }
    else
    {
        qDebug() << lua_tostring(L, -1);
    }

    lua_close(L);
}

bool CheckLua(lua_State *L, int r) // проверка на ошибки в выполнении скрипта lua
{
    if (r != LUA_OK)
    {
        QString errormsg = lua_tostring(L, -1);
        qDebug() << "ERROR While running lua script " << errormsg;
        return false;
    }
    return true;
}

void MainWindow::on_pushButton_fileOne_clicked() // простой вызов файла
{
    lua_State *L = luaL_newstate(); // lua машина
    luaL_openlibs(L); // открытие библиотек lua

    if (CheckLua(L, luaL_dofile(L, "script.lua"))) // Вызов файла
    {
        lua_getglobal(L, "a");

        if (lua_isnumber(L, -1))
        {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            qDebug() << "a_in_cpp = " << a_in_cpp ;
            ui->label_fileOne->setNum(a_in_cpp);
        }
    }

    lua_close(L);
}

void MainWindow::on_pushButton_PlayerfileLoad1_clicked() // загрузка информации из файла вариант 1
{
    lua_State *L = luaL_newstate(); // lua машина

    if (CheckLua(L, luaL_dofile(L, "PlayerScript.lua"))) // Вызов файла
    {
        // если последовательно извлекать глобалы, в стеке они будут хранится в след.порядке
        lua_getglobal(L, "PlayerName"); // -3
        lua_getglobal(L, "PlayerClass"); // -2
        lua_getglobal(L, "PlayerLevel"); // -1

        Player player1;

        if (lua_isstring(L, -3))
        {
            player1.name = lua_tostring(L, -3);
        }

        if (lua_isstring(L, -2))
        {
            player1.unit_class = lua_tostring(L, -2);
        }

        if (lua_isnumber(L, -1))
        {
            player1.level = (int)lua_tonumber(L, -1);
        }

        ui->textEdit_playerFile_one->append("player1.name = " + player1.name);
        ui->textEdit_playerFile_one->append("player1.class = " + player1.unit_class);
        ui->textEdit_playerFile_one->append("player1.level = " + QString::number(player1.level));
    }

    lua_close(L);
}

void MainWindow::on_pushButton_PlayerfileLoad2_clicked() // загрузка информации из файла вариант 2
{
    lua_State *L = luaL_newstate(); // lua машина

    if (CheckLua(L, luaL_dofile(L, "PlayerScript.lua"))) // Вызов файла
    {
        Player player1;

        lua_getglobal(L, "PlayerName");
        if (lua_isstring(L, -1))
        {
            player1.name = lua_tostring(L, -1);
        }

        lua_getglobal(L, "PlayerClass");
        if (lua_isstring(L, -1))
        {
            player1.unit_class = lua_tostring(L, -1);
        }

        lua_getglobal(L, "PlayerLevel");
        if (lua_isnumber(L, -1))
        {
            player1.level = (int)lua_tonumber(L, -1);
        }

        ui->textEdit_playerFile_one->append("player1.name = " + player1.name);
        ui->textEdit_playerFile_one->append("player1.class = " + player1.unit_class);
        ui->textEdit_playerFile_one->append("player1.level = " + QString::number(player1.level));
    }

    lua_close(L);
}

void MainWindow::on_pushButton_PlayerfileLoad3_clicked() // загрузка информации из файла с использованием таблиц
{
    lua_State *L = luaL_newstate(); // lua машина

    if (CheckLua(L, luaL_dofile(L, "PlayerScript2.lua"))) // Вызов файла
    {
        lua_getglobal(L, "player"); // player это таблица ключей-значений

        if (lua_istable(L, -1)) // проверяем таблица ли это
        {
            Player player1;

            lua_pushstring(L, "Name"); // пробросили вверх стека из таблицы поле Name, таблица теперь в -2
            lua_gettable(L, -2); // таким образом из таблицы можно извлечь значение по ключу
            player1.name = lua_tostring(L, -1);
            lua_pop(L, 1); // удалили самый нижний элемент

            lua_pushstring(L, "Class"); // пробросили вверх стека из таблицы поле Name, таблица теперь в -2
            lua_gettable(L, -2); // таким образом из таблицы можно извлечь значение по ключу
            player1.unit_class = lua_tostring(L, -1);
            lua_pop(L, 1); // удалили самый нижний элемент

            lua_pushstring(L, "Level"); // пробросили вверх стека из таблицы поле Name, таблица теперь в -2
            lua_gettable(L, -2); // таким образом из таблицы можно извлечь значение по ключу
            player1.level = (int)lua_tonumber(L, -1);
            lua_pop(L, 1); // удалили самый нижний элемент

            ui->textEdit_playerFile_two->append("player1.name = " + player1.name);
            ui->textEdit_playerFile_two->append("player1.class = " + player1.unit_class);
            ui->textEdit_playerFile_two->append("player1.level = " + QString::number(player1.level));
        }
    }

    lua_close(L);
}

void MainWindow::on_pushButton_function1_clicked() // использование функции, описанной в скрипте
{
    lua_State *L = luaL_newstate(); // lua машина
    luaL_openlibs(L);
    luaopen_base(L);

    double a = ui->doubleSpinBox_a->value();
    double b = ui->doubleSpinBox_b->value();

    if (CheckLua(L, luaL_dofile(L, "functions.lua")))
    {
        lua_getglobal(L, "AddStuff"); // Имя функции теперь в стеке

        if (lua_isfunction(L, -1)) // а функция ли там лежит?
        {
            lua_pushnumber(L, a);
            lua_pushnumber(L, b);

            // после всех добавлений стек выглядит как:
            // b		(2) -1
            // a		(1) -2
            // AddStuff (0) -3
            // После использования lua_pcall значения будут удалены

            // lua_pcall - аргументы Lua машина, количество аргументов отсчитывая от вершины стека(?), количество ожидаемых результатов, функция

            if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
            {
                qDebug() << "[Qt-C++] Called in lua 'AddStuff(" + QString::number(a) + "," + QString::number(b) + ")', got " << lua_tonumber(L, -1) << "\n";
                ui->label_functionResult1->setNum(lua_tonumber(L, -1));
            }
        }
    }

    lua_close(L);
}

void MainWindow::on_pushButton_PlayerfileLoad4_clicked() // загрузка информации по индексу таблицы
{
    lua_State *L = luaL_newstate(); // lua машина
    luaL_openlibs(L);

    if (CheckLua(L, luaL_dofile(L, "PlayerScript3.lua")))
    {
        lua_getglobal(L, "getPlayer"); // Имя функции теперь в стеке
        if (lua_isfunction(L, -1)) // а функция ли там лежит?
        {
            int index = ui->spinBox_index->value();
            lua_pushnumber(L, index);

            if (CheckLua(L, lua_pcall(L, 1, 1, 0)))
            {
                // lua вернул в стек таблицу с параметрами игрока, находится в -1

                if (lua_istable(L, -1))
                {
                    Player player1;

                    lua_pushstring(L, "Name");
                    lua_gettable(L, -2);
                    player1.name = lua_tostring(L, -1);
                    lua_pop(L, 1);

                    lua_pushstring(L, "Class");
                    lua_gettable(L, -2);
                    player1.unit_class = lua_tostring(L, -1);
                    lua_pop(L, 1);

                    lua_pushstring(L, "Level");
                    lua_gettable(L, -2);
                    player1.level = (int)lua_tonumber(L, -1);
                    lua_pop(L, 1);

                    ui->textEdit_playerFile_three->clear();
                    ui->textEdit_playerFile_three->append("player1.name = " + player1.name);
                    ui->textEdit_playerFile_three->append("player1.class = " + player1.unit_class);
                    ui->textEdit_playerFile_three->append("player1.level = " + QString::number(player1.level));
                }
                else
                {
                    ui->textEdit_playerFile_three->clear();
                    ui->textEdit_playerFile_three->append("No such Record in players table");
                    qDebug() << "[Qt-C++] - Error Lua returned false value type";
                }
            }
        }
    }

    lua_close(L);
}

int lua_HostFunction(lua_State * L) // хост функция
{
    // значения уже есть в стеке
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);

    qDebug() << "[Qt-C++] HostFunction(" << a << ", " << b << ") called";
    float c = a * b;
    lua_pushnumber(L, c);
    return 1; // индекс элемента в стеке
}

void MainWindow::on_pushButton_function2_clicked() // использование хост-функции, зарегистрированной в lua машине
{
    double a = ui->doubleSpinBox_a_2->value();
    double b = ui->doubleSpinBox_b_2->value();

    lua_State *L = luaL_newstate(); // lua машина
    luaL_openlibs(L); // открытие библиотек lua

    lua_register(L, "HostFunction", lua_HostFunction); // регистируем на имя функции, которую будем вызывать в скрипте

    if (CheckLua(L, luaL_dofile(L, "functions.lua")))
    {
        lua_getglobal(L, "DoAThing"); // Имя функции теперь в стеке
        if (lua_isfunction(L, -1)) // а функция ли там лежит?
        {
            lua_pushnumber(L, a);
            lua_pushnumber(L, b);

            if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
            {
                qDebug() << "[Qt-C++] Called in Lua 'DoAThing(" + QString::number(a) + " , " + QString::number(b) + ")' got " << lua_tonumber(L, -1);
                ui->label_functionResult2->setNum(lua_tonumber(L, -1));
            }
        }
    }

    lua_close(L);
}
