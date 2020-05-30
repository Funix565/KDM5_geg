#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

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
    ui->tableWidget_ist->setColumnCount(varCnt + 1);        // ���- �� �⮫�殢   // TODO: insert FOR self-dual; then remove FOR self-dual

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


















