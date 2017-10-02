#include "model.h"
lda_model* new_lda_model(int n_terms, int n_topics){
	lda_model* model;
	model = malloc(sizeof(lda_model));
	model->n_topics = n_topics;
	model->n_terms = n_terms;
	model->alpha = 1;
	model->lamda = malloc(sizeof(double*)*n_topics);
	model->lamda_sum =malloc(sizeof(double)*model->n_topics);
	model->Elogbeta = malloc(sizeof(double*)*model->n_topics);
	for(int i=0; i<n_topics ; i++){
		model->Elogbeta[i]=malloc(sizeof(double)*n_terms);
		model->lamda[i] = malloc(sizeof(double)*n_terms);
		model->lamda_sum[i]=0;
		for(int j=0; j<n_terms; j++){
			model->lamda[i][j]=0;
		}
	}
return model;
}
lda_ss* new_lda_ss(lda_model* model){
	lda_ss* ss;
	ss = malloc(sizeof(lda_ss));
	ss->class_word = malloc(sizeof(double*)*model->n_topics);
	ss->eta_sum = malloc(sizeof(double)*model->n_topics);
	ss->eta = malloc(sizeof(double*)*model->n_topics);
	for(int i =0; i<model->n_topics; i++){
		ss->eta_sum[i]=0;
		ss->eta[i] = malloc(sizeof(double)*model->n_terms);
		ss->class_word[i] = malloc(sizeof(double)*model->n_terms);
		for(int j=0; j<model->n_terms; j++){
			ss->eta[i][j]=0;
			ss->class_word[i][j] =0;
		}
	}
return ss;
}
void random_initil_lda_ss(lda_ss* ss, lda_model* model){
	for(int k=0; k<model->n_topics; k++){
		for(int w=0; w<model->n_terms; w++){
			
//printf("##-------------%d %d\n", model->n_topics, model->n_terms);
			ss->class_word[k][w] += 1.0/model->n_terms+myrand();//;
//printf("#-------------\n");
//printf("#-------------\n");
		}
	}
	for(int k=0; k<model->n_topics; k++){
		for(int w=0; w<model->n_terms; w++){
			ss->eta[k][w] = 0.01+myrand();
			ss->eta_sum[k]+=ss->eta[k][w];
		}
	}
	
}
void zero_initil_lda_ss(lda_ss* ss, lda_model* model){
	for(int k=0; k<model->n_topics; k++){
		for(int w=0; w<model->n_terms; w++){
			ss->class_word[k][w]=0;
		}
	}
}
void update_lamda(lda_model* model, lda_ss* ss){
	for(int k=0; k<model->n_topics; k++){
		model->lamda_sum[k]=0;
		for(int w=0; w<model->n_terms; w++){
			if(ss->class_word[k][w]>0){
				model->lamda[k][w]= (double)ss->class_word[k][w]+ss->eta[k][w];
			}else{
				model->lamda[k][w]=exp(-100);
			}
			model->lamda_sum[k]+=model->lamda[k][w];
			
		}
	}
		//diriclet_distribution(model->Elogbeta[k], model->lamda[k], model->n_terms);
	for(int k=0; k<model->n_topics; k++){
		for(int w=0; w<model->n_terms; w++){
				model->Elogbeta[k][w]= log(model->lamda[k][w]/model->lamda_sum[k]);
			
		}
	}
}
void save_lda_model(lda_model* model, char* model_root)
{
    char filename[100];
    FILE* fileptr;
    int i, j;

    sprintf(filename, "%s.lamda", model_root);
    fileptr = fopen(filename, "w");
    for (i = 0; i < model->n_topics; i++)
    {
	for (j = 0; j < model->n_terms; j++)
	{
	    fprintf(fileptr, " %5.10f", model->lamda[i][j]);
	}
	fprintf(fileptr, "\n");
    }
    fclose(fileptr);
}
