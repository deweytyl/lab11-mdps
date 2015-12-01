#include "mdp.h"
#include "utilities.h"

double calc_eu( const mdp*  p_mdp, unsigned int state, const double* utilities,
	      const unsigned int action)
{
  double eu = 0;   // Expected utility

  // Calculate expected utility: sum_{s'} P(s'|s,a)*U(s')
  
  return eu;
}

void calc_meu( const mdp* p_mdp, unsigned int state, const double* utilities,
	       double *meu, unsigned int *action )
{
  // Calculated maximum expected utility (use calc_eu):

}
