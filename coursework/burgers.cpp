#include <chrono>
#include "Model.h"
#include "Burgers.h"

/**
 * @brief Main function
 * @param argc  Number of input arguments to the program
 * @param argv  Array with the *.in file (input parameters) as element #1 
                These parameters (all floats) should be provided in this order:
                    x0, y0, Lx, Ly, T, Nx, Ny, Nt, ax, ay, b and c
                and with the following structure:
                
                    x0  0
                    y0  0
                    Lx  10
                    ...
                 
                Note: The delimiters are tabs.
 * @return 0 if succesfully executed
 */
int main(int argc, char* argv[]) {

    // Create the model with the problem parameters provided in a *.in file
    // and check valdity of such parameters
    Model m(argc, argv);
    m.PrintParameters();
    if (!m.IsValid()) {
        cout << "ERROR: Non-valid parameters provided in " << argv[1] << "." << endl;
        return 1;
    }

    // Initialise the problem
    Burgers b(m);
    b.SetIC();
    cout << "The initial Energy is: " << b.GetE() << endl;

    // Solve the problem
    typedef std::chrono::high_resolution_clock hrc;
    hrc::time_point start = hrc::now();

    b.Solve();

    hrc::time_point end = hrc::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Calculate final energy and computing time
    cout << "The final Energy is: " << b.GetE() << endl;
    cout << endl;
    cout << "Computing time: " << duration.count() << " ms" << endl;
    // Write output to a *.sol file
    b.SaveSol();

    return 0;
}