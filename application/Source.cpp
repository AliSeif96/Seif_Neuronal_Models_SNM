/************************************************************************************************/
/*** Topic: Kuramoto model with Runge-Kutta 4th Order Method with delay for snm application   ***/
/***                                                                                          ***/
/*** Version Release 17.12 rev 11256                                                Ali-Seif  ***/
/*** Date: 6/14/2021                                                                          ***/
/*** Code implemented in Microsoft Visual Studio Enterprise 2019 C++ compiler                 ***/
/*** MSI: PX60 6QD/ DDR4                                                                      ***/
/*** Run under a Intel® Core™ i7-6700HQ CPU @ 2.60GHz × 64 based processor with 16 GB RAM     ***/
/************************************************************************************************/
#include"kuramoto.h"                                                            //import library Kuramoto
int main() {                                                                    //Beginning main
    clock_t t = clock();                                                        //time start record
    double* data = new double[19];                                              //create pointer 1d
    read_data(data);                                                            //read data from data.txt to pointer 1d
    implementation(data);                                                       //Run and save data
    cout << "\nTime taken by program is :\t" << ((double)clock() - t) / CLOCKS_PER_SEC << " sec\nfinish\n";//calculate time run
    gnuplot(data);                                                              //plot in gnuplot by terminal
    return 0;                                                                   //run program was correct
}                                                                               //