void read_parameters(void){
 char cos[20], line[256];
 fparam.open("atria_parameters.txt");
   if (!fparam.good()){cout << "\n ERROR in out file \n"  ; return;}

    fparam.getline(line,256);
    fparam >> cos >> ACTIVE;     fparam.getline(line,256);
    fparam >> cos >> REFRACTORY;     fparam.getline(line,256);
    fparam >> cos >> FIRE;     fparam.getline(line,256);
    
    fparam >> cos >> deter;     fparam.getline(line,256);
 

    SAN_ACT= 60; SAN_REFR=27; SAN_FIRE=13;
    
    fparam.getline(line,256);
    fparam >> cos >> lateral;   fparam.getline(line,256);

    fparam >> cos >> twist; fparam.getline(line,256); ile_j= (int)(twist/prob_ch);
    fparam >> cos >> SAN_N_F;  fparam.getline(line,256);
    fparam >> cos >> refuse; fparam.getline(line,256);
    fparam >> cos >> AVN_threshold; fparam.getline(line,256);
    fparam >> cos >> APD; fparam.getline(line,256);

     vertical = 1;
	 horizontal = lateral;
	 SAN_density = 0.65;
	 N_F =0;
     fparam.close();
return;
}
