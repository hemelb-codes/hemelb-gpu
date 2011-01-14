#ifndef HEMELB_TOPOLOGY_CONCRETETOPOLOGYMANAGER_H
#define HEMELB_TOPOLOGY_CONCRETETOPOLOGYMANAGER_H

#include "topology/BaseTopologyManager.h"

namespace hemelb
{
  namespace topology
  {
    class TopologyManager : public BaseTopologyManager
    {
      public:
        TopologyManager(NetworkTopology* bNetworkTopology, bool *oSuccess);
    };
  }
}

#endif /* HEMELB_TOPOLOGY_CONCRETETOPOLOGYMANAGER_H */
