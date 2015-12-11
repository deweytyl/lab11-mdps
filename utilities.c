#include <math.h>
#include "mdp.h"
#include "utilities.h"

/*  Procedure
 *    calc_eu
 *
 *  Purpose
 *    Calculate the expected utility of a state and action in an MDP
 *
 *  Parameters
 *   p_mdp
 *   state
 *   utilities
 *   action
 *
 *  Produces
 *   eu
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struc
 *    0 <= state < p_mdp->numStates
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Practica
 *    The following are not preconditions, per se, but lend themselves
 *    to meaningful results:
 *      p_mdp->terminal[state] = 0 (No meaningful actions in a terminal state)
 *      action belongs to the array p_mdp->actions[state]
 *
 *  Postconditions
 *    eu is the average utility of subsequent states that arise from taking the
 *    specified action in the given state:
 *       eu = sum_{s'=0..p_mdp->numStates} P(s'|state,action) * utilities(s')
 *    where P(s'|s,a) represents the transition probability in the MDP
 *
 *  Authors
 *    Jerod Weinman (documentation & skeleton)
 *    Daniel NP & Tyler D (implementation)
 */
double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
	      const unsigned int action)
{
  double eu;   // Expected utility
  int successor;

  eu = 0;

  // Calculate expected utility: sum_{s'} P(s'|s,a)*U(s')

  // Go through every successor state
  for (successor = 0 ; successor < p_mdp->numStates ; successor++)
  {
     eu += p_mdp->transitionProb[successor][state][action] * utilities[successor];
  }

  return eu;
}

/*  Procedure
 *    calc_meu
 *
 *  Purpose
 *    Calculate the action of maximum expected utility of a state in an MDP
 *
 *  Parameters
 *   p_mdp
 *   state
 *   utilities
 *   meu
 *   action
 *
 *  Produces
 *   [Nothing.]
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struc
 *    0 <= state < p_mdp->numStates
 *    utilities points to a valid array of length p_mdp->numStates
 *    meu != NULL
 *    action != NULL
 *
 *  Postconditions
 *    *meu is the maximum expected utility of state in p_mdp:
 *        max_{a} EU(state,a) 
 *      = max_{a} sum_{s'=0..p_mdp->numStates} P(s'|state,a) * 
 *                                                  utilities(s')
 *    where P(s'|s,a) represents the transition probability in the MDP
 *    
 *    *action is a value of a that yields *meu: argmax_{a} EU(state,a) 
 *
 *  Authors
 *    Jerod Weinman (documentation & skeleton)
 *    Daniel NP & Tyler D (implementation)
 */
void calc_meu( const mdp* p_mdp, unsigned int state, const double* utilities,
	       double *meu, unsigned int *action )
{
  // Calculated maximum expected utility (use calc_eu):
  unsigned int i, current_action, num_available_actions, max_action;
  unsigned int *available_actions;
  double eu, max_eu;

  num_available_actions = p_mdp->numAvailableActions[state];
  available_actions = p_mdp->actions[state];

  max_eu = -INFINITY;
  max_action = 0;

  if (num_available_actions == 0) {
    *meu = 0; // max utility of no actions is zero
    *action = 0;
    return;
  }

  for (i = 0 ; i < num_available_actions ; i++)
  {
    current_action = available_actions[i];

    eu = calc_eu(p_mdp, state, utilities, current_action);

    if (eu > max_eu)
    {
      max_eu = eu;
      max_action = current_action;
    }
  }

  *meu = max_eu;
  *action = max_action;
}
