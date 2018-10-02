/*
only One evolution step
*/

void OneStep( int det,  int step ){
int i_rob ;
int neigh_R=0, neigh_F=0;
double k_n=0;

 for (int i=0; i<N; ++i)   {
           ++a[i][1];
           if (a[i][0] == 1){ //in FIRING
		     if (  san[i] == 0) // case atrial cell
			    {b[i][0]= -1; b[i][1] = 0 ; }   //imedietly to refractory state for atrial cell
			 if( san[i] == 1) { //case SAN cell
                   k_n= ((double) a[i][1]) /SAN_FIRE;
                   if (k_n >=1) {b[i][0] = -1; b[i][1] = 0;} //go to refractory
                       else { b[i][0] = 1; b[i][1] = a[i][1]; }
                 } 
            
           } 
           if (a[i][0] == -1){ // CELL in REFRACTORY
           	  if( san[i] ==0 ) {
           	        k_n= ((double)a[i][1]) / (a[i][2]); //  atrial cells with differnt APD
                    if (k_n >= 1) { b[i][0] = 0; b[i][1] = 0;}// intrinsic rule for atrial cell
                        else { b[i][0]= -1; b[i][1] = a[i][1]; } 
				  }  
			   if (san[i] == 1 ) {
					k_n= ((double)a[i][1])/(SAN_REFR);
                    if (k_n>=1) { b[i][0] = 0; b[i][1] = 0;}//intrinsic rule for SAN cell
                       else { b[i][0]= -1; b[i][1] = a[i][1]; }
				    }
	       
            } // if   -1
           if (a[i][0] == 0) { // in ACTIVITY
                if (san[i] == 0 )  {
                	  b[i][0] = 0; b[i][1] = a[i][1];
                      i_rob=links[i].mag(a);
                      double ksi = drand48();  // random refuse for firing of atrial cell
                      if (i_rob > N_F && ksi > refuse ) { b[i][0] = 1; b[i][1] = 0;  } 
                      }              
 			    if ( san[i] == 1) {
               			k_n= ((double)a[i][1]) / SAN_ACT;
               			if (k_n >= 1 ) { b[i][0] = 1; b[i][1] = 0; } // intrinsic rule for SAN cell
                		else {
                      		b[i][0] = 0; b[i][1] = a[i][1];
                      		i_rob=links[i].mag(a);
                      		if ( i_rob > SAN_N_F ) { b[i][0] = 1; b[i][1] = 0; }
                  		}	
             		}         
				
		   }   
   }                    



for (int i=0; i<N; ++i) {a[i][0]=b[i][0]; a[i][1]=b[i][1];}

return;
}




