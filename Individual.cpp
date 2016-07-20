#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>
#include "Benchmark.hpp"
#include "Objectives.hpp"
#include "Individual.hpp"
using namespace std;
Individual::Individual()
{
    this->Distance=0;
}
void Individual::InitializeIndividual(Benchmark *ObjBenchmark)
{

        this->nbits = ObjBenchmark->getNBits();
        this->NumberObjectives = ObjBenchmark->getNObjectives();
        this->Dimension = ObjBenchmark->getDimension();
        this->DecisionVariables.resize( this->Dimension, vector<bool>(nbits));
        this->setRandom();
        ObjObjectives.setBenchmark(ObjBenchmark);
        Bounds = ObjBenchmark->Bounds;
        this->EvalIndividual();

}
void Individual::EvalIndividual()
{
    this->DecodeIndividual(ObjObjectives.DecisionVariables);
    ObjObjectives.Eval();
}
void Individual::DecodeIndividual(vector<double> &Genotype)
{
        //double Min = -100.0, Max = 100.0;
        for(int i = 0; i < this->Dimension; i++)
        {
            Genotype[i] = (getBase10(this->DecisionVariables[i], Bounds[i][0], Bounds[i][1]));
        }

}
vector<bool> Individual::random_bool( double p )
{
    vector<bool> bits(this->nbits);
    std::random_device rd;
    std::mt19937 gen( rd());
    std::bernoulli_distribution d( p);
    /**
        si el número de bits son muchos,
        entonces existe la posibilidad de un desboradmiento
        por el tipo de dato, por lo que sólo se randomiza
        una sección de cromosoma, como un centrómero, o
        los extremos adaptadores de 3 prima o 5 prima.
    **/
    for( int n = 0; n < bits.size()/10; ++n) {
        bits[n] = d(gen);
    }
    return bits;
}
void Individual::setRandom()
{
        for(int j = 0; j < this->Dimension; j++)
        {
                DecisionVariables[j] = random_bool();
                /*for(int i = 0;  i < DecisionVariables[j].size(); i++)
                cout << DecisionVariables[j][i]<<" ";
                cout << endl;*/
        }
}
int Individual::BinarytoInt(vector<bool> &Individuo)
{
    int Integer=0;
    for(int i =0; i < Individuo.size(); i++)
    {
        if(Individuo[i])
            Integer+=pow(2, Individuo.size()-1-i);
    }
    return Integer;
}
double Individual::getBase10(vector<bool> &Individuo, double Min , double Max)
{
    double Delta = (double)(Max - Min) / ( pow(2,this->nbits) -1);
    double Xbase10 = (double) BinarytoInt(Individuo);
    Xbase10 =(Xbase10 *Delta) +Min;
    return Xbase10;
}

