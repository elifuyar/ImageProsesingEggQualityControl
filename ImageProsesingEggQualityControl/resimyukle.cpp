#include "resimyukle.h"
#include "ui_resimyukle.h"
#include <QMessageBox>
#include <QFileDialog>
#include "kalibrasyonislemi.h"

resimYukle::resimYukle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::resimYukle)
{
    ui->setupUi(this);
}

resimYukle::~resimYukle()
{
    delete ui;
}


void resimYukle::on_pushButton_resimSec_clicked()
{
    QString yol = QFileDialog::getOpenFileName(this, "Resim Seç", qApp->applicationDirPath(), "Images (*.jpg *.bmp *.tif *.png)");

    if(!QFile(yol).exists())
    {
        return;
    }

    QImage tmpresim(yol);
    yumurtaResim = tmpresim;
    ui->label_resimYumurta->setPixmap(QPixmap::fromImage(yumurtaResim));
}

void resimYukle::on_pushButton_hacimHesapla_clicked()
{
    int en_sayac = 0;
    int boy_sayac = 0;
    float hacim = 0.0;
    float yumurta_en_mm = 0.0;
    float yumurta_boy_mm = 0.0;
    double parlaklik;

    QVector<int> dizi_en;
    QVector<int> dizi_boy;


    for (int i = 0; i < yumurtaResim.width(); ++i)
    {
        for (int k = 0; k < yumurtaResim.height(); ++k)
        {
            QColor renkler(yumurtaResim.pixel(i,k));
            parlaklik = 0.299*renkler.red() + 0.587*renkler.green() + 0.114* renkler.blue(); //Renkleri griye çevirdik

            if (parlaklik < 90 )
            { // Renk değeri 128 in altındaysa siyah olarak gösterecek
                yumurtaResim.setPixel(i,k,qRgb(0,0,0));
            }
            else
            { // Renk değeri 128 in üstünde ise beyaz olarak gösterecek.
                yumurtaResim.setPixel(i,k,qRgb(255,255,255));
            }
        }
    }
   // ui->label_resimYumurta->setPixmap(QPixmap::fromImage(yumurtaResim));

    QMessageBox::information(this,"En: ", QString::number(yumurtaResim.width())); //Resmin enini yazdırdık
    QMessageBox::information(this,"Boy: ", QString::number(yumurtaResim.height())); //Resmin boynu yazdırdık

    //Yumurtanın eninin hesabını yaptığımız kısım
    for (int i = 0; i < yumurtaResim.width(); ++i)
    {
        for (int k = 0; k < yumurtaResim.height(); ++k)
        {
            QColor renkler(yumurtaResim.pixel(i,k));

            if (renkler.red() == 255) //Resimde beyaz olan satırların pixellerini sayaca ekledik
            {
                en_sayac++;
            }
        }
        dizi_en.append(en_sayac); //Her hesapladığımız satırı diziye ekledik
        en_sayac = 0;
    }

    float en_max= dizi_en[0];
    for(int j=1; j< dizi_en.size(); j++) //Hesapladıgımız satırlardan pixel sayısı en fazla olanı bulduk
    {
        if(dizi_en[j] > en_max )
        {
            en_max = dizi_en[j];
        }
    }
    //QMessageBox::information(this,"Kalibrazyon değeri", QString::number(kalibrasyonDegeri->Gonder()));

    float pixell= kalibrasyonDegeri->pixel_mm;
   yumurta_en_mm =  en_max * pixell; //1 pixel in cm değerii kadar pixelleri çarptık. Ve yumurtanın eninin kaç mm olduğunu bulduk
    QMessageBox::information(this,"Yumurta En mm", QString::number(yumurta_en_mm)); // Yumurtanın enini yazdırdık.


    //Aynı işlemleri yumurtanın boyu için de yaptık.
    for (int i = 0; i < yumurtaResim.height(); ++i)
    {
        for (int k = 0; k < yumurtaResim.width(); ++k)
        {
            QColor renkler(yumurtaResim.pixel(k,i));

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
    yumurta_boy_mm = boy_max * pixell;
    QMessageBox::information(this,"Yumurta Boy mm", QString::number(yumurta_boy_mm));

    hacim = (0.6057 - (0.0018*(yumurta_boy_mm/10))) * (yumurta_en_mm/10)* (yumurta_boy_mm/10) * (yumurta_boy_mm/10);//Hacim hesabını formil ile yaptık
    QMessageBox::information(this,"Hacim", QString::number(hacim));
}
