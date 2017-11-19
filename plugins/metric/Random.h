#ifndef RANDOMMETRIC_H
#define RANDOMMETRIC_H

#include <tulip/DoubleProperty.h>

/** \addtogroup metric */

/** This plugin assigns random values to nodes or edges or both.
 * The values are between 0 and 1.
 */
class RandomMetric : public tlp::DoubleAlgorithm {

public:
  PLUGININFORMATION("Random metric", "David Auber", "04/10/2001",
                    "Assigns random values to nodes and edges.", "1.1", "Misc")
  RandomMetric(const tlp::PluginContext *context);
  bool run() override;
};
#endif // RANDOMMETRIC_H
