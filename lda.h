#ifndef LDA_H
#define LDA_H
	int EM_MAX_ITER, VAR_MAX_ITER, BATCH_SIZE, N_TERMS;
typedef struct{
	int* words;
	int* counts;
	int length;	//So tu khac nhau trong van ban
	int total;	//Tong so tu cua van ban
}document;
typedef struct{
	document* docs;
	int n_terms;	//Kich thuoc cua tu dien
	int n_docs;	//So van ban trong du lieu
}corpus;
typedef struct{
	double alpha;
	double** lamda; 	//log Beta
	double* lamda_sum;
	double** Elogbeta;	
	int n_topics;		
	int n_terms;
}lda_model;
typedef struct{
 	double** class_word;
	double** eta;
	double* eta_sum;
}lda_ss;
#endif
