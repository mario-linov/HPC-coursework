#ifndef CLASS_BURGERS
#define CLASS_BURGERS

#include "Model.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

/**
 * @class Burgers
 * @author Mario Lino Valencia
 * @date 14/02/19
 * @file Burgers.h
 * @brief Class to solve brugers equation
 */
class Burgers {
	public:
		Burgers(Model m);
        ~Burgers();
        void    SetIC();
        void    SaveSol();
        double  GetE(); 
        void    Solve(int& rank,int& size); 

    private:
        // Variables from the model:
        // Numerics
	    double x0;
	    double y0;
	    double Lx;
	    double Ly;
	    double T;
	    int    Nx;
	    int    Ny;
	    int    Nt;
	    double dx;
	    double dy;
	    double dt;
	    // Physics
	    double ax;
	    double ay;
	    double b;
	    double c;
        
        // Coordinates
        double* x;
        double* y;
        // Velocity field
        double* u;
        double* v;
        
};

#endif