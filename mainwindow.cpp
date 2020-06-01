#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QMessageBox>
#include <vector>
#include <QDebug>

bool b_SELFd = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // connect'� ��� �롮� ��६����� � ����஥��� ⠡���� ��⨭����
    connect (ui->radioButton_2vars, SIGNAL(clicked()), this, SLOT(ChosenRadio()));
    connect (ui->radioButton_3vars, SIGNAL(clicked()), this, SLOT(ChosenRadio()));
    connect (ui->radioButton_4vars, SIGNAL(clicked()), this, SLOT(ChosenRadio()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// �-�� ����஥��� ⠡����
void MainWindow::ChosenRadio()
{
    // �������� ������� ����� ������������������
    if (b_SELFd == true)
    {
        b_SELFd = false;
        ui->tableWidget_ist->removeColumn(ui->tableWidget_ist->columnCount()-1);
        ui->tableWidget_ist->removeColumn(ui->tableWidget_ist->columnCount()-1);
    }

    ui->tableWidget_ist->clear();

    QRadioButton *rb = (QRadioButton *)sender();    // ᨣ��� �� ������

    int varCnt;

    // � �᫮���� ��������㥬 ��� ��ꥪ� � �롨ࠬ�� ���-�� ��६�����
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

    ui->tableWidget_ist->setRowCount( pow(2.0, varCnt) );   // ���-�� ��ப
    ui->tableWidget_ist->setColumnCount(varCnt + 1);        // ���- �� �⮫�殢

    char str_vars[4] = {'X', 'Y', 'Z', 'T'};                // ���ᨢ ���� ��६�����
    QStringList headersClmn;
    QStringList headersRow;

    // ������塞 ��ப� �᫠��
    for (int i = 0; i < ui->tableWidget_ist->rowCount(); i++)
    {
        ui->tableWidget_ist->setVerticalHeaderLabels(headersRow << QString::number(i));
    }
    // ������塞 �⮫��� ������� ��६�����
    for (int j = 0; j < ui->tableWidget_ist->columnCount() - 1; j++)
    {
        ui->tableWidget_ist->setHorizontalHeaderLabels(headersClmn << (QString)str_vars[j]);
    }
    ui->tableWidget_ist->setHorizontalHeaderLabels(headersClmn << "F"); // �⮫��� ��� ����� �㭪樨

    // ���������� ��ﬨ � �����栬�
    int zeros = pow(2.0, varCnt);
    int coef = 1;   // ���-�� �뢮���
    int pov = 1;    // ��砫쭠� �⥯���

    for (int i = 0; i < varCnt; i++)    // �⮫���
    {
        int row = 0;
        for (int k = 0; k < coef; k++)  // ��ப�
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
        zeros = zeros / 2;      // 㬥��襭�� ���-�� �㫥� � �⮫�� �����
        coef = pow(2.0, pov);
        pov++;
    }

    /* ������� ���, ��� ����. ��� ��� � ���� �� ������������
     *
     *
    ui->tableWidget_tab->setEditTriggers(QAbstractItemView::NoEditTriggers);                            // �����஢�� �祥�
    //ui->tableWidget_tab->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // �����஢�� ��� ।���஢���� �⮫�殢
    //ui->tableWidget_tab->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);   // �����஢�� ��� ।���஢���� �冷�
    ui->tableWidget_tab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget_tab->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    int w = ui->tableWidget_tab->verticalHeader()->width() +4;                                      // ��⠥� ࠧ��� ���� ��� ⠡���� (+4 �㦭�!)
    for (int i = 0; i < ui->tableWidget_tab->columnCount(); i++)                                    // ��⠥� �ਭ�
    {
        w += ui->tableWidget_tab->columnWidth(i);
    }
    int h = ui->tableWidget_tab->horizontalHeader()->height() + 4;
    for (int i = 0; i < ui->tableWidget_tab->rowCount(); i++)                                       // ��⠥� �����
    {
        h += ui->tableWidget_tab->rowHeight(i);
    }
    QSize p = QSize(w, h);
    ui->tableWidget_tab->setFixedSize(p);                                                           // ��⠭�������� ࠧ��� ⠡����
    *
    *
    * ������� ���, ��� ����. ��� ��� � ���� �� ������������ */
}

// �஢�ઠ ⠡����
bool MainWindow::CheckCorrect(Ui::MainWindow *ui)
{
    // �������� ������� ����� ������������������
    if (b_SELFd == true)
    {
        b_SELFd = false;
        ui->tableWidget_ist->removeColumn(ui->tableWidget_ist->columnCount()-1);
        ui->tableWidget_ist->removeColumn(ui->tableWidget_ist->columnCount()-1);
    }

    // �᫨ �� ��࠭� ���-�� ��६����� � ⠡��� �� ᮧ����
    if (ui->radioButton_2vars->isChecked() == false && ui->radioButton_3vars->isChecked() == false && ui->radioButton_4vars->isChecked() == false)
    {
        QMessageBox::warning(this, "Error", "Choose amount of vars");
        return false;
    }

    int rCount = ui->tableWidget_ist->rowCount();
    int clmn = ui->tableWidget_ist->columnCount();

    // ���� �஢�ન ��� ����⮢
    for (int i = 0; i < rCount; i++)
    {
        QTableWidgetItem *itm = ui->tableWidget_ist->item(i, (clmn - 1));    // -1, �������� � 0
        if (itm == nullptr)  // �᫨ �祩�� �����
        {
            QMessageBox::warning(this, "Error", "Invalid numbers in table");
            return false;
        }
        if (itm->text() != "1" && itm->text() != "0") // �᫨ �祩�� �� ���� � �� ����
        {
            QMessageBox::warning(this, "Error", "Invalid numbers in table");
            return false;
        }
    }

    return true;  // �ᯥ�
}

// ���࠭�� 0, ��࠭�� 1
void MainWindow::on_pushButton_save01_clicked()
{
    if ( CheckCorrect(ui) )  // �஢�ઠ ⠡���� ��⨭����
    {
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // ��᫥���� �������
        int row = ui->tableWidget_ist->rowCount() - 1;      // ��᫥���� �冷�

        QTableWidgetItem *zer0 = ui->tableWidget_ist->item(0, clmn);    // (0, 0, ..., 0)
        QTableWidgetItem *one1 = ui->tableWidget_ist->item(row, clmn);  // (1, 1, ..., 1)

        QString result = "Result: The function is  ";
        if (zer0->text() == "0")
        {
            result.append("0-preserving  ");    // ��࠭�� 0
        }
        if (one1->text() == "1")
        {
            result.append("1-preserving");      // ��࠭�� 1
        }
        if (zer0->text() != "0" && one1->text() != "1")         // ��㣮� �᫮���, �� �� �㯮�
        {
            result.append("not 0-preserving or 1-preserving");  // ��祣� �� ��࠭��
        }
        ui->label_res->setText(result); // �뢮�
    }
}

// ���������⢥����?
void MainWindow::on_pushButton_samo_clicked()
{
    if ( CheckCorrect(ui) )  // �஢�ઠ ⠡���� ��⨭����
    {
        ui->tableWidget_ist->insertColumn(ui->tableWidget_ist->columnCount()); // ��⠢�塞 ������� ��� ��
        ui->tableWidget_ist->setHorizontalHeaderItem(ui->tableWidget_ist->columnCount()-1, new QTableWidgetItem("F'"));

        // ��ப� ��室��� �-樨 � �����⢥����
        QString origin = "";
        QString dual ="";

        int rowC = ui->tableWidget_ist->rowCount();         // ���-�� �浪��
        int columnC = ui->tableWidget_ist->columnCount();   // ���-�� �������

        // ���� ��室� �� �ᥬ �浪��
        for (int i = 0; i < rowC; i++)
        {
            QTableWidgetItem *itm = ui->tableWidget_ist->item(i, (columnC - 2) );  // ���頥��� � ������� � �-樥�
            // ����頥� � ����� ������� १���� ���栭��
            if (itm->text() == "1")
            {
                origin.append(itm->text()); // �����뢠�� ����� �-樨
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

        ui->tableWidget_ist->insertColumn(ui->tableWidget_ist->columnCount());      // ��⠢�塞 ������� ��� �����⢥�����
        ui->tableWidget_ist->setHorizontalHeaderItem(ui->tableWidget_ist->columnCount()-1, new QTableWidgetItem("F*"));
        columnC = ui->tableWidget_ist->columnCount();

        // ���� ��室� �� �ᥬ �浪��
        for (int i = 0; i < rowC; i++)
        {
            QTableWidgetItem *item = ui->tableWidget_ist->item(i, (columnC - 2) );
            if (item->text() == "1")
            {
                dual.insert(0, item->text());
                QTableWidgetItem *one = new QTableWidgetItem (QObject::tr("%1").arg(1));
                ui->tableWidget_ist->setItem( (rowC - (i + 1)), columnC-1, one);            // �����뢠�� ������� � ���栭�� � ����� ������� � ���⭮� ���浪�
            }
            else if (item->text() == "0")
            {
                dual.insert(0, item->text());
                QTableWidgetItem *zero = new QTableWidgetItem (QObject::tr("%1").arg(0));
                ui->tableWidget_ist->setItem( (rowC - (i + 1)), columnC-1, zero);
            }
        }

        // �ࠢ������ �-�� � �����⢥���� ��.
        if (origin.compare(dual) == 0)
        {
            ui->label_res->setText("Resul:  is self-dual");  // �᫨ ࠢ�� - �-�� ᠬ������⢥���
        }
        else
        {
            ui->label_res->setText("Resul:  is not self-dual");
        }

        b_SELFd = true;
    }
}

// ����
void MainWindow::on_pushButton_SOP_clicked()        // ����⠭� ���!!!
{
    if ( CheckCorrect(ui) ) // �஢�ઠ
    {
        int rows = ui->tableWidget_ist->rowCount();         // �浪�
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // �������, ��६ � ������ (�-�� - 1)

        QString SOP = "Result:  ";  // ��ப� १����

        // ���� ��室� �� �浪�� ��
        for (int i = 0; i < rows; i++)
        {
            QString for_anal = "";  // ��� ������権
            QTableWidgetItem *dnf = ui->tableWidget_ist->item(i, clmn );    // ��६ ������� �� ��᫥���� �������
            if (dnf->text() == "1") // �᫨ ������
            {
                // ���� ��� �����뢠��� ������権
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

                SOP.append(" || "); // ����� ���権
            }
        }

        SOP.remove( (SOP.length() - 4), 4 );    // 㤠�塞 ��᫥���� ᨬ��� ����樨

        ui->label_res->setText(SOP);            // �뢮�            **** ������� �������, ���� 4 ���������� ��� �������

        QMessageBox::information(this, "Result", SOP);              // ������� ������-����. �뫮 �� ������ ����஫�஢��� ��� ࠧ��� � ����ᨬ��� �� SOP.size()
    }
}

// ����         // ����⠭� 1


// ��������
void MainWindow::on_pushButton_Polinom_clicked()
{
    if ( CheckCorrect(ui) ) // �஢�ઠ
    {
        int rows = ui->tableWidget_ist->rowCount();         // ����砥� �浪�
        int clmn = ui->tableWidget_ist->columnCount() - 1;  // �������

        std::vector<int> line_triag1;       // ����� ��� ᫮����� �� �����
        std::vector<int> final_line;        // 䨭���� ����� � "�����" ��஭�� ��㣮�쭨��

        // ���� ��室� �� �� � �ନ஢���� ����� �-樨
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

        // ���� ��� ᫮����� �� ����� ��� ������� ����� �����
        // ���� � ����� ��室���� ����� ������ �����
        while (line_triag1.size() != 1)
        {
            line_triag1 = Calc_mod2(line_triag1);   // ��।��� ⥪�騩 ����� � ����砥� ����
            final_line.push_back(line_triag1[0]);
        }

        // ��ନ஢���� १����
        QString res = "Zhegalkin polynomial:  ";    // ��ப�
        bool con_zero = true;                       // ��६����� ��� ��।������ ����⠭�� ���

        // ���� ��室� �� ������-१�����.
        // ��稭��� � �㫥����, ���� �� �ன��� ����
        for (int k = 0; k < final_line.size(); k++)
        {
            // �᫨ � ����� ������
            if (final_line[k] == 1)
            {
                int row = k;    // ���������� ������ � ����� - �� �冷� � ��
                if (row == 0)
                {
                    res.append("1 \u2295 ");        // ��� (0, 0, ..., 0) - ���祭�� 1
                    continue;
                }
                // ���� ��� ��室� ��� �ᥫ ������樨
                // � �㫥��� �� ��᫥���� �������
                for (int clmn = 0; clmn < ui->tableWidget_ist->columnCount()-1; clmn++)
                {
                    QTableWidgetItem *geg = ui->tableWidget_ist->item(row, clmn);
                    if (geg->text() == "1")     // �᫨ ��諨 �������
                    {
                        // ����砥� ��� ��६�����
                        QTableWidgetItem *var_h = ui->tableWidget_ist->horizontalHeaderItem(clmn);
                        res.append(var_h->text());  // ����ᨬ � ��ப�-१����
                        con_zero = false;       // 㦥 �� ����⠭�
                    }
                }
                res.append(QString::fromUtf8(" \u2295 "));  // ᨬ��� mod2
            }
        }

        if (con_zero)  // �᫨ � १���� ��祣� �� �뫮 ����ᠭ�
        {
            ui->label_res->setText("Constant of 0. Polynomial is zero");  // ������� - ����
            return; // ��室
        }

        res.remove( (res.length() - 3), 3);  // 㤠�塞 ��᫥���� ᨬ��� ����樨
        ui->label_res->setText(res);         // �뢮�
    }
}

// �-�� ᫮����� �� ����� 2
// �ਭ����� �����.
// �����頥� ����� �� �� �� ����� ����� ⥪�騬 � ᫥���騬 ����⠬�
std::vector<int> MainWindow::Calc_mod2(std::vector<int> line)
{
    std::vector<int> for_ret;
    for (int i = 0; i < line.size() - 1; i++)
    {
        int num = line[i] ^ line[i+1];      // ^ ��⮢�� ������, ᫮����� �� ����� (�᪫���騥 ��� XOR)
        for_ret.push_back(num);             // �����뢠�� � ����� �����
    }
    return for_ret;     // �����頥�
}
