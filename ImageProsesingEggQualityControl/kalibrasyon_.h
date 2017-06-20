#ifndef KALIBRASYON_H
#define KALIBRASYON_H

#include <QWidget>
#include <QMainWindow>

namespace Ui {
class kalibrasyon;
}


class kalibrasyon : public QWidget
{
    Q_OBJECT
public:
    explicit kalibrasyon(QWidget *parent = 0);
    ~kalibrasyon();

signals:

public slots:

private
    Ui::kalibrasyon *ui;
};

#endif // KALIBRASYON_H
