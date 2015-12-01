#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "policy_evaluation.h"
#include "mdp.h"


/*  Procedure
 *    policy_iteration
 *
 *  Purpose
 *    Optimize policy by alternating evaluation and improvement steps
 *
 *  Parameters
 *   p_mdp
 *   epsilon
 *   gamma
 *   policy
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    p_mdp is a pointer to a valid, complete mdp
 *    policy points to a valid array of length p_mdp->numStates
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    epsilon > 0
 *    0 < gamma < 1
 *
 *  Postconditions
 *    policy[s] contains the optimal policy for the given mdp
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 */			
void policy_iteration( const mdp* p_mdp, double epsilon, double gamma,
		      unsigned int *policy)
{
  
}

/*  Procedure
 *    randomize_policy
 *
 *  Purpose
 *    Initialize policy to random actions
 *
 *  Parameters
 *   p_mdp
 *   policy
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    p_mdp is a pointer to a valid, complete mdp
 *    policy points to a valid array of length p_mdp->numStates
 *
 *  Postconditions
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    when p_mdp->numAvailableActions[s] > 0.
 */
void randomize_policy( const mdp* p_mdp, unsigned int* policy)
{
  srandom(42);
  unsigned int state;
  unsigned int action;

  for ( state=0 ; state < p_mdp->numStates ; state++)
  {
    if (p_mdp->numAvailableActions[state] > 0)
    {
      action = (unsigned int)(random() % (p_mdp->numAvailableActions[state]));
      policy[state] = p_mdp->actions[state][action];
    }
  }

}

/*
 * Main: policy_iteration gamma epsilon mdpfile
 *
 * Runs policy_iteration algorithm using gamma and policy_evaluation with max
 * changes of epsilon on MDP in mdpfile.
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

  if ( (endptr - argv[1])/sizeof(char) < strlen(argv[1]) )
  {
    fprintf(stderr, "%s: Illegal non-numeric value in argument gamma=%s\n",
            argv[0],argv[1]);
      exit(EXIT_FAILURE);
  }

  // Read epsilon, maximum allowable state utility error, as a double
  epsilon = strtod(argv[2], &endptr); 

  if ( (endptr - argv[2])/sizeof(char) < strlen(argv[2]) )
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

  // Allocate policy array
  unsigned int * policy;

  policy = malloc( sizeof(unsigned int) * p_mdp->numStates );

  if (NULL == policy)
  {
    fprintf(stderr,
	    "%s: Unable to allocate policy (%s)",
	    argv[0],
	    strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Initialize random policy
  randomize_policy(p_mdp, policy);

  // Run policy iteration!
  policy_iteration ( p_mdp, epsilon, gamma, policy);

  // Print policies
  unsigned int state;
  for ( state=0 ; state < p_mdp->numStates ; state++)
    if (p_mdp->numAvailableActions[state])
      printf("%u\n",policy[state]);
    else
      printf("0\n",policy[state]);

  // Clean up
  free (policy);
  mdp_free(p_mdp);

}



