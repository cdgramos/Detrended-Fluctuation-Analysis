#include <stdio.h>
#include <math.h>

#define t 4000
#define boxSize 100

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
    int i = 0;
    float sum = 0;
    float xb = 0;
    int nBox = 0;

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



    PS "Integrating time series..." PE
    for(i=0;i<t;i++){
        y[i]=x[i]-xb;
    }


    PS "Opening document to write integrated series" PE
    fp = fopen("output.txt","w");


    PS "Outputing integrated time series..." PE
    for(i=0;i<t;i++){
        fprintf(fp,"%f\n",y[i]);
    }

    PS "Closing file..." PE
    fclose(fp);

    PS "Calculating number of boxes to compute..." PE
    nBox = t/boxSize;
    IS nBox IE









return 0;
}
