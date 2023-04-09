//ex4b
// This program it's a simple program, that calculate polynomial from user
// The program will calculate every part of the polynomial.the calculate will be done with writing the solution to a shared memory. This program will terminate by the input "done".
// Guy Cohen 206463606

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

void askForPolynomial();
void separatedPoly(char str[]);
int solvePoly(char* var);

char firstPoly[10];
char secondPoly[10];
char thirdPoly[10];
char forthPoly[10];
int x=0;

int main() {
    printf("PLEASE ENTER \"POLYNOM, VAL\"\n");//first message
    askForPolynomial();
    return 0;
}

void askForPolynomial(){
    char input[30];
    while (fgets(input, 30, stdin)) {// while input != "done"
        input[strlen(input) - 1] = '\0';// "enter char"
        memset(forthPoly, 0, strlen(forthPoly));//set array with 0
        memset(thirdPoly, 0, strlen(thirdPoly));//set array with 0
        memset(secondPoly, 0, strlen(secondPoly));//set array with 0
        memset(firstPoly, 0, strlen(firstPoly));//set array with 0
        separatedPoly(input);// now I can send each polynomial to his right thread.
        char **str_poly=(char**)malloc(sizeof(char*)*5);// array size of how many words you have.
        int place=0;// str_poly is arr of polynomials that is suitable to the user input. place is the index to fill it.
        if(firstPoly[0]!=0){
            str_poly[place]=firstPoly;
            place++;
        }
        if(secondPoly[0]!=0){
            str_poly[place]=secondPoly;
            place++;
        }
        if(thirdPoly[0]!=0){
            str_poly[place]=thirdPoly;
            place++;
        }
        if(forthPoly[0]!=0){
            str_poly[place]=forthPoly;
            place++;
        }
        if(place==0){// the input is wrong
            printf("PLEASE ENTER \"POLYNOM, VAL\"\n");
            free(str_poly);
            str_poly=NULL;
            continue;
        }
        pid_t tid[place];// pid array size of how many polynomials you have

        int key=ftok("/tmp",'x');// key
        /* allocate a shared memory segment  */
        int shm_id=shmget(key, sizeof (int)*place, IPC_CREAT | IPC_EXCL| 0600);
        if(shm_id==-1){
            perror("failed to get memory");
            free(str_poly);
            str_poly=NULL;
            continue;
        }
        /* attach the shared memory segment to our process's address space. */
        int *solu=(int *) shmat(shm_id,NULL, 0);
        if(solu<0){
            perror("failed to attach memory");
            free(str_poly);
            str_poly=NULL;
            continue;
        }
        for(int i=0; i<place; i++){
            tid[i]=fork();
            // I am going to malloc the object, and it will give me the id of it.(ID of the shared memory segment).
            if(tid[i]<0){
                perror("fork failed");
                free(str_poly);
                str_poly=NULL;
                exit(0);
            }
            if(tid[i]==0){// son processes
                solu[i]=solvePoly(str_poly[i]);
                exit(0);
            }
        }
        for(int i=0; i<place; i++){// father wait
                wait(NULL);
        }
        int sum=0;// will be our solution in the end
        for(int i=0; i<place; i++){
            sum+= solu[i];
        }
        /* detach the shared memory segment from our process's address space. */
        shmdt(solu);
        if(shmctl(shm_id, IPC_RMID, NULL)==-1)
        {
            perror("error");
            free(str_poly);
            str_poly=NULL;
            continue;
        }
        printf("\n solution: %d \n", sum);//print solution
        free(str_poly);
        str_poly=NULL;
        printf("PLEASE ENTER \"POLYNOM, VAL\"\n");// next input please..
    }
}

void separatedPoly(char str[]){
    if(strcmp(str, "done")==0){// end program
        exit(1);
    }
    if(str[0]==' ' || str[0]=='*'||str[strlen(str)-1]==' ' || str[0]==0 || str[strlen(str)-1]==',' || str[strlen(str)-1]=='*'|| str[0]==','){
        printf("\nerror with your input!!\n");
        return;
    }
    if(strstr(str, "**")!=NULL){//extreme case
        printf("\nerror with your input!!\n");
        return;
    }
    int polyCounter=1;// count how many polynomials we have, we must have only one.
    int spaceCounter=0;// count spaces.
    int commaCounter=0;// count ','.
    int indexOfSpace=0;// save the index of input.
    for(int i=0; str[i]!=0;i++){
        if(  !((str[i]==' ') || (str[i]=='^') || (str[i]=='*')  ||(str[i]=='x') || (str[i]=='+')  || (str[i]==',')  ||(str[i]>=48 && str[i]<=57) )){// if one of chars is different form those the input is wrong.
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

int solvePoly(char *var){
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
    int sum=0;
    if((strstr(arr, "^")!=NULL) && (strstr(arr, "*")!=NULL)) {//in case we have [val]*[base]^[exp]
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
            sum= (value * leftVal);
        }
        else{//flag==0// in case we get for example [x^2*4]
            int exp = atoi(temp1);
            while (exp != 0) {
                value *= base;
                --exp;
            }
            int rightVal = atoi(temp2);
            sum = (value * rightVal);
        }
    }
    else if(strstr(arr, "^")!=NULL){
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
        sum=value;
    }
    else if(strstr(arr, "*")!=NULL){
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
            sum=(x*atoi(temp1));
        }
        else{
            sum=(x*atoi(temp0));
        }
    }
    else if(strcmp(arr, "x")==0){
        sum=x;
    }
    else{
        int val= atoi(arr);
        sum=val;
    }
    return sum;
}