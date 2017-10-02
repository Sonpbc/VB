#ifndef LDA_EST_H
#define LDA_EST_H
#include "read_data.h"
#include "infer.h"
#include "model.h"
#include "lda.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
int EM_MAX_ITER;
float EM_CONVERGED;
double ALPHA;
int N_TOPICS;
void read_setting(char* file);
void run_EM(char* directory, corpus* corp);
double doc_e_step(document* doc, double* Elogtheta, double* gama, double** phi, lda_model* model, lda_ss* ss);
#endif
