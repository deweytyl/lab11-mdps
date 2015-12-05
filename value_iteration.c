#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"


void value_iteration( const mdp* p_mdp, double epsilon, double gamma,
          double *utilities)
{
  // Run value iteration!

  double *updated_utilities;
  double delta;

  int state, num_states, utilities_size;

  num_states = p_mdp->numStates;
  utilities_size = sizeof(double) * num_states;

  updated_utilities = malloc(utilities_size);
  bzero(updated_utilities, utilities_size);

  do 
  {
    memcpy(utilities, updated_utilities, utilities_size);
    delta = 0;

    for ( state = 0; state < num_states ; state++ )
    {
      double meu;
      unsigned int action;

      if (p_mdp->terminal[state]) // if this is a terminal state
      {
        // then the utility should just be the reward
        updated_utilities[state] = p_mdp->rewards[state]
      }
      else
      {
        calc_meu(
          p_mdp,
          state,
          utilities,
          &meu,
          &action
        );

        updated_utilities[state] = p_mdp->rewards[state] + gamma * meu;
      }

      if (fabs(updated_utilities[state] - utilities[state]) > delta)
      {
        delta = fabs(updated_utilities[state] - utilities[state]);
      }
    }
  } while(!(delta < (epsilon * (1 - gamma) / gamma)));

  // Clean up
  free(updated_utilities);
}


/*
 * Main: value_iteration gamma epsilon mdpfile
 *
 * Runs value_iteration algorithm using gamma and with max
 * error of epsilon on utilities of states using MDP in mdpfile.
 *
 * Author: Jerod Weinman
 */
int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    fprintf(stderr,"Usage: %s gamma epsilon mdpfile\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Read and process configurations
  double gamma, epsilon;
  char* endptr; // String End Location for number parsing
  mdp *p_mdp;

  // Read gamma, the discount factor, as a double
  gamma = strtod(argv[1], &endptr);

  if ( (endptr - argv[1]) < strlen(argv[1]) )
  {
    fprintf(stderr, "%s: Illegal non-numeric value in argument gamma=%s\n",
            argv[0],argv[1]);
      exit(EXIT_FAILURE);
  }

  // Read epsilon, maximum allowable state utility error, as a double
  epsilon = strtod(argv[2], &endptr); 

  if ( (endptr - argv[2]) < strlen(argv[2]) )
  {
    fprintf(stderr, "%s: Illegal non-numeric value in argument epsilon=%s\n",
            argv[0],argv[2]);
      exit(EXIT_FAILURE);
  }

  // Read the MDP file (exits with message if error)
  p_mdp = mdp_read(argv[3]);

  if (NULL == p_mdp)
  { // mdp_read prints a message
    exit(EXIT_FAILURE);
  }

  // Allocate utility array
  double * utilities;

  utilities = malloc( sizeof(double) * p_mdp->numStates );

  // Verify we have memory for utility array
  if (NULL == utilities)
  {
    fprintf(stderr,
      "%s: Unable to allocate utilities (%s)",
      argv[0],
      strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Run value iteration!
  value_iteration( p_mdp, epsilon, gamma, utilities );

  // Print utilities
  unsigned int state;
  for ( state=0 ; state < p_mdp->numStates ; state++)
    printf("%f\n",utilities[state]);
  
  // Clean up
  free (utilities);
  mdp_free(p_mdp);

  exit(EXIT_SUCCESS);
}


