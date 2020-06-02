#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QMessageBox>
#include <vector>

bool b_SELFd = false;

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
    ui->tableWidget_ist->setColumnCount(varCnt + 1);        // кол- во столбцов

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
                zero->setFlags(Qt::ItemIsEnabled);
                ui->tableWidget_ist->setItem(row, i, zero);
                row++;
            }
            for ( ; j < zeros; j++)
            {
                QTableWidgetItem *one = new QTableWidgetItem (QObject::tr("%1").arg(1));
                one->setFlags(Qt::ItemIsEnabled);
                ui->tableWidget_ist->setItem(row, i, one);
                row++;
            }
        }
        zeros = zeros / 2;      // уменьшение кол-ва нулей в столбце вдвое
        coef = pow(2.0, pov);
        pov++;
    }

    ui->tableWidget_ist->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // блокировка для редактирования столбцов
    ui->tableWidget_ist->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);   // блокировка для редактирования рядов
    ui->tableWidget_ist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget_ist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int w = ui->tableWidget_ist->verticalHeader()->width() +4;                                      // считаем размер окна для таблицы (+4 нужно!)
    for (int i = 0; i < ui->tableWidget_ist->columnCount(); i++)                                    // считаем ширину
    {
        w += ui->tableWidget_ist->columnWidth(i);
    }
    int h = ui->tableWidget_ist->horizontalHeader()->height() + 4;
    for (int i = 0; i < ui->tableWidget_ist->rowCount(); i++)                                       // считаем высоту
    {
        h += ui->tableWidget_ist->rowHeight(i);
    }
    QSize p = QSize(w, h);
    ui->tableWidget_ist->setFixedSize(p);                                                           // устанавливаем размер таблицы
}

// Проверка таблицы
bool MainWindow::CheckCorrect(Ui::MainWindow *ui)
{
    // УДАЛЕНИЕ КОЛОНОК ПОСЛЕ САМОДВОЙСТВЕННОСТИ
    if (b_SELFd == true)
    {
        b_SELFd = false;
        ui->tableWidget_ist->removeColumn(ui->tableWidget_ist->columnCount()-1);
        ui->tableWidget_ist->removeColumn(ui->tableWidget_ist->columnCount()-1);
        // считаем размер размеры для таблицы без 2-х колонок
        int w = ui->tableWidget_ist->verticalHeader()->width() +4;                                      // считаем размер окна для таблицы (+4 нужно!)
        for (int i = 0; i < ui->tableWidget_ist->columnCount(); i++)                                    // считаем ширину
        {
            w += ui->tableWidget_ist->columnWidth(i);
        }
        int h = ui->tableWidget_ist->horizontalHeader()->height() + 4;
        for (int i = 0; i < ui->tableWidget_ist->rowCount(); i++)                                       // считаем высоту
        {
            h += ui->tableWidget_ist->rowHeight(i);
        }
        QSize p = QSize(w, h);
        ui->tableWidget_ist->setFixedSize(p);
    }

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
        if (zer0->text() != "0" && one1->text() != "1")
        {
            result.append("not 0-preserving or 1-preserving");  // ничего не сохраняет
        }
        ui->label_res->setText(result); // вывод
    }
}

// Самодвойственная
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
                origin.append(itm->text()); // записываем вектор ф-ции
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
        // устанавливаем размер для таблицы
        int w = ui->tableWidget_ist->verticalHeader()->width() +4;                                      // считаем размер окна для таблицы (+4 нужно!)
        for (int i = 0; i < ui->tableWidget_ist->columnCount(); i++)                                    // считаем ширину
        {
            w += ui->tableWidget_ist->columnWidth(i);
        }
        int h = ui->tableWidget_ist->horizontalHeader()->height() + 4;
        for (int i = 0; i < ui->tableWidget_ist->rowCount(); i++)                                       // считаем высоту
        {
            h += ui->tableWidget_ist->rowHeight(i);
        }
        QSize p = QSize(w, h);
        ui->tableWidget_ist->setMinimumSize(p);
        // Цикл прохода по всем рядкам
        for (int i = 0; i < rowC; i++)
        {
            QTableWidgetItem *item = ui->tableWidget_ist->item(i, (columnC - 2) );
            if (item->text() == "1")
            {
                dual.insert(0, item->text());
                QTableWidgetItem *one = new QTableWidgetItem (QObject::tr("%1").arg(1));
                ui->tableWidget_ist->setItem( (rowC - (i + 1)), columnC-1, one);            // записываем колонку с отрицание в новую колонку в обратном порядке
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

        b_SELFd = true;
    }
}

