#ifndef WIDGET_H
#define WIDGET_H
#include <QVector>
#include <QtWidgets>
#include "qcustomplot.h"
#include <iostream>
#include <complex>
#include <fstream>
#include <QObject>
#include <QtNetwork/qudpsocket.h>
#include "C:\FIR\Shared\udp_protocol.h"
#include "fir_filter.h"
#include <QString>


class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = 0);
    ~Widget();

    std::complex <float> Xout[100];

private:


    FIR_FILTER * lowpass;

    QUdpSocket * ReceiveSocket;

    QCustomPlot *plot;
    QGridLayout *layout;

    QVector<double> t;
    QVector<double> outReal;
    QVector<double> outImag;

public slots:

        void Readcoming();

};

#endif // WIDGET_H
