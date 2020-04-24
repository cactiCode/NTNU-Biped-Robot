#include "eqep.h"

int main(int argc, char** argv)
{
    // Allocate an instane of 
    eQEP eqep2(eQEP::eQEP_Mode_Absolute);

    // Set the unit time period to 100,000,000 ns, or 0.1 seconds
    //eqep2.set_period(1000000000L); 10000000
    eqep2.set_period(10000000L);

    // Query back the period
    std::cout  << eqep2.get_period() << " ns" << std::endl;

    // Read position indefintely
    while (1)
    {
        std::cout << "Position = " << (eqep2.get_position()*0.12) << std::endl;
    }

    // Return success
    return 0;
}