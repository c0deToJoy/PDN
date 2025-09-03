/*
  This is a small program to demonstrate gcov. It
  can take no arguments or 1 argument. It prints
  a message if more than one is given. If the argument's
  value is odd it prints out odd, otherwise even.
  The goal is to show you need multiple test cases
  to cover all of the code.

  -richard.m.veras@ou.edu
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  // No arguments passed
  // note: argv[0] is the name of the
  // program, so we always have at least
  // 1 value in argv.
  if (argc == 1+0)
    printf("I need more arguments!\n");
  // If we have 1 argument
  else if( argc == 1+1 )
    {
      // convert the argument to a value
      int val = atoi(argv[1]);
      if ( val % 2 == 0)
	printf("Even!\n");
      else
	printf("Odd\n");
    }
  // more than 1 argument
  else
    printf("I need less arguments!\n");
  return 0;
}
