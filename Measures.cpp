#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Measures.hpp"
using namespace std;

Measures::Measures(vector<vector<double> >&SetPareto, vector<vector<double>> &ParetoOptimalFront)
{
    this->SetPareto = SetPareto;
    this->ParetoOptimalFront= ParetoOptimalFront;
}
vector<double> Measures::MinimumDistance()
{
    vector<double> MinDistance(SetPareto.size(), INFINITY);
    /**
        Encontrar los elementos que se encuentran a menor distancia.
    **/
    for(int i = 0; i < this->SetPareto.size(); i++)
    {

        for(int j = 0; j < this->ParetoOptimalFront.size(); j++)
        {
            double DistanceWithPareto = EuclideanDistance(SetPareto[i], ParetoOptimalFront[j] );
            if( DistanceWithPareto  <  MinDistance[i] )
            {
                MinDistance[i] =  DistanceWithPareto ;
            }
        }
    }
    return MinDistance;
}
double Measures::GenerationalDistance()
{
    int p = 2;

    vector<double> MinDistance = MinimumDistance( );
    double Suma=0;
    for(int i = 0; i < this->SetPareto.size(); i++)
    {
        Suma += pow(MinDistance[i], p);
    }
    return pow(Suma, 1.0/p)/this->SetPareto.size();

}
double Measures::EuclideanDistance(vector<double> SolutionA, vector<double> SolutionB )
{
    double Suma=0;
    for(int i = 0;i < SolutionA.size(); i++)
    {
        Suma += pow( SolutionA[i] - SolutionB[i] ,2);
    }
    return sqrt(Suma);
}

