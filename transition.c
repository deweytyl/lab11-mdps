/* start.c
 *
 * A small test program to load and print info on an MDP file
 *
 * Jerod Weinman
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <inttypes.h>

#include "mdp.h"



////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    fprintf(stderr,"Usage: %s mdpfile state action\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Pointer for the MDP struct
  mdp *p_mdp;

  unsigned int state, action;

  state = (unsigned int) strtol(argv[2], NULL, 10);
  action = (unsigned int) strtol(argv[3], NULL, 10);

  // Read the MDP file
  p_mdp = mdp_read(argv[1]);

  if (NULL == p_mdp)
    // mdp_read prints a message upon failure
    exit(EXIT_FAILURE);
    
	// Print header for transition table
  printf("s'\tP(s' | s=%u,a=%u\n)", state, action);
  
  unsigned int successor; // Loop variable over successor states
  double transition; // The transition probability for each successor state

  for (successor=0 ; successor < p_mdp->numStates ; successor++)
  {
    transition = p_mdp->transitionProb[successor][state][action];
    printf("%u\t%.2lf\n", successor, transition);
  }

  // Clean up
  mdp_free(p_mdp);

}