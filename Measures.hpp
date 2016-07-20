#ifndef MEASURES_HPP_INCLUDED
#define MEASURES_HPP_INCLUDED
#include <vector>
using namespace std;
class Measures
{
    private:
        vector<vector<double> > SetPareto;
        vector<vector<double> > ParetoOptimalFront;
        vector<int> IndexBoundsObjectives;
        vector<double> MinimumDistance();
        /**
            Obtiene las distancia entre las soluciones
        **/
        vector<double> DistanceSolutions();
        /**

        **/
        double EuclideanDistance(vector<double> SolutionA, vector<double> SolutionB );
        double DifferenceAbsolute(vector<double> SolutionA, vector<double> SolutionB );
        double Mean(vector<double> &data);
        double Sum(vector<double> &data);
        vector<double> DistancesExtremes();
        vector<double> MinimalAbsoluteDifference();

        struct ObjetiveSort
        {
            bool operator()(vector<double> A, vector<double> B) { return A[1] > B[1];  }
        };

    public:
        Measures(vector<vector<double> >&SetPareto, vector<vector<double>> &ParetoOptimalFront);
        /**
            Medicion de la convergencia
        **/
        double GenerationalDistance();

        /**
            Medicion de la dispersion
        **/
        double Spacing();
        double Spread(vector<int> &IndexBoundsObjectives);
        double HyperVolume(vector<double> &W);
        double HyperVolumeRatio(vector<double> &W);
        double HyperDistance(vector<double> &W);
};


#endif // MEASURES_HPP_INCLUDED
