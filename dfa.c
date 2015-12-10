#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PS printf(
#define PE );printf("\n");
#define IS printf("%d\n",
#define IE );
#define FS printf("%f\n",
#define FE );

#define NEWLINE printf("\n");

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

	//general
    FILE *fp;
    float *x; //input data
    float *y; //integrated data
    float *Y; //detrended time series
    int i = 0, j = 0, k = 0; //cicles
    float xb = 0.0; //input data average
    int nBox = 0; //number of bixes
    float l = 0.0;
    int nRecords = 0;
    //used in least squares 
    float sumX = 0.0, sumY = 0.0; 
    float sumXY = 0.0, sumXX = 0.0, sumYY = 0.0; 
    float slope = 0.0; 
    float yOrigin = 0.0; 
    float den = 0.0; 
    int startAt = 0; 
    float *slopeList;
    float *yOriginList;
    //standart deviation
    float *stdDeviationList;
    float mean = 0.0;
    float squareMean = 0.0;
    float stdDev = 0.0;
    //log log
    double logX = 0.0;





    PS "Opening the data file..." PE
    fp = fopen("data01.txt","r");


    PS "Counting number of records..." PE
    nRecords = countRecordsInFile(fp);
    rewind(fp);
    PS "Number of records: %d",nRecords PE

    PS "Allocating memory..." PE
    x = (float *)calloc(nRecords, sizeof(float));
    y = (float *)calloc(nRecords, sizeof(float));
    Y = (float *)calloc(nRecords, sizeof(float));
    if (x == NULL || y == NULL || Y == NULL){
        PS "Error while allocating memory..." PE
        exit(1);
    }

    PS "Reading records and closing file..." PE
    while(i < nRecords){
        fscanf(fp,"%f\n",&x[i]);
        i++;
    }
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


    PS "Allocating memory..." PE
    slopeList = (float *)calloc(nBox, sizeof(float));
    yOriginList = (float *)calloc(nBox, sizeof(float));
    stdDeviationList = (float *)calloc(nBox, sizeof(float));
    if (slopeList == NULL || yOriginList == NULL || stdDeviationList == NULL){
        PS "Error while allocating memory..." PE
        exit(1);
    }



    PS "Calculating Least Mean Squares" PE
    for(i=0;i<nBox;i++){
        for(j=startAt; j<(startAt+nBox); j++){
            sumX += k;
            sumY += y[j];
            sumXY += k*y[j];
            sumXX += k*k;
            sumYY += y[j]*y[j];
            k++;
        }

        startAt = (i*nBox)+nBox;

        den = (nBox * sumXX) - (sumX * sumX);
        slope = ((nBox * sumXY) - (sumX * sumY)) / den;
        yOrigin = ((sumY * sumXX) - (sumX * sumXY)) / den;

        slopeList[i] = slope;
        yOriginList[i] = yOrigin;

        //PS "Window %d have a slope of %f and a y intersection of %f",i,slope,yOrigin PE


        sumX = sumY = sumXX = sumYY = sumXY = 0.0;
    }


    PS "Calculating Detrended Fluctuation Function" PE
    j = k = 0;
    for(i = 0; i<nBox*nBox; i++){

    	Y[i] = y[i] - slopeList[k]*i+yOriginList[k];

    	//go to next window
    	if(j==nBox-1){
    		j=0;
    		k++;
    	}
    	j++;
    }


    PS "Calculating Standart Deviation" PE
    startAt = 0;
    mean = 0;
    squareMean = 0;
    for(i=0;i<nBox;i++){
        for(j=startAt; j<(startAt+nBox); j++){
            mean += Y[j];
            mean /= nBox;
        }
        for(j=startAt; j<(startAt+nBox); j++){
            k = Y[j]-mean;
            k = pow(k,2);
            squareMean += k;
            squareMean /= nBox;
        }
        stdDeviationList[i] = sqrt(squareMean);
        startAt = (i*nBox)+nBox;
        mean = squareMean = 0;
        //PS "Box %d have a Standart Deviation of %f",i,stdDeviationList[i] PE
        stdDev += stdDeviationList[i];
    }
    stdDev /= nBox;
    PS "Average Standart Deviation = %f",stdDev PE


    PS "LOG x LOG Points" PE
    sumX = sumY = sumXX = sumYY = sumXY = 0.0;
    for(i=0;i<nBox;i++){
    	PS "Window: %d X = %lf and Y = %lf",i,log10(i+1),log10((double) stdDeviationList[i]) PE
    	sumX += log10(i+1);
        sumY += log10(stdDeviationList[i]);
        sumXY += log10(i+1)*log10(stdDeviationList[i]);
        sumXX += log10(i+1)*log10(i+1);
        sumYY += log10(stdDeviationList[i])*log10(stdDeviationList[i]);

        den = (nBox * sumXX) - (sumX * sumX);
        slope = ((nBox * sumXY) - (sumX * sumY)) / den;
        yOrigin = ((sumY * sumXX) - (sumX * sumXY)) / den;
    }

    PS "ALPHA = %f",slope PE





    PS "Freeing memory..." PE
    free(x);
    free(y);
    free(Y);
    free(slopeList);
    free(yOriginList);
    free(stdDeviationList);

    PS "The end!" PE
    return 0;
}

