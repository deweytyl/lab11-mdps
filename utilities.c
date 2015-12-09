#include <math.h>
#include "mdp.h"
#include "utilities.h"

double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
	      const unsigned int action)
{
  double eu;   // Expected utility
  int successor;

  eu = 0;

  // Calculate expected utility: sum_{s'} P(s'|s,a)*U(s')

  for (successor = 0 ; successor < p_mdp->numStates ; successor++)
  {
     eu += p_mdp->transitionProb[successor][state][action] * utilities[successor];
  }

  return eu;
}

void calc_meu( const mdp* p_mdp, unsigned int state, const double* utilities,
	       double *meu, unsigned int *action )
{
  // Calculated maximum expected utility (use calc_eu):
  unsigned int i, current_action, num_available_actions;
  unsigned int *available_actions;
  double eu;

  num_available_actions = p_mdp->numAvailableActions[state];
  available_actions = p_mdp->actions[state];

  *meu = -INFINITY;

  for (i = 0 ; i < num_available_actions ; i++)
  {
    current_action = available_actions[i];

    eu = calc_eu(p_mdp, state, utilities, current_action);

    if (eu > *meu)
    {
      *meu = eu;
      *action = current_action;
    }
  }
}
