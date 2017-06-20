#ifndef RESIMYUKLE_H
#define RESIMYUKLE_H

#include <QWidget>
#include <kalibrasyonislemi.h>

namespace Ui {
class resimYukle;
}

class resimYukle : public QWidget
{
    Q_OBJECT

public:
    explicit resimYukle(QWidget *parent = 0);
    ~resimYukle();


private slots:

    void on_pushButton_resimSec_clicked();

    void on_pushButton_hacimHesapla_clicked();

private:
    Ui::resimYukle *ui;
    QImage yumurtaResim;
    kalibrasyonIslemi *kalibrasyonDegeri;
};

#endif // RESIMYUKLE_H
