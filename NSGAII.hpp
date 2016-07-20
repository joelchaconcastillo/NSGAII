#ifndef NSGAII_HPP_INCLUDED
#define NSGAII_HPP_INCLUDED
#include <vector>
#include <iostream>
#include "Individual.hpp"
using namespace std;
class NSGAII
{
    public:
        NSGAII(int SizePool, double ProbCross, int NBitsMut, Benchmark &ObjBenchmark, int TotalGenerations, string FileNameSummary );
        NSGAII();
        ~NSGAII();
        void Init_NSGAII();
        /**
            Esta estructura define el criterio para ordenar
            un conjunto de individuos en base al valor de
            la función objetivo indicada por el índice proporcionado.
        **/
        struct LocalIndividual
        {
            LocalIndividual(int IndexObjective){this->IndexObjective = IndexObjective;}
            bool operator()(Individual A, Individual B) { return A.getObjectives().SpaceObjectives[IndexObjective].Fitness < B.getObjectives().SpaceObjectives[IndexObjective].Fitness;  }
            //bool operator()(int i, int j){return i < j;}
            int IndexObjective;
        };
        /**
            Operador "Crowded-Comparison"
        **/
        struct CrowdedComparationOperator
        {
            CrowdedComparationOperator(){}
            bool operator()(Individual A, Individual B)
            {
                if(A.getRank()  < B.getRank())
                    return true;
                else if(A.getRank() == B.getRank())
                {
                    if(A.getDistance() > B.getDistance() )
                        return true;
                    else
                        return false;
                }
                else
                    return false;
            }
        };
        void PlotInterfaceR();
        void getSolutions(vector<vector<double>> &SetFront, vector<vector<double>> &SetParetoOptimal, vector<int> &IndexBoundsObjectives);
        void WriteFilePool();
    private:
        Benchmark *ObjBenchmark;
        int SizePool, NumberObjectives, Dimension, TotalGenerations, CurrentGeneration;
        double ProbCross;
        string FileNameSummary, FileNameFront;
        int NBitsMut;
        vector<Individual> Pool_P, Pool_Q;
        vector< vector<Individual> > Fronts;
        void InitializePool(vector<Individual> &Pool_P);
        void Fast_Non_Dominated_Sort(vector<Individual> &Pool_P);
        void Crowding_Distance_Assignment(vector<Individual> &Front);
        void New_Pop(vector <Individual> &Poop_P, vector <Individual> &Pool_Q);
        bool CriterionStop();
        void Selection(vector <Individual> &Poop_P, vector <Individual> &Pool_Q);
        void Recombination(vector <Individual> &Pool_Q);
        void Mutation(vector <Individual> &Pool_Q);
        void UniformCross(vector <Individual> &Pool_Q);
        void Evaluation(vector<Individual> &Pool_Q);
        void RecombinationDE(vector <Individual> &Pool_P, vector <Individual> &Pool_Q);
        void CrossDE(vector <Individual> &Pool_P, vector <Individual> &Pool_Q);
        void MutationDE(vector <Individual> &Pool_P, vector <Individual> &Pool_Q);
        void EvaluationDE(vector<Individual> &Pool_Q);
        double getRandom(double Min, double Max);
        void ReportSpreadConvergence();
};
vector< Individual > operator+(vector<Individual> PoolA, vector<Individual> PoolB );
ostream & operator << ( ostream &out,vector<Individual> Pool);
ostream & operator << ( ostream &out,vector<double> data);
#endif // NSGAII_HPP_INCLUDED
