#include <iostream>
#include "Measures.hpp"
#include "Benchmark.hpp"
#include "NSGAII.hpp"
using namespace std;

int main()
{
    int SizePool=100, TotalGenerations=250;
    double ProbCross= 0.5, NBitsMut=1;
    Benchmark ObjBenchmark(SCH1);

    NSGAII ObjNSGAII(SizePool, ProbCross, NBitsMut, ObjBenchmark, TotalGenerations, "Summary.txt");
    ObjNSGAII.Init_NSGAII();
    //ObjNSGAII.PlotInterfaceR();
    /**
        Obtener el conjunto de soluciones óptimos y obtenidos por NSGAII y
        efectuar una métrica.
    **/
    vector<vector<double>> SetFront, SetParetoOptimal;
    vector<int> IndexBoundsObjectives;
    /*vector<double> Temp;


    Temp.push_back(1.0);
    Temp.push_back(7.5);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();

    Temp.push_back(1.1);
    Temp.push_back(5.5);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();

    Temp.push_back(2.0);
    Temp.push_back(5.0);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();

    Temp.push_back(3.0);
    Temp.push_back(4.0);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();

    Temp.push_back(4.0);
    Temp.push_back(2.8);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();

    Temp.push_back(5.5);
    Temp.push_back(2.5);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();


    Temp.push_back(6.8);
    Temp.push_back(2.0);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();

    Temp.push_back(8.4);
    Temp.push_back(1.2);
    SetParetoOptimal.push_back(Temp);
    Temp.clear();
////////////////////////
    Temp.push_back(1.2);
    Temp.push_back(7.8);
    SetFront.push_back(Temp);
    Temp.clear();

    Temp.push_back(2.8);
    Temp.push_back(5.1);
    SetFront.push_back(Temp);
    Temp.clear();


    Temp.push_back(4.0);
    Temp.push_back(2.8);
    SetFront.push_back(Temp);
    Temp.clear();

    Temp.push_back(7.0);
    Temp.push_back(2.2);
    SetFront.push_back(Temp);
    Temp.clear();


    Temp.push_back(8.4);
    Temp.push_back(1.2);
    SetFront.push_back(Temp);
    Temp.clear();

    IndexBoundsObjectives.push_back(0);
    IndexBoundsObjectives.push_back(7);*/

    ObjNSGAII.getSolutions(SetFront, SetParetoOptimal, IndexBoundsObjectives);
    Measures ObjMeasures(SetFront, SetParetoOptimal);

    cout << "Distancia Generacional (mide la convergencia): "<<ObjMeasures.GenerationalDistance()<<endl;
    cout << ObjMeasures.Spacing()<<endl;
    cout << ObjMeasures.Spread(IndexBoundsObjectives)<<endl;
    vector <double> W(2);
    W[0] = 20;
    W[1] = 20;
    //cout << ObjMeasures.HyperVolume(W);
    cout << ObjMeasures.HyperVolumeRatio(W);
   // cout << ObjMeasures.HyperDistance(W);
    return 0;
}
