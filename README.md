# Calculate-polynomials-in-a-distributed-manner

Calculate polynomials in a distributed manner
Authored by Guy Cohen

This program it’s a simple program based on input of polynom from user.
the program separate the polynomial input into small polynomials, for every one of it,  it will calculate by the suitable case.
The program can to get a infinite quantity of commands with out limit.
(input must to be like: "POLYNOM, VAL", or the user will get an error).
If the user will decide to finish the program he have to put the word "done" , otherwise it will continue .

==Description ex4a.c==
will calculate the polynomial with threads.
==Description ex4b.c==
will calculate the polynomial with writing the solution of each small plynomial to a shared memory.

Program DATABASE:
1.	 The main data base of this program, it is an array of characters, in size of 30, it gets the input from user.  The program will analyze this array with “saperatedPoly()”  function. the program will organize the input  into arrays and will work on it. 
2.	  firstPoly[10], secondPoly[10], thirdPoly[10], forthPoly[10] = Those are global arrays kind of char. They will be able to hand small parts of the whole polynomial. I separated each polynomial by ‘+’ char and copy it to one of those arrays.
3.	 str_poly= This is an array that is going to point the small pieces of the whole polynomial. each one of those pieces is going to be calculated by suitable function.
In ex4a program it will calculate with threads , with function “myThreadFun”.
In ex4b it will calculate with writing to a shared memory by the function “solvePoly”.


functions:
1.	void separatedPoly(char str[]): This function appears as it is for ex4a and ex4b programs. This function gets the input containing polynomial and the value x. The goal is to check if the input content is OK, and then, if so, the function will separate the polynomial  by the '+' character. Each part of it will be copied to a small global char array. In addition this function will update the global value x, which will be useful for calculating the solution.



2.	  void askForPolynomial():This function works in the format of an infinite loop. In each iteration the loop will request input from the user and save it in a dedicated array called "input". In a function there is a call to the "separatedPoly" function, after which the polynomial is saved as small arrays defined globally. The function will create an array of pointers called "str_poly" that will create pointers for each polynomial piece. For ex4a program, the function will use the "<pthread.h>" directory. It is send’s each part of the polynomial to the "myThreadFun" function that will perform the appropriate calculation. The result will be read from a common global variable and printed on the screen.
For the ex4b program, the function will generate son processes that will perform the calculation of the polynomial parts. Each son will calculate a segment in the polynomial with using the function "solvePoly" and write the result to the shared memory segment.The result will counted from the shared memory and will printed.

3.	  myThreadFun(void *var) , solvePoly(char* var):   "myThreadFunc" is the function that calculates the polynomial segments for ex4a program. It takes care of updating the global variable of the solution. "SolvePolly" works similarly. Here the result is re-written into a shared memory segment through the child processes.


==Program Files==
Ex4a.c , Ex4b.c
==How to compile?==
compile: gcc ex4a.c -o e4a -pthread
run: ./ex4a
compile: gcc ex4b.c -o ex4b
run: ./ex4b


==Input:==
1.	String from the user, in the following format: "POLYNOM, VAL".  

==Output:==
1.	The solution of the polynomial.





