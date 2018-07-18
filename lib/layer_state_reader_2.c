#include "crkbd.h"

#define L_BASE 0
#define L_OREORE 2
#define L_LOWER 8
#define L_RAISE 16
#define L_ADJUST 65536
#define L_ADJUST_TRI 65560

char layer_state_str[40];

char *read_layer_state(void)
{
  switch (layer_state)
  {
    case L_BASE:
       snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Default");
       break;
    case L_OREORE:
       snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Oreore");
       break;
    case L_RAISE:
       snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Raise");
       break;
    case L_LOWER:
       snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Lower");
       break;
    case L_ADJUST:
    case L_ADJUST_TRI:
       snprintf(layer_state_str, sizeof(layer_state_str), "Layer: Adjust");
       break;
    default:
       snprintf(layer_state_str,sizeof(layer_state_str), "Layer: Undef-%ld", layer_state);
  }

  return layer_state_str;
}