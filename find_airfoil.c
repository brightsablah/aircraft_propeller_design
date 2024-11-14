#include "main.h"

/* Function to find an airfoil based on CL */
const char* find_airfoil(double CL) {
    if (CL >= 0.1 && CL <= 1.2) return "NACA0012";
    else if (CL > 1.2 && CL <= 1.4) return "NACA2412";
    else if (CL > 1.4 && CL <= 1.6) return "ClarkY";
    else if (CL > 1.6 && CL <= 1.8) return "Selig1223";
    return "No match";
}