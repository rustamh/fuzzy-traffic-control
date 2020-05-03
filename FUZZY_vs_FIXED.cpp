#include "fuzzy.h"
#include "calculate.h"

const int samples = 72;
int traffic1[samples];
int traffic2[samples];
					
int main()
{
	float c1 = 100; // road capacity car/100 sec [capacity] for road 1
	float c2 = 60; // road capacity car/100 sec [capacity] for road 2
	
	// FUZZY DECLARATION
	float X, Y;
	float FRB[5][5], ACTR[5][5]; // 1st dimension - input 1 , 2nd dimension - input 2
	float FUZX[7], FUZY[7];
//	float FRB[4][4], ACTR[4][4]; // 1st dimension - input 1 , 2nd dimension - input 2
//	float FUZX[5], FUZY[5];
	float change_of_green[samples];
	float change[samples];
	
	//			  VS   S   M   L    VL
	float XX[] = {10, 20, 40, 70, 200};  // centers for input 1 (delay of road 1-big)
	float YY[] = {10, 20, 40, 70, 200};  // centers for input 1 (delay of road 1-big)

	//			  VS   S   M   L    VL
//	float YY[] = {30, 50, 80, 100, 200};  // centers for input 2 (delay of road 2-small)
//	float XX[] = {20, 40, 60, 80, 100};  // centers for input 1 (delay of road 1-big)
	
	//			  VS   S   M   L    VL
//	float YY[] = {12, 24, 36, 48, 60};  // centers for input 2 (delay of road 2-small)
	
	  	// Print the centers of the MF for Input XX on the Screen:
/*    printf("\n --- Locations of the Membership Functions for Input XX (R1's Delay)\n");
     	for(int i=0; i<MF1; i++) {
        printf(" %.0f  ", XX[i]);
        }
    printf(" \n"); 

// Print the centers of the MF for Input YY on the Screen:
    printf("\n --- Locations of the Membership Functions for Input YY (R2's Delay)\n");
     	for(int i=0; i<MF2; i++) {
        printf(" %.0f  ", YY[i]);
        }
    printf(" \n");*/
    
	
	
	//* Random traffic generation for 1 hour for both roads [samples are taken after 500 sec] , [samples samples x 100 sec = 7200 secs = 2 hours]
	generate_random_traffic(80, 85, 50, 55, 1, 5);	   // [500 seconds = 8.3 minutes]        
	generate_random_traffic(85, 90, 50, 55, 5, 10);     // for 8.3 mins       
	generate_random_traffic(85, 90, 50, 55, 10, 15);     // for 8.3 mins          
	generate_random_traffic(90, 95, 50, 55, 15, 20);    // for 8.3 mins        
	generate_random_traffic(95, 100, 55, 60, 20, 25);   // for 8.3 mins          
	generate_random_traffic(100, 105, 55, 60, 25, 30);   // for 8.3 mins    
	generate_random_traffic(105, 110, 55, 60, 30, 35);	   // for 8.3 mins       
	generate_random_traffic(105, 110, 55, 60, 35, 40);     // for 8.3 mins        
	generate_random_traffic(110, 115, 55, 60, 40, 45);     // for 8.3 mins           
	generate_random_traffic(115, 120, 60, 65, 45, 50);    // for 8.3 mins       
	generate_random_traffic(115, 120, 60, 65,50, 55);   // for 8.3 mins          
	generate_random_traffic(115, 120, 60, 65, 55, 60);   // for 8.3 mins   
	generate_random_traffic(115, 121, 65, 70, 60, 65);	   // for 8.3 mins          
	generate_random_traffic(115, 118, 65, 70, 65, 70);     // for 8.3 mins      
	generate_random_traffic(110, 115, 65, 70, 70, 72);     // for 3.3 mins

	
	
	float delay_fixed1[samples], delay_fuzzy1[samples]; // road 1 [delay array]
	float delay_fixed2[samples], delay_fuzzy2[samples]; // road 2 [delay array]
	float delay_fixed[samples], delay_fuzzy[samples]; // [total delay array]
	
	float green = 80, red = 20;
	float total_time = green + red;
	
	calculate_delay_fixed(traffic1, delay_fixed1, c1, green, red); // calculate delay for big road
	calculate_delay_fixed(traffic2, delay_fixed2, c2, red, green); // calculate delay for small road
	
	fuzzy_rule_base(FRB); // Create FUZZY RULE BASE
	float input_big = 0, input_small = 0;
	
	for(int i = 0; i < samples; i++){
		delay_fuzzy1[i] = calculate_delay_fuzzy(traffic1, i, c1, green, red); // calculate delay for big road
		delay_fuzzy2[i] = calculate_delay_fuzzy(traffic2, i, c2, red, green); // calculate delay for small road
		change[i] = green;

		input_big = delay_fuzzy1[i] * traffic1[i]/100;
		input_small = delay_fuzzy2[i] * traffic2[i]/100;
		fuzzification(input_big, input_small, XX, YY, FUZX, FUZY); // FUZZIFICATION
		//fuzzification(delay_fuzzy1[i], delay_fuzzy2[i], XX, YY, FUZX, FUZY); // FUZZIFICATION
		//fuzzification(traffic1[i], traffic2[i], XX, YY, FUZX, FUZY); // FUZZIFICATION
		fuzzy_inference(FUZX, FUZY, ACTR); // FUZZY INFERENCE
		//change_of_green[i] = defuzzification(FRB, ACTR);
		green = defuzzification(FRB, ACTR);
		//green = green + change_of_green[i];
		if(green > 90)
			green = 90;
		else if(green < 10)
			green = 10;
		red = total_time - green;
	}
	
	
	//printf("\nFuzzy Output: %f", defuzzification(FRB, ACTR));
	
	
	for (int i = 0; i < samples; i++){
//		delay_fixed[i] = delay_fixed1[i] + delay_fixed2[i];
//		delay_fuzzy[i] = delay_fuzzy1[i] + delay_fuzzy2[i];
		delay_fixed[i] = (delay_fixed1[i] * traffic1[i] + delay_fixed2[i] * traffic2[i]) / (traffic1[i] + traffic2[i]);
		delay_fuzzy[i] = (delay_fuzzy1[i] * traffic1[i] + delay_fuzzy2[i] * traffic2[i]) / (traffic1[i] + traffic2[i]);
	}
	
	/* Writing results to CSV file */
	write_to_csv(delay_fixed1, delay_fixed2, delay_fixed, delay_fuzzy1, delay_fuzzy2, delay_fuzzy, green, red, change_of_green, change);
}

