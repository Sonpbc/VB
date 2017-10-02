#ifndef PER_H
#define PER_H
#include "lda.h"
#include "util.h"
#include <math.h>
#include "model.h"
#include "read_data.h"
double** doc_e_stepp(corpus* corp, lda_model* model){
	double** gamma;
	double** phi;
	double** Elogtheta;
	gamma = malloc(sizeof(double*)*corp->n_docs);
	for(int d=0; d<corp->n_docs; d++){
		gamma[d] = malloc(sizeof(double)*model->n_topics);	
	}
	Elogtheta = malloc(sizeof(double*)*corp->n_docs);
	for(int d=0; d<corp->n_docs; d++){
		Elogtheta[d] = malloc(sizeof(double)*model->n_topics);	
	}
	int max_length = max_corpus_length(corp);
	phi = malloc(sizeof(double*)*max_length);
	for(int l=0; l<max_length; l++){
		phi[l] = malloc(sizeof(double*)*model->n_topics);
	}
	for(int d=0; d<corp->n_docs; d++){
		for(int k=0; k<model->n_topics; k++){
			gamma[d][k] = 0.01+myrand();
		}
		diriclet_distribution(Elogtheta[d], gamma[d], model->n_topics);
	}
	for(int n=0; n<max_length; n++){
		for(int k=0; k<model->n_topics; k++){
			phi[n][k]=1.0/model->n_topics;
		}
	}
	double phisum,sum;
	for(int d=0; d<corp->n_docs; d++){
		int iter=0;
		while(iter<VAR_MAX_ITER){
			iter++;
			for(int n=0; n<corp->docs[d].length; n++){
				phisum=0;
				for(int k=0; k<model->n_topics; k++){
					phi[n][k]=exp(Elogtheta[d][k])*exp(model->Elogbeta[k][corp->docs[d].words[n]]);
					phisum+=phi[n][k];
				}
				for(int k=0; k<model->n_topics; k++){
					phi[n][k]=phi[n][k]/phisum;
				}
			}
			for(int k=0; k<model->n_topics; k++){
				for(int n=0; n<corp->docs[d].length; n++){
					gamma[d][k]=model->alpha+corp->docs[d].counts[n]*phi[n][k];
				}
			}
			diriclet_distribution(Elogtheta[d],gamma[d], model->n_topics);
		}
		sum=0;
		for(int k=0; k<model->n_topics; k++){
			sum+=gamma[d][k];
		}
		for(int k=0; k<model->n_topics; k++){
			gamma[d][k]=gamma[d][k]/sum;
		}
		
							
	}
	for(int d=0; d<corp->n_docs; d++){
		free(Elogtheta[d]);	
	}
	free(Elogtheta);
	for(int l=0; l<max_length; l++){
		free(phi[l]);
	}
	free(phi);
return gamma;
}
double compute_lp(double* gammad,document* doc, lda_model* model){
	double ld=0,p=0;
	for(int n=0; n<doc->length; n++){
		p=0;
		for(int k=0; k<model->n_topics; k++){
			p+=gammad[k]*model->lamda[k][doc->words[n]]/model->lamda_sum[k];
		}
		ld+=doc->counts[n]*log(p);
	}
	if(doc->total!=0){
		ld=ld/doc->total;
	}
return ld;
	
}
double compute_perplexity(lda_model* model,corpus* cp1, corpus* cp2){
	double** gamma;
	double PP=0;
		gamma=doc_e_stepp(cp1, model);
		for(int d=0; d<cp2->n_docs; d++){
			PP+=compute_lp(gamma[d], &(cp2->docs[d]), model);
		}
return PP;
}
#endif
