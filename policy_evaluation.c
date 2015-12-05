#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/*  Procedure
 *    policy_evaluation
 *
 *  Purpose
 *    Iteratively estimate state utilities under a fixed policy
 *
 *  Parameters
 *   policy
 *   p_mdp
 *   epsilon
 *   gamma
 *   utilities
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    policy points to a valid array of length p_mdp->numStates
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    p_mdp is a pointer to a valid, complete mdp
 *    epsilon > 0
 *    0 < gamma < 1
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Postconditions
 *    utilities[s] has been updated according to the simplified Bellman update
 *    so that no update is larger than epsilon
 */
void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
      double epsilon, double gamma,
      double* utilities)
{
  double *updated_utilities;
  double delta;

  int state, num_states, utilities_size;

  num_states = p_mdp->numStates;
  utilities_size = sizeof(double) * num_states;

  updated_utilities = malloc(utilities_size);
  bzero(updated_utilities, utilities_size);

  do
  {
    delta = 0;
    for ( state = 0 ; state < num_states ; state++ )
    {
      if (p_mdp->terminal[state]) // if this is a terminal state
      {
        // then the utility is just the reward
        updated_utilities[state] = p_mdp->rewards[state];
      }
      else
      {
        double eu;
        eu = calc_eu(p_mdp, state, utilities, policy[state]);

        updated_utilities = p_mdp->rewards[state] + gamma * eu;
      }

      if (fabs(updated_utilities[state] - utilities[state]) > delta)
      {
        delta = fabs(updated_utilities[state] - utilities[state]);
      }
    }

    memcpy(utilities, updated_utilities, utilities_size);

  } while (!(delta <= epsilon));

  // Clean up
  free(updated_utilities);
}
