#include <fstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "Measures.hpp"
#include "Benchmark.hpp"
#include "Individual.hpp"
#include "NSGAII.hpp"
ostream & operator << ( ostream &out,vector<Individual> Pool)
{
    for(int i = 0; i < Pool.size(); i++)
    {
        for(int j = 0; j < Pool[i].getObjectives().getNumberObjectives() ; j++)
         out << Pool[i].getObjectives().SpaceObjectives[j].Fitness<<" ";
         out << endl;
    }
    return out;
}
ostream & operator << ( ostream &out,vector<double> data)
{
    for(int i = 0; i < data.size(); i++)
    {
        out << data << endl;
    }
    return out;
}
vector<Individual> operator+(vector<Individual> PoolA, vector<Individual> PoolB )
{
    vector<Individual> C;
    for(int i=0 ; i < PoolA.size(); i++ )
        C.push_back(PoolA[i]);
    for(int i = 0; i < PoolB.size(); i++)
        C.push_back(PoolB[i]);
    return C;
}
NSGAII::NSGAII(int SizePool, double ProbCross, int NBitsMut, Benchmark &ObjBenchmark, int TotalGenerations, string FileNameSummary )
{
    srand(time(NULL));
    this->SizePool = SizePool;
    this->NumberObjectives = ObjBenchmark.getNObjectives();
    this->Dimension = ObjBenchmark.getDimension();
    this->ProbCross = ProbCross;
    this->NBitsMut = NBitsMut;
    this->ObjBenchmark = &ObjBenchmark;
    this->TotalGenerations = TotalGenerations;
    this->CurrentGeneration=0;
    this->FileNameSummary = FileNameSummary ;
    this->FileNameFront = "ParetoFront.txt";
    remove(FileNameSummary.c_str());

}
NSGAII::~NSGAII()
{
}
void NSGAII::Init_NSGAII()
{
    InitializePool(Pool_P);
    Fast_Non_Dominated_Sort(Pool_P);
    New_Pop(Pool_P, Pool_Q);
    while(CriterionStop())
    {
        Fronts.clear();
        vector<Individual> Pool_R = Pool_P + Pool_Q;
        Fast_Non_Dominated_Sort(Pool_R);

        int i = 0;
        Pool_P.clear();

        while(Pool_P.size() + Fronts[i].size() <= this->SizePool)
        {
            Crowding_Distance_Assignment(Fronts[i]);
            Pool_P  = Pool_P  + Fronts[i];
            i++;
        }
            /**
                Obtener las distancias del último frente calculado
            **/
            Crowding_Distance_Assignment(Fronts[i]);
            sort( Fronts[i].begin(), Fronts[i].end(), CrowdedComparationOperator());
            if(Pool_P.size() + Fronts[i].size() > this->SizePool)
            {
                int OverSize = Pool_P.size() + Fronts[i].size() - this->SizePool ;
                int Diference = Fronts[i].size() - OverSize;
                vector<Individual> Temporal(Fronts[i].begin(),Fronts[i].begin()+Diference);
                Pool_P = Pool_P +  Temporal ;
            }
        Pool_Q.clear();
        New_Pop(Pool_P, Pool_Q);
        ReportSpreadConvergence();
    }
        WriteFilePool();
        //cout << Pool_P<<endl;
        this->PlotInterfaceR();
}
/**
    Agregar información al final del archivo...
**/
void NSGAII::ReportSpreadConvergence()
{
    //cout << Pool_P<< endl;
   // getchar();
    //this->PlotInterfaceR();
    vector<vector<double>> SetFront, SetParetoOptimal;
    vector<int> IndexBoundsObjectives;
    getSolutions(SetFront, SetParetoOptimal,IndexBoundsObjectives);
    Measures ObjMeasures(SetFront, SetParetoOptimal);

    ofstream Summary;
    Summary.open (FileNameSummary, ios::app);
    if(this->CurrentGeneration == 1)
    {
       // Summary << "Generation\tGenerational_Distance\tSpacing\tSpread\tHyperVolume\tHyperVolumeRatio" <<endl;
    }
    vector<double> W;
    for(int i = 0; i < ObjBenchmark->getNObjectives();i++) W.push_back(8);
    Summary << this->CurrentGeneration<<"\t";
    Summary << ObjMeasures.GenerationalDistance()<<"\t";
    Summary<<ObjMeasures.Spacing()<<"\t";
    Summary<<ObjMeasures.Spread(IndexBoundsObjectives)<<"\t";
    Summary<<ObjMeasures.HyperVolume(W)<<"\t";
    Summary<<ObjMeasures.HyperVolumeRatio(W)<<endl;
    Summary.close();


//    cout << ObjMeasures.GenerationalDistance()<< " "<<Cont<<endl;


}
void NSGAII::WriteFilePool()
{
    ofstream Summary;
    Summary.open (FileNameFront);
    for(int i =0 ; i < this->Pool_P.size(); i++)
    {
        for(int j = 0; j < this->ObjBenchmark->getNObjectives(); j++)
        {
            Summary<<Pool_P[i].getObjectives().SpaceObjectives[j].Fitness<< "\t";
        }
        Summary<<endl;
    }


    Summary.close();

}
void NSGAII::InitializePool(vector<Individual> &Pool_P)
{
    Pool_P.resize(this->SizePool);
    for(int i = 0; i < this->SizePool; i++)
    {
        Pool_P[i].InitializeIndividual(this->ObjBenchmark);
    }
}

