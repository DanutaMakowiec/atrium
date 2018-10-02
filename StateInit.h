void StateInit( double prob_out, double SAN_density){

double LocalRewirePref(int , double , double );

int OKRES = SAN_FIRE + SAN_REFR +SAN_ACT;

// homogeneous setting of initial state for atrial cells 
for(int i=0; i<N; ++i) { san[i]=  (drand48() < san_cells_density )? 1 : 0; }

// setting initial states ofor SAN cells 
for(int i=0; i<N; ++i)  {
         if (san[i] == 0 ) { a[i][0]=  1;    a[i][1]= 0 ;  a[i][2]= REFRACTORY + (int) ( drand48()* APD);}
		 else {
		 	int ile = ( drand48()* OKRES );
                if (ile < SAN_FIRE) { a[i][0]= 1; a[i][1]= ile;}
			    else 
			      if  (ile <  SAN_FIRE +SAN_REFR)  {a[i][0]= -1; a[i][1]= ile - SAN_FIRE;}
    	 	     else  {  a[i][0]= 0; a[i][1]= OKRES -ile ;}
                 
		    }
		 }

int SAN_corner, size_X = SAN_X, size_Y = SAN_Y ;

int  x, y;

fstream fst;
fst.open("network.txt",ios::out);
   if (!fst.good()){cout << "\n ERROR in out file \n"  ; return ;}
// setting the place for the donor SAN


x=5; y=5;
SAN_corner = y*L +x;
for(int i = SAN_corner ; i< SAN_corner  + size_X ; ++i)
    	 for (int j = 0; j< size_Y; ++j) {
			  int ile;
              ile= (int) (drand48()*OKRES );
              if (ile > SAN_FIRE) 	{a[i+j*L][0]= -1; a[i+j*L][1]= (int) (drand48() * SAN_REFR);}
    	 	  if (ile > SAN_REFR + SAN_FIRE) {a[i+j*L][0]= 0; a[i+j*L][1]= (int) (drand48() * SAN_ACT);}
    	      san[i+j*L]= 1;
			 }


fst<< "\n SAN of size X = "<< size_X <<" and Y = "<< size_Y<< " starts at  " << SAN_corner  << endl;
fst<< "\n SAN consists of cells \n ";

// deleting existing intercellar connections for SAN cells
for(int I = SAN_corner ; I< SAN_corner  + size_X ; ++I)
    	 for (int J = 0; J < size_Y; ++J)   {
    	 	    int i= I+J*L;
    	 		fst<< '\t' << i; 
    	 		links[i].kasuj();
    	 		int ile_ngb;
    	 		for (int k=0; k<N; ++k) {
    	 			ile_ngb=links[k].showNeigh(ngb); 
    	 			for ( int jj=0; jj< ile_ngb; ++jj)	if ( ngb[jj] == i ) links[k].del(i);
    	 		}			 
		 }	


		 
// establishing connections within SAN : see NetInit.h


for(int I = SAN_corner ; I< SAN_corner  + size_X  ; ++I)
    	 for (int J = 0; J < size_Y  ; ++J) {
			int i= I+J*L;
			if (drand48()< SAN_density )   links[i].enq((i+1));   // horizontal
  			if (drand48()< SAN_density)    links[i].enq((i+L+1));   //this
  			if (drand48()< SAN_density)    links[i].enq((i+L-1));   //this
  			if (drand48()< SAN_density)    links[i].enq((i+L)); // vertical down

		}	
		
	

for(int I = SAN_corner +1; I< SAN_corner  + size_X ; ++I)
    	 for (int J = 1; J< size_Y; ++J) { 
			int i= I+J*L;
			int ile_ngb;
			ile_ngb=links[i-1].showNeigh(ngb); 	for ( int j=0; j< ile_ngb;++j)	if ( ngb[j] == i ) links[i].enq(i-1);
			ile_ngb=links[i-L-1].showNeigh(ngb); for ( int j=0; j< ile_ngb;++j) if ( ngb[j] == i ) links[i].enq(i-L-1);
			ile_ngb=links[i-L+1].showNeigh(ngb);  for ( int j=0; j< ile_ngb;++j) if ( ngb[j] == i ) links[i].enq(i-L+1);
			ile_ngb=links[i-L].showNeigh(ngb); for ( int j=0; j< ile_ngb;++j) if ( ngb[j] == i ) links[i].enq(i-L);
}

  { int I = SAN_corner ;
    	 for (int J = 1; J< size_Y; ++J) { 
			int i= I+J*L;
			int ile_ngb;
			ile_ngb=links[i-L+1].showNeigh(ngb);  for ( int j=0; j< ile_ngb;++j) if ( ngb[j] == i ) links[i].enq(i-L+1);
			ile_ngb=links[i-L].showNeigh(ngb); for ( int j=0; j< ile_ngb;++j) if ( ngb[j] == i ) links[i].enq(i-L);
}}


int margines = size_X/5 ;
for(int j=0; j < J_SAN ; ++j)
  { 
    for(int I = SAN_corner   ; I< SAN_corner  + size_X  ; ++I)
    	 for (int J = 0; J< size_Y ; ++J) { 
			int i= I+J*L;  	degs[i]= links[i].degree(); }
  
    for(int I = SAN_corner +1; I< SAN_corner  + size_X -2 ; ++I)
    	 for (int J = 1; J< size_Y-2; ++J) { 
			int i= I+J*L; 
            num_ch=(int) LocalRewirePref(i,prob_ch,1);
   }  
  }



// delete all connections between SAN and atria


for (int i=0;i<N;++i)
  if (san[i] == 0 )    {
     for(int I = SAN_corner ; I< SAN_corner  +size_X ; ++I)
  	     for (int J = 0; J< size_Y; ++J){
		     int i_san=I+J*L;
             links[i].del(i_san); links[i_san].del(i);
             
		} 
}

for(int I = SAN_corner ; I< SAN_corner  +size_X ; ++I)
	     for (int J = 0; J< size_Y; ++J){
	     	for (int i=0;i<N;++i)
                 if (san[i] == 0 )    {
 			     int i_san=I+J*L;
    	         links[i].del(i_san); links[i_san].del(i);
    	     
		} 
}



//at random SAN cell is linked to left or right atrial cell
fst <<"\n\n p_{SAN_EXITS} " << prob_out <<'\t'<< prob_exits_from_SAN <<endl;
int counter_SAN_exits = 0;
for (int i=0;i<N;++i)  degs[i]= links[i].degree();


for (int j = 0; j< size_Y; ++j){
      double ksi= drand48();
   	if (ksi < prob_exits_from_SAN ) {   int i = SAN_corner  + size_X - 1;
   	    if (degs[i+L*j]>0 ){   
			links[i+L*j].enq(SAN_corner  + size_X + L*j );
			links[SAN_corner  + size_X + L*j ].enq(i+L*j);
			fst <<" \n"<< ++counter_SAN_exits << " exit from " <<i+L*j<< " to " << SAN_corner + size_X +L*j ;
		}
		}
	ksi = drand48();
	if (ksi < prob_exits_from_SAN && degs[j]>0){ int i = SAN_corner  ;
	   if (degs[i+L*j]>0 ){   
	 		links[i+L*j].enq(SAN_corner  + L*j -1 );
	   		links[SAN_corner  -1 +  L*j ].enq(i+L*j);
	   		fst <<" \n" << ++counter_SAN_exits << " exit from " <<i+L*j<< " to " << SAN_corner + L*j -1 ;
	   	}
	    }
}

for (int i=0;i<N;++i)  if (degs[i]==0) san[i] = 0; // if the cell is not connected then is atrial





//SAN configuration  to the file 
SAN_exits= counter_SAN_exits;
 max_SAN_degree =0;
fst<< "\n\n final SAN connections: " ;
for(int i = SAN_corner ; i < SAN_corner  +size_X ; ++i)
	for (int j = 0; j< size_Y; ++j)    {
      int ile=links[i+j*L].showNeigh(ngb);
      if (degs[i+j*L] > max_SAN_degree) max_SAN_degree = degs[i+j*L];
      fst<< "\n"<< ile << " "<<i+j*L <<": ";
         for( int k=0;k<ile;++k) fst<<ngb[k]<<" ";
      }

fst << "\n max SAN degree is " << max_SAN_degree<< endl;

fst.close();

return ;
}






