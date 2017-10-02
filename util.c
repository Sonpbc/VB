#include "util.h"
void diriclet_distribution(double* a, double* x,int k){
	double sum=0;
	for(int i=0; i<k; i++){
		sum+=x[i];
	}
	double disum=digamma(sum);
	for(int i=0; i<k; i++){
		a[i] = digamma(x[i])-disum;
	}
}
double trigamma(double x){
    	double p;
    	int i;

	x=x+6;
    	p=1/(x*x);
    	p=(((((0.075757575757576*p-0.033333333333333)*p+0.0238095238095238)
         	*p-0.033333333333333)*p+0.166666666666667)*p+1)/x+0.5*p;
    	for (i=0; i<6 ;i++){
        	x=x-1;
        	p=1/(x*x)+p;
    	}
return(p);
}
double digamma(double x){
    	double p;

    	x=x+6;
    	p=1/(x*x);
    	p=(((0.004166666666667*p-0.003968253986254)*p+
		0.008333333333333)*p-0.083333333333333)*p;
    	p=p+log(x)-0.5/x-1/(x-1)-1/(x-2)-1/(x-3)-1/(x-4)-1/(x-5)-1/(x-6);
return p;
}
void make_directory(char* name){
    	mkdir(name, S_IRUSR|S_IWUSR|S_IXUSR);
}
int argmax(int* x, int n){
    	int i;
    	double max = x[0];
    	int argmax = 0;
    	for (i = 1; i < n; i++){
        	if (x[i] > max){
            		max = x[i];
            		argmax = i;
        	}
    	}
return(argmax);
}
