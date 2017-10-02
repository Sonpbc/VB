#ifndef INFER_H
#define INFER_H
#include "util.h"
#include "lda.h"
#include "math.h"
#include "assert.h"
float VAR_CONVERGED;
int VAR_MAX_ITER;
double lda_infer(document* doc, lda_model* model, double** phi,double*elog, double* gammar, lda_ss* eta);
double compute_likelihood(document* doc, lda_model* model, double** phi,double* elog, double* gammar, lda_ss* eta);
#endif
