#ifndef READ_DATA_H
#define READ_DATA_H
#include <stdio.h>
#include <stdlib.h>
#include "lda.h"
double** read_prior_f(char* filename, int n_topic, int n_terms);
corpus* read_data(char* file_name);
corpus* read_data_online(int batch_size, FILE* file_name);
int max_corpus_length(corpus* c);
void free_corpus(corpus* corp);
#endif