double Measures::DifferenceAbsolute(vector<double> SolutionA, vector<double> SolutionB )
{
    double Suma=0;
    for(int i = 0; i < SolutionA.size(); i++)
    {
        Suma += fabs( SolutionA[i] - SolutionB[i]);
    }
    return Suma;
}
vector<double> Measures::DistanceSolutions()
{
 /*   vector<double> Distances;
    for(int i =0 ; i < this->SetPareto-1; i++)
    {
        Distance.push_back( this->EuclideanDistance(this->SetPareto[i], this->SetPareto[i+1]) );
    }*/
}
vector<double> Measures::MinimalAbsoluteDifference()
{
    vector<double> MinDifference(this->SetPareto.size(), INFINITY);

    for(int i = 0; i < this->SetPareto.size(); i++)
    {
        for(int j = 0; j < this->SetPareto.size(); j++)
        {
            if(i == j) continue;
            double DistanceBetweenPareto = DifferenceAbsolute(SetPareto[i], SetPareto[j] );
            if( DistanceBetweenPareto <  MinDifference[i] )
            {
                MinDifference[i] =  DistanceBetweenPareto ;
            }
        }
    }
    return MinDifference;
}
double Measures::Spacing()
{
    vector<double> MinAbsoluteDifference = MinimalAbsoluteDifference();
    double MeanDifferences = Mean(MinAbsoluteDifference);

    double Suma = 0;
    for(int i = 0; i < MinAbsoluteDifference.size(); i++)
    Suma += pow( MinAbsoluteDifference[i]- MeanDifferences ,2);
    return sqrt(Suma / MinAbsoluteDifference.size());
}
double Measures::Spread(vector<int> &IndexBoundsObjectives)
{
    this->IndexBoundsObjectives = IndexBoundsObjectives;
    vector<double> DistancesEx = DistancesExtremes();
    double Termino1 = 0;

    for(int i = 0; i < DistancesEx.size(); i++)
        Termino1 += DistancesEx[i];


    vector<double> MinAbsoluteDifference = MinimalAbsoluteDifference();
    MinAbsoluteDifference.resize(MinAbsoluteDifference.size()-1);
    double MeanDifferences = Mean(MinAbsoluteDifference);

    double Termino2 = 0;

    for(int i = 0; i < MinAbsoluteDifference.size(); i++)
        Termino2 += fabs( MinAbsoluteDifference[i]- MeanDifferences);

    /**
            Obtener los indices del frente de forma ordenada
    **/
    double Termino3 = MinAbsoluteDifference.size()*MeanDifferences ;

    double Numerador = Termino1 + Termino2;
    double Denominador = Termino1 + Termino3;
    return Numerador / Denominador;
}
double Measures::Mean(vector<double> &data)
{
    double Suma = 0 ;
    for(int i=0; i < data.size(); i++)
    Suma+=data[i];
    return Suma / data.size();
}
double Measures::Sum(vector<double> &data)
{
    double Suma = 0;
    for(int i = 0; i < data.size(); i++)
        Suma+= data[i];
    return Suma;
}
vector<double> Measures::DistancesExtremes()
{

    /**
        En teoría sólo se tienen dos extremos.
    **/
    int NExtremos=this->IndexBoundsObjectives.size();
    vector<double> Extremes( NExtremos, INFINITY);
    /**
        Obtener las distancias minimas...
    **/
    for(int i = 0; i < Extremes.size(); i++)
    {
        for(int j = 0; j < this->SetPareto.size(); j++)
        {
            double Diff = DifferenceAbsolute( this->ParetoOptimalFront[ this->IndexBoundsObjectives[i]], this->SetPareto[j]);
            if(  Diff < Extremes[i] )
            {
                Extremes[i] = Diff;
            }
        }
    }
    return Extremes;
}
double Measures::HyperVolume(vector<double> &W)
{

    /**
        Ordenar el frente calculado en base a una función objetivo

    **/
    sort( this->SetPareto.begin(), this->SetPareto.end(), ObjetiveSort());
    double Volume = 0;

    //Extremo superior..
    double Volumen =  (W[0]-SetPareto[0][0])*(W[1]-SetPareto[0][1]);
    for(int i = 1; i < this->SetPareto.size(); i++)
    {
        Volumen +=  (W[0]-SetPareto[i][0])*(SetPareto[i-1][1]-SetPareto[i][1]);
    }
    return Volumen;

}
double Measures::HyperVolumeRatio(vector<double> &W)
{

    /**
        Ordenar el frente calculado en base a una función objetivo

    **/
    sort( this->SetPareto.begin(), this->SetPareto.end(), ObjetiveSort());
    sort( this->ParetoOptimalFront.begin(), this->ParetoOptimalFront.end(), ObjetiveSort());

    //Extremo superior..
    double HVPareto =  (W[0]-SetPareto[0][0])*(W[1]-SetPareto[0][1]);
    for(int i = 1; i < this->SetPareto.size(); i++)
    {
        HVPareto +=  (W[0]-SetPareto[i][0])*(SetPareto[i-1][1]-SetPareto[i][1]);
    }

    double HVOptimal =  (W[0]-ParetoOptimalFront[0][0])*(W[1]-ParetoOptimalFront[0][1]);
    for(int i = 1; i < this->ParetoOptimalFront.size(); i++)
    {
        HVOptimal +=  (W[0]-ParetoOptimalFront[i][0])*(ParetoOptimalFront[i-1][1]-ParetoOptimalFront[i][1]);
    }
    return HVPareto/HVOptimal;

}
double Measures::HyperDistance(vector<double> &W)
{
    double SumDistantcesV = 0;
    for(int i = 0; i < this->SetPareto.size(); i++)
    {
        SumDistantcesV += EuclideanDistance(W, SetPareto[i]);
    }

    double SumDistantcesOptimal = 0;
    for(int i = 0; i < this->ParetoOptimalFront.size(); i++)
    {
        SumDistantcesOptimal += EuclideanDistance(W, ParetoOptimalFront[i]);
    }
    return (SumDistantcesV / this->ParetoOptimalFront.size())/(SumDistantcesOptimal/this->SetPareto.size());
}
