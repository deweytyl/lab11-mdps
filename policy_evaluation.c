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
 *
 *  Authors
 *    Jerod Weinman (documentation & skeleton)
 *    Daniel NP & Tyler D (implementation)
 */
void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
      double epsilon, double gamma,
      double* utilities)
{
  double *updated_utilities;
  double max_utilities_change, utilities_change, eu;

  int state, num_states, utilities_size;

  num_states = p_mdp->numStates;
  utilities_size = sizeof(double) * num_states;

  updated_utilities = malloc(utilities_size);
  bzero(updated_utilities, utilities_size);

  do
  {
    max_utilities_change = 0;
    
    for ( state = 0 ; state < num_states ; state++ )
    {
      if (p_mdp->terminal[state]) // if this is a terminal state
      {
        // then the utility is just the reward
        updated_utilities[state] = p_mdp->rewards[state];
      }
      else
      {
        // otherwise, it's the reward plus the discounted expected utility
        // of the policy's action
        eu = calc_eu(p_mdp, state, utilities, policy[state]);

        updated_utilities[state] = p_mdp->rewards[state] + gamma * eu;
      }

      // Check if we've found a new max change in utilities
      utilities_change = fabs(updated_utilities[state] - utilities[state]);

      if (utilities_change > max_utilities_change)
      {
        max_utilities_change = utilities_change;
      }
    }

    // Update our utilities
    memcpy(utilities, updated_utilities, utilities_size);

  } while (!(max_utilities_change <= epsilon));

  // Clean up
  free(updated_utilities);
}
