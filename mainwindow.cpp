#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // connect'ы для выбора переменных и построения таблицы истинности
    connect (ui->radioButton_2vars, SIGNAL(clicked()), this, SLOT(ChosenRadio()));
    connect (ui->radioButton_3vars, SIGNAL(clicked()), this, SLOT(ChosenRadio()));
    connect (ui->radioButton_4vars, SIGNAL(clicked()), this, SLOT(ChosenRadio()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Ф-ция построение таблицы
void MainWindow::ChosenRadio()
{
    ui->tableWidget_ist->clear();

    QRadioButton *rb = (QRadioButton *)sender();    // сигнал от кнопки

    int varCnt;

    // В условиях анализируем имя объекта и выбирамем кол-во переменных
    if (rb->objectName() == "radioButton_2vars")
    {
        varCnt = 2;
    }
    else if (rb->objectName() == "radioButton_3vars")
    {
        varCnt = 3;
    }
    else if (rb->objectName() == "radioButton_4vars")
    {
        varCnt = 4;
    }

    ui->tableWidget_ist->setRowCount( pow(2.0, varCnt) );   // кол-во строк
    ui->tableWidget_ist->setColumnCount(varCnt + 1);        // кол- во столбцов   // TODO: insert FOR self-dual; then remove FOR self-dual

    char str_vars[4] = {'X', 'Y', 'Z', 'T'};                // массив имен переменных
    QStringList headersClmn;
    QStringList headersRow;

    // Заполняем строки числами
    for (int i = 0; i < ui->tableWidget_ist->rowCount(); i++)
    {
        ui->tableWidget_ist->setVerticalHeaderLabels(headersRow << QString::number(i));
    }
    // Заполняем столбцы именами переменных
    for (int j = 0; j < ui->tableWidget_ist->columnCount() - 1; j++)
    {
        ui->tableWidget_ist->setHorizontalHeaderLabels(headersClmn << (QString)str_vars[j]);
    }
    ui->tableWidget_ist->setHorizontalHeaderLabels(headersClmn << "F"); // Столбец для ввода функции

    // Заполнение нулями и единицами
    int zeros = pow(2.0, varCnt);
    int coef = 1;   // кол-во выводов
    int pov = 1;    // начальная степень

    for (int i = 0; i < varCnt; i++)    // столбцы
    {
        int row = 0;
        for (int k = 0; k < coef; k++)  // строки
        {
            int j;
            for (j = 0; j < zeros / 2; j++)
            {
                QTableWidgetItem *zero = new QTableWidgetItem (QObject::tr("%1").arg(0));
                ui->tableWidget_ist->setItem(row, i, zero);
                row++;
            }
            for ( ; j < zeros; j++)
            {
                QTableWidgetItem *one = new QTableWidgetItem (QObject::tr("%1").arg(1));
                ui->tableWidget_ist->setItem(row, i, one);
                row++;
            }
        }
        zeros = zeros / 2;      // уменьшение кол-ва нулей в столбце вдвое
        coef = pow(2.0, pov);
        pov++;
    }

    /* РАСПИШИ ТУТ, КАК НАДО. ЭТО КОД С ЛАБЫ ПО КАЛЬКУЛЯТОРУ
     *
     *
    ui->tableWidget_tab->setEditTriggers(QAbstractItemView::NoEditTriggers);                            // блокировка ячеек
    //ui->tableWidget_tab->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // блокировка для редактирования столбцов
    //ui->tableWidget_tab->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);   // блокировка для редактирования рядов
    ui->tableWidget_tab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget_tab->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int w = ui->tableWidget_tab->verticalHeader()->width() +4;                                      // считаем размер окна для таблицы (+4 нужно!)
    for (int i = 0; i < ui->tableWidget_tab->columnCount(); i++)                                    // считаем ширину
    {
        w += ui->tableWidget_tab->columnWidth(i);
    }
    int h = ui->tableWidget_tab->horizontalHeader()->height() + 4;
    for (int i = 0; i < ui->tableWidget_tab->rowCount(); i++)                                       // считаем высоту
    {
        h += ui->tableWidget_tab->rowHeight(i);
    }
    QSize p = QSize(w, h);
    ui->tableWidget_tab->setFixedSize(p);                                                           // устанавливаем размер таблицы
    *
    *
    * РАСПИШИ ТАМ, КАК НАДО. ЭТО КОД С ЛАБЫ ПО КАЛЬКУЛЯТОРУ */
}


















