#include "infer.h"
double lda_infer(document* doc, lda_model* model, double** phi,double* Elogtheta, double* var_gamma, lda_ss* ss){
    	double converged = 1;
    	double phisum = 0, likelihood = 0;
    	double likelihood_old = 0, oldphi[model->n_topics];
    	int k, n, var_iter;
    // compute posterior dirichlet

    	for (k = 0; k < model->n_topics; k++){
        	var_gamma[k] = model->alpha + (doc->total/((double) model->n_topics));
        	for (n = 0; n < doc->length; n++)
            		phi[n][k] = 1.0/model->n_topics;
    	}
	diriclet_distribution(Elogtheta, var_gamma, model->n_topics);
    	var_iter = 0;
    	while ((converged > VAR_CONVERGED) &&
           	((var_iter < VAR_MAX_ITER) || (VAR_MAX_ITER == -1))){
		var_iter++;
		for (n = 0; n < doc->length; n++){
            		phisum = 0;
            		for (k = 0; k < model->n_topics; k++){
                		phi[n][k] = exp(Elogtheta[k])*exp(model->Elogbeta[k][doc->words[n]]);			
				phisum +=phi[n][k];
            		}
			for (k = 0; k < model->n_topics; k++){
                		phi[n][k] = phi[n][k]/phisum;
			}
		}
            	for (k = 0; k < model->n_topics; k++){
			double temp=0;
			for (n = 0; n < doc->length; n++){
                		temp+=doc->counts[n]*phi[n][k];
            		}
			var_gamma[k]=model->alpha+temp;
        	}
		diriclet_distribution(Elogtheta, var_gamma, model->n_topics);
//        likelihood = compute_likelihood(doc, model, phi,Elogtheta, var_gamma, ss);
//	printf("likelihood doc %f\n", likelihood);
//        assert(!isnan(likelihood));
//        converged = (likelihood_old - likelihood) / likelihood_old;
//        likelihood_old = likelihood;

        // printf("[LDA INF] %8.5f %1.3e\n", likelihood, converged);
    	}
return (likelihood);
}

double compute_likelihood(document* doc, lda_model* model, double** phi, double* Elogtheta, double* gamma, lda_ss* ss){ //Lower Bound
	double likelihood=0, dig[model->n_topics], digsum=0, gammasum=0;
	int k,n,i;
	for(k=0; k<model->n_topics; k++){
		gammasum+=gamma[k];
	}
	likelihood = 	lgamma(model->n_topics*model->alpha)
			-model->n_topics*lgamma(model->alpha)
			-lgamma(gammasum);
	for(k=0; k<model->n_topics; k++){
		likelihood+=	(model->alpha-1)*Elogtheta[k]
				+lgamma(gamma[k])
				-(gamma[k]-1)*Elogtheta[k];
		for(n=0; n<doc->length; n++){
			if(phi[n][k]>0){
				likelihood +=	doc->counts[n]*
						(phi[n][k]*(Elogtheta[k]-log(phi[n][k])+model->Elogbeta[k][doc->words[n]]));
			}
		}
	}
	for(k=0; k<model->n_topics; k++){
		likelihood+=lgamma(ss->eta_sum[k])-lgamma(model->lamda_sum[k]);
		for(n=0; n<model->n_terms; n++){
			likelihood+=-lgamma(ss->eta[k][n])+(ss->eta[k][n]-model->lamda[k][n])*model->Elogbeta[k][n]
					+lgamma(model->lamda[k][n]);
		}
	}
return likelihood;
}

