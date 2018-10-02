#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "Sinus Node";

int uTimer;
int x_size = 3, y_size = 5;
const HBRUSH firing = CreateSolidBrush(RGB(0,0,0));
const HBRUSH refractory= CreateSolidBrush(RGB(255,255,255)); 
const HBRUSH activity= CreateSolidBrush(RGB(000,255,0)); 
const HPEN firing_pioro= CreatePen(PS_SOLID,0,RGB(0,0,0));
const HPEN refractory_pioro= CreatePen(PS_SOLID,1,RGB(255,255,255));
const HPEN activity_pioro= CreatePen(PS_SOLID,1,RGB(000,255,0));


#define L 100
#define ODSWIEZ 1
#define TIME_LIMIT 10000    // the limit for the observed  steps of evolution


void read_parameters(void);

const int cell_vector =3;  // ( a state, time step  counter,  indi APD )
                          // -1 : refractory: r=0,... R
                          // 0  : depolarization: d:0,...D;
                          // 1 :  activity: 0;

const  int  N=L*L;     // Lattice L x L is coded as a line of N = L x L cells.
const  int MaxEdges = N;
int a[N][cell_vector], b[N][cell_vector],  degs[N],degs_reduced[N];

int changes[N];         //  here are put edges to move
int ngb[N];             // list of nearest neighbors
int san[N];             // cell type:  1 = SAN , 0  = ATRIUM,  2 - FAT - not implemented at present, 3....


int ACTIVE, REFRACTORY, FIRE, SAN_ACT, SAN_REFR, SAN_FIRE;  // cell dynamics thresholds , set by ReadParameters()
int APD  ;              // variability of refractory of an atrial cell, set in atria_parameters.txt
int AVN_threshold  ;    // how many cells in Fire results in  AVN excitation, set in atria_parameters.txt


// different  counters 
bool PRESS = 0; 
int diff, step= 0, AP_last=0, ile_RR= 0 , num_ch, deter;
int first_AP =1;

// different characteristics of interactions among cells
double init_prob=0.3,      //how many in FIRE
           N_F= 0,         // > 0,  means every single neighbor in FIRE, fires a cell.
           SAN_N_F =1,     //threshold for FIRE in donor SAN, > 1,  means that at least 2 neighbors in FIRE, fires a cell.
           N_R =1.0,       //  threshold for refactory
           lateral ,       // probability to create lateral edge
           horizontal, vertical,
           SAN_density;

  int     ile_j =0,       // how many Monte Carlo Steps
          evolve=0;       // evolve =0 network static ; evolve = 1 network changes in time
  double  prob_ch=0.01 ;   //probability to change links,
  double  twist;           // expectated value for twists of each edge ile_j =twist/prob_ch
    
  int     ile_restart = 1; // how many restarts after completing  the main simulation
  
  
  int     SAN_exits  ;      // numer of SAN cells connected to atria
  double  prob_exits_from_SAN = 0.125; // probability to link SAN with atria
  
  int     SAN_X = 11, SAN_Y = 31;  // size of SAN
  int     J_SAN = 100;      // paraameter for procedure of rewiring of connections between SAN cells
  
  int  max_SAN_degree ;      //  stopien maksymalny z SAN 
  
  
  int AVNode= L-2;          // here you can change the location of AVN 
                            // AVN structure: atrial cells connected  with the same  probability  as the atrial cells

double refuse = 0.0 ;      // probability for an atrial cell to refuse to fire
double san_cells_density = 0;

fstream fstate;
ifstream fparam;

void DrawState(HDC);

#include "Edges.h"
#include "NetInit.h"
#include "StateInit.h"
#include "LocalRewirePref.h"
#include "OneStep.h"
#include "CallBack.h"
#include "ReadParameters.h"
#include "DrawState.h"

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    //LPSTR lpszArgument,
                    LPSTR lpCmdLine,
                    int nCmdShow)
                    //int nFunsterStil)

{
	
	
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */


static time_t ms;
unsigned int jj;
jj=time(&ms);srand48(jj);

    HWND hwnd;              /* This is the handle for our window */
    memset(&wincl,0,sizeof(wincl));
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Right Atrium as Oscillator Timed Automata: randomly switched refuse",  /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           10, //CW_USEDEFAULT,       /* Windows decides the position */
           10, //CW_USEDEFAULT,       /* where the window ends up on the screen */
           600,                 /* The programs width */
           620,                 /* and height in pixels */
           NULL, // HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );


  read_parameters();
  char out_file[30]="d.";
  char cus[5];
  int i_lateral = 100*lateral;
  itoa(i_lateral,cus,10); strcat(out_file,cus); strcat(out_file, "_r.");
  int i_refuse = 100 *refuse; 
  itoa(i_refuse,cus,10); strcat(out_file,cus); strcat(out_file, "_APD(");
  itoa(APD,cus,10); strcat(out_file,cus);strcat(out_file, ")AVN(");
  itoa(AVN_threshold,cus,10); strcat(out_file,cus);strcat(out_file, ").txt");

  fstate.open(out_file, std::fstream::out | std::fstream::app );

  if ( !fstate.good()){ cout << "error" ; return -1;}
  fstate << "\n-----\t------ \t------\t------ \t------\t------  \t------ \t------ \t------ \t------\t----";
  fstate << "\nobieg \tstep \t num  \t cells   \t RR  \t lat  \t p_san \t refuse  \t SANs \t rewire\t extra";
  fstate << "\nobieg \tstep \t RR   \t in fire \t len \t lat \t exits \t refuse \t exits \t rewire\t SANs";
  fstate << "\n------\t----- \t------ \t------ \t------\t------ \t------ \t------ \t------ \t------\t---";

    /* Make the window visible on the screen */
    HWND hwnd_extra = GetConsoleWindow();    
    ShowWindow (hwnd_extra, SW_SHOWNORMAL);
	ShowWindow (hwnd, SW_SHOWNORMAL);

    UpdateWindow(hwnd);
   
	AllocConsole();
	AttachConsole( GetCurrentProcessId() );
	freopen("CON","w", stdout) ;
	std::cout << "Model settings:\n" ;
	
	std:cout << "SAN size " << SAN_X << " x " << SAN_Y ;
	std::cout << "\t SAN edge rewired with  p=  " << J_SAN*prob_ch << endl;
	//printf("AVNode sensitivity set to %d :\n ", AVN_threshold);
	std::cout << " \n SAN exits = " << SAN_exits <<std::endl;
 

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while ( GetMessage (&messages, NULL, 0, 0) >0)
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;

    fstate.close();
    
    
}





