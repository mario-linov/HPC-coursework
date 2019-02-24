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
void Burgers::Solve(int& rank,int& size,const int Px,const int Py) {
    
    // Number of nodes in y direction for each process
    const int nx =  Nx/Px;
    const int ny =  Ny/Py;
    // Declare arrays for local (i.e. for each process) velocity field
    double* u_loc = new double[nx*ny];
    double* v_loc = new double[nx*ny];
    
    // Fluxes
    double fluxU;
    double fluxV;
    
    // Variables to increase enficiency
    const double invDx  = 1./dx;
    const double invDy  = 1./dy;
    const double invDx2 = 1./(dx*dx);
    const double invDy2 = 1./(dy*dy);
    double axbu;
    double aybv;

    int J, I, I0J0, IrJ0, IlJ0, I0Jr, I0Jl; // Global indices
    int i, j, i0j0;                         // Local indices
    int n;                                  // Time index
    
    for (j = 0; j < ny; ++j) {                          // Loop to iterate in y
        J = floor(rank/Px)*ny+j;        // Index for y direction in the whole domain
        for (i = 0; i < nx; ++i) {                    // Loop to iterate in x
            I = floor(rank/Py)*nx+i;    // Index for x direction in the whole domain
            if (I == 0 || I == Nx-1 || J == 0 || J == Ny-1) {
                i0j0 = nx*j+i;
                u_loc[i0j0] = 0.;
                v_loc[i0j0] = 0.;      
            }
        }
    }
    
    // Iterate over time to solve the equation
    for (n = 1; n <= Nt; ++n) {                             // Loop to iterate in time
        for (j = 0; j < ny; ++j) {                          // Loop to iterate in y
            J = floor(rank/Px)*ny+j; // Index for y direction in the whole domain
            if (J == 0 || J == Ny-1) continue;
            for (i = 0; i < nx; ++i) {                    // Loop to iterate in x
                I = floor(rank/Py)*nx+i; // Index for x direction in the whole domain
                if (I == 0 || I == Nx-1) continue;
                // Compute global indices
                I0J0 = J*Nx+I;
                IrJ0 = J*Nx+(I+1);
                IlJ0 = J*Nx+(I-1);
                I0Jr = Nx*(J+1)+I;
                I0Jl = Nx*(J-1)+I;
                // Local index for current node
                i0j0 = nx*j+i;
                
//                cout << rank << "   " << i << "  " << j << "  " << i0j0 << "  " << I << "  " << J << "  " << I0J0 << endl;
                
                // Compute convective velocities
                axbu = ax + b*u[I0J0];
                aybv = ay + b*v[I0J0];
                
                // Compute flux in firts equation
                fluxU =    c*(  (u[IrJ0] - 2.*u[I0J0] + u[IlJ0])*invDx2
                              + (u[I0Jr] - 2.*u[I0J0] + u[I0Jl])*invDy2 )
                        
                           - (  axbu*(u[I0J0] - u[IlJ0])*invDx
                              + aybv*(u[I0J0] - u[I0Jl])*invDy  );
                
                // Compute flux in second equation
                fluxV =   c*(  (v[IrJ0] - 2.*v[I0J0] + v[IlJ0])*invDx2
                             + (v[I0Jr] - 2.*v[I0J0] + v[I0Jl])*invDy2 )
                        
                          -  (  axbu*(v[I0J0] - v[IlJ0])*invDx
                              + aybv*(v[I0J0] - v[I0Jl])*invDy  );
                              
                // Update the velocity field
                u_loc[i0j0] = u[I0J0] + fluxU*dt;
                v_loc[i0j0] = v[I0J0] + fluxV*dt;
                
            }
        }
        //Gather all chucks of local velcoity field into the global velocity field
        MPI_Allgather(u_loc,nx*ny,MPI_DOUBLE,u,nx*ny,MPI_DOUBLE,MPI_COMM_WORLD);        
        MPI_Allgather(v_loc,nx*ny,MPI_DOUBLE,v,nx*ny,MPI_DOUBLE,MPI_COMM_WORLD);        
    }
    
}