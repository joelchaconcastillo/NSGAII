#include <iostream>
#include <cmath>
#include "Benchmark.hpp"
using namespace std;
Benchmark::Benchmark()
{
}
Benchmark::Benchmark(int TypeProblem)
{
    this->TypeProblem = TypeProblem;
    this->DefaultDimension = 3;
    this->SizeChrom = 30;
    InitConfig();
}
void Benchmark::SetAllMinimize()
{
    TypeDuality.resize(this->NObjectives, MINIMIZE);
}
void Benchmark::SetAllBounds(vector<vector<double>> &MatrixBound,double Min, double Max)
{
        for(int i=0; i < this->NObjectives; i++)
            {
                vector<double> temp(2);
                temp[0] = Min;
                temp[1] = Max;
                MatrixBound.push_back(temp);
            }
}

void Benchmark::Eval(vector<double> &X, vector<double> &obj)
{
   switch(this->TypeProblem)
    {
        case SCH1:
            sch1(X, obj);
        break;
        case SCH2:
            sch2(X, obj);
        break;
        case FON:
            fon(X, obj);
        break;
        case KUR:
            kur(X, obj);
        break;
        case POL:
            pol(X, obj);
        break;
        case VNT:
            vnt(X, obj);
        break;
        case ZDT1:
            zdt1(X, obj);
        break;
        case ZDT2:
            zdt2(X, obj);
        break;
        case ZDT3:
            zdt3(X, obj);
        break;
        case ZDT4:
            zdt4(X, obj);
        break;
        case ZDT5:
            zdt5(X, obj);
        break;
        case ZDT6:
            zdt6(X, obj);
        break;
        case BNH:
            bnh(X, obj);
        break;
        case OSY:
            osy(X, obj);
        break;
        case SRN:
            srn(X, obj);
        break;
        case TNK:
            tnk(X, obj);
        break;
    }
}

