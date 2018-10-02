
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{ int x_step =50, y_step=30;
  int p1=6, p2=7, p3=8;
    switch (message)                 /* handle the messages */
    {
        case WM_CREATE:
        {
             read_parameters();
             NetInit( horizontal, lateral, vertical,  prob_ch);
             StateInit(prob_ch, SAN_density);
             uTimer =SetTimer(hwnd , 1, ODSWIEZ, NULL);

             CreateWindow("button", "Pause", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // pause in execution
                           0, 0, x_step, y_step,  hwnd, (HMENU) 1, NULL, NULL);
             CreateWindow("button", "Start", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // restart
                           x_step, 0, x_step, y_step, hwnd, (HMENU) 2, NULL, NULL);
              
		  	CreateWindow("button", "R+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,				//atrial cell refractory
                        p1*x_step,  0*y_step, x_step, y_step, hwnd, (HMENU) 20, NULL, NULL); 
            CreateWindow("button", "R-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,				
                         p2*x_step, 0*y_step, x_step, y_step, hwnd, (HMENU) 21, NULL, NULL);	
		    
			CreateWindow("button", " + ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,              // ksi for APD
                        p1*x_step,  1*y_step , x_step, y_step, hwnd, (HMENU) 26, NULL, NULL);
            CreateWindow("button", " - ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                         p2*x_step,  1*y_step, x_step, y_step, hwnd, (HMENU) 27, NULL, NULL);
                         
		   
		    CreateWindow("button", "n+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,              //density horizontal and lateral connections in atrium
                         p1*x_step, 3*y_step, x_step, y_step, hwnd, (HMENU) 7, NULL, NULL);
            CreateWindow("button", "n-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p2*x_step, 3*y_step, x_step, y_step, hwnd, (HMENU) 8, NULL, NULL);

            CreateWindow("button", "v+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,             // density of vertical connections in atrium
                        p1*x_step, 4*y_step, x_step, y_step, hwnd, (HMENU) 28, NULL, NULL);
            CreateWindow("button", "v-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p2*x_step, 4*y_step, x_step, y_step, hwnd, (HMENU) 29, NULL, NULL);                                                                                            
                           
                           
            CreateWindow("button", "N_F+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,            // settings for SAN dynamics
                          p1*x_step, 6*y_step, x_step, y_step, hwnd, (HMENU) 3, NULL, NULL);
            CreateWindow("button", "N_F-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p2*x_step, 6*y_step, x_step, y_step, hwnd, (HMENU) 4, NULL, NULL);
              CreateWindow("button", "r+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p1*x_step, 7*y_step, x_step, y_step, hwnd, (HMENU) 12, NULL, NULL);
            CreateWindow("button", "r-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                         p2*x_step, 7*y_step, x_step, y_step, hwnd, (HMENU) 13, NULL, NULL);
            CreateWindow("button", "a+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p1*x_step, 8*y_step, x_step, y_step, hwnd, (HMENU) 14, NULL, NULL);
            CreateWindow("button", "a-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p2*x_step, 8*y_step, x_step,y_step, hwnd, (HMENU) 15, NULL, NULL);
              
			CreateWindow("button", "d+", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,              // density of connection in SAN 
        		                 p1*x_step, 9*y_step, x_step,y_step, hwnd, (HMENU) 30, NULL, NULL);
			CreateWindow("button", "d-", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p2*x_step, 9*y_step, x_step,y_step, hwnd, (HMENU) 31, NULL, NULL);                         
                          
			CreateWindow("button", " + ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,             // setting exits from SAN
                          p1*x_step, 14*y_step , x_step, y_step, hwnd, (HMENU) 22, NULL, NULL);
            CreateWindow("button", " - ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          p2*x_step, 14*y_step, x_step, y_step, hwnd, (HMENU) 23, NULL, NULL);
            CreateWindow("button", " + ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,             // setting refuse to fire  
                         p1*x_step,  13*y_step , x_step, y_step, hwnd, (HMENU) 24, NULL, NULL);
            CreateWindow("button", " - ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                         p2*x_step,  13*y_step, x_step, y_step, hwnd, (HMENU) 25, NULL, NULL);
		    break;
        }
        case WM_COMMAND:
             if ( LOWORD (wParam) == 1){
                     if (PRESS == 0) { KillTimer(hwnd, 1); PRESS =1 ;}
                     else
                      { uTimer =SetTimer(hwnd , 1, ODSWIEZ, NULL); PRESS=0;}
                break;
                }
             if (LOWORD (wParam) == 2) { step = 0;   std::cout<< "\nrestart";   break; }
             if (LOWORD (wParam) == 3) { ++SAN_N_F;  break; }
             if (LOWORD (wParam) == 4) { --SAN_N_F; break; }
             if (LOWORD (wParam) == 7) { step = 0;
                        lateral += 0.10; horizontal += 0.10;  vertical = vertical;
                        std::cout<< "\nrestart: \tl = " << lateral  << "\th= " << horizontal <<"\tv= "<<vertical;
                        break; }
             if (LOWORD (wParam) == 8) { step=0;
                        lateral -= 0.05;   horizontal  -= 0.05;   vertical = vertical;
                        std::cout<< "\nrestart: \tl = " << lateral  << "\th= " << horizontal <<"\tv= "<<vertical;
                        break; }
          if ( LOWORD (wParam) == 11){
                     if (PRESS == 0) { deter=0; PRESS =1 ;}
                     else  { deter=1; PRESS=0;}
                break;
                }
            if (LOWORD (wParam) == 12) { ++SAN_REFR; break;}
            if (LOWORD (wParam) == 13) { --SAN_REFR; break;}
            if (LOWORD (wParam) == 14) { ++SAN_ACT; break; }
            if (LOWORD (wParam) == 15) { --SAN_ACT; break;}
            if (LOWORD (wParam) == 20) { ++REFRACTORY; break;}
            if (LOWORD (wParam) == 21) { --REFRACTORY; break;}
            if (LOWORD (wParam) == 22) { refuse += 0.01; std::cout<< "\nrefuse = " << refuse ; break; }
            if (LOWORD (wParam) == 23) { refuse -= 0.01; std::cout<< "\nrefuse = " << refuse ;break; }
            if (LOWORD (wParam) == 24) { prob_exits_from_SAN += 0.01; step=0;
                        std::cout<< "\nrestart: \t SAN exits  = " << prob_exits_from_SAN ;
                        break; }
            if (LOWORD (wParam) == 25) { prob_exits_from_SAN -= 0.01; step=0;
                        std::cout<< "\nrestart: \t SAN exits  = " << prob_exits_from_SAN ;
                        break; }
            if (LOWORD (wParam) == 26) { ++APD; step=0; std::cout<< "\nrestart: \tAPD = "<<APD ; break;}
            if (LOWORD (wParam) == 27) { --APD; step=0; std::cout<< "\nrestart: \tAPD = "<<APD ; break;}
            
			
			
			if (LOWORD (wParam) == 28) { step=0;
                        lateral = lateral; horizontal = horizontal;  vertical += 0.10;
                        std::cout<< "\nrestart: \tl = " << lateral  << "\th= " << horizontal <<"\tv= "<<vertical;
                        break; }
             if (LOWORD (wParam) == 29) { step=0;
                         lateral = lateral; horizontal = horizontal;  vertical -= 0.05;
                         std::cout<< "\nrestart: \tl = " << lateral  << "\th= " << horizontal <<"\tv= "<<vertical;
                         break; }
			
			
			
			if (LOWORD (wParam) == 30) { SAN_density  += 0.10 ; step=0; 
			              std::cout<< "\nrestart: \t SAN density  = " << SAN_density  ; break;}
			if (LOWORD (wParam) == 31) { SAN_density  -= 0.05 ; step=0; 
			              std::cout<< "\nrestart: \t SAN density  = " << SAN_density  ; break;}


         case WM_PAINT:
          {
             PAINTSTRUCT ps;
             RECT      rect;
             HDC hdc=BeginPaint(hwnd,&ps);
             GetClientRect(hwnd, &rect);
             char a[512]="";
             char bufor[512];
             
             strcat(a, "  Atrial cell: R =");  itoa(REFRACTORY, bufor,10);  strcat(a,bufor);
             TextOut(hdc, p3*x_step, 10, a, strlen(a));

			 strcpy( a,"   APD = R + ksi_"); itoa(APD, bufor, 10); strcat(a,bufor); strcat(a,"  ");
             TextOut(hdc, p3*x_step, 40, a, strlen(a));



             int rb= (int)(10*(2*vertical+2*horizontal+4* lateral));
             if (rb < 10) {
                    strcpy(a, "  nghb = 0."); itoa(rb%10, bufor,10);  strcat(a,bufor);  }
             if (rb >= 10) {
                     strcpy(a, "  nghb = "); itoa(rb/10, bufor,10);   strcat(a,bufor);
                     strcat(a, ".");  itoa(rb%10, bufor,10);  strcat(a,bufor);  }
                             
             strcat(a, ",  N_F > ");   rb= (int) N_F;   itoa(rb, bufor,10);  strcat(a,bufor);
             TextOut(hdc, p3*x_step, 70, a, strlen(a));

             

             strcpy(a,"  horizontal = "); rb=(int)(100*horizontal);  itoa(rb, bufor,10); strcat(a,bufor); strcat(a,"%");
             TextOut(hdc, p3*x_step, 90 ,a, strlen(a) );
             strcpy(a,"  lateral = "); rb=(int)(100*lateral);  itoa(rb, bufor,10); strcat(a,bufor); strcat(a,"%");
             TextOut(hdc, p3*x_step, 110 ,a, strlen(a) );
             strcpy(a,"  vertical = "); rb=(int)(100*vertical);  itoa(rb, bufor,10); strcat(a,bufor); strcat(a,"%");
             TextOut(hdc, p3*x_step, 130 ,a, strlen(a) );


			 TextOut(hdc, p3*x_step, 6*y_step ,"  Donor SAN: ", 12 );
			 strcpy(a,"  N_F > ");  itoa(SAN_N_F, bufor,10); strcat(a,bufor);
             strcat(a, ".");  itoa(rb%10, bufor,10);  strcat(a,bufor);
			 TextOut(hdc, p3*x_step, 7*y_step ,a, strlen(a));
			 
			 strcpy(a, " nghb = ");	 rb= (int) (10* SAN_density*8);  
			 if (rb >= 10) {
                     itoa(rb/10, bufor,10);   strcat(a,bufor);
                     strcat(a, ".");  itoa(rb%10, bufor,10);  strcat(a,bufor);  }
             TextOut(hdc,p3*x_step,  9*y_step ,a, strlen(a));
			 
             strcpy(a,"  r = ");  itoa(SAN_REFR, bufor,10); strcat(a,bufor);
             strcat(a,",   a = ");  itoa(SAN_ACT, bufor,10); strcat(a,bufor);
             strcat(a,",   f = ");  itoa(SAN_FIRE, bufor,10); strcat(a,bufor);
			 TextOut(hdc,p3*x_step, 8*y_step ,a, strlen(a));


             strcpy(a," exits from SAN ="); rb=(int)(100*prob_exits_from_SAN); itoa(rb, bufor,10); strcat(a,bufor); strcat(a,"%");
             TextOut(hdc, p3*x_step, 13*y_step+5, a, strlen(a));

             strcpy(a,"  refuse to fire = "); rb=(int)(100*refuse); itoa(rb, bufor,10); strcat(a,bufor); strcat(a,"%");
             TextOut(hdc, p3*x_step, 14* y_step+5, a, strlen(a));
             
             DrawState(hdc);
           ++step;
             EndPaint(hwnd,&ps);
             break;
          }

        case WM_DESTROY:
           {
            PostQuitMessage (0);     
            if (uTimer) KillTimer(hwnd,uTimer);
            return 0;
            }
        case WM_TIMER:
             {
              if(step < 1 ) {
                 NetInit(horizontal,lateral, vertical, prob_ch);
                 StateInit(prob_exits_from_SAN, SAN_density );
                 std::cout <<"\nRestart  "<< ile_restart<< "\t SAN exits = " << SAN_exits  << "\t max degree = " << max_SAN_degree<<endl;
                 InvalidateRect(hwnd,0,false);
                 UpdateWindow(hwnd);
                       }
                else if (step < TIME_LIMIT) {                 
                 OneStep(deter,step);
                 InvalidateRect(hwnd,0,false);
                   }
            return 0;
           }
         default:             
         return DefWindowProc (hwnd, message, wParam, lParam);
}
}



