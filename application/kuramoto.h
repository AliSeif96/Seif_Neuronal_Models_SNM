/************************************************************************************************/
/*** Topic: Kuramoto model with Runge-Kutta 4th Order Method with delay for snm application   ***/
/***                                                                                          ***/
/*** Version Release 17.12 rev 11256                                                Ali-Seif  ***/
/*** Date: 6/14/2021                                                                          ***/
/*** Code implemented in Microsoft Visual Studio Enterprise 2019 C++ compiler                 ***/
/*** MSI: PX60 6QD/ DDR4                                                                      ***/
/*** Run under a Intel® Core™ i7-6700HQ CPU @ 2.60GHz × 64 based processor with 16 GB RAM     ***/
/************************************************************************************************/
#include<iostream>                                                              //for cout
#include<fstream>                                                               //infile
#include<cstdlib>
#include<cmath>
#include<vector>                                                                //for vector
#include <chrono>                                                               //chrono
#include <random>                                                               //random_device & ...
#include <string>
#define Pi 3.14159265359                                                        //pi number
using namespace std;
string address_save = "time_synchrony";
string address_matrix = "Matrix";
string address_Omega = "Omega";
string address_theta = "Theta";
ofstream syncrony("Save/coupling_synchrony.txt");//data17
ofstream T_sync("Save/Time_synchrony.txt");//data16
ofstream T_phi("Save/Time_Sin(phi).txt");//data16
ofstream cds("Save/coupling_delay_synchrony.txt");//data18
ofstream T_sync1("Save/Time_synchrony1.txt");//data16
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                                                                @@@@
//@@@                                class Kuramoto                                  @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
class Kuramoto {                                                                //create and define class 
private:                                                                        //private values
    const int N;                                                                //number of node
    double dt;                                                                  //step lenth
    double tfinal;                                                              //time final
    double coupling;                                                            //stringh cupling
    int** adj;
    double* phi_initial;//for all copling
    double* phi;
    double* phi2;
    double** phi_history;//for delay
    double* W;
    double create_Matrix;
    double deviation_Omega;
    double deviation_Theta;
    double delay;
    double data16,data17;
    int history;
public:                                                                         //public values
    Kuramoto                                                                    //
    (int N_, double dt_, double t_final_, double coupling_, double create_Matrix_, double deviation_Omega_, double deviation_Theta_, double delay_, double data16_, double data17_) ://input data in class
        N(N_), dt(dt_), tfinal(t_final_), coupling(coupling_), create_Matrix(create_Matrix_), deviation_Omega(deviation_Omega_), deviation_Theta(deviation_Theta_), delay(delay_), data16(data16_), data17(data17_) {//change name input to privet
        phi_initial = new double[N]; //Making Array
        phi = new double[N]; //Making Array
        phi2 = new double[N]; //Making Array
        W = new double[N]; //Making Array
        if (create_Matrix == 0) {
            adj = read_to_array(address_matrix + ".txt");
            cout << address_matrix + ".txt" + "\t\tloaded" << endl;
        }
        else {
            adj = pro_Matrix(create_Matrix);              //read adjacency matrix from file
            cout << "matrix-P=" << create_Matrix << "\t\tloaded" << endl;
        }
        if (deviation_Omega == 0) {
            read_init(W, address_Omega);
            cout << address_Omega + ".txt" + "\t\tloaded" << endl;
        }
        else {
            create_Omega(deviation_Omega);
            cout << "Omega-d=" << deviation_Omega << "\t\tloaded" << endl;

        }
        if (deviation_Theta == 0) {
            read_init(phi_initial, address_theta);
            cout << address_theta + ".txt" + "\t\tloaded" << endl;
        }
        else {
            create_Theta(deviation_Theta);
            cout << "Theta-d=" << deviation_Theta << "\t\tloaded" << endl;
        }
        if (data17 == 1)
            cout << "coupling_synchrony.txt\tloaded" << endl;
        if (data16 == 1) {
            cout << "Time_synchrony.txt\tloaded" << endl;
            T_sync << '\t';
            for (double t = 0; t < tfinal; t++) {                    //loop over time step
                T_sync << t * dt << '\t';
            }
            T_sync << endl;
        }
    }
    void initial(double,double);                                                      //call initial in wangbuzsaki class
    double Run();
    int** read_to_array(string);
    void create_Omega(double);
    void create_Theta(double);
    int** pro_Matrix(double);
    void moving_Theta();
    void read_init(double*, string);
    double  dphidt(double, int*, double, double**);     //calculate dphi/dt
    void RK4_allnode(double*, int**, double*, double*);//calculate runge kutta4
    double order_parameter(double*);  //calculate syncrony
    void scale_2_pi(double*);     //put variable phi in 0 to 2pi
};                                                                              //
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                                                                @@@@
//@@@                                    initial                                     @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::initial(double copl, double dil) {                                                      //call initial in wangbuzsaki class
    coupling = copl;
    delay = dil;
    moving_Theta();
}                                                                               //
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                                                                @@@@
//@@@                                     RUN                                        @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
double Kuramoto::Run() {                                                      //call initial in wangbuzsaki class
    double sync = 0.0;
    double syncron = 0.0;
    int timestep_save = 2000;//(4.0*tfinal)/5.0;
    if (data16 == 1) {
        T_sync << coupling << '\t';

        T_phi << '\t';
        for (int i = 0; i < N; i++)
        {
            T_phi << i << '\t';
        }
        T_phi << '\n';
    }
    for (int t = 0; t < tfinal; t++)                    //loop over time step
    {
        if (data16 == 1) {
            T_phi << t * dt << '\t';
            for (int i = 0; i < N; i++)
            {
                T_phi << sin(phi[i]) << '\t';
            }
            T_phi << '\n';
        }
        double R = 0.0;
        RK4_allnode(W, adj, phi, phi2);
        scale_2_pi(phi);                //change values to be in range 0 to 2*Pi
        R = order_parameter(phi);


        if (data16 == 1)
            T_sync << R << '\t';
        if (t >= timestep_save)
            sync += R;
        if (data16 == 1)
            T_sync1 << t * dt << '\t' << R << endl;

    }
    if (data16 == 1)
        T_sync << endl;

    syncron = sync / (tfinal - timestep_save);
    if (syncron > 1)
        syncron = 1;
    if (data17 == 1)
        syncrony << coupling << '\t' << syncron << endl;
    cout << delay << '\t' << coupling << '\t' << syncron << endl;

    return syncron;
}                                                                               //
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                            runge_kutta4_integrator                             @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::RK4_allnode(double* W, int** adj, double* phi, double* phi2)
{



    for (int i = 0; i < N; i++)
    {

        double k1 = dphidt(W[i], adj[i], phi[i], phi_history);
        double k2 = dphidt(W[i], adj[i], phi[i] + k1 * dt / 2.0, phi_history);
        double k3 = dphidt(W[i], adj[i], phi[i] + k2 * dt / 2.0, phi_history);
        double k4 = dphidt(W[i], adj[i], phi[i] + k3 * dt, phi_history);
        phi2[i] = phi[i] + dt / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    }


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < history - 1; j++) {
            phi_history[i][j] = phi_history[i][j + 1];
        }
        phi_history[i][history - 1] = phi2[i];
    }

    for (int i = 0; i < N; i++) { phi[i] = phi2[i]; }
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                     dydt                                       @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
double Kuramoto::dphidt(double W, int* is_connected, double phi, double** phi_hist)
{
    double M = 0;
    double a = 0.0;
    for (int i = 0; i < N; i++)
    {
        a += (is_connected[i] * sin((phi_hist[i][0] - phi)));
    }
    M = W + (coupling / (N * 1.0)) * a;
    return M;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                          Read matrix connection                                @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int** Kuramoto::read_to_array(string str)
{
    int** a;
    a = new int* [N];
    for (int i = 0; i < N; i++)
        a[i] = new int[N];
    ifstream fp("Example/" + str);
    if (!fp)
    {
        cout << "Matrix file is not here!" << endl;
        return a;
    }
    else
    {
        for (int i = 0; i < N; i++)
        {

            for (int j = 0; j < N; j++)
            {
                int elem = 0;
                fp >> elem;
                a[i][j] = elem;
            }
        }
        //cout << "Matrix\tloaded" << endl;
    }
    return a;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                          fully matrix connection                               @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int** Kuramoto::pro_Matrix(double P)
{
    int** a;
    a = new int* [N];
    for (int i = 0; i < N; i++)
        a[i] = new int[N];
    random_device rd;                                                           //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());                                                          //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<double> uniform_distribution_teta(0.0, 1.0);      //Uniform real distribution from 0 to 1
    for (int i = 0; i < N; i++) {
        if (i == 0) {
            a[i][0] = 0;
        }
        else {
            if (uniform_distribution_teta(gen) <= P) {
                a[i][0] = 1;

            }
            else {
                a[i][0] = 0;

            }
        }

        for (int j = 1; j < N; j++) {
            if (i == j) {
                a[i][j] = 0;
            }
            else {
                if (uniform_distribution_teta(gen) <= P) {
                    a[i][j] = 1;

                }
                else {
                    a[i][j] = 0;

                }
            }
        }
    }
    return a;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@           Omega & theta rider                                                  @@@@
//@@@           Omega:    rand mean 1 and standard deviation 0.01 normal_distribution@@@@
//@@@           theta:    crate rand between 0 to 1 uniform_real_distribution        @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::read_init(double* phi_initial, string fname)
{
    ifstream ff("Example/" + fname + ".txt");
    for (int i = 0; i < N; i++)
    {
        ff >> phi_initial[i];
    }
    ff.close();
    //cout << fname+"\tloaded" << endl;
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                     Omega                                      @@@@
//@@@         rand mean 1 and standard deviation 0.01 normal_distribution            @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::create_Omega(double deviation_Omega) {                                                         //calculate w
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();     //using seed time
    default_random_engine generator(seed);                                      //random generator
    normal_distribution<double> dist(0, deviation_Omega);                                //creat random that This distribution produces random numbers around the distribution mean 1 with a specific standard deviation 0.01.
    for (int i = 0; i < N; i++)                                                 //for loop all node
        W[i] = dist(generator);                                                 //create w random for each node
}                                                                               //
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                   moving_Theta                                 @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::moving_Theta() {                                                        //calculate initial theta
    phi_history = new double* [N];

    history = (delay / dt) + 1;
    for (int i = 0; i < N; i++)
        phi_history[i] = new double[history];//[node][delay]

    for (int i = 0; i < N; i++) {                                                 //for loop all node
        //phi[i] = phi_initial[i];                       //create phi random for each node
        phi_history[i][0]= phi_initial[i];//put initial phi rand in 0 history
    }
    //cout << history << endl;
    //put all history acording initional 
    for (int t = 1; t < history; t++)
    {
        for (int i = 0; i < N; i++)
        {
            if (phi_history[i][t - 1] >= (double)(Pi / 2.0))
            {
                phi_history[i][t] = phi_history[i][t - 1] - (double)(1.5 * Pi);
            }
            else
            {
                phi_history[i][t] = phi_history[i][t - 1] + (double)(Pi / 2.0);
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (phi_history[i][history - 1] >= (double)(Pi / 2.0))
        {
            phi[i] = phi_history[i][history - 1] - (double)(1.5 * Pi);
        }
        else
        {
            phi[i] = phi_history[i][history - 1] + (double)(Pi / 2.0);
        }
    }


}                                                                               //
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                     theta                                      @@@@
//@@@              crate rand between 0 to 1 uniform_real_distribution               @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::create_Theta(double deviation_Theta) {                                                        //calculate initial theta
    random_device rd;                                                           //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd());                                                          //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<float> uniform_distribution_teta(0.0, deviation_Theta);       //Uniform real distribution from 0 to 1
    for (int i = 0; i < N; i++)                                                 //for loop all node
        phi_initial[i] = uniform_distribution_teta(gen) * 2 * Pi;                       //create phi random for each node
}                                                                               //
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                                     sync                                       @@@@
//@@@                              Order parameter synchrony                         @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
double Kuramoto::order_parameter(double* phi)
{
    double rc = 0.0, rs = 0.0;
    for (int j = 0; j < N; j++)
    {
        rc += cos(phi[j]);
        rs += sin(phi[j]);
    }
    return sqrt(pow(rc, 2) + pow(rs, 2)) / (1.0 * N);
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@                         scaled in 2_pi if Cross 2pi                            @@@@
//@@@                                                                                @@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Kuramoto::scale_2_pi(double* phi)
{
    for (int i = 0; i < N; i++)
    {
        phi[i] = phi[i] - 2 * Pi * static_cast<int>(phi[i] / (2 * Pi));
    }
}


double* read_data(double* data)
{

    string kk;
    ifstream fp("data.txt");
    if (!fp)
    {
        cout << "Matrix file is not here!" << endl;
    }
    else
    {
        fp >> kk;
        for (int i = 1; i < 19; i++)
        {
            fp >> kk;
            data[i] = stod(kk);
        }
    }

    cout << "coupling_synchrony =\t" << data[1] << endl;
    cout << "Time_synchrony =\t" << data[2] << endl;
    cout << "delay_coupling_sync =\t" << data[3] << endl;
    cout << "number =\t\t" << data[4] << endl;
    cout << "dt =\t\t\t" << data[5] << endl;
    cout << "step =\t\t\t" << data[6] << endl;
    cout << "probability_Matrix =\t" << data[7] << endl;
    cout << "deviation_Omega =\t" << data[8] << endl;
    cout << "deviation_Theta =\t" << data[9] << endl;
    cout << "coupling_start =\t" << data[10] << endl;
    cout << "coupling_end =\t\t" << data[11] << endl;
    cout << "coupling_step =\t\t" << data[14] << endl;
    cout << "delay_start =\t\t" << data[12] << endl;
    cout << "delay_end =\t\t" << data[13] << endl;
    cout << "delay_step =\t\t" << data[15] << endl;
    return data;
}



void  gnuplot(double* data)
{
    if (data[1] == 1)
        system("gnuplot -p -e \"set style line 12 lc rgb '#808080' lt 0 lw 1;set grid back ls 12;set xlabel 'coupling';set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5;set ylabel 'synchrony';set title 'coupling VS. synchrony';set autoscale fix;set terminal png size 1024,768;set output 'Save\\coupling_synchrony.png';plot 'Save\\coupling_synchrony.txt' title 'r ' with linespoints ls 1;set terminal pop;set output;replot\"");
    if (data[2] == 1)
        system("gnuplot -p -e \"set style line 12 lc rgb '#808080' lt 0 lw 1;set grid back ls 12;set xlabel 'time (t)';set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5;set ylabel 'synchrony (r)';set title 'time VS. synchrony';set autoscale fix;set terminal png size 1024,768;set output 'Save\\time_synchrony1.png';plot 'Save\\Time_synchrony1.txt' title 'k ' with lines ls 1;set terminal pop;set output;replot\"");
    if (data[3] == 1)
        system("gnuplot -p -e \"set ylabel 'coupling (k)';set xlabel 'delay (t)';set palette grey;set title 'synchrony in terms of delay and coupling';set autoscale fix;set terminal png size 1024,768;set output 'Save\\coupling_delay_synchrony.png';plot 'Save\\coupling_delay_synchrony.txt' matrix with image;set terminal pop;set output;replot\"");
}

void  implementation(double* data)
{
    //call class kuramato (number,dt,step,coupling,probability_Matrix,deviation_Omega,deviation_Theta,delay)
    Kuramoto kR(int(data[4]), data[5], (int(data[6]) / data[5]), data[10], data[7], data[8], data[9], data[12], data[16], data[17]);//call class and input initial variable
    for (double i = data[10]; i < data[11] + data[5]; i += data[14]) {                                //copling
        for (double j = data[12]; j < data[13] + data[5]; j += data[15]) {
            kR.initial(i, j);
            double syn = kR.Run();
            if (data[18] == 1)
                cds << syn << '\t';
        }
        if (data[18] == 1)
            cds << endl;
    }
}