/**
    Establece las variables de configuración para
    inicializar la configuración de las funciones de prueba
**/
void Benchmark::InitConfig()
{

    switch(this->TypeProblem)
    {
        case SCH1:
            this->NObjectives = 2;
            this->Dimension = 1;
            SetAllMinimize();
            SetAllBounds(Bounds,-1000, 1000);
            SetAllBounds(OptimalBounds,0,2);

        break;
        case SCH2:
            this->NObjectives = 2;
            this->Dimension = 1;
            SetAllMinimize();
            SetAllBounds(Bounds, -1000, 1000);
        break;
        case FON:
            this->NObjectives = 2;
            this->Dimension = DefaultDimension;
            SetAllMinimize();
            SetAllBounds(Bounds, -4, 4);
        break;
        case KUR:
            this->NObjectives = 2;
            this->Dimension = 3;
            SetAllMinimize();
            SetAllBounds(Bounds, -5, 5);
        break;
        case POL:
            this->NObjectives = 2;
            this->Dimension = 2;
            SetAllMinimize();
            SetAllBounds(Bounds, -M_PI, M_PI);
        break;
        case VNT:
            this->NObjectives = 3;
            this->Dimension = 2;
            SetAllMinimize();
            SetAllBounds(Bounds, -5, 5);
        break;
        case ZDT1:
            this->NObjectives = 2;
            this->Dimension = 30;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case ZDT2:
            this->NObjectives = 2;
            this->Dimension = 30;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case ZDT3:
            this->NObjectives = 2;
            this->Dimension = 30;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case ZDT4:
            this->NObjectives = 2;
            this->Dimension = 10;
            SetAllMinimize();
            SetAllBounds(Bounds, -5, 5);
            this->Bounds[0][0] = 0;
            this->Bounds[0][1] = 1;
        break;
        case ZDT5:
            this->NObjectives = 2;
            this->Dimension = 11;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case ZDT6:
            this->NObjectives = 2;
            this->Dimension = 10;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case BNH:
            this->NObjectives = 2;
            this->Dimension = 2;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case OSY:
            this->NObjectives = 2;
            this->Dimension = 6;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case SRN:
            this->NObjectives = 2;
            this->Dimension = 2;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
        case TNK:
            this->NObjectives = 2;
            this->Dimension = 2;
            SetAllMinimize();
            SetAllBounds(Bounds, 0, 1);
        break;
    }
}
/**
    # de variables binarias 1
    # de objetivos 2
**/
void Benchmark::sch1(vector<double> &X, vector<double> &obj)
{
    //Objetivo 1
    obj[0] = pow(X[0], 2.0);
    //Objetivo 2
    obj[1] = pow(X[0]-2.0, 2.0);
}
/**
    # de variables binarias 1
    # de objetivos 2
**/
void Benchmark::sch2(vector<double> &X, vector<double> &obj)
{
     if (X[0]<=1.0)
    {
        obj[0] = -X[0];
        obj[1] = pow((X[0]-5.0),2.0);
        return;
    }
    if (X[0]<=3.0)
    {
        obj[0] = X[0]-2.0;
        obj[1] = pow((X[0]-5.0),2.0);
        return;
    }
    if (X[0]<=4.0)
    {
        obj[0] = 4.0-X[0];
        obj[1] = pow((X[0]-5.0),2.0);
        return;
    }
    obj[0] = X[0]-4.0;
    obj[1] = pow((X[0]-5.0),2.0);
    return;
}
/**
    # de variables binarias n
    # de objetivos 2
**/
void  Benchmark::fon(vector<double> &X, vector<double> &obj)
{
    double s1, s2;
    int i;
    s1 = s2 = 0.0;
    int nreal = X.size();
    for (i=0; i<nreal; i++)
    {
        s1 += pow((X[i]-(1.0/sqrt((double)nreal))),2.0);
        s2 += pow((X[i]+(1.0/sqrt((double)nreal))),2.0);
    }
    obj[0] = 1.0 - exp(-s1);
    obj[1] = 1.0 - exp(-s2);
    return;
}
/**
    # de variables binarias 3
    # de objetivos 2
**/
void Benchmark::kur(vector<double> &X, vector<double> &obj)
{
    int i;
    double res1, res2;
    res1 = -0.2*sqrt((X[0]*X[0]) + (X[1]*X[1]));
    res2 = -0.2*sqrt((X[1]*X[1]) + (X[2]*X[2]));
    obj[0] = -10.0*( exp(res1) + exp(res2));
    obj[1] = 0.0;
    for (i=0; i<3; i++)
    {
        obj[1] += pow(fabs(X[i]),0.8) + 5.0*sin(pow(X[i],3.0));
    }
    return;
}
/**
    # de variables binarias 2
    # de objetivos 2
**/
void Benchmark::pol(vector<double> &X, vector<double> &obj)
{
    double a1, a2, b1, b2;
    a1 = 0.5*sin(1.0) - 2.0*cos(1.0) + sin(2.0) - 1.5*cos(2.0);
    a2 = 1.5*sin(1.0) - cos(1.0) + 2.0*sin(2.0) - 0.5*cos(2.0);
    b1 = 0.5*sin(X[0]) - 2.0*cos(X[0]) + sin(X[1]) - 1.5*cos(X[1]);
    b2 = 1.5*sin(X[0]) - cos(X[0]) + 2.0*sin(X[1]) - 0.5*cos(X[1]);
    obj[0] = 1.0 + pow((a1-b1),2.0) + pow((a2-b2),2.0);
    obj[1] = pow((X[0]+3.0),2.0) + pow((X[1]+1.0),2.0);
    return;
}
/**
    # de variables binarias 2
    # de objetivos 3
**/
void Benchmark::vnt(vector<double> &X, vector<double> &obj)
{
    obj[0] = 0.5*(X[0]*X[0] + X[1]*X[1]) + sin(X[0]*X[0] + X[1]*X[1]);
    obj[1] = (pow((3.0*X[0] - 2.0*X[1] + 4.0),2.0))/8.0 + (pow((X[0]-X[1]+1.0),2.0))/27.0 + 15.0;
    obj[2] = 1.0/(X[0]*X[0] + X[1]*X[1] + 1.0) - 1.1*exp(-(X[0]*X[0] + X[1]*X[1]));
    return;
}
/**
    # de variables binarias 30
    # de objetivos 2
**/
void Benchmark::zdt1(vector<double> &X, vector<double> &obj)
{
        double f1, f2, g, h;
        int i;
        f1 = X[0];
        g = 0.0;
        for (i=1; i<30; i++)
        {
            g += X[i];
        }
        g = 9.0*g/29.0;
        g += 1.0;
        h = 1.0 - sqrt(f1/g);
        f2 = g*h;
        obj[0] = f1;
        obj[1] = f2;
        return;
}
/**
    # de variables binarias 30
    # de objetivos 2
**/
void Benchmark::zdt2(vector<double> &X, vector<double> &obj)
{
      double f1, f2, g, h;
    int i;
    f1 = X[0];
    g = 0.0;
    for (i=1; i<30; i++)
    {
        g += X[i];
    }
    g = 9.0*g/29.0;
    g += 1.0;
    h = 1.0 - pow((f1/g),2.0);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
/**
    # de variables binarias 30
    # de objetivos 2
**/
void Benchmark::zdt3(vector<double> &X, vector<double> &obj)
{
      double f1, f2, g, h;
    int i;
    f1 = X[0];
    g = 0.0;
    for (i=1; i<30; i++)
    {
        g += X[i];
    }
    g = 9.0*g/29.0;
    g += 1.0;
    h = 1.0 - sqrt(f1/g) - (f1/g)*sin(10.0*M_PI*f1);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
/**
    # de variables binarias 10
    # de objetivos 2
**/
void Benchmark::zdt4(vector<double> &X, vector<double> &obj)
{
      double f1, f2, g, h;
    int i;
    f1 = X[0];
    g = 0.0;
    for (i=1; i<10; i++)
    {
        g += X[i]*X[i] - 10.0*cos(4.0*M_PI*X[i]);
    }
    g += 91.0;
    h = 1.0 - sqrt(f1/g);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;
}
/**
    # de variables binarias 11
    # de objetivos 2
**/
void Benchmark::zdt5(vector<double> &X, vector<double> &obj)
{
    /*  int i, j;
    int u[11];
    int v[11];
    double f1, f2, g, h;
    for (i=0; i<11; i++)
    {
        u[i] = 0;
    }
    for (j=0; j<30; j++)
    {
        if (gene[0][j] == 1)
        {
            u[0]++;
        }
    }
    for (i=1; i<11; i++)
    {
        for (j=0; j<4; j++)
        {
            if (gene[i][j] == 1)
            {
                u[i]++;
            }
        }
    }
    f1 = 1.0 + u[0];
    for (i=1; i<11; i++)
    {
        if (u[i] < 5)
        {
            v[i] = 2 + u[i];
        }
        else
        {
            v[i] = 1;
        }
    }
    g = 0;
    for (i=1; i<11; i++)
    {
        g += v[i];
    }
    h = 1.0/f1;
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;*/

}
/**
    # de variables binarias 10
    # de objetivos 2
**/
void Benchmark::zdt6(vector<double> &X, vector<double> &obj)
{
      double f1, f2, g, h;
    int i;
    f1 = 1.0 - ( exp(-4.0*X[0]) ) * pow( (sin(6.0*M_PI*X[0])),6.0 );
    g = 0.0;
    for (i=1; i<10; i++)
    {
        g += X[i];
    }
    g = g/9.0;
    g = pow(g,0.25);
    g = 1.0 + 9.0*g;
    h = 1.0 - pow((f1/g),2.0);
    f2 = g*h;
    obj[0] = f1;
    obj[1] = f2;
    return;

}

/**
    # de variables binarias 2
    # de objetivos 2
**/
void Benchmark::bnh(vector<double> &X, vector<double> &obj)
{
    obj[0] = 4.0*(X[0]*X[0] + X[1]*X[1]);
    obj[1] = pow((X[0]-5.0),2.0) + pow((X[1]-5.0),2.0);
//    constr[0] = 1.0 - (pow((xreal[0]-5.0),2.0) + xreal[1]*xreal[1])/25.0;
//    constr[1] = (pow((xreal[0]-8.0),2.0) + pow((xreal[1]+3.0),2.0))/7.7 - 1.0;
    return;
}

/**
    # de variables binarias 6
    # de objetivos 2
**/
void Benchmark::osy(vector<double> &X, vector<double> &obj)
{
    obj[0] = -(25.0*pow((X[0]-2.0),2.0) + pow((X[1]-2.0),2.0) + pow((X[2]-1.0),2.0) + pow((X[3]-4.0),2.0) + pow((X[4]-1.0),2.0));
    obj[1] = X[0]*X[0] +  X[1]*X[1] + X[2]*X[2] + X[3]*X[3] + X[4]*X[4] + X[5]*X[5];
   /* constr[0] = (xreal[0]+xreal[1])/2.0 - 1.0;
    constr[1] = 1.0 - (xreal[0]+xreal[1])/6.0;
    constr[2] = 1.0 - xreal[1]/2.0 + xreal[0]/2.0;
    constr[3] = 1.0 - xreal[0]/2.0 + 3.0*xreal[1]/2.0;
    constr[4] = 1.0 - (pow((xreal[2]-3.0),2.0))/4.0 - xreal[3]/4.0;
    constr[5] = (pow((xreal[4]-3.0),2.0))/4.0 + xreal[5]/4.0 - 1.0;*/
    return;
}

/**
    # de variables binarias 2
    # de objetivos 2
**/
void Benchmark::srn(vector<double> &X, vector<double> &obj)
{
    obj[0] = 2.0 + pow((X[0]-2.0),2.0) + pow((X[1]-1.0),2.0);
    obj[1] = 9.0*X[0] - pow((X[1]-1.0),2.0);
    /*constr[0] = 1.0 - (pow(xreal[0],2.0) + pow(xreal[1],2.0))/225.0;
    constr[1] = 3.0*xreal[1]/10.0 - xreal[0]/10.0 - 1.0;*/
    return;
}


/**
    # de variables binarias 2
    # de objetivos 2
**/
void Benchmark::tnk(vector<double> &X, vector<double> &obj)
{
    obj[0] = X[0];
    obj[1] = X[1];
   /* if (xreal[1] == 0.0)
    {
        constr[0] = -1.0;
    }
    else
    {
        constr[0] = xreal[0]*xreal[0] + xreal[1]*xreal[1] - 0.1*cos(16.0*atan(xreal[0]/xreal[1])) - 1.0;
    }
    constr[1] = 1.0 - 2.0*pow((xreal[0]-0.5),2.0) + 2.0*pow((xreal[1]-0.5),2.0);*/
    return;
}


