#include "Burgers.h"

/**
 * @brief Load the problem parameter from a Model instance and initilise x, y, u and v
 * @param m MOdel instnce defining the burgers problem to be solved
 */
Burgers::Burgers(Model m) {
    
    // Lad Numerics
    x0 = m.GetX0();
    y0 = m.GetY0();
    Lx = m.GetLx();
    Ly = m.GetLy();
    T  = m.GetT ();
    Nx = m.GetNx();
    Ny = m.GetNy();
    Nt = m.GetNt();
    dx = m.GetDx();
    dy = m.GetDy();
    dt = m.GetDt();
    // Load Physics
    ax = m.GetAx();
    ay = m.GetAy();
    b  = m.GetB ();
    
    // Initilise coordinates
    x = new double[Nx];
    for (int i = 0; i < Nx; ++i) x[i] = -Lx/2 + i*dx;
    y = new double[Ny];
    for (int j = 0; j < Ny; ++j) y[j] = -Ly/2 + j*dy;
    // Initilise velocity field
    u = new double[Nx*Ny];
    v = new double[Nx*Ny];
    
}
        
/**
 * @brief Destructor
 */
Burgers::~Burgers() {

}

/**
 * @brief Set the initial condition
 */
void Burgers::SetIC() {
    
    double y2, r;
    
    int i, j, i0j0;
    
    for (j = 0; j < Ny; ++j) {
        y2 = pow( y[j]-y0 , 2);
        for (i = 0; i < Nx; ++i) {
            
            i0j0 = Nx*j+i; // Index
            
            r = pow( y2 + pow( x[i]-x0 , 2),0.5); // Radial coordinate
            
            if (r <= 1.)    u[i0j0] = v[i0j0] = 2.*pow(1.-r,4)*(4.*r+1.);
            else            u[i0j0] = v[i0j0] = 0.;
        }
    }
    
}

/**
 * @brief Save u and v to a *.sol file that can be read by gnuplot
 */
void Burgers::SaveSol() {
    
    ofstream f_out("velocity.sol");
    int width(20);
    
    int i, j, i0j0;
    
    for (j = 0; j < Ny; ++j) {
        for (i = 0; i < Nx; ++i) {
            i0j0 = Nx*j+i; // Index
            f_out << setw(width) << x[i] << setw(width) << y[j] << setw(width) << u[i0j0] << setw(width) << v[i0j0] << endl;
        }
        f_out << endl;
    }
    
    f_out.close();
    
}

/**
 * @brief Compute energy
 */
double Burgers::GetE() {
    
    double E = 0.;
    int i, j, i0j0;
    
    for (j = 0; j < Ny; ++j) {
        for (i = 0; i < Nx; ++i) {
            i0j0 = Nx*j+i; // Index
            E += pow(pow(u[i0j0],2.)+pow(v[i0j0],2.), 0.5);
        }
    }
    
    return E*dx*dy/2.;
    
}
 
/**
 * @brief Solve burgers equation to obtain the velocity field. Parallelisation
          is applied in y direction. The interior nodes (excluding the boundaries)
          are evenly distributted between processes. 
 * @param rank Current process
 * @param size Number of processes
 */ 
void Burgers::Solve(int& rank,int& size) {
    
    // Number of nodes in y direction for each process
    const int ny =  (Ny-2)/size;
    // Declare arrays for local (i.e. for each process) velocity field
    double* u_loc = new double[Nx*ny];
    double* v_loc = new double[Nx*ny];
    // Set the boundary conditions on the local velocity field
    for (int j = 0; j < ny; ++j) {
        u_loc[Nx*j] = u_loc[Nx*(j+1)-1] = v_loc[Nx*j] = v_loc[Nx*(j+1)-1] = 0.;
    }
    
    // Fluxes
    double fluxU;
    double fluxV;
    
    // Variables to increase enficiency
    const double invDx = 1/dx;
    const double invDy = 1/dy;
    const double invDx2 = 1/(dx*dx);
    const double invDy2 = 1/(dy*dy);
    double axbu;
    double aybv;
    int j0;
    // Global indices
    int J, i0j0, irj0, ilj0, i0jr, i0jl;
    // Local indices
    int i, j, n, Nxj, Nxji;
    
    // Iterate over time to solve the equation
    for (n = 1; n <= Nt; ++n) {                             // Loop to iterate in time
        for (j = 0; j < ny; ++j) {                          // Loop to iterate in y
            J = rank*ny+1+j;    // Index for y direction in the whole domain
            j0 = Nx*J;          // Index to reduce number of opeartions
            Nxj = Nx*j;         // Index to reduce number of opeartions
            for (i = 1; i < Nx-1; ++i) {                    // Loop to iterate in x
                // Compute global indices
                i0j0 = j0+i;
                irj0 = j0+(i+1);
                ilj0 = j0+(i-1);
                i0jr = Nx*(J+1)+i;
                i0jl = Nx*(J-1)+i;
                // Local index for current node
                Nxji = Nxj+i;
                
                // Compute convective velocities
                axbu = ax + b*u[i0j0];
                aybv = ay + b*v[i0j0];
                
                // Compute flux in firts equation
                fluxU =    c*(  (u[irj0] - 2.*u[i0j0] + u[ilj0])*invDx2
                              + (u[i0jr] - 2.*u[i0j0] + u[i0jl])*invDy2 )
                        
                           - (  axbu*(u[i0j0] - u[ilj0])*invDx
                              + aybv*(u[i0j0] - u[i0jl])*invDy  );
                
                // Compute flux in second equation
                fluxV =   c*(  (v[irj0] - 2.*v[i0j0] + v[ilj0])*invDx2
                             + (v[i0jr] - 2.*v[i0j0] + v[i0jl])*invDy2 )
                        
                          -  (  axbu*(v[i0j0] - v[ilj0])*invDx
                              + aybv*(v[i0j0] - v[i0jl])*invDy  );
                              
                // Update the velocity field
                u_loc[Nxji] = u[i0j0] + fluxU*dt;
                v_loc[Nxji] = v[i0j0] + fluxV*dt;
                
            }
        }
        //Gather all chucks of local velcoity field into the global velocity field
        MPI_Allgather(u_loc,Nx*ny,MPI_DOUBLE,u+Nx,Nx*ny,MPI_DOUBLE,MPI_COMM_WORLD);        
        MPI_Allgather(v_loc,Nx*ny,MPI_DOUBLE,v+Nx,Nx*ny,MPI_DOUBLE,MPI_COMM_WORLD);        
    }
    
}