// ДДНФ
void MainWindow::on_pushButton_SOP_clicked()
{
    if ( CheckCorrect(ui) ) // проверка
    {
        int rows = ui->tableWidget_ist->rowCount();         // рядки
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // колонки, берем с индекса (ф-ция - 1)

        QString SOP = "Result:  ";  // строка результат
        int const_zero = 0;

        // Цикл прохода по рядкам ТИ
        for (int i = 0; i < rows; i++)
        {
            QString for_anal = "";  // для интерпретаций
            QTableWidgetItem *dnf = ui->tableWidget_ist->item(i, clmn );    // берем елемент из последней колокни
            if (dnf->text() == "1") // Если единица
            {
                // Цикл для записывания интерпретаций
                for (int k = 0; k < clmn; k++)
                {
                    QTableWidgetItem *interp = ui->tableWidget_ist->item(i, k);
                    for_anal.append(interp->text());
                }

                for (int clmn = 0;  clmn < for_anal.size(); clmn++)
                {
                    QTableWidgetItem *var_h = ui->tableWidget_ist->horizontalHeaderItem(clmn);
                    if (for_anal[clmn] == "1")
                    {
                        SOP.append(var_h->text());
                    }
                    else
                    {
                        SOP.append(var_h->text() + "'");
                    }
                }

                SOP.append(" || "); // дизюнкция конюнкций
            }
            else
            {
                const_zero++;
            }
        }

        if (const_zero == ui->tableWidget_ist->rowCount())
        {
            ui->label_res->setText("This function is constant of 0. SOP does not exist");
            return;
        }

        SOP.remove( (SOP.length() - 4), 4 );    // удаляем последний символ операции

        QMessageBox::information(this, "Result", SOP);
    }
}

// ДКНФ
void MainWindow::on_pushButton_POS_clicked()
{
    if ( CheckCorrect(ui))      // проверка
    {
        int rows = ui->tableWidget_ist->rowCount();         // рядки
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // колонки, берем с индекса (ф-ция - 1)

        QString POS = "Result:  ";  // строка результат
        int const_one = 0;

        for (int i = 0; i < rows; i++)
        {
            QString for_anal = "";
            QTableWidgetItem *knf = ui->tableWidget_ist->item(i, clmn );    // берем елемент из последней колокни
            if (knf->text() == "0")
            {
                // Цикл для записывания интерпретаций
                for (int k = 0; k < clmn; k++)
                {
                    QTableWidgetItem *interp = ui->tableWidget_ist->item(i, k);
                    for_anal.append(interp->text());
                }

                // Цикл анализа интерпретаций и формирования результата
                for (int clmn = 0;  clmn < for_anal.size(); clmn++)
                {
                    QTableWidgetItem *var_h = ui->tableWidget_ist->horizontalHeaderItem(clmn);
                    if (clmn == 0)
                    {
                        POS.append("(");    // перед первым элементом скобка
                    }
                    if (for_anal[clmn] == "1")
                    {
                        POS.append(var_h->text() + "'" + "\u2228");
                    }
                    else
                    {
                        POS.append(var_h->text() + "\u2228");
                    }
                    if (clmn == ui->tableWidget_ist->columnCount() - 2)
                    {
                        POS.remove( (POS.length() - 1), 1);     // удаляем символ дизъюнкции после последнего
                        POS.append(")");                        // после последнего скобка
                    }
                }
            }
            else
            {
                const_one++; // считаем константу 1
            }
        }
        // если единиц столько же, сколько и рядков
        if (const_one == ui->tableWidget_ist->rowCount())
        {
            ui->label_res->setText("This function is constant of 1. POS does not exist");
            return;
        }
        // вывод результата
        QMessageBox::information(this, "Result", POS);
    }
}

// Проверка на монотонность
void MainWindow::on_pushButton_mono_clicked()
{
    if ( CheckCorrect(ui) )     // проверка
    {
        int clmn = ui->tableWidget_ist->columnCount() - 1;              // последняя колонка с результатом функции
        for (int j = 0; j < ui->tableWidget_ist->rowCount(); j++)       // цикл прохода во рядкам всей ТИ
        {
            QTableWidgetItem *mono = ui->tableWidget_ist->item(j, clmn);
            if (mono->text() == "1")                                // если нашли 1
            {
                QString orig = "";  // строка интерпертация
                for (int k = 0; k < ui->tableWidget_ist->columnCount()-1; k++) // получаем интерпретацию, где 1
                {
                    QTableWidgetItem *test = ui->tableWidget_ist->item(j, k);
                    orig.append(test->text());
                }
                for (int row_c = j + 1; row_c < ui->tableWidget_ist->rowCount(); row_c++)  // цикл прохода по всем строчкам после найденной 1
                {
                    QString comp = "";  // строка, с которой будем сравнивать
                    for (int kol = 0; kol < ui->tableWidget_ist->columnCount() - 1; kol++)    // получаем интерепретацию
                    {
                        QTableWidgetItem *cmp = ui->tableWidget_ist->item(row_c, kol);
                        comp.append(cmp->text());
                    }
                    // Если интерпретации сравнимы
                    if ( Compare_Inter(orig, comp) == true)
                    {
                        QTableWidgetItem *cmp = ui->tableWidget_ist->item(row_c, clmn);
                        // Сравниваем значение функций на этих интерпретациях
                        if (mono->text() > cmp->text()) // если верхняя больше нижней
                        {
                            ui->label_res->setText("This function is not monotonic");   // не монотонная
                            return;
                        }
                    }
                }
            }
        }

        // Если прошли все рядки и условие монотонности не нарушилось
        ui->label_res->setText("This function is monotonic");   // Функция монотонная
    }
}

