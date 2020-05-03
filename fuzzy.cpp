#include "fuzzy.h"

const int MF1 = 5, MF2 = 5; // number of membership functions for both inputs


void fuzzy_rule_base(float FRB[MF1][MF2])
{
	float VSH, SHT, RSH, MED, RLN, LNG, VLN;  

	//    ---- Fixed Values of the 7 Singletons :
    // ---- 3 Negative Values ----- 
      VSH = 10;     SHT = 25;   RSH = 40; // VERY SHORT; SHORT; RELATIVELY SHORT
    // The Central ZERO Value  
      MED  =  50; // MEDIUM
    // ---- 3 Positive Values -----  
      RLN =  60;  LNG =  75;   VLN =  90; // RELATIVELY LONG; LONG; VERY SHORT

//    ------------------------
    //  Fixed Number of All Singleton Levels: NumSL:
    int NumSL = 7; // number of singletons
/*    printf("\n  --- All %d Singleton Levels \n", NumSL);

// Print these levels on the Screen:
    printf(" VSH = %.0f;   SHT = %.0f;   RSH = %.0f \n", VSH, SHT, RSH);
    printf(" MED  = %.0f \n", MED);
    printf(" VLN = %.0f;   LNG = %.0f;   RLN = %.0f \n", VLN, LNG, RLN);*/
    
    
// -------- FUZZY RULE BASE

//      --- Horizontal Line 1 (Bottom Line)
		FRB[0][0] = MED;
		FRB[1][0] = LNG; // LNG
		FRB[2][0] = LNG;
		FRB[3][0] = LNG;
		FRB[4][0] = VLN;
		
//	--- Horizontal Line 2 (Second from Bottom)
        FRB[0][1] = RSH;
		FRB[1][1] = MED; 
		FRB[2][1] = RLN; //RLN
		FRB[3][1] = LNG;
		FRB[4][1] = VLN;
		
//	--- Horizonatl Line 3 (Third from Bottom) 
        FRB[0][2] = SHT;
		FRB[1][2] = RSH;
		FRB[2][2] = MED;
		FRB[3][2] = RLN; //RLN
		FRB[4][2] = LNG;
		
//	---- Horizontal Line 4 (CENTRAL)       
        FRB[0][3] = SHT;
		FRB[1][3] = SHT;
		FRB[2][3] = RSH;
		FRB[3][3] = MED; // MED
		FRB[4][3] = LNG; //RLN
		
//	--- Horizontal Line 5 
        FRB[0][4] = VSH;
		FRB[1][4] = VSH;
		FRB[2][4] = SHT;
		FRB[3][4] = RSH;
		FRB[4][4] = MED;

//	------ END of the FUZZY RULE BASE of the 7-7-7 type Fuzzy Controller 

// --- Printing Fuzzy Rule Base
/*  	printf("\n\n  ** The FUZZY RULE BASE of TRAFFIC Controller ** \n");
	for(int j = MF1-1; j >= 0; j--){
		printf("MFY%i | ", (j+1));
		for(int i = 0; i < MF2; i++)
			printf(" %2.0f\t", FRB[i][j]);
			printf("\n");
	}
    
    printf("\t______________________________________ \n");
    printf("       MFX1     MFX2    MFX3    MFX4    MFX5 \n");*/
}

void fuzzification(float X, float Y, float XX[5], float YY[5], float FUZX[7], float FUZY[7])
{
	float Fx, Fy;
	for(int i = 0; i < 7; i++){
		FUZX[i] = 0;
		FUZY[i] = 0;
	}
	
	// Initialize the Vector FUZX:
	if(X <= XX[0])
	FUZX[0] = 1;
	else if(X >= XX[MF1-1])
	FUZX[MF1-1] = 1;
	else{
		for(int i = 0; i < MF1-1; i++){
			if(X > XX[i] && X <= XX[i+1]){
				Fx = (X - XX[i]) / (XX[i+1] - XX[i]);
				FUZX[i+1] = Fx;
				FUZX[i] = 1 - Fx;
			}
		}
		
	}
	
	// Initialize the Vector FUZY:
	if(Y <= YY[0])
	FUZY[0] = 1;
	else if(Y >= YY[MF2-1])
	FUZY[MF2-1] = 1;
	else{
		for(int i = 0; i < MF2-1; i++){
			if(Y > YY[i] && Y <= YY[i+1]){
				Fy = (Y - YY[i]) / (YY[i+1] - YY[i]);
				FUZY[i+1] = Fy;
				FUZY[i] = 1 - Fy;
			}
		}
		
	}
	
	// Tempoary Prints of FUZX and FUZY:
/*    printf("\n --- Fuzzyfied Values for Input X: \n");
    for(int i = 0; i < 7; i++)
    printf("%.3f  ", FUZX[i]);
    printf("\n");
    
    printf("\n === Fuzzyfied Values for Input Y: \n");
    for(int i = 0; i < 7; i++)
    printf("%.3f  ", FUZY[i]);
    printf("\n");*/
}

void fuzzy_inference(float FUZX[7], float FUZY[7], float ACTR[5][5])
{
	for(int j = 0; j < MF2; j++){
		for(int i = 0; i < MF1; i++){
			ACTR[i][j] = FUZX[i]*FUZY[j];
			//printf("x-%f  y-%f", FUZX[i], FUZY[j]);
		}
	}
	
	// --- Printing Activation Degrees
/*  	printf("\n\n  ** Temporary Activation Degrees ** \n");
	for(int j = MF1-1; j >= 0; j--){
		printf("MFY%i | ", (j+1));
		for(int i = 0; i < MF2; i++)
			printf(" %2.3f\t", ACTR[i][j]);
			printf("\n");
	}
    
    printf("\t______________________________________ \n");
    printf("       MFX1     MFX2    MFX3    MFX4    MFX5 \n"); */
	
}

float defuzzification(float FRB[5][5], float ACTR[5][5])
{
	float output;
	float numerator = 0;
	float denumerator = 0;
	
	for(int i = 0; i < MF1; i++)
	{
		for(int j = 0; j < MF2; j++){
			numerator = numerator + ACTR[i][j]*FRB[i][j];
			denumerator = denumerator + ACTR[i][j];
		}
	}
	
//	printf("\nNumerator: %f, Denumerator:%f\n", numerator, denumerator);
	output = numerator / denumerator;
	
	return round(output);
	
}
