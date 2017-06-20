#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QVector>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_resimAc_clicked()
{
    QString yol = QFileDialog::getOpenFileName(this, "Resim Dosyası Aç", qApp->applicationDirPath(), "Images (*.jpg *.bmp *.tif *.png)");

    if(!QFile(yol).exists())
    {
        return;
    }

    QImage tmpresim(yol);
    resim = tmpresim;
    ui->labelResim->setPixmap(QPixmap::fromImage(resim));
}



void MainWindow::on_pushButton_hacimHasepla_clicked()
{
    int en_sayac = 0;
    int boy_sayac = 0;
  //  float hacim = 0.0;
    float yumurta_en_mm = 0.0;
    float yumurta_boy_mm = 0.0;
    double parlaklik;

    QVector<int> dizi_en;
    QVector<int> dizi_boy;


    for (int i = 0; i < resim.width()-10; ++i)
    {
        for (int k = 0; k < resim.height(); ++k)
        {
            QColor renkler(resim.pixel(i,k));
            parlaklik = 0.299*renkler.red() + 0.587*renkler.green() + 0.114* renkler.blue(); //Renkleri griye çevirdik

            if (parlaklik < 128 )
            { // Renk değeri 128 in altındaysa siyah olarak gösterecek
                resim.setPixel(i,k,qRgb(0,0,0));
            }
            else
            { // Renk değeri 128 in üstünde ise beyaz olarak gösterecek.
                resim.setPixel(i,k,qRgb(255,255,255));
            }
        }
    }

    //QMessageBox::information(this,"En: ", QString::number(resim.width())); //Resmin enini yazdırdık
    //QMessageBox::information(this,"Boy: ", QString::number(resim.height())); //Resmin boynu yazdırdık




    ui->labelResim->setPixmap(QPixmap::fromImage(resim));


    /********************************************************************************RESİM BÖLME İŞLEMİ********************************************************************************/

    double parlakkontrol1;
    double parlakkontrol2;//Bir önceki sütun
    double parlakkontrol3; //Bir sonraki sütun
    double parlakkontrol4; //Bir sonrakinin sonraki sütun

     QVector <QPoint> bosSutunlar;
     int dogruyanlis1=0;
      int dogruyanlis2=0;
      int dogruyanlis3=0;
       int dogruyanlis4=0;
    for(int k = 5; k < resim.width()-10; k++)
    {
        for(int m = 5; m < resim.height(); m++)
        {
            QColor renkkontrol1(resim.pixel(k,m));
            parlakkontrol1 = renkkontrol1.red();

            QColor renkkontrol2(resim.pixel(k-1,m));
            parlakkontrol2 = renkkontrol2.red();

            QColor renkkontrol3(resim.pixel(k+1,m));
            parlakkontrol3 = renkkontrol3.red();

            QColor renkkontrol4(resim.pixel(k +2,m));
            parlakkontrol4 = renkkontrol4.red();


            if(parlakkontrol1 !=  0 ) //Mevcut sutun
            {
                dogruyanlis1 = 1;
            }

            if(parlakkontrol3 ==  255 )
            {
                dogruyanlis3 = 1;
            }


            if(parlakkontrol4 ==  255 ) //Mevcut sutun
            {
                dogruyanlis4 = 1;
            }



            if(parlakkontrol2 ==  255 ) //Bir önceki sutun
            {
                dogruyanlis2 = 1;
            }
        }

        if(dogruyanlis1 == 0 && dogruyanlis3 == 1 )
        {
            bosSutunlar.append(QPoint(k-3,0));
            //QMessageBox::information(this,"parlaklık", QString::number(1));
        }
        if(dogruyanlis1 == 0 && dogruyanlis2 == 1)//Bir önceki sütun da kırmızı pixel varsa ve mevcut sütunda kırmızı pixel yoksa
        {
            bosSutunlar.append(QPoint(k+3,resim.height()));
          //  QMessageBox::information(this,"parlaklık", QString::number(2));
        }
        dogruyanlis1=0;
        dogruyanlis2=0;
        dogruyanlis3=0;
        dogruyanlis4=0;
    }

    int c = 1;
    for(int a = 0; a <bosSutunlar.size(); a+=2)
    {

        ui->editTextSonuclar->textCursor().insertText(QString::number(c) + ". YUMURTA \n");

        //QMessageBox::information(this,"parlaklık", QString::number(bosSutunlar[a+1].x()));

       fonksiyon(bosSutunlar[a].x(), bosSutunlar[a].y(), bosSutunlar[a+1].x(), bosSutunlar[a+1].y());
       c++;

    }
}












