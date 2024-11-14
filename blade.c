#include "main.h"

#define PI = 3.141592653589793

/* defining constants*/
double R = 1.965;                       /* Propeller radius in meters */ 
double V_inf = 139;                     /* Freestream velocity in m/s */
double omega;      /* Rotational speed in rad/s */
double rho = 0.652;                     /* Air density at cruise altitude in kg/m^3 */
double target_thrust = 2090;            /* Target thrust per blade in N */
int num_sections = 10;                  /* Number of sections along the blade */

omega = 1200*2*PI/60; 

/* Chord length parameters*/
double c_root = 0.25;                   /* Root chord length in meters */
double c_tip = 0.1;                     /* Tip chord length in meters */

// Function to calculate chord length at a given radial position
double calculate_chord_length(double r) {
    return c_root - ((c_root - c_tip) / R) * r;
}

int main() {
    double section_positions[num_sections];
    double local_thrusts[num_sections];
    double chord_lengths[num_sections];
    const char* airfoils_used[num_sections];

    // Step 1: Calculate section positions along the radius
    double total_weight = 0.0;
    for (int i = 0; i < num_sections; i++) {
        section_positions[i] = 0.1 * R + i * (R - 0.1 * R) / (num_sections - 1);
        total_weight += section_positions[i] * section_positions[i]; // Weighting factor sum
    }

    // Step 2: Calculate local thrusts with quadratic weighting and varying chord lengths
    double total_thrust = 0;
    for (int i = 0; i < num_sections; i++) {
        double r = section_positions[i];
        double weight_factor = (r * r) / total_weight;
        double local_thrust = target_thrust * weight_factor; // Weighted thrust for this section

        double V_relative = sqrt(V_inf * V_inf + (omega * r) * (omega * r));  // Relative velocity
        double phi = atan(V_inf / (omega * r));  // Inflow angle (radians)
        double theta = 20 * PI / 180;  // Initial guess for pitch angle (20 degrees in radians)
        double alpha = theta - phi;  // Angle of attack

        // Calculate chord length for this section
        double chord_length = calculate_chord_length(r);
        chord_lengths[i] = chord_length;

        // Calculate required lift coefficient CL for local thrust
        double CL = (2 * local_thrust) / (rho * V_relative * V_relative * chord_length);

        // Find suitable airfoil based on CL
        airfoils_used[i] = find_airfoil(CL);
        local_thrusts[i] = local_thrust;
        total_thrust += local_thrust;

        // Print results for this section
        printf("Section %d at r = %.2f m: V_rel = %.2f m/s, phi = %.2f°, alpha = %.2f°, CL = %.2f, Chord = %.3f m, Airfoil = %s\n",
               i + 1, r, V_relative, phi * 180 / PI, alpha * 180 / PI, CL, chord_length, airfoils_used[i]);
    }

    // Print total thrust
    printf("\nTotal estimated thrust: %.2f N (Target: %.2f N)\n", total_thrust, target_thrust);
    printf("Airfoils used for each section:\n");
    for (int i = 0; i < num_sections; i++) {
        printf("Section %d: %s\n", i + 1, airfoils_used[i]);
    }

    return 0;
}
