#include "Creature_set.cpp"
#include "Genome.hpp"

Creature Creature_set::get_fit_creature(Data_set data){
  int xb_max = 5;
  int yb_max = 5;
  int xd_max = 1;
  int xd_max = 1;
  double max_fitness;
  Creature best_creature;
  for(int xb = 0; xb <= xb_max; xb++){
    for(int yb = 0; yb <= yb_max; yb++){
      for(int xd = 0; xd <= xd_max; xd++){
	for(int yd = 0; yd <= yd_max; yd++){
	  Creature c(xb,yb,xd,yd);
	  
	}
      }
    }
  }
}
