#include<stdio.h>

#define t 4000

#define PS printf(
#define PE );printf("\n");
#define NEWLINE printf("\n");

int main(){
    FILE *fp;
    float x[t];
    int i = 0;

    PS "Opening the data file..." PE
    fp = fopen("data01.txt","r");

    PS "Reading records..." PE
    while(i < t){
        fscanf(fp,"%f\n",&x[i]);
        i++;
    }

    PS "Closing file..." PE
    fclose(fp);


    PS "Opening document to write integrated series" PE
    fp = fopen("data01OUTPUT.txt","w");


    PS "Outputing integrated time series..." PE
    for(i=3999;i>=0;i--){
        fprintf(fp,"%f\n",x[i]);
    }

    PS "Closing file..." PE
    fclose(fp);

return 0;
}
