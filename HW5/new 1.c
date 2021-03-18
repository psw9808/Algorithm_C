#include <stdio.h>
#include <math.h>  // using sqrt function at distance

void quicksort(int left,int right, int* data1, int*data2);
void quicksort2(int left,int right, float* data1, int*data2);
float distance(int i,int j);
float distance2(int i,int j);
void getdot1(float dsum);
void getdot2(float dsum);
void printd(float dsum);
int x[100];		//change data of x,y (sorting all times)
int y[100];
int x2[100];	//original data of x,y (sorting one time)
int y2[100];
int off[98];	//use or not use of the dots
float d[98];	//distance profit (change)
float d2[98];	//distance profit (original)
int index[98];	//change index (sorting)
int index2[98];	//original index 
float dsum;		// given distance 200,400,600,800,1000,1500
float dtmp,dtmp2;	//method1 and 2 distance
int count,count2;	//method1 and 2 number of dots
int n;
FILE *fp2;

//data sort using quicksort (input data : related in x but change both of x,y)
void quicksort(int left,int right, int* data1, int*data2){
   int p=left, j=p, i=left+1;
   if(left<right){
      for(;i<=right; i++){
         if(data1[i]<data1[p]){
            j++;
            int tmp1 = data1[j];
            int tmp2 = data2[j];
            data1[j] = data1[i];
            data2[j] = data2[i];
            data1[i] = tmp1;
            data2[i] = tmp2;
         }
         else if(data1[i]==data1[p]){
            if(data2[i]<data2[p]){
            j++;
            int tmp1 = data1[j];
            int tmp2 = data2[j];
            data1[j] = data1[i];
            data2[j] = data2[i];
            data1[i] = tmp1;
            data2[i] = tmp2;
         }
         }
      }
      int tmp1 = data1[j];
      int tmp2 = data2[j];
      data1[j] = data1[left];
      data2[j] = data2[left];
      data1[left] = tmp1;
      data2[left] = tmp2;
      p = j;
      
      quicksort(left,p-1,data1,data2);
      quicksort(p+1,right,data1,data2);
   }
}

//data sort using quicksort (distance data : related in distance profit 'd' , and change both of d,index)
void quicksort2(int left,int right, float* data1, int*data2){
   int p=left, j=p, i=left+1;
   if(left<right){
      for(;i<=right; i++){
         if(data1[i]>data1[p]){
            j++;
            int tmp1 = data1[j];
            int tmp2 = data2[j];
            data1[j] = data1[i];
            data2[j] = data2[i];
            data1[i] = tmp1;
            data2[i] = tmp2;
         }
      }
      int tmp1 = data1[j];
      int tmp2 = data2[j];
      data1[j] = data1[left];
      data2[j] = data2[left];
      data1[left] = tmp1;
      data2[left] = tmp2;
      p = j;
      
      quicksort2(left,p-1,data1,data2);
      quicksort2(p+1,right,data1,data2);
   }
}

//calculate distance for x,y  (use in method 1)
float distance(int i,int j){
   int xx = x[j] - x[i];
   int yy = y[j] - y[i];
   return sqrt(xx*xx+yy*yy);
}

//calculate distance for x2,y2	(use in main)
float distance2(int i,int j){
   int xx = x2[j] - x2[i];
   int yy = y2[j] - y2[i];
   return sqrt(xx*xx+yy*yy);
}

//get points under given distance (first method : update data all times)
void getdot1(float dsum){
   count=0;
   dtmp = 0;
   
	for(int i=0; i<n; i++){
		x[i]=x2[i];
		y[i]=y2[i];
	}
	for(int i=0; i<n-2; i++){
		d[i]=d2[i];
		index[i]=index2[i];
	}
   
	for(int i=0; i<n-2; i++){
	   
		for(int j=1,h=0; j<n-i; j++){
			if(x[j]!=0){
				count++;
				dtmp += distance(h,j);
				h=j;
			}
		}
		
      if(dtmp<dsum){
         break;
      }
	  
      dtmp = 0;
	  count = 0;
	  for(int j=index[0]+1; j<n-1-i; j++){
		  x[j]=x[j+1];
		  y[j]=y[j+1];
	  }
	  
	  for(int i=0; i<n-2-i; i++){
		  index[i] = i;
		  d[i] = distance(i,i+1)+distance(i+1,i+2)-distance(i,i+2);
	  }
	  
	  quicksort2(0,n-2-i,d,index);

   }
   count++;
}

//get points under given distance (second method : no update data)
void getdot2(float dsum){
   memset(off,0,sizeof(off));  //initializing
   count2=0;
   dtmp2 = 0;
	
   for(int i=0; i<n-2; i++){
      for(int i=1,h=0; i<n; i++){
         if(off[i-1]==0){
			count2++;
            dtmp2 += distance2(h,i);
            h=i;
         }
      }
      if(dtmp2<dsum){
		 count2++;
         break;
      }
	  count2=0;
	  dtmp2=0;
	  off[index2[i]] = 1;
   }		
}

//compare results of method1 and method2 and print the better result
void printd(float dsum){
	float d1=0,d2=0;
	
	getdot1(dsum);  //do method1 and 2
	getdot2(dsum);
	
	//compare the result 1 and 2
	
	if(count>count2){ 				//print method 1 result
		fprintf(fp2,"%.0f %.2f\n",dsum,dtmp);
		for(int j=1,h=0; j<count; j++){
			if(x[h]!=100){
				d1 += distance(h,j);
				fprintf(fp2,"%d %d\n",x[h],y[h]);
				h=j;
			}
		}
		fprintf(fp2,"%d %d\n",x2[n-1],y2[n-1]);
	}
	else{							//print method 2 result
		fprintf(fp2,"%.0f %.2f\n",dsum,dtmp2);
		for(int j=1,h=0; j<n; j++){
			if(off[j-1]==0){
				d2 += distance2(h,j);
				fprintf(fp2,"%d %d\n",x2[h],y2[h]);
				h=j;
			}
		}
		fprintf(fp2,"%d %d\n",x2[n-1],y2[n-1]);
	}
	fprintf(fp2,"\n");
}

int main(){
   FILE *fp;
   int h;
   char in[1024];
   char out[1024];
   printf("input file name? : ");
   scanf("%s",&in);
   fp = fopen(in,"r");
   if(fp==NULL){
		printf("cannot read file");
		exit(0);
	}
	
   printf("output file name? : ");
   scanf("%s",&out);
   fp2 = fopen(out,"w");
   fscanf(fp,"%d",&n);
   
   //scan input data
   for(int i=0; i<n; i++){
      fscanf(fp,"%d %d",&x2[i],&y2[i]);
   }
   
   //sorting input data
   quicksort(0,n-1,x2,y2);
   
   //calculate distance profit
   for(int i=0; i<n-2; i++){
      index2[i] = i;
      d2[i] = distance2(i,i+1)+distance2(i+1,i+2)-distance2(i,i+2);
   }
   
   //sorting distance data
   quicksort2(0,n-2,d2,index2);
   
   //set different distance sum
	printd(200);
	printd(400);
	printd(600);
    printd(800);
    printd(1000);
    printd(1500);
}