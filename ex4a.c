//ex4a
// This program it's a simple program, that calculate polynomial from user
// The program will calculate every part of the polynomial.The calculate will be done with threads. This program will terminate by the input "done".
// Guy Cohen 206463606

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define INPUT_SIZE 30
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
void askForPolynom();
void saperatedPoly(char str[]);
void* myThreadFun(void *var);
char firstPoly[20];
char secondPoly[20];
char thirdPoly[20];
char forthPoly[20];
int x=0;
int solution=0;

int main() {
    printf("PLEASE ENTER \"POLYNOM, VAL\"\n");//first message
    askForPolynom();
    return 0;
}

void askForPolynom(){
    char input[INPUT_SIZE];
    while (fgets(input, INPUT_SIZE, stdin)) {// while input != "done"
        input[strlen(input) - 1] = '\0';// "enter char"
        solution=0;
        memset(forthPoly, 0, strlen(forthPoly));//set array with 0
        memset(thirdPoly, 0, strlen(thirdPoly));//set array with 0
        memset(secondPoly, 0, strlen(secondPoly));//set array with 0
        memset(firstPoly, 0, strlen(firstPoly));//set array with 0
        saperatedPoly(input);// now I can send each polynomial to his right thread.
        int num_of_threads=1;
        int place=0;
        char **str_poly=(char**)malloc(sizeof(char*)*5);// str_poly is arr of polynomials that is suitable to the user input. place is the index to fill it.
        if(firstPoly[0]!=0){
            str_poly[place]=firstPoly;
            place++;
            num_of_threads++;
        }
        if(secondPoly[0]!=0){
            str_poly[place]=secondPoly;
            place++;
            num_of_threads++;
        }
        if(thirdPoly[0]!=0){
            str_poly[place]=thirdPoly;
            place++;
            num_of_threads++;
        }
        if(forthPoly[0]!=0){
            str_poly[place]=forthPoly;
            place++;
            num_of_threads++;
        }
        if(place==0){// the input is wrong
            printf("PLEASE ENTER \"POLYNOM, VAL\"\n");
            free(str_poly);
            str_poly=NULL;
            continue;
        }

        pthread_t tid[place+1];// an array of threads identifiers, needed by pthread_join() later
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        for(int i= 0; i<place; i++){
            pthread_create(&tid[i], &attr, myThreadFun, str_poly[i]);//create new threads, each executing myThreadFun
        }
        for(int i= 0; i<place; i++){// wait for all threads to terminate.
            if(pthread_join(tid[i], NULL)!=0){
                perror("pthread_join");
                exit(EXIT_FAILURE);
            }
        }
        free(str_poly);
        str_poly=NULL;

        printf("solution: %d\n", solution);
        printf("\nPLEASE ENTER \"POLYNOM, VAL\"\n");// print pwd like a real shell
    }
}

void saperatedPoly(char str[]){
    if(strcmp(str, "done")==0){// end program
        exit(1);
    }
    if(str[0]==' ' || str[0]=='*'||str[strlen(str)-1]==' ' || str[0]==0 || str[strlen(str)-1]==',' || str[strlen(str)-1]=='*'|| str[0]==','){
        printf("\nerror with your input!!\n");
        return;
    }
    if((strstr(str, "**")!=NULL)|| (strstr(str, "^^")!=NULL)||(strstr(str, "++")!=NULL)){//extreme case
        printf("\nerror with your input!!\n");
        return;
    }
    int polyCounter=1;// count how many polynomials we have, we must have only one.
    int spaceCounter=0;// count spaces.
    int commaCounter=0;// count ','.
    int indexOfSpace=0;// save the index of input.
    for(int i=0; str[i]!=0;i++){
        if( !((str[i]==' ') || (str[i]=='^') || (str[i]=='*')  ||(str[i]=='x') || (str[i]=='+')  || (str[i]==',')  ||(str[i]>=48 && str[i]<=57) )){// if one of chars is different form those
            printf("\nerror with your input!!\n");
            return;
        }
        if(i>0 && i< strlen(str)-1){// extreme cases like input with ** or *+ or *^ or +* or ++...
            if((str[i]=='*'||str[i]=='+' || str[i]=='^' ) && (str[i-1]=='*' || str[i-1]=='+'|| str[i-1]=='^') ){
                printf("\nerror with your input!!\n");
                return;
            }
        }
        if(str[i]==' '){
            if(spaceCounter!=0){// you can have only one space per input
                printf("\ntoo many spaces in your input!!\n");
                return;
            }
            if(str[i-1]!=','){// if you have space in your input it must have ',' before
                printf("\nproblem with your input!!\n");
                return;
            }
            indexOfSpace=i-1;
            spaceCounter++;
        }
        if(str[i]==','){// coma counter must be 1
            commaCounter++;
        }
        if(str[i]=='+'){
            polyCounter++;// how many polynomials we have
        }
    }
    if(commaCounter==0 || commaCounter>1 || spaceCounter==0){//  you must have one comma in your input and only one space.
        printf("\nproblem with your input!!\n");
        return;
    }
    int j=indexOfSpace+2;
    char x_val[10];// might to be x
    int x_val_ind=0;
    while ((str[j]>=48 && str[j]<=57)){
        x_val[x_val_ind]=str[j];// copy x value into array x_val
        x_val_ind++;
        j++;
    }
    if(j < strlen(str)-1  ){
        printf("\nproblem with your input!!\n");
        return;
    }
//    now I know the input is ok.
    x= atoi(x_val);// x is getting is real val
    const char plus[2] = "+";
    const char comaa[2]=",";
    char polynomal[20];
    char *token;
    token = strtok(str, comaa);// input is separated for 2 parts polynomial and x
    strcpy(polynomal, token);//polynomial
    token = strtok(str, plus);
    while( token != NULL ) {// each polynomial to his right array
        if(polyCounter==4){
            strcpy(forthPoly,token);
        }
        if(polyCounter==3){
            strcpy(thirdPoly,token);
        }
        if(polyCounter==2){
            strcpy(secondPoly,token);
        }
        if(polyCounter==1){
            strcpy(firstPoly,token);
        }
        polyCounter--;
        token = strtok(NULL, plus);
    }
// now I have polynomials in arrays.
}

