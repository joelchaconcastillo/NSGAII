#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <random>
#include "Benchmark.hpp"
#include "Objectives.hpp"
using namespace std;
Objectives::Objectives()
{
}
Objectives::Objectives(int NObjectivesByIndividual)
{
    this->NObjectivesByIndividual = NObjectivesByIndividual;
}
void Objectives::setObjectivesByIndividual(int N)
{
    this->NObjectivesByIndividual = N;
}
bool Objectives::Dominate(Objectives &objB)
{
    for(int i = 0; i < this->NObjectivesByIndividual; i++)
    {
        if(this->SpaceObjectives[i].Type== MAXIMIZE )
        {
            if( this->SpaceObjectives[i].Fitness < objB.SpaceObjectives[i].Fitness)
            {
                return false;
            }
        }
         else if(this->SpaceObjectives[i].Type == MINIMIZE )
         {
            if( this->SpaceObjectives[i].Fitness > objB.SpaceObjectives[i].Fitness)
            {
                return false;
            }
         }
    }
    return true;
}

void Objectives::Eval()
{
        vector <double> obj(this->NObjectivesByIndividual);
        this->ObjBenchmark->Eval(this->DecisionVariables, obj);

        for(int i = 0; i < obj.size(); i++)
            this->SpaceObjectives[i].Fitness = obj[i];
}
void Objectives::setBenchmark(Benchmark *ObjBenchmark)
{
        this->ObjBenchmark = ObjBenchmark;

        this->SpaceObjectives.resize(ObjBenchmark->getNObjectives());

        for(int i =0 ;i< ObjBenchmark->getNObjectives(); i++)
        {
            this->SpaceObjectives[i].Type = ObjBenchmark->getTypeDuality()[i];
        }
        this->DecisionVariables.resize(ObjBenchmark->getDimension());
        this->NObjectivesByIndividual = ObjBenchmark->getNObjectives();
        this->Dimension = ObjBenchmark->getDimension();

}
