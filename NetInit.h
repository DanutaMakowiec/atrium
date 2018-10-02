/*
network construction:  Lattice L x L is coded as a line of N = L x L cells.
*/

void NetInit( double horizontal, double lateral, double vertical, double rewire ){
double LocalRewirePref(int , double , double );
int ile_ngb;

for(int i=0; i<N;++i){ links[i].kasuj(); }

/* networek arrangement: horizontal, vertical  and lateral connections
       --*--*--*-
        /|\/|\/|\
       --*--*--*--
       /|\/|\/|\
       --*--*--*--
           
           
*/
// the up-left cell
if (drand48()<horizontal)  links[0].enq(1);    // horizontal lines
if (drand48()<lateral )    links[0].enq(L+1);  //lateral to the right
if (drand48()<vertical)    links[0].enq(L);    // vertical down


// remaining cells of the first row 
for(int i=1; i<L; ++i){
  ile_ngb=links[i-1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb; ++j)
             if ( ngb[j] == i ) links[i].enq(i-1); //back nghb

  if (drand48()<horizontal)    links[i].enq((i+1)%N);    // horizontal
  if (drand48()<lateral)       links[i].enq((i+L+1)%N);  //lateral to the right
  if (drand48()<lateral)       links[i].enq((i+L-1)%N);  //lateral to the left
  if (drand48()<vertical)      links[i].enq((i+L)%N);    // vertical down
}
// BODY
for(int i=L; i<N-L ; ++i){
	// start with completing connections established in the previous step
  ile_ngb=links[i-1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-1);

 ile_ngb=links[i-L-1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-L-1);

 ile_ngb=links[i-L+1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-L+1);

 ile_ngb=links[i-L].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-L);

  if (drand48()<horizontal)    links[i].enq((i+1)%N);   // horizontal
  if (drand48()<lateral)       links[i].enq((i+L+1)%N);   //lateral to the right
  if (drand48()<lateral)       links[i].enq((i+L-1)%N);   //lateral to the left
  if (drand48()<vertical)       links[i].enq((i+L)%N); // vertical down
}

// cells of the last row
for(int i=N-L; i<N ; ++i){
  ile_ngb=links[i-1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-1);

 ile_ngb=links[i-L-1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-L-1);

 ile_ngb=links[i-L+1].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-L+1);

 ile_ngb=links[i-L].showNeigh(ngb);
  for ( int j=0; j< ile_ngb;++j)
           if ( ngb[j] == i ) links[i].enq(i-L);

  if (drand48()<horizontal)    links[i].enq((i+1)%N);   // horizontal

}


// delete cross border connections
for(int  i=0; i <N; ++i) {
   if((i%L)==0){
           links[i].del((i+N-L-1)%N);
           links[i].del((i+N-1)%N);
           links[i].del((i+N+L-1)%N);
           }
   if((i%L)== (L-1)) {
              links[i].del((i+L+1)%N);
              links[i].del((i+N-L+1)%N);
              links[i].del((i+1)%N);
              }
   if (i<L) {
            links[i].del((i+N-L)%N);
            links[i].del((i+N-L-1)%N);
            links[i].del((i+N-L+1)%N);
            }
   if (i>N-L-1) {
            links[i].del((i+L)%N);
            links[i].del((i+L+1)%N);   //this
            links[i].del((i+L-1)%N);   //this
              }
      }


//local preferential rewiring
for(int j=0; j<ile_j; ++j)
  { 
    for (int i=0;i<N;++i)  degs[i]= links[i].degree(); 
    for(int i=0;i<N;++i)   num_ch=(int)LocalRewirePref(Nrand(),rewire,1);
   }  
// reset the list of changes
for(int i=0; i<MaxEdges; ++i) changes[i]= 0;
return;
}



