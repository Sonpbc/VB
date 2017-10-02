#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <sys/stat.h>
#include <sys/types.h>
double log_sum(double a, double b);
double digamma(double x);
double trigamma(double x);
double log_gamma(double x);
void make_directory(char* name);
void diriclet_distribution(double* a,double* x, int k);
int argmax(int* x, int n);
#endif
