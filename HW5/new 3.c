#include <stdio.h>
#include <math.h>

void main(){
	int n;
	int x[100];
	int y[100];
	float d[99];
	float sum=0;
	scanf("%d",&n);
	for(int i=0; i<n; i++){
		scanf("%d %d",&x[i],&y[i]);
	}
	for(int i=0; i<n-1; i++){
		d[i] = sqrt((x[i+1]-x[i])*(x[i+1]-x[i])+(y[i+1]-y[i])*(y[i+1]-y[i]));
		sum+=d[i];
	}
	printf("sum = %f",sum);
}