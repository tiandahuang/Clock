#line 1 "\\\\tienda\\network_files\\Projects\\Clock\\inc\\steps-lookup-table.h"
/**
 * @file steps-lookup-table.h
 */

#pragma once

/**
 * 4096 steps/revolution (half-steps)
 * 11 panel positions
 */

uint16_t steps_lookup[] = {
    0,      // 0
    372,
    745,
    1117, 
    1489,
    1862,
    2234,
    2607,
    2979,
    3351,   // 9
    3724    // Blank
};