#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <kalibrasyonislemi.h>
#include <resimyukle.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int deneme = 0;

private slots:


    void on_pushButton_kalibrasyonYap_clicked();

    void on_pushButton_resimAc_clicked();

    void on_pushButton_hacimHasepla_clicked();

    void fonksiyon(int, int, int, int);

private:
    Ui::MainWindow *ui;

    QImage resim;
    kalibrasyonIslemi *kalibrasyon_;
    float pixel_mm = 0.0;
};

#endif // MAINWINDOW_H
