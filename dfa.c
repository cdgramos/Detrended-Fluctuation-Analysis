#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "auxiliar.h"

#define minValues 16 //mininum records to compute Detredended Fluctuation Analysis ! Imppotencyical Value

int main(){
    FILE *fp; // general purpose file pointer

    int totRecords = 0; // number of data records
    int totWindows = 0; // number of windows to compute
    int minWindow = 0; // lower bound of the windows = (2 * degree of the fit function) + 2
    int maxWindow = 0; // upper bound of the windows = totRecords / 4
    int i = 0; // aux var
    int j = 0; // aux var
    int k = 0; // aux var
    int z = 0; // aux var
    long size = 0; // aux var
    int nWindow = 0; //window number to compute

    int degreeFit = 1; // degree of the fit function
    double boxratio = pow(2.0, 1.0/8.0);; // the scale of the windows that will be computed related to each other

    int totWindowsCertainSize = 0; // number of possible windows of a certain size

    double xMean = 0.0; //raw data average

    double *x; // store input / raw data
    double *y; // integrated time series y = integrate(x)
    double *Y; // detrended function
    double *YY; // YY VALUES

    double *avgStdDeviation;
    double *windowSizes; // store window sizes


    reg_Linear r;

    int potency = 1;

    double stat;
    double sumFn = 0.0;
    double sumAux = 0.0;



    printf("Opening the raw data file...\n");
    fp = fopen("hurst_050.txt","r");  //H = 0.50

    printf("Counting number of records...\n");
    totRecords = countRecordsInFile(fp);
    rewind(fp);
    printf( "\tNumber of records: %d\n",totRecords);
    if(totRecords <= minValues){
    	printf( "\t\tError: Insuficient data\n");
    	exit(1);
    }


    printf("Allocating memory...\n");
    x = (double *) calloc(totRecords, sizeof(double));
    y = (double *) calloc(totRecords, sizeof(double));
    Y = (double *) calloc(totRecords, sizeof(double));
    YY = (double *) calloc(totRecords, sizeof(double));

    if(x == NULL || y == NULL || Y == NULL || YY == NULL){
        printf( "\t\tError: allocating memory\n");
        exit(1);
    }

    printf("Reading raw data and closing the raw data file...\n");
    while(i < totRecords){
        fscanf(fp,"%lf\n",&x[i]);
        i++;
    }


    printf("Calculating average...\n");
    xMean = average(x, totRecords);

    printf("Integrating time series | cumulative sums...\n");
    integratingTimeSeries(x,y,xMean,totRecords);

    printf("Defining lower and upper bounds\n");
    minWindow = (2 * degreeFit) + 2; // (2 * degree of the fit function) + 2
    maxWindow = totRecords / 4; // total records / 4
    printf("\tBOUNDS: min-> %d \t max-> %d\n",minWindow,maxWindow);

    totWindows = log10(maxWindow / (double)minWindow) / log10(boxratio) + 1.5;

    printf("Allocating memory to store the window sizes and mean standard deviation...\n");
    windowSizes = (double *) calloc(totWindows, sizeof(double));
    avgStdDeviation = (double *) calloc(totWindows, sizeof(double));
    if(avgStdDeviation == NULL){
        printf( "\t\tError: allocating memory\n");
        exit(1);
    }

    printf("Calculating aproximate number of windows to compute...\n");

    windowSizes[0] = minWindow;

    for (i = 0; windowSizes[i-1] < maxWindow; potency++){
        size = minWindow * pow(boxratio, potency);
            if (size > windowSizes[i-1]){
                windowSizes[i] = size;
                i++;
            }
        if(i > totWindows){
            windowSizes[i-1] = maxWindow;
        }
    }

    totWindows = i;

    printf( "\t%d windows will be computed\n",totWindows);

    // for(i=0;i<totWindows;i++) printf("%d\tRS:%.lf\n",i,windowSizes[i]);

    for (i=0;i<totRecords;i++){// Continuous values in X
        x[i] = i;
    }

    /// --- DFA START ---
    for (i = 0; i < totWindows; i++) {    /// For each window size

        stat = (int)(totRecords / windowSizes[i]) * windowSizes[i];
         avgStdDeviation[i] = 0.0;
         sumFn = 0.0;
         sumAux = 0.0;

        for (j = 0; j <= totRecords-windowSizes[i]; j += windowSizes[i]){  /// For each window of a certain size
            r = reg_LeastSquareMeans(x,y,j,j+windowSizes[i]);

            for(z = j; z <= j+windowSizes[i]; z++){  /// For each values in a certain window
                sumAux = y[z] - r.m - (r.b * z);
                sumFn += pow( sumAux ,2);
            }
        }

        // Average of the values of a certain window size
        avgStdDeviation[i] = sumFn/stat;

    }
    /// --- DFA END ---

    //FILE *fd;
    //fd = fopen("data.txt","w");
    for (i = 0; i < totWindows; i++){ /// log x log scales
        //fprintf(fd,"%lf %lf\n", log10((double)windowSizes[i]), log10((double)avgStdDeviation[i])/2);
        windowSizes[i] = log10((double)windowSizes[i]);
        avgStdDeviation[i] = log10((double)avgStdDeviation[i])/2;
    }
    //fclose(fd);

    r = reg_LeastSquareMeans(windowSizes,avgStdDeviation,0,totWindows);

    printf("\n_______________________________\n\n");
    printf( "\tHurst = %.4lf\n_______________________________\n\n",r.b);

    printf("Freeing the memory...\n");
    fclose(fp);
    free(YY);
    free(windowSizes);
    free(x);
    free(y);
    free(Y);
    free(avgStdDeviation);

    printf("The end!\n");
    return 0;
}