void MainWindow::fonksiyon(int baslangicXekseni, int baslagicYekseni, int bitisXekseni, int bitisYekseni)
{
    //*********************Kenar Bulma Kısmı**********************//

    float cikarma;
    double parlak1;
    double parlak2;

    QVector <QPoint> kirmiziNoktalar;
    QVector <QPoint> enUzunNoktalar;
    QVector < QVector <QPoint> > noktalar; //Birden fazla yumurta pixelini tutailmek için iki boyutlu dizi oluşturduk.


    //QMessageBox::information(this,"mmmmmmmmmmmm", QString::number(baslagicYekseni));

    for (int i = baslangicXekseni; i < bitisXekseni; ++i)
    {
        for (int k = baslagicYekseni; k < bitisYekseni; ++k)
        {

       //     QMessageBox::information(this,"oooooooooooooooo", QString::number(i));

            QColor renk1(resim.pixel(i,k));
            parlak1 = renk1.red() + renk1.green() + renk1.blue();
            QColor renk2(resim.pixel(i,k+1));
            parlak2 = renk2.red() + renk2.green() + renk2.blue();
            cikarma = fabs(parlak1 - parlak2);
            if(fabs(cikarma) >= 250)
            {

                resim.setPixel(i,k,qRgb(255,0,0));

                kirmiziNoktalar.append(QPoint(i,k));
            }

            else
            {
                resim.setPixel(i,k,qRgb(0,0,0));
            }
        }

    }

    //noktalar.push_back(kirmiziNoktalar); //Kirmizi noktaları iki boyutlu diziye aktardık.
    ui->labelResim->setPixmap(QPixmap::fromImage(resim));



    double uzunluk = 0.0;
    double enuzun = 0.0;


    for(int i = 0; i<kirmiziNoktalar.size(); i++)
    {
        for(int j = 0; j<kirmiziNoktalar.size(); j++)
        {
            uzunluk = sqrt( ((kirmiziNoktalar[i].x() - kirmiziNoktalar[j].x()) * (kirmiziNoktalar[i].x() - kirmiziNoktalar[j].x())) +
                           ((kirmiziNoktalar[i].y() - kirmiziNoktalar[j].y()) * (kirmiziNoktalar[i].y() - kirmiziNoktalar[j].y())) );

            if(uzunluk >= enuzun )
            {
                enuzun = uzunluk;
                for(int n = 0; n<enUzunNoktalar.size(); n++)
                {
                    enUzunNoktalar.remove(n);
                }

                enUzunNoktalar.append(QPoint(kirmiziNoktalar[i].x(), kirmiziNoktalar[i].y()));
                enUzunNoktalar.append(QPoint(kirmiziNoktalar[j].x(), kirmiziNoktalar[j].y()));

            }
        }
    }


    /*****************************************İKİ NOKTA ARASI PİXEL SAYISI HESABI İLE ORTA NOKTA BULMA*****************************************/
    /*****************************************************************LineDDA ALGORİTMASI İLE***********************************************************/


    int dx = fabs (enUzunNoktalar[1].x() - enUzunNoktalar[0].x() );
        int dy = fabs ( enUzunNoktalar[1].y() - enUzunNoktalar[0].y() );
        int steps;
        float incrementX, incrementY, x = enUzunNoktalar[0].x(), y = enUzunNoktalar[0].y();
        if (fabs(dx) > fabs(dy)) {
            steps = fabs(dx);
        } else {
            steps = fabs(dy);
        }
        incrementX = (float) dx / (float) steps;
        incrementY = (float) dy / (float) steps;
        resim.setPixel(round(x), round(y),qRgb(255,0,0));
         ui->labelResim->setPixmap(QPixmap::fromImage(resim));
         QVector <QPoint> ortaNokta;

        for (int k = 0; k < steps; ++k)
        {
            x += incrementX;
            y += incrementY;

            if( k== (steps / 2 ) ) // Eğer tam orta noktaysa...
            {
                resim.setPixel(round(x), round(y), qRgb(255,0,0));
                ortaNokta.append(QPoint(round(x),round(y))); // Orta nokta değerini kaydettik
                 ui->labelResim->setPixmap(QPixmap::fromImage(resim));
            }
            resim.setPixel(round(x), round(y), qRgb(255,0,0)); // Uzun kenara denk gelen pixellerin tamamını kırmızıya boyadık
             //ui->label->setPixmap(QPixmap::fromImage(resim));
         }



        /*****************************************KISA KENAR BULMA KISMI********************************************/

         QVector <QPoint> kisakenarbulma;
         QVector <QPoint> kisakenarnoktalari;

         kisakenarbulma.append(QPoint(ortaNokta[0].x()+2, ortaNokta[0].y()+2));

         double  pixelYegim = 0.0, pixelXegim = 0.0;


         pixelXegim = fabs(enUzunNoktalar[0].x() - enUzunNoktalar[1].x());
         pixelYegim = fabs(enUzunNoktalar[0].y() - enUzunNoktalar[1].y());

         double egimUK = pixelYegim / pixelXegim;
         double egimKK = -(pixelXegim / pixelYegim);

          //QMessageBox::information(this,"egimKK", QString::number(egimKK ));

         double kisakenarparlaklik=0.0;

         QColor kisakenarrenk(resim.pixel(kisakenarbulma[0].x(), kisakenarbulma[0].y()));
         kisakenarparlaklik = kisakenarrenk.red() + kisakenarrenk.green() + kisakenarrenk.blue();

        // QMessageBox::information(this,"kisa kenar X", QString::number(kisakenarbulma[0].x() ));
          //QMessageBox::information(this,"kisa kenar Y", QString::number(kisakenarbulma[0].y() ));

         //QMessageBox::information(this,"parlaklık", QString::number(kisakenarparlaklik ));


               int i = 0;
               QVector <QPoint> kisakenarSonNokta;
               QVector <QPoint> cevrePixeller;

         while(kisakenarparlaklik < 250) //Parlaklık 250 nin altındaysa yani pixel kırmızı değilse
         {
            // QMessageBox::information(this,"SELAM", QString::number(kisakenarparlaklik));


             int kisakenarY =  round(egimKK * (round(kisakenarbulma[i].x()) - ortaNokta[0].x() ) + ortaNokta[0].y() ) ; // (y = m*(x - x1) + y1) formülünden yeni y değerleri hesaplandı. x1- y1 orta nokta

            // QMessageBox::information(this,"kisakenarY", QString::number(kisakenarY ));

              kisakenarnoktalari.append(QPoint(round(kisakenarbulma[i].x()+5), round(kisakenarY )+5)); // kısa kenar için buladugumuz pikseller kisakenar noktaları vektöründe tutuldu

              kisakenarSonNokta.clear() ;
             kisakenarSonNokta.append(QPoint(kisakenarnoktalari[i].x(), kisakenarnoktalari[i].y()));

              kisakenarbulma.append(QPoint(kisakenarbulma[i].x()+1 , ortaNokta[0].y())); // x değeri sürekli arttırıldı ki x e karşılık gelen y değeri hesaplanabilsin.
             QColor renkKisaKenar(resim.pixel(kisakenarnoktalari[i].x(),  kisakenarnoktalari[i].y())); // Bulunan her bir pixel de renk kontrolü yapıldı.

             kisakenarparlaklik = renkKisaKenar.red() + renkKisaKenar.green() + renkKisaKenar.blue(); //Eğer renk kırmızı ise döngüyü sonlandıracak.

             /*******************************PİXEL ÇEVRESİNİN RENK KONTROLÜ*******************************/

             cevrePixeller.clear() ;
             cevrePixeller.append(QPoint(kisakenarnoktalari[i].x() +1 , round(kisakenarnoktalari[i].y() - 1)));
             cevrePixeller.append(QPoint(kisakenarnoktalari[i].x() +1 , round(kisakenarnoktalari[i].y())));
             cevrePixeller.append(QPoint(kisakenarnoktalari[i].x() +1 , round(kisakenarnoktalari[i].y() + 1)));

             double cevreRenkParlaklik=0.0;
             for (int a = 0; a < cevrePixeller.size(); a++)
             {
                 QColor cevreRenkler(resim.pixel(cevrePixeller[a].x(),  cevrePixeller[a].y())); // Bulunan her bir pixel de renk kontrolü yapıldı.
                 cevreRenkParlaklik = cevreRenkler.red() + cevreRenkler.green() + cevreRenkler.blue();
                 if(cevreRenkParlaklik > 250)
                 {
                       kisakenarparlaklik = 255;
                     //  QMessageBox::information(this,"SELAM", QString::number(cevreRenkParlaklik));
                 }
             }

             resim.setPixel(round(kisakenarnoktalari[i].x()), round(kisakenarnoktalari[i].y()), qRgb(255,0,0)); // Belirlediğimiz kısa kenar pixellerinin renkleri kırmızıya boyandı.
              ui->labelResim->setPixmap(QPixmap::fromImage(resim));

             i++;

         }


        int Xpixelsayisi =2 * fabs(kisakenarSonNokta[0].x() - kisakenarnoktalari[0].x()); // kisa kenarı bulabilmek için pisagor teoremi kullanılarak kisa kenarın real uzunşugu heaplandı
        int Ypixalsayisi = 2 * fabs(kisakenarSonNokta[0].y() - kisakenarnoktalari[0].y());

        Xpixelsayisi = Xpixelsayisi * kalibrasyon_->pixel_mm; // Bir pixel e karşılık gelen uzunluk kalibrasyon sonucu elde edilmişti.
        Ypixalsayisi = Ypixalsayisi * kalibrasyon_->pixel_mm;

        double kisaKenarUzunluk = sqrt((Xpixelsayisi * Xpixelsayisi) + (Ypixalsayisi * Ypixalsayisi)); // Real uzunluk bulundu

      //  QMessageBox::information(this,"KISA KENAR EFSANE UZUNLUK", QString::number(kisaKenarUzunluk));


        ui->editTextSonuclar->textCursor().insertText( "Kısa Kenar Uzunluğu: " + QString::number(kisaKenarUzunluk) + "\n");

    int pixelY = 0, pixelX = 0;


    pixelX = fabs(enUzunNoktalar[0].x() - enUzunNoktalar[1].x());
    pixelY = fabs(enUzunNoktalar[0].y() - enUzunNoktalar[1].y());

    pixelX = pixelX * kalibrasyon_->pixel_mm;
    pixelY = pixelY * kalibrasyon_->pixel_mm;

    double enUzunKenar = sqrt((pixelX * pixelX) + (pixelY * pixelY));

    QMessageBox::information(this,"Uzun Kenar", QString::number(enUzunKenar));
     ui->editTextSonuclar->textCursor().insertText("Uzun Kenar Uzunluğu: " + QString::number(enUzunKenar)+"\n");

    float hacim = 0.0;

    hacim = (0.6057 - (0.0018*(kisaKenarUzunluk/10))) *
            (enUzunKenar/10)* (kisaKenarUzunluk/10) *
            (kisaKenarUzunluk/10);//Hacim hesabını formül ile yaptık

   QMessageBox::information(this,"Hacim", QString::number(hacim));

     ui->editTextSonuclar->textCursor().insertText("Hacim: " + QString::number(hacim)+"\n\n");
}

void MainWindow::on_pushButton_kalibrasyonYap_clicked()
{
    kalibrasyon_ = new kalibrasyonIslemi();
    kalibrasyon_->show();
}
