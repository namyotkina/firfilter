#ifndef FIR_FILTER_H
#define FIR_FILTER_H
#include <iostream>
#include <complex>
#include <fstream>
#include <QVector>

class FIR_FILTER
{
private:

public:
    int in_num,
        out_num,
        k_num;

    std::complex <float> inData [100];
    std::complex <float> outData[100];
    std::complex <float> buff[51];


    float Real [100];
    float Image[100];
    float   K[51];
    QVector<double> Real_fil;
    QVector<double> Imag_fil;

void setDate(std::complex <float> indata[], std::complex <float> outdata[], float k[])
{

    for ( int i = 0; i < 100; i++)
    {
        inData[i] = indata[i];
        outData[i] = outdata[i];
    }
    for ( int i = 0; i < 51; i++)
    {
        K[i] = k[i];
    }


}
void Filter()
{

    for (int i = 0; i < 100; i++)
    {
        outData[i] = 0;
        for (int k = 51; k > 0; k-- )
        {

            buff[k] = buff[k - 1];


        }
        buff[0] = inData[i];


        for (int j = 0; j < 51; j++)
        {
            outData[i] += K[j] * buff[j];

        }
    }

//    for (int i = 0; i < 100; i++)
//    {
//        outData[i] = 0;
//        for (int j = 0; j < 11-1; j++)
//        {
//            if ((i - j >= 0) ) outData[i] += K[j] * inData[i-j];
//        }
//    }
}
void getDate()
{
    Real_fil.clear();
    Imag_fil.clear();

   for (int i = 0; i<100; i++)
   {
        Real[i] = outData[i].real();
        Image[i]= outData[i].imag();
        /* _ */
        Real_fil.append(Real[i]);
        Imag_fil.append(Image[i]);

    }

}
void inFile ()
{
    std::ofstream fileout;
    fileout.open("out.txt");
    //for (int i = 0; i<100; i++) fileout << Real[i] << "," << Image[i] <<std::endl ;
    for (int i = 0; i<100; i++) fileout <<outData[i] <<std::endl ;
    fileout.close();
}

};

#endif // FIR_FILTER_H
