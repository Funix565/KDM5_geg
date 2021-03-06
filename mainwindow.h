#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool CheckCorrect(Ui::MainWindow *ui);
    std::vector<int> Calc_mod2 (std::vector <int> line);
    bool Compare_Inter(QString, QString);

    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void ChosenRadio();
    void on_pushButton_save01_clicked();
    void on_pushButton_samo_clicked();
    void on_pushButton_SOP_clicked();
    void on_pushButton_Polinom_clicked();
    void on_pushButton_POS_clicked();
    void on_pushButton_mono_clicked();
};

#endif // MAINWINDOW_H