void *myThreadFun(void *var){
    int flag=1;

    char *arr=(char *)var;
    int base=x;
    int value=1;
    char temp0[10] = {'\0'};
    memset(temp0, 0, 10);
    char temp1[10] = {'\0'};
    memset(temp1, 0, 10);
    char temp2[10] = {'\0'};
    memset(temp2, 0, 10);
    int len= strlen(arr);
    if((strstr(arr, "^")!=NULL) && (strstr(arr, "*")!=NULL)) {// case of [val]*[base]^[exp]
        int i=0;
        while (1) {
            if(arr[i]== '*'){
                break;
            }
            if(arr[i]=='^'){
                flag=0;
                break;
            }
            temp0[i]=arr[i];
            i++;
        }
        i++;
        int j=0;
        while (1) {
            if(arr[i]== '^'){
                break;
            }
            if(arr[i]=='*'){
                break;
            }
            temp1[j]=arr[i];
            j++;
            i++;
        }
        j=0;
        i++;
        while (arr[i]!=0){
            temp2[j]=arr[i];
            j++;
            i++;
        }
        if(flag==1) {// in case we get for example [4*x^2]
            int exp = atoi(temp2);
            while (exp != 0) {
                value *= base;
                --exp;
            }
            int leftVal = atoi(temp0);
            pthread_mutex_lock(&lock);
            solution += (value * leftVal);
            pthread_mutex_unlock(&lock);
        }
        else{//flag==0// in case we get for example [x^2*4]
            int exp = atoi(temp1);
            while (exp != 0) {
                value *= base;
                --exp;
            }
            int rightVal = atoi(temp2);
            pthread_mutex_lock(&lock);
            solution += (value * rightVal);
            pthread_mutex_unlock(&lock);
        }
    }
    else if(strstr(arr, "^")!=NULL){// only exponent case
        for(int i=0, j=0; i< len ; i++){
            if(arr[i]=='^'){
                arr[i]=' ';
                break;
            }
            arr[i]=' ';
        }
        int exp= atoi(arr);
        while(exp!=0){
            value*=base;
            --exp;
        }
        pthread_mutex_lock(&lock);
        solution+=value;
        pthread_mutex_unlock(&lock);
   }
    else if(strstr(arr, "*")!=NULL){// only multiple case
        int i=0;
        while(1){
            if(arr[i]=='*'){
                break;
            }
            temp0[i]=arr[i];
            i++;
        }
        i++;
        int j=0;
        while(arr[i]!=0){
            temp1[j]=arr[i];
            i++;
            j++;
        }
        if(strcmp(temp0, "x")==0){
            pthread_mutex_lock(&lock);
            solution+=(x*atoi(temp1));
            pthread_mutex_unlock(&lock);
        }
        else{
            pthread_mutex_lock(&lock);
            solution+=(x*atoi(temp0));
            pthread_mutex_unlock(&lock);
        }
    }
    else if(strcmp(arr, "x")==0){
        pthread_mutex_lock(&lock);
        solution+=x;
        pthread_mutex_unlock(&lock);
    }
    else{
        int val= atoi(arr);
        pthread_mutex_lock(&lock);
        solution+=val;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}