#ifndef KALIBRASYONISLEMI_H
#define KALIBRASYONISLEMI_H

#include <QWidget>

namespace Ui {
class kalibrasyonIslemi;
}

class kalibrasyonIslemi : public QWidget
{
    Q_OBJECT

public:
    explicit kalibrasyonIslemi(QWidget *parent = 0);
    ~kalibrasyonIslemi();
    float pixel_mm = 0.0;

private slots:
    void on_pushButton_kalibrasyoYapma_clicked();

    void on_pushButton_kalibrasyonIcinResim_clicked();

private:
    Ui::kalibrasyonIslemi *ui;
    QImage resimKalibrasyon;

};

#endif // KALIBRASYONISLEMI_H
