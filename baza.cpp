#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);



/*  Make the class name into a global variable  */
char szClassName[ ] = "Sinus Node";

int uTimer;
int x_size=2, y_size=3;
const HBRUSH firing = CreateSolidBrush(RGB(250,0,0));
const HBRUSH refractory= CreateSolidBrush(RGB(255,255,205)); //Slate Blue
const HBRUSH activity= CreateSolidBrush(RGB(000,255,000)); //lime
const HPEN firing_pioro= CreatePen(PS_SOLID,0,RGB(250,0,0));
const HPEN refractory_pioro= CreatePen(PS_SOLID,1,RGB(255,255,205));
const HPEN activity_pioro= CreatePen(PS_SOLID,1,RGB(000,255,000));

void MalujStan(HDC);
void czytaj_param(void);

#define L 150
//#define Li 100
//#define Lj 150
#define ODSWIEZ 1
#define TIME_LIMIT 1600000

const int cell_vector =3;
const  int  N=L*L;
const  int MaxEdges=N;
int a[N][cell_vector], b[N][cell_vector], // first variable for a state, second for a counter
                          // -1 : refractory: r=0,... R
                          // 0  : depolarization: d:0,...D;
                          // 1 :  activity: 0;
        degs[N],degs_reduced[N];
int changes[N]; //  here are put edges to move
int ngb[N];// tu bedzie lista sasiadow biezca
int san[N]; // typ komorki: 1 komórka rozrusznikowa, 0 zwykla komorka miesnia sercoweg0


int ACTIVE, REFRACTORY, FIRE, SAN1_ACT, SAN1_REFR, SAN1_FIRE, SAN2_ACT, SAN2_REFR, SAN2_FIRE  ;
int APD  ; // variability of refractory of an atrial cell, set in param.txt
int AVN_threshold  ;  // >= to excite AVN, set in param.txt
bool PRESS=0;
int diff, step=1, AP_last=0, ile_RR= -1 , num_ch, deter;
int first_AP =1;

double init_prob=0.3,  //how many in FIRE
           N_F= 0,     // threshold for FIRE in atrium
           SAN1_N_F =1,    //threshold for FIRE in donor SAN
           SAN2_N_F =1,    //threshold for FIRE in native SAN
           N_R =1.0,          //  threshold for refactory
           lateral , // probability to create lateral edge
           horizontal, vertical,
           SAN_density;

 int       ile_j =0, // how many Monte Carlo Steps
           evolve=0;    // evolve =0 network static ; evolve = 1 network changes in time
  int ile_restart = 1; // ile_razy powtorzyc przed zmiana parametrow
double prob_ch=0.01,   //probability to change links,
       prob_wyjscia_z_SAN = 1.0, // probability to link SAN with atria
       twist;       // expectated value for twists of each edge ile_j =twist/prob_ch
double ksi_int=10;   //
double refuse =0.01 ; // probability for an atrial cell to fire
double san_cells_density = 0;
//double Fcount, Rcount, Acount;
fstream fstate;
ifstream fparam;

#include "Edges.h"
#include "NetInit.h"
#include "StateInit.h"
#include "NewLocalChangesPref.h"
#include "OneStep.h"
#include "LocalTwist.h"
#include "CallBack.h"
#include "CzytajParam.h"
#include "MalujStan.h"

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
           510,                 /* The programs width */
           612,                 /* and height in pixels */
           NULL, // HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );


  char out_file[30]="s0.45_APD(";
  char cus[5];
  itoa(APD,cus,10); strcat(out_file,cus);strcat(out_file, ")AVN(");
  itoa(AVN_threshold,cus,10); strcat(out_file,cus);strcat(out_file, ").txt");

  fstate.open(out_file, std::fstream::out | std::fstream::app );

  if ( !fstate.good()){ cout << "error" ;return -1;}
  fstate << "\n-----\t------ \t------\t------ \t------\t------  \t------ \t------ \t------ \t------";
  fstate << "\nobieg \tstep \t num  \t cells   \t RR   \t lateral  \t san \t refuse   \t extra \t rewire";
  fstate << "\nobieg \tstep \t RR   \t in fire \t lenght\t lateral  \t exits \t refuse \t SANs \t rewire";
  fstate << "\n------\t----- \t------ \t------ \t------\t------ \t------ \t------ \t------ \t------";

    /* Make the window visible on the screen */
    //ShowWindow (hwnd, nFunsterStil);
     ShowWindow (hwnd, nCmdShow);


    UpdateWindow(hwnd);


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while ( GetMessage (&messages, NULL, 0, 0) >0)
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;

    fstate.close();
}




