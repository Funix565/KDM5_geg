#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QMessageBox>
#include <QDebug>

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

// Проверка таблицы
bool MainWindow::CheckCorrect(Ui::MainWindow *ui)
{
    // Если не выбрано кол-во переменных и таблица не создана
    if (ui->radioButton_2vars->isChecked() == false && ui->radioButton_3vars->isChecked() == false && ui->radioButton_4vars->isChecked() == false)
    {
        QMessageBox::warning(this, "Error", "Choose amount of vars");
        return false;
    }

    int rCount = ui->tableWidget_ist->rowCount();
    int clmn = ui->tableWidget_ist->columnCount();

    // Цикл проверки всех элементов
    for (int i = 0; i < rCount; i++)
    {
        QTableWidgetItem *itm = ui->tableWidget_ist->item(i, (clmn - 1));    // -1, индексация с 0
        if (itm == nullptr)  // если ячейка пустая
        {
            QMessageBox::warning(this, "Error", "Invalid numbers in table");
            return false;
        }
        if (itm->text() != "1" && itm->text() != "0") // если ячейка не ноль и не один
        {
            QMessageBox::warning(this, "Error", "Invalid numbers in table");
            return false;
        }
    }
    return true;  // успех
}

// Сохраняет 0, сохраняет 1
void MainWindow::on_pushButton_save01_clicked()
{
    if ( CheckCorrect(ui) )  // проверка таблицы истинности
    {
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // последняя колонка
        int row = ui->tableWidget_ist->rowCount() - 1;      // последний рядок

        QTableWidgetItem *zer0 = ui->tableWidget_ist->item(0, clmn);    // (0, 0, ..., 0)
        QTableWidgetItem *one1 = ui->tableWidget_ist->item(row, clmn);  // (1, 1, ..., 1)

        QString result = "Result: The function is  ";
        if (zer0->text() == "0")
        {
            result.append("0-preserving  ");    // сохраняет 0
        }
        if (one1->text() == "1")
        {
            result.append("1-preserving");      // сохраняет 1
        }
        if (zer0->text() != "0" && one1->text() != "1")         // Другое условие, это иф тупой
        {
            result.append("not 0-preserving or 1-preserving");  // ничего не сохраняет
        }
        ui->label_res->setText(result); // вывод
    }
}

// Самодвойственная?
void MainWindow::on_pushButton_samo_clicked()
{
    if ( CheckCorrect(ui) )  // проверка таблицы истинности
    {
        ui->tableWidget_ist->insertColumn(ui->tableWidget_ist->columnCount()); // вставляем колонку для НЕ
        ui->tableWidget_ist->setHorizontalHeaderItem(ui->tableWidget_ist->columnCount()-1, new QTableWidgetItem("F'"));

        // Строки исходной ф-ции и двойственной
        QString origin = "";
        QString dual ="";

        int rowC = ui->tableWidget_ist->rowCount();         // кол-во рядков
        int columnC = ui->tableWidget_ist->columnCount();   // кол-во колонок

        // Цикл прохода по всем рядкам
        for (int i = 0; i < rowC; i++)
        {
            QTableWidgetItem *itm = ui->tableWidget_ist->item(i, (columnC - 2) );  // обращаемся к колонке с ф-цией
            // Помещаем в новую колонку результат отрицания
            if (itm->text() == "1")
            {
                origin.append(itm->text()); // запис?ваем вектор ф-ции
                QTableWidgetItem *zer0 = new QTableWidgetItem (QObject::tr("%1").arg(0));
                ui->tableWidget_ist->setItem(i, columnC-1, zer0);
            }
            else if (itm->text() == "0")
            {
                origin.append(itm->text());
                QTableWidgetItem *one1 = new QTableWidgetItem (QObject::tr("%1").arg(1));
                ui->tableWidget_ist->setItem(i, columnC-1, one1);
            }
        }

        ui->tableWidget_ist->insertColumn(ui->tableWidget_ist->columnCount());      // вставляем колонку для двойственности
        ui->tableWidget_ist->setHorizontalHeaderItem(ui->tableWidget_ist->columnCount()-1, new QTableWidgetItem("F*"));
        columnC = ui->tableWidget_ist->columnCount();

        // Цикл прохода по всем рядкам
        for (int i = 0; i < rowC; i++)
        {
            QTableWidgetItem *item = ui->tableWidget_ist->item(i, (columnC - 2) );
            if (item->text() == "1")
            {
                dual.insert(0, item->text());
                QTableWidgetItem *one = new QTableWidgetItem (QObject::tr("%1").arg(1));
                ui->tableWidget_ist->setItem( (rowC - (i + 1)), columnC-1, one);            // запис?ваем колонку с отрицание в новую колонку в обратном порядке
            }
            else if (item->text() == "0")
            {
                dual.insert(0, item->text());
                QTableWidgetItem *zero = new QTableWidgetItem (QObject::tr("%1").arg(0));
                ui->tableWidget_ist->setItem( (rowC - (i + 1)), columnC-1, zero);
            }
        }

        // Сравниваем ф-цию и двойственную ей.
        if (origin.compare(dual) == 0)
        {
            ui->label_res->setText("Resul:  is self-dual");  // Если равны - ф-ция самодвойственна
        }
        else
        {
            ui->label_res->setText("Resul:  is not self-dual");
        }
    }


}









































