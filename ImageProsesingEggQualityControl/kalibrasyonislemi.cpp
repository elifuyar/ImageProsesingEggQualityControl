#include "kalibrasyonislemi.h"
#include "ui_kalibrasyonislemi.h"
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QVector>

kalibrasyonIslemi::kalibrasyonIslemi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::kalibrasyonIslemi)
{
    ui->setupUi(this);

}

kalibrasyonIslemi::~kalibrasyonIslemi()
{
    delete ui;
}




void kalibrasyonIslemi::on_pushButton_kalibrasyonIcinResim_clicked()
{
    QString yol = QFileDialog::getOpenFileName(this, "Kalibrasyon İçin Resim Seç", qApp->applicationDirPath(), "Images (*.jpg *.bmp *.tif *.png)");

    if(!QFile(yol).exists())
    {
        return;
    }

    QImage tmpresim(yol);
    resimKalibrasyon = tmpresim;
    ui->label_kalibrasyon->setPixmap(QPixmap::fromImage(resimKalibrasyon));
}



void kalibrasyonIslemi::on_pushButton_kalibrasyoYapma_clicked()
{
    int boy_sayac = 0;
    float boy_mm = 0.0;
    QString cisim_uzunkenar = 0;
    int digit =0;
    QVector<int> dizi_boy;


   // kalibrasyon.load( ":kalibrasyon/" + QString::number(4) + ".jpg" );

    double parlaklik;
    for (int i = 0; i < resimKalibrasyon.width(); ++i)
    {
        for (int k = 0; k < resimKalibrasyon.height(); ++k)
        {
            QColor renkler(resimKalibrasyon.pixel(i,k));
            parlaklik = 0.299*renkler.red() + 0.587*renkler.green() + 0.114* renkler.blue(); //Renkleri griye çevirdik

            if (parlaklik < 90 )
            { // Renk değeri 128 in altındaysa siyah olarak gösterecek
                resimKalibrasyon.setPixel(i,k,qRgb(0,0,0));
            }
            else
            { // Renk değeri 128 in üstünde ise beyaz olarak gösterecek.
                resimKalibrasyon.setPixel(i,k,qRgb(255,255,255));
            }
        }
        //ui->label_kalibrasyon->setPixmap(QPixmap::fromImage(resimKalibrasyon));
    }
    QMessageBox::information(this,"Resmin Eni: ", QString::number(resimKalibrasyon.width()));
    QMessageBox::information(this,"Resmin Boyu: ", QString::number(resimKalibrasyon.height()));


   /* for (int i = 0; i < kalibrasyon.width(); ++i)
        {
            for (int k = 0; k < kalibrasyon.height(); ++k)
            {
                QColor renkler(kalibrasyon.pixel(i,k));

                if (renkler.red() == 255)
                {
                    en_sayac++;
                }
            }
            dizi_en.append(en_sayac);

            en_sayac = 0;
        }

    float en_max= dizi_en[0];
    for(int j=1; j< dizi_en.size(); j++)
    {
        if(dizi_en[j] > en_max )
        {
            en_max = dizi_en[j];
        }
    }
    en_mm = 56 / en_max; // Seçtiğimiz resimdeki cismin eni 56 mm olarak hesapladık ve bunu beyaz pixel sayısına böldük
    QMessageBox::information(this,"En", QString::number(en_max));
    QMessageBox::information(this,"En mm", QString::number(en_mm)); */



    for (int i = 0; i < resimKalibrasyon.height(); ++i)
    {
        for (int k = 0; k < resimKalibrasyon.width(); ++k)
        {
            QColor renkler(resimKalibrasyon.pixel(k,i));
            if (renkler.red() == 255)
            {
                boy_sayac++;
            }
        }
        dizi_boy.append(boy_sayac);
        boy_sayac = 0;
    }
    float boy_max= dizi_boy[0];
    for(int j=1; j< dizi_boy.size(); j++)
    {
        if(dizi_boy[j] > boy_max )
        {
            boy_max = dizi_boy[j];
        }
    }

    cisim_uzunkenar = ui->lineEdit_cisimUzunKenar->text();

    bool ok = true;
    boy_mm = cisim_uzunkenar.toInt(&ok) / boy_max; // Seçtiğimiz resimdeki cismin boyu 87 mm olarak hesapladık ve bunu beyaz pixel sayısına böldük
    QMessageBox::information(this,"Cismin boyu kaç pixel?", QString::number(boy_max));
    QMessageBox::information(this,"1 pixel kaç mm?", QString::number(boy_mm));
    pixel_mm = boy_mm; // 1 pixel in kaç mm olduğunu bulduk ve bunu pixel_mm değişkenine attık
}