void NSGAII::New_Pop(vector <Individual> &Poop_P, vector <Individual> &Pool_Q)
{
    Selection(Poop_P, Pool_Q);
    Recombination(Pool_Q);
    Mutation(Pool_Q);
    Evaluation(Pool_Q);

    /*RecombinationDE(Pool_P, Pool_Q);
    MutationDE(Pool_P, Pool_Q);
    EvaluationDE(Pool_Q);*/
}
void NSGAII::Selection(vector <Individual> &Poop_P, vector <Individual> &Pool_Q)
{
    for(int i = 0; i < Pool_P.size(); i++)
    {
        int Index1 = rand()%Pool_P.size();
        int Index2 = rand()%Pool_P.size();

        if( Pool_P[Index1].getRank() < Pool_P[Index2].getRank() )
        {
            Pool_Q.push_back(Pool_P[Index1]);
        }
        else if(Pool_P[Index1].getRank() == Pool_P[Index2].getRank())
        {
            if(Pool_P[Index1].getDistance() > Pool_P[Index2].getDistance())
            {
                Pool_Q.push_back(Pool_P[Index1]);
            }
            else if(Pool_P[Index1].getDistance() < Pool_P[Index2].getDistance())
            {
                Pool_Q.push_back(Pool_P[Index2]);
            }else
            {
                if(getRandom(0,1.0) <= 0.5)
                {
                    Pool_Q.push_back(Pool_P[Index1]);
                }else
                    Pool_Q.push_back(Pool_P[Index2]);
            }
        }
        else
        {
            Pool_Q.push_back(Pool_P[Index2]);
        }
    }
}
void NSGAII::Recombination(vector <Individual> &Pool_Q)
{
    UniformCross(Pool_Q);
}
void NSGAII::Mutation(vector <Individual> &Pool_Q)
{
    for(int i = 0; i < Pool_Q.size(); i++)
    {
        for(int d = 0; d < Pool_Q[i].getDimension(); d++)
        {
            int Size = Pool_Q[i].DecisionVariables[d].size();
            for(int k = 0; k < NBitsMut; k++)
            {
                      int IndexRandom = rand()%Size;
                      Pool_Q[i].DecisionVariables[d][IndexRandom] = (Pool_Q[i].DecisionVariables[d][IndexRandom])?0:1;
            }
        }
    }
}
void NSGAII::Evaluation(vector<Individual> &Pool_Q)
{
    for(int i = 0; i < Pool_Q.size(); i++)
    {
        Pool_Q[i].EvalIndividual();
        Pool_Q[i].setDistance(0);
    }
}
double NSGAII::getRandom(double Min, double Max)
{
    double f = (double)rand() / RAND_MAX;
    return Min + f * (Max - Min);
}
void NSGAII::UniformCross(vector <Individual> &Pool_Q)
{
    vector <Individual> Temporal(Pool_Q.size());
    Temporal = Pool_Q;
    for(int i = 0; i < Pool_Q.size(); i++)
    {
        int Index1 = rand() % Pool_Q.size();
        int Index2 = rand() % Pool_Q.size();

        for(int d = 0; d < Pool_Q[i].getDimension(); d++)
        {

            for(int gen = 0 ; gen < Pool_Q[i].DecisionVariables[d].size(); gen++)
            {
                double RandomDouble = this->getRandom(0.0, 1.0);
                if(RandomDouble < ProbCross)
                {
                    Temporal[i].DecisionVariables[d][gen] = Pool_Q[Index1].DecisionVariables[d][gen];
                }else
                {
                    Temporal[i].DecisionVariables[d][gen] = Pool_Q[Index2].DecisionVariables[d][gen];
                }
            }
        }
    }
    Pool_Q = Temporal;
}
void NSGAII::RecombinationDE(vector <Individual> &Pool_P, vector <Individual> &Pool_Q)
{
    CrossDE(Pool_P, Pool_Q);
}
void NSGAII::MutationDE(vector <Individual> &Pool_P, vector <Individual> &Pool_Q)
{
    double F = 0.9;
    for(int i = 0 ; i <  Pool_Q.size(); i++)
    {
            double r1 = rand()%Pool_Q.size();

            while(r1==i) r1 = rand()%Pool_Q.size();

            double r2 = rand()%Pool_Q.size();
            while(r2==i || r1==r2) r2 = rand()%Pool_Q.size();

            double r3 = rand()%Pool_Q.size();

            while(r3==i || r3==r1 || r3 == r2) r3 = rand()%Pool_Q.size();

            for(int j = 0; j < Pool_Q[i].getDimension(); j++)
            {
                //Pool_Q[i].DecisionVariablesReal[j] = Pool_P[r1].DecisionVariablesReal[j] + F*( Pool_P[r2].DecisionVariablesReal[j] - Pool_P[r3].DecisionVariablesReal[j] );
            }
    }
}
void NSGAII::EvaluationDE(vector<Individual> &Pool_Q)
{
    for(int i = 0; i < Pool_Q.size(); i++)
    {
//        Pool_Q[i].getObjectives().EvalReal();
    }
}
void NSGAII::CrossDE(vector <Individual> &Pool_P, vector <Individual> &Pool_Q)
{
    for(int i = 0; i < Pool_Q.size(); i++)
        {
            for(int j = 0; j < Pool_P[i].getDimension(); j++)
            {
                int JRand = rand()%Pool_P.size();
                if( !(this->getRandom(0,1.0) <= this->ProbCross || JRand == j ) )
                  {
//                    Pool_Q[i].getObjectives().DecisionVariablesReal[j] = Pool_P[i].getObjectives().DecisionVariablesReal[j];
  //                  this->PoblacionTemporal[i][j] = this->Poblacion[i][j];
                  }
//              this->PoblacionTemporal[i][this->Dim] = FuncionObjetivo(this->PoblacionTemporal[i]);
            }
        }
}
bool NSGAII::CriterionStop()
{
    if(this->CurrentGeneration < this->TotalGenerations)
    {
        this->CurrentGeneration++;
        return true;
    }
    else
    {
        return false;
    }
}
void NSGAII::Crowding_Distance_Assignment(vector<Individual> &I)
{
    for(int i = 0 ; i < I.size(); i++)
        I[i].setDistance(0);
      /*if(this->Fronts.size()==2)
      {
        return;
      }*/
        for(int m = 0; m < I[0].getNumberObjectives(); m++)
        {
            sort( I.begin(), I.end(), LocalIndividual(m));

            I[0].setDistance(1e14);
            I[I.size()-1].setDistance(1e14);
            double MinFitness = I[0].getObjectives().SpaceObjectives[m].Fitness;
            double MaxFitness = I[I.size()-1].getObjectives().SpaceObjectives[m].Fitness;
            for(int k = 1; k < I.size()-1; k++)
            {

                double NormalizedDiference = (I[k+1].getObjectives().SpaceObjectives[m].Fitness - I[k-1].getObjectives().SpaceObjectives[m].Fitness);
                NormalizedDiference/=(MaxFitness-MinFitness);
                /*cout << I[k+1].getObjectives().SpaceObjectives[m].Fitness<<endl;
                cout << I[k-1].getObjectives().SpaceObjectives[m].Fitness<<endl;
                cout << NormalizedDiference<<endl;
                getchar();
                */I[k].setDistance(I[k].getDistance() + NormalizedDiference );
                //cout << I[k].getDistance()<<endl;
            }
        }

}
void NSGAII::Fast_Non_Dominated_Sort(vector<Individual> &Pool_P)
{
    vector< vector<int> > F(1), S(Pool_P.size());
    vector<int> n(Pool_P.size(),0);

    for(int i = 0; i < Pool_P.size(); i++)
    {
        for(int j = 0; j < Pool_P.size(); j++)
        {
            if(j==i) continue;
            if( Pool_P[i].getObjectives().Dominate(Pool_P[j].getObjectives()) )
            {
                S[i].push_back(j);
            }
            else if( Pool_P[j].getObjectives().Dominate(Pool_P[i].getObjectives()))
            {
                n[i]++;
            }
        }
        if( n[i] == 0)
        {
            F[0].push_back(i);
        }
    }
    int k=0;
    while(F[k].size())
    {
        vector<int> Q;
        for(int i = 0; i  < F[k].size(); i++)
        {
            int indexSi = F[k][i];
            for(int j = 0; j < S[indexSi].size(); j++)
            {
                n[S[indexSi][j]]--;
                if(n[S[indexSi][j]]== 0)
                {
                    Q.push_back(S[indexSi][j]);
                }
            }
        }
        k++;
        F.push_back(Q);
    }
     for(int i = 0; i < F.size(); i++)
    {
        //cout << "Frente "<< i<<endl;
        vector <Individual> Front;
        for(int j = 0; j < F[i].size(); j++)
        {
            int indexIndividual = F[i][j];
            Front.push_back(Pool_P[indexIndividual]);
            Pool_P[indexIndividual].setRank(i);
            //cout << F[i][j]<<endl;
        }
        Fronts.push_back(Front);
    }
}
void NSGAII::PlotInterfaceR()
{
    string Comand, X = "", Y = "";

    for(int i = 0; i < this->Pool_P.size(); i++)
    {
            std::stringstream temporal;
            temporal << Pool_P[i].getObjectives().SpaceObjectives[0].Fitness << " , ";
            X += temporal.str();
            temporal.str("");
            temporal << Pool_P[i].getObjectives().SpaceObjectives[1].Fitness << " , ";
            Y += temporal.str();
            temporal.str("");
    }
    X = X.substr(0, X.size()-2);
    Y = Y.substr(0, Y.size()-2);
    Comand = "echo \" plot(x = c("+ X +"), y = c("+Y+"), main=('NSGAII') ,xlab='f1', ylab='f2' ) \" | R --Silent --no-save 2>/dev/null | tail -n 0";
    //Comand = "echo \"plot(x = c(3.47788,4), y = c(4,5) ) \" | R --Silent --no-save 2>/dev/null | tail -n 0";
   // cout << Comand<<endl;
    system(Comand.c_str());
}
void NSGAII::getSolutions(vector<vector<double>> &SetFront, vector<vector<double>> &SetParetoOptimal, vector<int> &IndexBoundsObjectives)
{
    IndexBoundsObjectives.resize(this->ObjBenchmark->getNObjectives(), 0);

    int H = 500;
    for(int i = 0 ; i < Pool_P.size(); i++)
    {
        vector<double> Temp;
        for(int j = 0 ; j < Pool_P[i].getObjectives().SpaceObjectives.size() ;j++)
        {
            Temp.push_back(Pool_P[i].getObjectives().SpaceObjectives[j].Fitness);
        }
        SetFront.push_back(Temp);
    }
    /**
        Obtener el frente de pareto real discretizado.
    **/
    for(int i=0; i < H; i++)
    {
        vector<double> X, obj(this->ObjBenchmark->getNObjectives());
        for(int d=0; d < ObjBenchmark->getDimension(); d++)
        {
            double Min = ObjBenchmark->OptimalBounds[d][0], Max = ObjBenchmark->OptimalBounds[d][1];
            double Delta =  (Max - Min)/H;
            X.push_back( Min+(i*Delta) );
        }
        ObjBenchmark->Eval(X,obj);
        SetParetoOptimal.push_back(obj);
        for(int k = 0; k < this->ObjBenchmark->getNObjectives(); k++)
        {
            if(obj[k] > SetParetoOptimal[IndexBoundsObjectives[k] ][k]  && ObjBenchmark->getTypeDuality()[k] == MAXIMIZE)
            {
                IndexBoundsObjectives[k] = obj.size()-1;
            }
            else if(obj[k] < SetParetoOptimal[IndexBoundsObjectives[k] ][k]  && ObjBenchmark->getTypeDuality()[k] == MINIMIZE)
                IndexBoundsObjectives[k] = obj.size()-1;
        }
    }
    //for(int i = 0; i < SetParetoOptimal.size(); i++)
    //cout << SetParetoOptimal[i]<<endl;
    //getchar();

}
