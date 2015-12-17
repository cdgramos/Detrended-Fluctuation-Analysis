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
    double *Y; //detrended time series
    int i = 0, j = 0, k = 0; //cicles
    float xb = 0.0; //input data average
    int nBox = 0; //number of bixes
    float l = 0.0;
    int nRecords = 0;
    int window = 0;
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
    double *stdDeviationList;
    float mean = 0.0;
    float squareMean = 0.0;
    float stdDev = 0.0;
    //log log
    double logX = 0.0;

    int lowerBound = 0.0;

    int *windowSet;
    float *functionWindowSet;
    int windows = 0.0;





    PS "Opening the data file..." PE
    fp = fopen("data01.txt","r");


    PS "Counting number of records..." PE
    nRecords = countRecordsInFile(fp);
    rewind(fp);
    PS "Number of records: %d",nRecords PE

    PS "Allocating memory..." PE
    x = (float *)calloc(nRecords, sizeof(float));
    y = (float *)calloc(nRecords, sizeof(float));
    Y = (double *)calloc(nRecords, sizeof(double));
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


    PS "Writing integrated series to file..." PE
    fp = fopen("o_integrated.txt","w");
    for(i=0;i<nRecords;i++){
        fprintf(fp,"%f\n",y[i]);
    }
    fclose(fp);


    PS "Enter the lower bound for the windows: " PE
    scanf("%d",&lowerBound);


    PS "Enter number of windows to compute: " PE
    scanf("%d",&windows);

    PS "Calculating windows set" PE
    /* Peng et al. 1994 - The set of windows must be equally
    spaced in a logarithmic scale*/
    j = 0;
    windowSet = (int *)calloc(windows, sizeof(int));
    functionWindowSet = (float *)calloc(windows, sizeof(float));
    j = nRecords - lowerBound;
    j = j / windows;
    for(i=0; i<windows; i++){
        windowSet[i] = lowerBound+(i*j);
        PS "BOUND: %d",windowSet[i] PE
    }




    int iter = 0;


    for(iter=0; iter<windows; iter++){

        startAt = 0;
        nBox = (int) nRecords / windowSet[iter];
        window = windowSet[iter];
        PS "Number of boxes: %d",nBox PE
        PS "%d elements will be computed",window*nBox PE


        PS "Allocating memory..." PE
        slopeList = (float *)calloc(nBox, sizeof(float));
        yOriginList = (float *)calloc(nBox, sizeof(float));
        stdDeviationList = (double *)calloc(nBox, sizeof(double));
        if (slopeList == NULL || yOriginList == NULL || stdDeviationList == NULL){
            PS "Error while allocating memory..." PE
            exit(1);
        }


        PS "Calculating Least Mean Squares" PE
        for(i=0;i<nBox;i++){
            for(j=startAt; j<(startAt+window); j++){
                sumX += k;
                sumY += y[j];
                sumXY += k*y[j];
                sumXX += k*k;
                sumYY += y[j]*y[j];
                k++;
            }

            startAt = (i*window)+window;

            den = (window * sumXX) - (sumX * sumX);
            slope = ((window * sumXY) - (sumX * sumY)) / den;
            yOrigin = ((sumY * sumXX) - (sumX * sumXY)) / den;

            slopeList[i] = slope;
            yOriginList[i] = yOrigin;

            //PS "Window %d have a slope of %f and a y intersection of %f",i,slope,yOrigin PE
            sumX = sumY = sumXX = sumYY = sumXY = 0.0;
        }


        PS "Calculating Detrended Fluctuation Function" PE
        j = k = 0;
        for(i = 0; i<nBox*window; i++){

            Y[i] = y[i] - slopeList[k]*i+yOriginList[k];
            //go to next window
            if(j==window-1){
                j=0;
                k++;
            }
            j++;
        }

        /*
        PS "Writing detrended series to file..." PE
        fp = fopen("o_detrended.txt","w");
        for(i=0;i<nBox*window;i++){
            fprintf(fp,"%f\n",Y[i]);
        }
        fclose(fp);
        */


        PS "Calculating Standart Deviation" PE
        startAt = 0;
        mean = 0;
        squareMean = 0;
        stdDev = 0;
        for(i=0;i<nBox;i++){
            for(j=startAt; j<(startAt+window); j++){
                mean += Y[j];
                mean /= window;
            }
            for(j=startAt; j<(startAt+window); j++){
                k = Y[j]-mean;
                k = pow(k,2);
                squareMean += k;
                squareMean /= window;
            }
            stdDeviationList[i] = sqrt(squareMean+0.0);
            //PS "Window %d StdDev %f",i,stdDeviationList[i] PE
            startAt = (i*window)+window;
            mean = squareMean = 0;
            //PS "Box %d have a Standart Deviation of %f",i,stdDeviationList[i] PE
            stdDev += stdDeviationList[i];
        }
        stdDev /= nBox;
        PS "Average Standart Deviation = %f",stdDev PE
        functionWindowSet[iter] = stdDev;

        free(slopeList);
        free(yOriginList);
        free(stdDeviationList);
    }




    PS "LOG x LOG Points" PE
    sumX = sumY = sumXX = sumYY = sumXY = 0.0;
    for(i=0;i<windows;i++){
    	sumX += log10(windowSet[i]);
        sumY += log10(functionWindowSet[i]);
        sumXY += log10(windowSet[i])*log10(functionWindowSet[i]);
        sumXX += log10(windowSet[i])*log10(windowSet[i]);
        sumYY += log10(functionWindowSet[i])*log10(functionWindowSet[i]);

        den = (window * sumXX) - (sumX * sumX);
        slope = ((window * sumXY) - (sumX * sumY)) / den;
        yOrigin = ((sumY * sumXX) - (sumX * sumXY)) / den;
    }

    PS "ALPHA = %f",slope PE




    PS "Freeing memory..." PE
    free(x);
    free(y);
    free(Y);
    free(windowSet);
    free(functionWindowSet);

    PS "The end!" PE
    return 0;
}


