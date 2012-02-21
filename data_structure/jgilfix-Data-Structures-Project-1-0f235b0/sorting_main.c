//------------------------------------------------------------
// Sorting_main
//  establish the main (menu driven) program to help with
//  project 1.
//  
//  compile:  gcc -Wall -Werror -O3 sorting.c sorting_main.c -o proj1
//------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include "sorting.h"

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000
#endif

#define MAXFILELEN   (50)   // length of string (path and filename)

// functions

int main (
  int Argc,
  char **Argv)
{
  // main program - menu for functions with using database

  double cstart, cend;
  int Response = 0;
  int Saved = 0;
  int Size = 0;
  double N_Comp = 0;
  double N_Move = 0;
  long *Array = NULL;
  char Filename[MAXFILELEN] = "";

  while (1)
  {
    printf("\n");
    printf("TEST MENU\n");
    printf("1. Load array from file\n");
    printf("2. Save array to file\n");
    printf("3. Shell Sort (Insertion) with Sequence h(k) = 2h(k-1)+1\n");
    printf("4. Improved Bubble Sort with Sequence 2^p3^q\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d",&Response);
    getchar();

    if (Response == 5) // quit program
    {
      if (Array != NULL)
      {
        printf("Removing and deallocating array\n");
	free((void *)Array);
        Array = NULL;
        printf("done!\n");
      }
      return (OK);
    }

    if (Response == 1) // load file
    {
      if (Array != NULL)
      {
        printf("\nRemoving and deallocating array\n");
	free((void *)Array);
        Array = NULL;
        printf("done!\n");
      }
      printf("\nEnter input file (including path): ");
      scanf("%s", Filename);
      Array = Load_File (Filename, &Size);
      if (Size <= 0)
      {
        printf("\nError in inputs, file not loaded..\n");
        if (Array != NULL)
        {
          printf("Removing and deallocating array\n");
	  free((void *)Array);
          Array = NULL;
          printf("done!\n");
        }
      }
      else
      {
        printf("\nLoaded %d long integers\n", Size);
      }
    }

    if (Response == 2) // save file
    {
      if (Array == NULL)
      {
        printf("\nMust load in data from file (option 1) before saving one!\n");
      }
      else
      {
        printf("\nEnter output file (including path): ");
        scanf("%s", Filename);
        Saved = Save_File (Filename, Array, Size);
        if (Saved != Size)
        {
          printf("Error in saving!  Only %d out of %d long integers saved\n",
                      Saved, Size);
        }
        else
        {
          printf("Saved all %d long integers\n", Saved);
        }
      }
    }

    if (    (Response > 2)
         && (Response < 5))
    {
      if (Array == NULL)
      {
        printf("\nMust load in data from file (option 1) before sorting one!\n");
      }
      else
      {
        // initialize time function

        cstart = (double) clock();

        // initialize numbers of comparisons and moves
        N_Comp = 0;
	N_Move = 0;

        switch(Response)
        {
          case 3:
            printf("Sorting by Shell Sort\n");
            Shell_Insertion_Sort (Array, Size, &N_Comp, &N_Move);
            break;
          case 4:
            printf("Sorting by Improved Bubble Sort\n");
            Improved_Bubble_Sort (Array, Size, &N_Comp, &N_Move);
            break;
        }

        // print results: Time, N_Comp, N_Move

        cend = (double) clock();
        printf("\n");
        printf(" Elapsed Time (sec): %f\n", (cend - cstart)/CLOCKS_PER_SEC);
        printf("      # Comparisons: %f\n", N_Comp);
        printf("            # Moves: %f\n", N_Move);
      }
    }
  }  
  return (OK);

} // main()

