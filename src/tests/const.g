//genesis

///todo make this a parameter
randseed = 0

//float dt = 2.0e-5
float dt = 2.0e-4

float PI = 3.14159

float GNaP
float GKdr

float GNaF = 28e-9

//float somaDia = 20e-6

//float dend1Len = 50e-6
//float dend1Dia = 5e-6

float CMs = 21e-12   
float RMs = 1/{2.8e-9}

//float CMd = 20e-15
//float RMd = 2.8e9

float RA = 0

float ELEAK = -57.5e-3           // Ek value used for the leak conductance
float EREST_ACT = -57.5e-3 + 5e-3       // Vm value used for the RESET

float ENa = 0.05
float EK = -0.085

/* The model is quite sensitive to these values in NO_INTERP (caclmode=0) */
float tab_xmin = -0.10; float tab_xmax = 0.10;

int tab_xdivs = 149
int tab_xfills = 2999


