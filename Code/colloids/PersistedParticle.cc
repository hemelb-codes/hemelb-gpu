// 
// Copyright (C) University College London, 2007-2012, all rights reserved.
// 
// This file is part of HemeLB and is CONFIDENTIAL. You may not work 
// with, install, use, duplicate, modify, redistribute or share this
// file, or any part thereof, other than as allowed by any agreement
// specifically made by you with University College London.
// 

#include "colloids/PersistedParticle.h"

namespace hemelb
{
  namespace colloids
  {
    PersistedParticle::PersistedParticle(io::xml::XmlAbstractionLayer& xml)
    {
      // assume we are currently at a <SubgridParticle> node

      bool ok = true;
      ok &= xml.GetUnsignedLongValue("ParticleId", particleId);
      ok &= xml.GetDoubleValueAndConvert("InputRadiusA0", smallRadius_a0);
      ok &= xml.GetDoubleValueAndConvert("HydrostaticRadiusAh", largeRadius_ah);
      ok &= xml.GetDoubleValue("Mass", mass);
      ok &= xml.GetLatticePosition("initialPosition", globalPosition);
      lastCheckpointTimestep = 0;
      markedForDeletionTimestep = BIG_NUMBER2;
    };
  }
}
