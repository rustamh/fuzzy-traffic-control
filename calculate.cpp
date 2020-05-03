#include "calculate.h"

void write_to_csv(float fixed_delay1[], float fixed_delay2[], float fixed_delay[], 
					float fuzzy_delay1[], float fuzzy_delay2[], float fuzzy_delay[], 	
					int g, int r, float change_of_green[], float change[])
{ 	
 	int ge = 70, re = 30; 
	char location[100];
	char green[5], red[5];
	sprintf(green, "%d", ge);
	sprintf(red, "%d", re);
	strcpy(location, "C:/Users/Rustam/Desktop/Diploma work/ADATA/Scenarios/Ofuzzy_");
	strcat(location, green);
	strcat(location, "_");
	strcat(location, red);
	strcat(location, ".csv");
	
	//FILE *out_file = fopen("C:/Users/Rustam/Desktop/Diploma work/simulation/data/traffic_data_70_30.csv", "w");
	FILE *out_file = fopen(location, "w");
	FILE *out_file2 = fopen("C:/Users/Rustam/Desktop/Diploma work/ADATA/Scenarios/weighted.csv", "w");

	if(out_file == NULL){
	printf("Error! Couldn't open file");
	exit(-1);
	}
	
	fprintf(out_file2, "%d-%d\n", ge, re);
	float fx1, fx2, fz1, fz2;
	
	for(int i = 0; i < samples; i++){
			fprintf(out_file, "%d, (%d%%), %d, (%d%%),, ", traffic1[i], traffic1[i], traffic2[i], traffic2[i]*10/6);
			fprintf(out_file, "%f, %f, %f,,", fixed_delay1[i], fixed_delay2[i], fixed_delay[i]); 
			fprintf(out_file, "%f, %f, %f,,", fuzzy_delay1[i], fuzzy_delay2[i], fuzzy_delay[i]); 
			fprintf(out_file, "%.0f, %.0f\n", change[i], 100-change[i]);
			
			fx1 = fixed_delay1[i] * traffic1[i]/100;
			fx2 = fixed_delay2[i] * traffic2[i]/100;
			fz1 = fuzzy_delay1[i] * traffic1[i]/100;
			fz2 = fuzzy_delay2[i] * traffic2[i]/100;
			
			fprintf(out_file2, "%d, (%d%%), %d, (%d%%),, ", traffic1[i], traffic1[i], traffic2[i], traffic2[i]*10/6);
			fprintf(out_file2, "%f, %f, %f,,", fx1, fx2, fixed_delay[i]); 
			fprintf(out_file2, "%f, %f, %f,,", fz1, fz2, fuzzy_delay[i]); 
			fprintf(out_file2, "%.0f, %.0f\n", change[i], 100-change[i]);
	} 
				
	fclose(out_file);
	fclose(out_file2);
}

void generate_random_traffic (int lower1, int upper1, int lower2, int upper2, int from, int to)
{
	lower2 = 60*lower2/100;
	upper2 = 60*upper2/100;
	srand(time(NULL));
	int i = from-1;
	while (i < to){
	traffic1[i] = lower1 + rand()% (upper1 - lower1);
	traffic2[i] = lower2 + rand()% (upper2 - lower2);
	i++;
	}
}


float min(int a)
{
	if(a>1)
		return 1;
	return a;
}

void calculate_delay_fixed(int traffic[], float d[], float c, float g, float r)
{
	//static float d[samples]; // average delay on intersection [delay]
	float C = g+r; // green + red time [total cycle_time]
	float l; // g/C [green_ratio]
	float x; // v/c [saturation_degree]
	float f;
	
	/***
		d, C, l, and x are average delay (s), cycle time (s),
		green ratio, and degree of saturation, respectively. l = g/C
		and x = v/c, where c, g, and v are capacity (vehicle per hour),
		green time (sec), and input volume, respectively.	
	
	***/
	
	int i = 0;
	while (i < samples){
		
	l = g / C;
	x = traffic[i] / c;
	f = c * l;
	
	d[i] = 0.38 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 173 * pow(x, 2) * ((x - 1) + sqrt(pow((x-1), 2) + 16*x/f)); // HCM 1994
//	d[i] = 0.385 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 173 * ((x - 1) + sqrt(pow((x-1), 2) + 32*(x-0.5)/c)); // Alcelik + HCM 1994
//	d[i] = 0.5 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 225 *  ((x - 1) + sqrt(pow((x-1), 2) + 16*x/c)); // Canadian 
//	d[i] = 0.38 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 173 *  ((x - 1) + sqrt(pow((x-1), 2) + 32*x/c)); // Alcelik + Canadian
	
	if(d[i]<0)
		d[i] = 0;
		
	i++;
	}
	
}

float calculate_delay_fuzzy(int traffic[], int i, float c, float g, float r)
{
	
	//static float d[samples]; // average delay on intersection [delay]
	float C = g+r; // green + red time [total cycle_time]
	float l; // g/C [green_ratio]
	float x; // v/c [saturation_degree]
	float delay;
	float f;
	/***
	
		d, C, l, and x are average delay (s), cycle time (s),
		green ratio, and degree of saturation, respectively. l = g/C
		and x = v/c, where c, g, and v are capacity (vehicle per hour),
		green time (sec), and input volume, respectively.	
	
	***/
	
	l = g / C;
	x = traffic[i] / c;
	f = c * l;

	delay = 0.38 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 173 * pow(x, 2) * ((x - 1) + sqrt(pow((x-1), 2) + 16*x/f)); // HCM 1994
//	delay = 0.385 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 173 * ((x - 1) + sqrt(pow((x-1), 2) + 32*(x-0.5)/c)); // Alcelik + HCM 1994
//	d[i] = 0.5 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 225 *  ((x - 1) + sqrt(pow((x-1), 2) + 16*x/c)); // Canadian 
//	d[i] = 0.38 * (C * pow((1-l), 2)) / (1 - l*min(x)) + 173 *  ((x - 1) + sqrt(pow((x-1), 2) + 32*x/c)); // Alcelik + Canadian
	
	if(delay<0)
		delay = 0;
	
	return delay;
}
