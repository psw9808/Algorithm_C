#include <stdio.h>
#define MAXLINE	1024
#define maxitem 100
#define maxbagsize 1000
#define weight 0
#define value 1

int knapsack(int i, int size);
int num_data;
float item[maxitem][2];
float dp[maxbagsize][maxitem];
int dp2[maxitem];
float item2[maxitem][2][maxitem];
float dp3[maxbagsize][maxitem][maxitem];
int dp4[maxitem][maxitem];
int dp5[maxitem][maxitem];

int main(){
	char line[MAXLINE];
	FILE *fp;
	int index=0;
	float data1;
	float data2;
	int i;
	int size;
	memset(dp, -1, sizeof(dp));
	int max2 = 0;
	int maxindex = 0;

	printf("input file name? : ");
	scanf("%s",&line);
	
	fp = fopen(line,"r");
	if(fp==NULL){
		printf("cannot read file");
		exit(0);
	}
	
	printf("\n");
	
	while(data2 != -1){
		fscanf(fp,"%f %f",&data1,&data2);
		item[index][weight] = data1;
		item[index][value] = data2;
		index++;
	}
	num_data = --index;
	size = item[num_data][0];
	printf("\t(weight , value)  bag size = %d \n",size);


	for(i=0; i<num_data; i++){
		printf(" item %d : (%3.1f , %3.1f)\n",i+1,item[i][weight],item[i][value]);
	}
	printf("\n--------- num1 : knapsack problem ---------\n");				//		num1

	knapsack(0,size);

	printf("\n");
	for(i=0; i<num_data; i++){
		if(dp2[i]==1){
			printf(" %d",i+1);
		}
	}

	printf("  %3.1f\n",dp[size][num_data]);  //print max value

	printf("\n--------- num2 : with one item split ---------\n");			//				num2

	for(int i=1; i<=num_data; i++){
		for(int j=0; j<num_data; j++ ){
			item2[j][weight][i] = item[j][weight]*2;
			item2[j][value][i] = item[j][value]*2;
		}
		item2[num_data][weight][i] = item[i-1][weight];
		item2[i-1][weight][i] = item2[num_data][weight][i];
		item2[num_data][value][i] = item[i-1][value];
		item2[i-1][value][i] = item2[num_data][value][i];
	}

	size = size*2;

	for(int i=1; i<=num_data; i++){
		knapsack2(i,size);
	}

	max2 = 0;
	maxindex = 0;
	for(int i=1; i<=num_data+1; i++){
		knapsack2(i,size);
		if(dp3[size][num_data+1][i]>max2){
			max2 = dp3[size][num_data+1][i];
			maxindex = i;
		}
	}

	printf("\n");
	for(i=0; i<num_data; i++){
		if(dp4[i][maxindex]==1){
			printf(" %d",i+1);

			if(i==maxindex-1 && dp4[num_data][maxindex]==0){
				printf("x0.5");
			}
		}
	}
	printf("  %3.1f\n",dp3[size][num_data+1][maxindex]/2);

	size=size/2;

    printf("\n--------- num3 : knapsack with one duplicate item ---------\n");			//				num3

	for(int i=1; i<=num_data; i++){
		for(int j=0; j<num_data; j++ ){
			item2[j][weight][i] = item[j][weight];
			item2[j][value][i] = item[j][value];
		}
		item2[num_data][weight][i] = item[i-1][weight];
		item2[num_data][value][i] = item[i-1][value];
	}

	max2 = 0;
	maxindex = 0;

	for(int i=1; i<=num_data; i++){
		knapsack3(i,size);
	}


	for(int i=1; i<=num_data+1; i++){
		knapsack2(i,size);
		if(dp3[size][num_data+1][i]>max2){
			max2 = dp3[size][num_data+1][i];
			maxindex = i;
		}
	}

	printf("\n");
	for(i=0; i<num_data; i++){
		if(dp5[i][maxindex]==1){
			printf(" %d",i+1);

			if(i==maxindex-1 && dp5[num_data][maxindex]==1){
				printf("x2");
			}
		}
	}
	printf("  %3.1f\n",dp3[size][num_data+1][maxindex]);




}

int knapsack(int ind, int size){

	for(int j=0; j<=size; j++){
		dp[j][0] = 0;
	}
	for(int i=1; i<=num_data; i++){
		dp[0][i] = 0;
		for(int j=0; j<=size; j++){

			if(item[i-1][weight]<=j){

				if(item[i-1][value]+dp[j-(int)item[i-1][weight]][i-1]>dp[j][i-1]){
					dp[j][i] = item[i-1][value]+dp[j-(int)item[i-1][weight]][i-1];

				}
				else{
					dp[j][i] = dp[j][i-1];
				}
			}
			else{
				dp[j][i] = dp[j][i-1];
			}
		}
	}

	int w=size;
	for(int i=num_data; i>0; i--){
		if(dp[w][i]==0){
			break;
		}
		else if(dp[w][i]>dp[w][i-1]){
			dp2[i-1] = 1;
			w -= item[i-1][weight];
		}
		else{

		}
	}
	return 0;
}

int knapsack2(int ind, int size){

	for(int j=0; j<=size; j++){
		dp3[j][0][ind] = 0;
	}
	for(int i=1; i<=num_data+1; i++){
		dp3[0][i][ind] = 0;
		for(int j=0; j<=size; j++){

			if(item2[i-1][weight][ind]<=j){

				if(item2[i-1][value][ind]+dp3[j-(int)item2[i-1][weight][ind]][i-1][ind]>dp3[j][i-1][ind]){
					dp3[j][i][ind] = item2[i-1][value][ind]+dp3[j-(int)item2[i-1][weight][ind]][i-1][ind];

				}
				else{
					dp3[j][i][ind] = dp3[j][i-1][ind];
				}
			}
			else{
				dp3[j][i][ind] = dp3[j][i-1][ind];
			}
		}
	}

	int w=size;
	for(int i=num_data+1; i>0; i--){
		if(dp3[w][i][ind]==0){
			break;
		}
		else if(dp3[w][i][ind]>dp3[w][i-1][ind]){
			dp4[i-1][ind] = 1;
			w -= item2[i-1][weight][ind];
		}
		else{

		}
	}
	return 0;
}

int knapsack3(int ind, int size){

	for(int j=0; j<=size; j++){
		dp3[j][0][ind] = 0;
	}
	for(int i=1; i<=num_data+1; i++){
		dp3[0][i][ind] = 0;
		for(int j=0; j<=size; j++){

			if(item2[i-1][weight][ind]<=j){

				if(item2[i-1][value][ind]+dp3[j-(int)item2[i-1][weight][ind]][i-1][ind]>dp3[j][i-1][ind]){
					dp3[j][i][ind] = item2[i-1][value][ind]+dp3[j-(int)item2[i-1][weight][ind]][i-1][ind];

				}
				else{
					dp3[j][i][ind] = dp3[j][i-1][ind];
				}
			}
			else{
				dp3[j][i][ind] = dp3[j][i-1][ind];
			}
		}
	}

	int w=size;
	for(int i=num_data+1; i>0; i--){
		if(dp3[w][i][ind]==0){
			break;
		}
		else if(dp3[w][i][ind]>dp3[w][i-1][ind]){
			dp5[i-1][ind] = 1;
			w -= item2[i-1][weight][ind];
		}
		else{

		}
	}
	return 0;
}








