// ** PLEASE ONLY CHANGE THIS FILE WHERE INDICATED **
// In particular, do not change the names of the input parameters.

int             N = ...;
int m[1..N][1..N] = ...;


// Define here your decision variables and
// any other auxiliary program variables you need.
// You can run an execute block if needed.

//>>>>>>>>>>>>>>>>
range M = 1..N;
dvar boolean x_ij[i in M, j in M];
dvar int r[i in M];
dvar int+ z;    
//We set a time limit of 30 min for the program
execute{
  cplex.epgap = 0.01;		// used only for the input of size >= 40
  cplex.tilim = 900;
}
//<<<<<<<<<<<<<<<<


// Write here the objective function.

//>>>>>>>>>>>>>>>>
maximize z;
//<<<<<<<<<<<<<<<<

subject to {

    // Write here the constraints.

    //>>>>>>>>>>>>>>>>
    //Constraint 1, definition of z
    z <= sum(i in M, j in M : i !=j) m[i][j]* x_ij[i][j]; 
  	//Constraint 2, only one out of two members can have the priority
  	forall (i in M, j in M : i<j)
    	x_ij[i][j] + x_ij[j][i] ==1;
  	//Constraint 3, the rank is lower than the number of member
  	forall(i in M)
    	r[i]<=N;
  	//Constraint 4, the member with priority has the lower rank in the topological order
  	forall (i in M, j in M : i!= j)
    	r[i]-r[j] +1 <= (1-x_ij[i][j]) * N;
    //<<<<<<<<<<<<<<<<
}

// You can run an execute block if needed.

//>>>>>>>>>>>>>>>>

//<<<<<<<<<<<<<<<<
