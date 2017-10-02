#include "read_data.h"
double** read_prior_f(char* filename, int n_topic, int n_terms){
	FILE* ptr=NULL;
	ptr=fopen(filename,"r");
	double** f;
	f=malloc(sizeof(double)*n_topic);
	for(int n=0; n<n_topic; n++){
		f[n]=malloc(sizeof(double)*n_terms);
	}
	for(int n=0; n<n_topic; n++){
		for(int l=0; l<n_terms; l++){
			fscanf(ptr,"%lf",&f[n][l]);
		}
	}
	return f;
}
corpus* read_data(char* filename){
    	FILE *fileptr;
    	int length, count, word, n, nd, nw;
    	corpus* c;

    	printf("Reading data from %s\n", filename);
    	c = malloc(sizeof(corpus));
    	c->docs = 0;
    	c->n_terms = 0;
    	c->n_docs = 0;
    	fileptr = fopen(filename, "r");
    	nd = 0; nw = 0;
    	while ((fscanf(fileptr, "%d", &length) != EOF)){
		c->docs = (document*) realloc(c->docs, sizeof(document)*(nd+1));
		c->docs[nd].length = length;
		c->docs[nd].total = 0;
		c->docs[nd].words = malloc(sizeof(int)*length);
		c->docs[nd].counts = malloc(sizeof(int)*length);
		for (n = 0; n < length; n++){
	    		fscanf(fileptr, "%d:%d", &word, &count);
	    		c->docs[nd].words[n] = word;
	    		c->docs[nd].counts[n] = count;
	    		c->docs[nd].total += count;
	    		if (word >= nw) { nw = word + 1; }
		}
		nd++;
    	}
    	fclose(fileptr);
    	c->n_docs = nd;
    	c->n_terms = nw;
    	printf("number of docs    : %d\n", nd);
    	printf("number of terms   : %d\n", nw);
 return(c);
}
corpus* read_data_online(int batch_size, FILE* fileptr){
    	int length, count, word, n, nd, nw,docs;
    	corpus* c;
    	c = malloc(sizeof(corpus));
    	c->docs = 0;
    	c->n_terms = 0;
    	c->n_docs = 0;
    	nd = 0; nw = 0;
	docs=0;
    	while (docs<batch_size){
		if((fscanf(fileptr, "%d", &length) == EOF)) break;
		docs++;
		c->docs = (document*) realloc(c->docs, sizeof(document)*(nd+1));
		c->docs[nd].length = length;
		c->docs[nd].total = 0;
		c->docs[nd].words = malloc(sizeof(int)*length);
		c->docs[nd].counts = malloc(sizeof(int)*length);
		for (n = 0; n < length; n++){
	    		fscanf(fileptr, "%d:%d", &word, &count);
	    		c->docs[nd].words[n] = word;
	    		c->docs[nd].counts[n] = count;
	    		c->docs[nd].total += count;
	    		if (word >= nw) { nw = word + 1; }
		}
		nd++;
    	}
    	c->n_docs = nd;
    	c->n_terms = nw;
    	printf("number of docs    : %d\n", nd);
    	printf("number of terms   : %d\n", nw);
	printf("max length %d\n",max_corpus_length(c));
	return (c);
}
void free_corpus(corpus* c){
	for(int i=0; i<c->n_docs; i++){
		free(c->docs[i].words);
		free(c->docs[i].counts);	
	}
	free(c->docs);
}
int max_corpus_length(corpus* c){
    	int n, max = 0;
    	for (n = 0; n < c->n_docs; n++)
		if (c->docs[n].length > max) max = c->docs[n].length;
return(max);
}
