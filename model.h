#ifndef MODEL_H
#define MODEL_H
#include "lda.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#define myrand() (double) (((unsigned long) random()) / 4294967296.)
lda_model* new_lda_model(int n_terms, int n_topics);
lda_ss* new_lda_ss(lda_model* model);
void random_initil_lda_ss(lda_ss* ss,lda_model* model);
void zero_initil_lda_ss(lda_ss* ss,lda_model* model);
void update_lamda(lda_model* model, lda_ss* ss);
void save_lda_model(lda_model* model, char* model_root);
#endif