// Функция проверки: Сравнимы ли интерпретации
bool MainWindow::Compare_Inter(QString orig, QString cmp)
{
    int compare = 0;    // Счетчик сравнений.
    // Каждое значение верхней интерпретации должно быть меньше равно каждому значению нижней интерпретации
    // Цикл сравнения значний
    for (int i = 0; i < orig.length(); i++)
    {
        if ( orig[i] <= cmp[i] ) // условие выполняется
        {
            compare++;  // счетчик++
        }
    }
    // Если счетчик равен кол-ву элементов
    if (compare == orig.length())
    {
        return true;  // интерпретации сравнимы
    }
    else    // иначе
    {
        return false;   // не сравниваем
    }
}

// Построение полинома Жегалкина
void MainWindow::on_pushButton_Polinom_clicked()
{
    if ( CheckCorrect(ui) ) // проверка
    {
        int rows = ui->tableWidget_ist->rowCount();         // получаем рядки
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // колонки

        std::vector<int> line_triag1;       // вектор для сложений по модулю
        std::vector<int> final_line;        // финальный вектор с "левой" стороной треугольника

        // Цикл прохода по ТИ и формирование вектора ф-ции
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem *triag = ui->tableWidget_ist->item(i, clmn);
            bool ok;
            int el = triag->text().toInt(&ok, 10);
            line_triag1.push_back(el);
            if (i == 0)
            {
                final_line.push_back(el);
            }
        }

        // Цикл для сложения по модулю для каждого элемента вектора
        // Пока в векторе находится больше одного элемента
        while (line_triag1.size() != 1)
        {
            line_triag1 = Calc_mod2(line_triag1);   // передаем текущий вектор и получаем новый
            final_line.push_back(line_triag1[0]);
        }

        // Формирование результата
        QString res = "Zhegalkin polynomial:  ";    // Строка
        bool con_zero = true;                       // Переменная для определения константы нуля

        // Цикл прохода по вектору-результату.
        // Начинаем с нулевого, пока не пройдем весь
        for (int k = 0; k < final_line.size(); k++)
        {
            // Если в векторе единица
            if (final_line[k] == 1)
            {
                int row = k;    // Запоминаем индекс в векторе - это рядок в ТИ
                if (row == 0)
                {
                    res.append("1 \u2295 ");        // для (0, 0, ..., 0) - значение 1
                    continue;
                }
                // Цикл для прохода всех чисел интерпретации
                // С нулевой до последней колонки
                for (int clmn = 0; clmn < ui->tableWidget_ist->columnCount()-1; clmn++)
                {
                    QTableWidgetItem *geg = ui->tableWidget_ist->item(row, clmn);
                    if (geg->text() == "1")     // если нашли единицу
                    {
                        // Получаем имя переменной
                        QTableWidgetItem *var_h = ui->tableWidget_ist->horizontalHeaderItem(clmn);
                        res.append(var_h->text());  // Заносим в строку-результат
                        con_zero = false;       // уже не константа
                    }
                }
                res.append(QString::fromUtf8(" \u2295 "));  // символ mod2
            }
        }

        if (con_zero)  // если в результат ничего не было записано
        {
            ui->label_res->setText("Constant of 0. Polynomial is zero");  // Полином - ноль
            return; // выход
        }

        res.remove( (res.length() - 3), 3);  // удаляем последний символ операции
        ui->label_res->setText(res);         // вывод
    }
}

// Ф-ция сложения по модулю 2
// Принимает вектор.
// Возвращает вектор из сум по модулю между текущим и следующим элементами
std::vector<int> MainWindow::Calc_mod2(std::vector<int> line)
{
    std::vector<int> for_ret;
    for (int i = 0; i < line.size() - 1; i++)
    {
        int num = line[i] ^ line[i+1];      // ^ битовая операция, сложение по модулю (исключающие ИЛИ XOR)
        for_ret.push_back(num);             // записываем в конец вектора
    }
    return for_ret;     // возвращаем
}
