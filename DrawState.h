void DrawState(HDC hdc)
{    char bufor[26];
     int y_suw=35, RR ;
     for(int x=0; x < L ;x++)
       for( int y=0; y<L;y++)
        {
           if(a[x+L*y][0]==1){
                   HPEN pioro=(HPEN) SelectObject(hdc,firing_pioro);
                   HBRUSH chory1= (HBRUSH) SelectObject(hdc,firing);
                   Rectangle(hdc,x*x_size,y_suw+y*y_size,(x+1)*x_size,y_suw+(y+1)*y_size);
                   SelectObject(hdc,chory1);
                   SelectObject(hdc,pioro);
                    }
           if(a[x+L*y][0]==-1){
                   HPEN pioro=(HPEN) SelectObject(hdc,refractory_pioro);
                   HBRUSH chory1= (HBRUSH) SelectObject(hdc,refractory);
                   Rectangle(hdc,x*x_size,y_suw+y*y_size,(x+1)*x_size,y_suw+(y+1)*y_size);
                   SelectObject(hdc,chory1);
                   SelectObject(hdc,pioro);
                    }
           if(a[x+L*y][0]==0){
                   HPEN pioro=(HPEN) SelectObject(hdc,activity_pioro);
                   HBRUSH chory1= (HBRUSH) SelectObject(hdc,activity);
                   Rectangle(hdc,x*x_size,y_suw+y*y_size,(x+1)*x_size,y_suw+(y+1)*y_size);
                   SelectObject(hdc,chory1);
                   SelectObject(hdc,pioro);
                    }
        }


      //first excitation
      char cus[5];
	  if (ile_RR < 1 && first_AP == 1 && step > 60) {
	  	int czy_F=0;
	  	for(int i=0;  i < 20; ++i){
	  	   if ( a[i + L*(L-1)][0] == 1) ++czy_F;
		  }
	  	if(czy_F > 1) {
	  		    first_AP = 0;
	  		  	fstate <<'\n'<<ile_restart<<'\t'<<step<<'\t' << ile_RR <<'\t'<< czy_F	<< '\t'<< ile_RR;
				fstate <<'\t'<<lateral <<'\t'<< prob_exits_from_SAN << '\t'<<refuse <<'\t'<<SAN_exits <<'\t'<<twist;
				fstate <<'\t'<<san_cells_density ;
            	strcpy(bufor,"  ");
		        itoa(step, cus,10);  strcat(bufor,cus); strcat(bufor,",   ");
		        TextOut(hdc, (31*ile_RR)%497  , y_size *(L+ 23),  bufor , strlen(bufor));
			}

	  }
/* layout of AVN cells
             **
            ***
            ***
 */
      
      int ile_F=0;
      if (a[AVNode -1 +L*(L-3)][0] == 1) ++ile_F;
      if (a[AVNode+L*(L-3)][0] == 1) ++ile_F;
      if (a[AVNode-2 +L*(L-2)][0] == 1) ++ile_F;
      if (a[AVNode-1 +L*(L-2)][0] == 1) ++ile_F;
      if (a[AVNode +L*(L-2)][0] == 1) ++ile_F;
      if (a[AVNode -2 +L*(L-1)][0] == 1) ++ile_F;
      if (a[AVNode -1 +L*(L-1)][0] == 1) ++ile_F;
      if (a[AVNode + L*(L-1)][0] == 1) ++ile_F;



      if (ile_F >= AVN_threshold ) {
         RR=step-AP_last;
	     AP_last=step; 
         HBRUSH avBrush=CreateSolidBrush(RGB(255,0,0));
         SelectObject(hdc,avBrush);
         if (RR>5) {
         	    ++ile_RR;
                fstate <<'\n'<<ile_restart<<'\t'<<step<<'\t' << ile_RR<<'\t'<<ile_F	<< '\t'<< RR;
				fstate<<'\t'<<lateral <<'\t'<< prob_exits_from_SAN << '\t'<<refuse <<'\t'<<SAN_exits <<'\t'<<twist;
				fstate <<'\t'<<san_cells_density ;
            }
		  if (RR>5) { char cus[5]; strcpy(bufor,"  "); cout << '\t'<< RR ;}
     } else {
      	HBRUSH avBrush= CreateSolidBrush(RGB(255,255,255));
      	SelectObject(hdc,avBrush);
	  }

      Ellipse(hdc,x_size *(AVNode-10), y_size *(L+8), x_size *(AVNode+20),y_size *(L+15));
           

}

