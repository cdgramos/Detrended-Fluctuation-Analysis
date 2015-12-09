#include <stdio.h>
#include <math.h>

#define PS printf(
#define PE );printf("\n");
#define IS printf("%d\n",
#define IE );
#define FS printf("%f\n",
#define FE );

#define NEWLINE printf("\n");

/*
t = number of records
x = records
xb = x average

*/


int countRecordsInFile(FILE *fp){
    int lines = 0;
    char ch;
    while(!feof(fp)){
      ch = fgetc(fp);
      if(ch == '\n')
        lines++;
    }
    return lines;
}


int main(){

    FILE *fp;
    float *x; //input data
    float *y; //integrated data
    int i = 0, j = 0, k = 0; //cicles
    float xb = 0; //input data average
    int nBox = 0; //number of bixes
    float l = 0;
    int nRecords = 0;
    //--------------------
    float sumX = 0.0, sumY = 0.0;
    float sumXY = 0.0, sumXX = 0.0, sumYY = 0.0;
    float slope = 0.0;
    float yOrigin = 0.0;
    float den = 0.0;



    PS "Opening the data file..." PE
    fp = fopen("data01.txt","r");


    PS "Counting number of records..." PE
    nRecords = countRecordsInFile(fp);
    rewind(fp);
    PS "Number of records: %d",nRecords PE

    PS "Allocating memory..." PE
    x = (float *)calloc(nRecords, sizeof(float));
    y = (float *)calloc(nRecords, sizeof(float));
    if (x == NULL || y == NULL){
        PS "Error while allocating memory..." PE
        exit(1);
    }

    PS "Reading records..." PE
    while(i < nRecords){
        fscanf(fp,"%f\n",&x[i]);
        PS "read: %f",x[i] PE
        i++;
    }


    PS "Closing file..." PE
    fclose(fp);

    PS "Calculating average..." PE
    for(i=0;i<nRecords;i++){
        xb+=x[i];
    }
    xb = xb/nRecords;


    PS "Integrating time series | cumulative sums..." PE
    for(i=0;i<nRecords;i++){
    	y[i] = 0; //clearing the value
    	for(j=0;j<=i;j++){
    		y[i] += (x[j] - xb);
    	}
    }


    PS "Opening document to write integrated series" PE
    fp = fopen("output.txt","w");


    PS "Outputing integrated time series..." PE
    for(i=0;i<nRecords;i++){
        fprintf(fp,"%f\n",y[i]);
    }

    PS "Closing file..." PE
    fclose(fp);



    PS "Calculating number and size of the boxes to compute..." PE
    //Number of boxes must be equal to the nuber of elements in each box
    for(i=1;i<nRecords;i++){
    	l = nRecords/i;
    	if(l<i){
    		nBox = i-1;
    		break;
    	}
    }
    PS "Number of boxes: %d",nBox PE
    PS "%d elements will be computed",nBox*nBox PE


    PS "Calculating Detrended Fluctuation Function" PE
    //for each window of values
    int aux = 1;
    int startAt = 0;
    for(i=0;i<3;i++){
        for(j=startAt; j<(startAt+nBox); j++){
            sumX += k; //check if needed additional variable to counr X always from start
            sumY += y[j];
            sumXY += k*y[j];
            sumXX += k*k;
            sumYY += y[j]*y[j];
            //PS "y value: %f    x value: %d  sumXX value: %f",y[j],k,sumXX PE
            //if(aux == 1){
            //    PS "Window %d have records from %d to %d",i,startAt,((i*nBox)+nBox-1) PE
            //    aux = 0;
            //}
            //printf("%d ( %f ),",k,y[j]);


               k++;


        }
        PS " " PE
        startAt = (i*nBox)+nBox;
        //k = 0;
        aux = 1;
        //PS "X: %f Y %f  XY %f  XX %f",sumX,sumY,sumXY,sumXX PE
        //slope = ((sumX*sumY) - (nBox*sumXY)) / ((sumX*sumX) - (nBox*sumXX));
        //yOrigin = (sumY - (slope*sumX)) / nBox;
        den = (nBox * sumXX) - (sumX * sumX);
        slope = ((nBox * sumXY) - (sumX * sumY)) / den;
        yOrigin = ((sumY * sumXX) - (sumX * sumXY)) / den;

        PS "Window %d have a slope of %f and a y intersection of %f",i,slope,yOrigin PE


        sumX = 0.0;
        sumY = 0.0;
        sumXX = 0.0;
        sumYY = 0.0;
        sumXY = 0.0;
        den = 0.0;
        slope = 0.0;
        yOrigin = 0.0;
    }





    PS "Freeing memory..." PE
    free(x);
    free(y);

    PS "Success!" PE
    return 0;
}

