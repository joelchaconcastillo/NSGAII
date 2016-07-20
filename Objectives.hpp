#ifndef OBJECTIVES_HPP_INCLUDED
#define OBJECTIVES_HPP_INCLUDED
#include <vector>
#include <bitset>
using namespace std;
struct Objective{
            double Fitness;
            int Type;
        };
class Objectives
{
    private:
        int NObjectivesByIndividual, Dimension;
        Benchmark *ObjBenchmark;
    public:
        vector<double> DecisionVariables;
        vector<Objective> SpaceObjectives;
        Objectives();
        Objectives(int NObjectivesByIndividual);

        inline double getNumberObjectives(){return this->NObjectivesByIndividual;}
        inline void ConfigurateObjectives();

        inline void setDimension(int Dimension){this->Dimension = Dimension;}
        void setObjectivesByIndividual(int N);
        void setBenchmark(Benchmark *ObjBenchmark);

        bool Dominate(Objectives &ObjB);
        void Eval();

};


#endif // OBJECTIVES_HPP_INCLUDED
