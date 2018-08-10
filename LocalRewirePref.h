/*
T=1 means - no preferences on degree for selection of the new link

*/
double LocalRewirePref(int from, double prob_T, double T){
int where ,no_changes=0, temp, ile_ngb=0, ile=0;
double rob1, rob2;

   no_changes=links[from].sw(prob_T); // list of  the neighbors to be unlinked (without preferences) 
                                      // the global vector changes[] is set.
       if (no_changes >0)
        { for (int i=0; i < no_changes; ++i)
          {
             ile_ngb=links[changes[i]].showNeigh(ngb);  
             ile=0;
             for( int j=0;j<ile_ngb;++j) if (ngb[j]== from) ++ile;
             if ( ile != ile_ngb ) // selection among neighbors of the unwanted neighbor 
                                   // if the unwated neighbor is a leaf, we cannot unlink it
               { if(ile_ngb ==2)   //case  unwanted neighbor has two neighbors
                 {
                   where=(ngb[0] == from)? ngb[1]: ngb[0];
                   links[from].change(changes[i],where);
                   links[changes[i]].del(from);
                   links[where].enq(from);
                 }
                if(ile_ngb>2)     // case the unwanted neighbor has more  than 2 neighbors
                  {
                  do {            //preferential linking follows vertex degree
                       where=lrand48()%ile_ngb;
                       temp=ngb[where];
                       rob1=drand48();
                       int cos= degs[temp];
                       rob2=((double)cos)/T;  
                       if ( rob1 > rob2) temp = from;
                       }while (temp==from);
                    links[from].change(changes[i],ngb[where]);
                    links[changes[i]].del(from);
                    links[ngb[where]].enq(from);
                 }
           } // ile!= ile_ngb
         } //next change
 }
 return (double)no_changes ;
 }

