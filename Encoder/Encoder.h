#ifndef _TI_EQEP_API_
#define _TI_EQEP_API_

#include <iostream>
#include <cstdint>
#include <string>

#define eQEP0 "/sys/devices/platform/ocp/48300000.epwmss/48300180.eqep"
#define eQEP1 "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep"
#define eQEP2 "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep"


// Class which defines an interface to my eQEP driver
class eQEP
{
    // Base path for the eQEP unit
    std::string path = "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep";
public:
    // Modes of operation for the eQEP hardware
    typedef enum
    {
        // Absolute positioning mode
        eQEP_Mode_Absolute = 0,

        // Relative positioning mode
        eQEP_Mode_Relative = 1,

        // Error flag
        eQEP_Mode_Error = 2,
    } eQEP_Mode;

    // Default constructor for the eQEP interface driver
    eQEP(eQEP_Mode _mode);

    // Reset the value of the encoder
    void set_position(int32_t position);

    // Get the position of the encoder, pass poll as true to poll the pin, whereas passing false reads the immediate value
    int32_t get_position(bool _poll = true);

    // Set the polling period
    void set_period(uint64_t period);

    // Get the polling period of the encoder
    uint64_t get_period();

    // Set the mode of the eQEP hardware
    void set_mode(eQEP_Mode mode);

    // Get the mode of the eQEP hardware
    eQEP_Mode get_mode();
};

#endif#pragma once
