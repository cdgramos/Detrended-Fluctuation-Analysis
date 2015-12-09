#include <stdio.h>
#include <math.h>

#define t 4000 //elements in file

#define PS printf(
#define PE );printf("\n");
#define IS printf("%d\n",
#define IE );

#define NEWLINE printf("\n");

// PS "..." PE



/*
t = number of records
x = records
xb = x average

*/

int main(){

    FILE *fp;
    float x[t];
    float y[t];
    int i = 0, j = 0;
    float sum = 0;
    float xb = 0;
    int nBox = 0;
    float l;

    PS "Opening the data file..." PE
    fp = fopen("data01.txt","r");

    PS "Reading records..." PE
    while(i < t){
        fscanf(fp,"%f\n",&x[i]);
        i++;
    }

    PS "Closing file..." PE
    fclose(fp);

    PS "Calculating average..." PE
    for(i=0;i<t;i++){
        sum+=x[i];
    }
    xb = sum/t;


    PS "Integrating time series | comulative sums..." PE
    for(i=0;i<t;i++){
    	y[i] = 0; //clearing the value
    	for(j=0;j<=i;j++){
    		y[i] += (x[j] - xb);
    	}
    }


    PS "Opening document to write integrated series" PE
    fp = fopen("output.txt","w");


    PS "Outputing integrated time series..." PE
    for(i=0;i<t;i++){
        fprintf(fp,"%f\n",y[i]);
    }

    PS "Closing file..." PE
    fclose(fp);

    PS "Calculating number and size of the boxes to compute..." PE
    //Number of boxes must be equal to the nuber of elements in each box
    for(i=1;i<t;i++){
    	l = t/i;
    	if(l<i){
    		nBox = i-1;
    		break;
    	}
    }
    PS "Number of boxes: %d",nBox PE
    PS "%d elements will be computed",nBox*nBox PE

    PS "Calculating Detrended Fluctuation Function" PE


    //hummm importar funções de memória dinâmica








return 0;
}
