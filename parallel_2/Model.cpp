#include "Model.h"


/**
 * @brief This constructor takes a *.in with the parameters of the problem and parses them to the apropiate varibles. 
 *        Checks the validty of  these parameteers and then computes dx,dy and dt.
 * @param argc  Number of input arguments.
 * @param argv  *.in file with x0, y0, Lx, Ly, T, Nx, Ny, Nt, ax, ay, b and c.
                These parameters (all floats) should be provided in this order and with the following structure:
                
                x0  0
                y0  0
                Lx  10
                ...
                 
                Note: The delimiters are tabs.
 */
Model::Model(int argc,char* argv[]) {
    ParseParameters(argc,argv);
    ValidateParameters();
    dx = Lx/(Nx-1);
    dy = Ly/(Ny-1);
    dt = T/Nt; 
}

/**
 * @brief Destructor.
 */
Model::~Model() {
    
}

/**
 * @brief Parse the parameters in a *.in file to the class private variables.
 * @param argc  Number of input arguments.
 * @param argv  *.in file with x0, y0, Lx, Ly, T, Nx, Ny, Nt, ax, ay, b and c.
 */
void Model::ParseParameters(int argc, char* argv[]) {
    
    ifstream f_in(argv[1]);
    cout << "Reading parameters from " << argv[1] << "..." << endl;
    string input;
    
    /// Parse x0:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> x0;
    /// Parse y0:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> y0;
    /// Parse Lx:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> Lx;
    /// Parse Ly:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> Ly;
    /// Parse T:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> T;
    /// Parse Nx:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> Nx;
    /// Parse Ny:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> Ny;
    /// Parse Nt:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> Nt;
    /// Parse ax:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> ax;
    /// Parse ay:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> ay;
    /// Parse b:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> b;
    /// Parse c:
    getline(f_in,input,'\t');
    getline(f_in,input);
    stringstream(input) >> c;
    
    f_in.close();
}

/**
 * @brief Check that:
                        -x0 and y0 are inside the domain.
                        -Lx and Ly are positive.
                        -Nx, Ny and Nt are positive.
 */
void Model::ValidateParameters() {
    validity = x0<Lx/2 && y0<Ly/2 && Lx>0 && Ly>0 && T>0 && Nx>0 && Ny>0 && Nt>0;
}


/**
 * @brief Public member function to print to the terminal the input parameters.
 */
void Model::PrintParameters() {
    
    cout.precision(6);
    int width(10);
    
    cout << "_______________________________________________________" << endl;
    cout << endl;
    
    cout << "The parameteters of the domain are:" << endl;
    cout << setw(width) << "x0 = " << setw(width) << x0 << endl;
    cout << setw(width) << "y0 = " << setw(width) << y0 << endl;
    cout << setw(width) << "Lx = " << setw(width) << Lx << endl;
    cout << setw(width) << "Ly = " << setw(width) << Ly << endl;
    cout << setw(width) << "T  = " << setw(width) << T  << endl;
    cout << setw(width) << "Nx = " << setw(width) << Nx << endl;
    cout << setw(width) << "Ny = " << setw(width) << Ny << endl;
    cout << setw(width) << "Nt = " << setw(width) << Nt << endl;
    
    cout << endl;
    
    cout << "The steps are:" << endl;
    cout << setw(width) << "dx = " << setw(width) << dx << endl;
    cout << setw(width) << "dy = " << setw(width) << dy << endl;
    cout << setw(width) << "dt = " << setw(width) << dt << endl;
    
    cout << endl;
    
    cout << "The physic parameters are:" << endl;
    cout << setw(width) << "ax = " << setw(width) << ax << endl;
    cout << setw(width) << "ay = " << setw(width) << ay << endl;
    cout << setw(width) << "b  = " << setw(width) << b  << endl;
    cout << setw(width) << "c  = " << setw(width) << c  << endl;
    
    cout << "_______________________________________________________" << endl;
    cout << endl;
}