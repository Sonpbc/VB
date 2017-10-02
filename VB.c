#include "lda_est.h"
void run_EM(char* directory, corpus* corp){
	double** var_gamma;
	double** Elogtheta;
	double** phi;
	char filename[100];
	int em_iter=0;
	var_gamma = malloc(sizeof(double*)*corp->n_docs);
	for(int i=0; i<corp->n_docs; i++){
		var_gamma[i] = malloc(sizeof(double)*N_TOPICS);
	}
	int max_length=max_corpus_length(corp);
	phi = malloc(sizeof(double*)*max_length);
	for(int n=0; n<max_length; n++){
		phi[n]= malloc(sizeof(double)*N_TOPICS);
	}
	Elogtheta = malloc(sizeof(double*)*corp->n_docs);
	for(int d=0; d<corp->n_docs; d++){
		Elogtheta[d]= malloc(sizeof(double)*N_TOPICS);
	}
	lda_model* model=NULL;
	lda_ss* ss=NULL;
	model = new_lda_model(corp->n_terms, N_TOPICS);
//printf("-------------\n");
	ss = new_lda_ss(model);
//printf("-------------\n");
	printf("INITIL RANDOM\n");
	random_initil_lda_ss(ss, model);
//printf("-------------\n");
//	lda_mle(model, ss, 0);
///printf("-------------\n");
	for(int k=0; k<model->n_topics; k++){
		model->lamda_sum[k]=0;
		for(int n=0; n<model->n_terms; n++){
			model->lamda[k][n]=ss->eta[k][n];
			model->lamda_sum[k]+=model->lamda[k][n];
		}
		diriclet_distribution(model->Elogbeta[k], model->lamda[k], corp->n_terms);
	}
	model->alpha =ALPHA;
	double oldlikelihood=0, likelihood, converged=1;
	while(em_iter<EM_MAX_ITER&&((converged>EM_CONVERGED)||(converged<0))){
		em_iter++;
		likelihood=0;
		zero_initil_lda_ss(ss, model);
		//E STEP
		for(int d=0; d<corp->n_docs; d++){
			if(d%1000==0){printf("documemt %d\n", d);}
			likelihood+=doc_e_step(&(corp->docs[d]),Elogtheta[d], var_gamma[d], phi, model,ss);
		}
		//M STEP

		update_lamda(model, ss);
//		converged = (oldlikelihood-likelihood)/oldlikelihood;
//		oldlikelihood = likelihood;
		printf("Iter %d likelihood %10f \n", em_iter, likelihood);
		if(em_iter%5==0){
			sprintf(filename,"%s/%d",directory, em_iter);
			save_lda_model(model, filename);		
		}
	
	}
	sprintf(filename,"%s/final",directory);
	save_lda_model(model, filename);
}
double doc_e_step(document* doc, double* Elogtheta, double* gamma, double** phi, lda_model* model, lda_ss* ss){
	double likelihood;
	likelihood = lda_infer(doc, model, phi, Elogtheta, gamma,ss);
	for(int n=0; n<doc->length; n++){
		for(int k=0; k<model->n_topics; k++){
			ss->class_word[k][doc->words[n]] += doc->counts[n]*phi[n][k];
		}	
	}
return likelihood;
}

void read_setting(char* file){
	FILE* ptr;
	ptr = fopen(file, "r");
	fscanf(ptr, "var max iter %d\n", &VAR_MAX_ITER);
	fscanf(ptr, "var converged %f\n", &VAR_CONVERGED);
	fscanf(ptr, "em max iter %d\n", &EM_MAX_ITER);
	fscanf(ptr, "em converged %f", &EM_CONVERGED);
}
int main(int argc, char *argv[]){
	// lda [alpha] [n topic] [setting] [data] [directory]

	corpus* corp;
	ALPHA = atof(argv[1]);
	N_TOPICS = atoi(argv[2]);
	read_setting(argv[3]);
	corp = read_data(argv[4]);
	make_directory(argv[5]);
	run_EM(argv[5], corp);
}
