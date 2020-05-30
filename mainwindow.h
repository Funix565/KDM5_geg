#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    bool CheckCorrect(Ui::MainWindow *ui);

    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void ChosenRadio();
    void on_pushButton_save01_clicked();
};

#endif // MAINWINDOW_H
