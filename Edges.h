
void srand48(time_t t){ srand ((unsigned) time(&t)); return;}
double drand48(){ return (double)rand()/(double)RAND_MAX; }
int lrand48(){ return rand();}
int Nrand(){ return lrand48()% N;}

class EDGES{
	private:
   	  struct node
	      {int which; node *next;
          node(int x, node *t) { which = x; next = t;}
          };
      typedef node *link;
      link head, tail;
	public :
     EDGES();
     void kasuj();
     int empty();
     void enq(int );
     void change(int ,int );
      int sw(double);
     void del(int );
     int mag(int (*)[cell_vector]);
     int mag_hetero(int (*arr)[cell_vector], int);
     int showNeigh(int * );
     void showStates(int (*)[cell_vector]);
     int degree();
     int sw_inten(double);
  //   int sw_inten_line(double,int);
};
// classs methods definitions

// list constructor
EDGES::EDGES()	{head=tail=NULL; }

// list destructor
void EDGES::kasuj(){link  t=head , dalej;
     while( t ){dalej=t;  t=t->next; delete dalej; }
     head=tail=NULL; }

// if  list is empty 
int EDGES::empty(){return head == NULL;}

// count degree of this vertex 
int EDGES::degree() { int i=0; link t;
	for(t=head; t !=NULL; t=t->next) ++i; return i;}

// add to this vertex list the vertex with given number
void EDGES::enq(int item){ link t= new node(item, NULL);
      if(head){tail->next=t; tail=t; } else head=tail=t; }

//delete from this vertex number before, add after
void EDGES::change(int before,int after){  link  t;
      for ( t=head; t!=NULL; t=t->next)
          { if (t->which == before) {t->which =after; return;}}
	      cout << "\n BLAD "<< before << " to " << after;
          return;}

// in changes[] we bulit a list of neighbors selected with probability p to be rewired
//sw() from small world rewiring
int EDGES::sw(double p){int i=0; link  t=head ; double rob;
      while(t){ rob=drand48();
      	if ( rob <p )  changes[i++] = t->which;
      	        t=t->next; }
      return i;}


// CHANGE is MADE in INTENTIONAL UNILINKING

void EDGES::del(int item){link t, parent=NULL;
      for (t=head; t !=NULL; t=t->next){
        if ( t->which == item) {
           if( t== head)   head=t->next;
               	else if(t== tail){tail=parent;(parent->next)=NULL;}
               		else parent ->next = t->next;
           delete t;  return; }
        parent=t;}
       }

int EDGES::mag(int (*arr)[cell_vector]){ int ile=0;   link  t;
         for (t=head; t!=NULL; t=t->next) if (arr[t->which][0]==1 ) ++ile;
         return ile;}

int EDGES::mag_hetero(int (*arr)[cell_vector],int ja){ int ile=0;   link  t;
         for (t=head; t!=NULL; t=t->next)
              if (arr[t->which][0]==1) {
                     ++ile;
                     if ( ja - t->which ==  1 || t->which -ja ==1 )
                             ile +=1;       // double if horizontal;

            }
         return ile;}



// counts this degree
int EDGES::showNeigh(int *nb ){link  t; int i=0;
          for ( t=head; t!=NULL; t=t->next)   {  nb[i++]= (t->which) ;}
           return i;          }
void EDGES::showStates(int (*arr)[cell_vector]){  link  t;
          for ( t=head; t!=NULL; t=t->next)
                  cout <<" (" << arr[t->which][0]<<"," <<arr[t->which][1]<<")";
          }


static EDGES links[N];


