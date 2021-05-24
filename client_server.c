/* National University of Mexico
 * School of Engineering
 * Electrical Engineering Division
 * Operative Systems
 * Coding by Daniel Zarco 
 *
 * */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
int factorial(int number); //Declaring factorial function to explicit form

int main(){
  srand(time(NULL)); //Creating random runtime point ti dont get duplicate numbers
  printf("**** Starting process %d ****\n",getpid()); //Starting process to be Parent process and check parent's pid
  pid_t pidH;
  int pipe_client[2], pipe_server[2], factorial_result = 0; //factorial_result is a global variable to parent and son
  //declaring pipes to stablish comunications with parent and their son
  pipe(pipe_client);
  pipe(pipe_server);
  pidH = fork(); //creating a son

  int m = 0;
  while(m < 10){ //declare a while to iterating at 10 to check requirement
    if( pidH ){
      //Parent execution (CLIENT)
      int  number;
      printf("Parent process %d\n", getpid());
      number = rand() % 11; //Generating random number between 0 and 10
      printf("Sending number of CLIENT for system process [VALUE = %d], Memory location: %p\n",number, (void*)&number);

      close(pipe_server[0]); //Closing server pipe to write
      write(pipe_server[1], &number, sizeof(number)); //write in server pipe and send this value through pipe

      close(pipe_client[1]); //Close pipe client to write
      read(pipe_client[0], &factorial_result, sizeof(factorial_result)); //reading value storage in pipe and puts into factorial result
      //income from server pipe
      printf("Incoming factorial number from System Process [VALUE = %d], Memory location: %p\n\n", factorial_result, (void*)&factorial_result);
    }
    else{
    //Child execution (SERVER)
      int system_number, system_number_f;
      close(pipe_server[1]);//close to read  value and puts into system_number
      read(pipe_server[0], &system_number, sizeof(system_number));//put pipe's value in system_number in server pipe
      printf("Incoming number to SERVER [VALUE = %d], Memory location %p\n", system_number, (void*)&system_number);
      system_number_f = factorial(system_number); //gets factorial of number given in server pipe, passing as parameter system_number value
      printf("Sending System process number to CLIENT (Factorial) [VALUE = %d], Memory location %p\n", system_number_f, (void*)&system_number_f);
      close(pipe_client[0]); //close client pipe to write
      write(pipe_client[1], &system_number_f, sizeof(system_number_f)); //sending factorial value through client pipe_client
    }
    m = m + 1; //increment if counter
  }

  return 0; //if execution is succeful we return an integer
}

int factorial(int number){ //definition of recursive factorial funcion
  if(number == 0 || number == 1){ //base case
    return 1;
  }
  else{
    return (number*factorial(number - 1)); //perform multplication
  }
}
