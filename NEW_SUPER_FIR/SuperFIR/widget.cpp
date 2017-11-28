#include "widget.h"

using namespace std;
struct data_struct datastruct;
struct header msghead;
struct end_of_msg msgend;

const double PI = 3.14159265358979323846;


float Koef[51] = {0.014314660852973024,
                  -0.012510567345810974,
                  -0.010636616861588885,
                  -0.010330834207241516,
                  -0.010745757229966379,
                  -0.011315042598484227,
                  -0.011678533756669648,
                  -0.011562270179191717,
                  -0.010754741045659299 ,
                  -0.0091035530734695051,
                  -0.0065126167399962992,
                  -0.0029375817712019884,
                   0.0016174563037523531,
                   0.0071000239698769743,
                   0.013407537267277819 ,
                   0.020387275379179975 ,
                   0.027840807080752407 ,
                   0.03553306793524369  ,
                   0.043204199006625359 ,
                   0.050582038742037358 ,
                   0.057394394738576875 ,
                   0.063381394539254135 ,
                   0.068308229559195896 ,
                   0.071977675784818548 ,
                   0.074240980824726965 ,
                   0.075005835528613896 ,
                   0.074240980824726965 ,
                   0.071977675784818548 ,
                   0.068308229559195896 ,
                   0.063381394539254135 ,
                   0.057394394738576875 ,
                   0.050582038742037358 ,
                   0.043204199006625359 ,
                   0.03553306793524369  ,
                   0.027840807080752407 ,
                   0.020387275379179975 ,
                   0.013407537267277819 ,
                   0.0071000239698769743,
                   0.0016174563037523531,
                  -0.0029375817712019884,
                  -0.0065126167399962992,
                  -0.0091035530734695051,
                  -0.010754741045659299 ,
                  -0.011562270179191717 ,
                  -0.011678533756669648 ,
                  -0.011315042598484227 ,
                  -0.010745757229966379 ,
                  -0.010330834207241516 ,
                  -0.010636616861588885 ,
                  -0.012510567345810974 ,
                   0.014314660852973024  };



Widget::Widget(QWidget *parent) : QWidget(parent)
{

    lowpass = new FIR_FILTER();

    ReceiveSocket = new QUdpSocket(this);
    ReceiveSocket -> bind (4661);

    connect(ReceiveSocket, SIGNAL(readyRead()), SLOT(Readcoming()));
///------------------------------------------------------------------------
    layout = new QGridLayout();
    this->setLayout(layout);
    this->resize(800, 500);


    const float STEP = 5e-2f;
    for (int i = 0; i < 100; i++) {
        t.append(STEP * (float)i);
    }
    plot = new QCustomPlot();
    plot->addGraph();
    /*plot->graph(0)->setData(t, outReal);*/
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->addGraph();
    /*plot->graph(1)->setData(t, outImag);*/
    plot->graph(1)->setPen(QPen(Qt::red));
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(1.0f);
    plot->xAxis->setRange(0,5.0f);
    plot->yAxis->setRange(-2.0f, 2.0f);
    /*plot->replot();*/
    layout->addWidget(plot);

///------------------------------------------------------------------------

}

Widget::~Widget()
{

}

void Widget::Readcoming()
{
    QByteArray baDatagram;

    int resultCounter = 0;
    bool isTriggered = false;
    float THR = 0.5;
    complex <float> result [400];
    complex <float> datai [100];


    do
    {
        baDatagram.resize(ReceiveSocket -> pendingDatagramSize());
        ReceiveSocket -> readDatagram(baDatagram.data(), baDatagram.size());
    } while (ReceiveSocket -> hasPendingDatagrams());


    if(baDatagram.startsWith("_CH1") && baDatagram.endsWith("_END"))
    {
        qDebug()<<" Wow ";

        memcpy(datai, &baDatagram.data()[4], 800);

do{
        for (int i = 0; i < 100; i++)
        {
            if (!isTriggered)
            {
                if (sqrt(datai[i].real()*datai[i].real() + datai[i].imag()*datai[i].imag()) > THR)
                {
                    result[resultCounter++] = datai[i];
                    isTriggered = true;
                }
            }
        }
   } while(resultCounter >= 400);

        if(isTriggered)
        {


                lowpass->setDate(result,Xout,Koef);
                lowpass->Filter();
                lowpass->getDate();
                lowpass->inFile();

                plot->graph(0)->setData(t, lowpass->Real_fil);
                plot->graph(1)->setData(t, lowpass->Imag_fil);
                plot->replot();

                resultCounter = 0;

        }

}
}

