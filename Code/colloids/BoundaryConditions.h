
// This file is part of HemeLB and is Copyright (C)
// the HemeLB team and/or their institutions, as detailed in the
// file AUTHORS. This software is provided under the terms of the
// license in the file LICENSE.

#ifndef HEMELB_COLLOIDS_BOUNDARYCONDITIONS_H
#define HEMELB_COLLOIDS_BOUNDARYCONDITIONS_H

#include "io/xml/XmlAbstractionLayer.h"
#include "units.h"
#include <vector>
#include <map>
#include "colloids/Particle.h"
#include "geometry/LatticeData.h"

namespace hemelb
{
  namespace colloids
  {
    /** base class for all representations of a boundary condition */
    class BoundaryCondition
    {
      public:
        /** applies the boundary condition by directly modifying the particle
         *  returns false if the particle must now be deleted, true otherwise
         */
        virtual bool DoSomethingToParticle(Particle&, const std::vector<LatticePosition>) =0;
        virtual const std::vector<Particle> CreateNewParticles() =0;
      protected:
        virtual ~BoundaryCondition() {};
    };

    typedef BoundaryCondition*(*BoundaryConditionFactory_Create)(io::xml::Element& xml);

    template <class TClass>
    class BoundaryConditionFactory
    {
      public:
        static BoundaryCondition* Create(io::xml::Element& xml)
        {
          return TClass::ReadFromXml(xml);
        };
    };

    /** container for all body forces currently active in the simulation */
    class BoundaryConditions
    {
      public:
        /** factory method - gets initial values from xml configuration file */
        static void InitBoundaryConditions(
                            const geometry::LatticeData* const latticeData,
                            io::xml::Document& xml);

        static void AddBoundaryCondition(
                            const std::string name,
                            const BoundaryCondition* const);

        /** allows all registered boundary conditions to do something to this particle
            returns false if the particle should be deleted, true otherwise
         */
        static bool DoSomeThingsToParticle(
                            const LatticeTimeStep currentTimestep,
                            Particle& particle);

        /** allows all registered boundary conditions to create new particles */
        static const std::vector<Particle> CreateNewParticles();

      private:
        /**
         * stores the details of all known body forces
         * the value type must be a base class pointer
         * as only pointers are type-compatible in C++
         */
        static std::vector<BoundaryCondition* > boundaryConditionsWall;
        static std::vector<BoundaryCondition* > boundaryConditionsInlet;
        static std::vector<BoundaryCondition* > boundaryConditionsOutlet;

        const static geometry::LatticeData* latticeData;
    };
  }
}
#endif /* HEMELB_COLLOIDS_BOUNDARYCONDITIONS_H */
