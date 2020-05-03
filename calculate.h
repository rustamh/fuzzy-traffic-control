#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

extern const int samples;
extern int traffic1[];
extern int traffic2[];

void generate_random_traffic (int lower1, int upper1, int lower2, int upper2, int from, int to);
void calculate_delay_fixed(int traffic[], float d[], float c, float g, float r);
float calculate_delay_fuzzy(int traffic[], int i, float c, float g, float r);
void write_to_csv(float fixed_delay1[], float fixed_delay2[], float fixed_delay[], 
					float fuzzy_delay1[], float fuzzy_delay2[], float fuzzy_delay[], 	
					int g, int r, float change_of_green[], float change[]);

