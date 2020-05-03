#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


void fuzzy_rule_base(float FRB[5][5]);
void fuzzification(float X, float Y, float XX[5], float YY[5], float FUZX[7], float FUZY[7]);
void fuzzy_inference(float FUZX[7], float FUZY[7], float ACTR[5][5]);
float defuzzification(float FRB[5][5], float ACTR[5][5]);

