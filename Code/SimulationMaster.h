// 
// Copyright (C) University College London, 2007-2012, all rights reserved.
// 
// This file is part of HemeLB and is CONFIDENTIAL. You may not work 
// with, install, use, duplicate, modify, redistribute or share this
// file, or any part thereof, other than as allowed by any agreement
// specifically made by you with University College London.
// 

#ifndef HEMELB_SIMULATIONMASTER_H
#define HEMELB_SIMULATIONMASTER_H
#include "lb/lattices/Lattices.h"
#include "extraction/PropertyActor.h"
#include "lb/lb.hpp"
#include "lb/StabilityTester.h"
#include "net/net.h"
#include "steering/ImageSendComponent.h"
#include "steering/SteeringComponent.h"
#include "lb/EntropyTester.h"
#include "lb/iolets/BoundaryValues.h"
#include "util/UnitConverter.h"
#include "configuration/CommandLine.h"
#include "io/PathManager.h"
#include "reporting/Reporter.h"
#include "reporting/Timers.h"
#include "reporting/BuildInfo.h"
#include "lb/IncompressibilityChecker.hpp"
#include "colloids/ColloidController.h"
#include "redblood/CellController.h"
#include "net/phased/StepManager.h"
#include "net/phased/NetConcern.h"
#include "geometry/neighbouring/NeighbouringDataManager.h"
#include "Traits.h"

namespace hemelb
{
  template<class TRAITS = Traits<>>
  class SimulationMaster
  {
    public:
      typedef TRAITS Traits;

      SimulationMaster(hemelb::configuration::CommandLine &options,
                       const hemelb::net::IOCommunicator& ioComms);
      virtual ~SimulationMaster();

      void Abort();

      bool IsCurrentProcTheIOProc();

      int GetProcessorCount();

      void RunSimulation();
      hemelb::lb::SimulationState const * GetState() const
      {
        return simulationState;
      }
      hemelb::util::UnitConverter const & GetUnitConverter() const;
      void Finalise();
#     ifdef HEMELB_DOING_UNITTESTS
      //! Makes it easy to add cell controller without messy input files
      void RegisterActor(net::phased::Concern &concern, net::phased::StepManager::Phase phase)
      {
        stepManager->RegisterIteratedActorSteps(concern, phase);
      }
      //! Access to lattice data for debugging
      hemelb::geometry::LatticeData & GetLatticeData()
      {
        return *latticeData;
      }
      std::shared_ptr<hemelb::net::IteratedAction> GetCellController() {
        return cellController;
      }
#     endif
    protected:

      hemelb::lb::iolets::BoundaryValues* inletValues;
      hemelb::lb::iolets::BoundaryValues* outletValues;
      virtual void DoTimeStep();

      /* The next quantities are protected because they are used by MultiscaleSimulationMaster */
      // Set the lattice type via a build parameter
      typedef typename Traits::Lattice latticeType;
      hemelb::geometry::LatticeData* latticeData;
      hemelb::lb::LBM<Traits>* latticeBoltzmannModel;
      hemelb::geometry::neighbouring::NeighbouringDataManager *neighbouringDataManager;
      const hemelb::net::IOCommunicator& ioComms;

    private:
      void Initialise();
      void SetupReporting(); // set up the reporting file
      unsigned int OutputPeriod(unsigned int frequency);
      void HandleActors();
      void OnUnstableSimulation();
      void WriteLocalImages();
      void GenerateNetworkImages();
      /**
       * Updates the property caches record of which properties need to be calculated
       * and cached on this iteration.
       */
      void RecalculatePropertyRequirements();

      /**
       * Helper method to log simulation parameters related to stability and accuracy
       */
      void LogStabilityReport();

      hemelb::configuration::SimConfig *simConfig;
      hemelb::io::PathManager* fileManager;
      hemelb::reporting::Timers timings;
      hemelb::reporting::Reporter* reporter;
      hemelb::reporting::BuildInfo build_info;
      typedef std::multimap<unsigned long, unsigned long> MapType;

      MapType writtenImagesCompleted;
      MapType networkImagesCompleted;

      hemelb::steering::Network* network;
      hemelb::steering::ImageSendComponent *imageSendCpt;
      hemelb::steering::SteeringComponent* steeringCpt;

      hemelb::lb::SimulationState* simulationState;

      /** Struct containing the configuration of various checkers/testers */
      const hemelb::configuration::SimConfig::MonitoringConfig* monitoringConfig;
      hemelb::lb::StabilityTester<latticeType>* stabilityTester;
      hemelb::lb::EntropyTester<latticeType>* entropyTester;
      /** Actor in charge of checking the maximum density difference across the domain */
      hemelb::lb::IncompressibilityChecker<hemelb::net::PhasedBroadcastRegular<> >* incompressibilityChecker;

      std::shared_ptr<hemelb::net::IteratedAction> cellController;
      hemelb::colloids::ColloidController* colloidController;
      hemelb::net::Net communicationNet;

      const hemelb::util::UnitConverter* unitConverter;

      hemelb::vis::Control* visualisationControl;
      hemelb::extraction::IterableDataSource* propertyDataSource;
      hemelb::extraction::PropertyActor* propertyExtractor;

      hemelb::net::phased::StepManager* stepManager;
      hemelb::net::phased::NetConcern* netConcern;

      unsigned int imagesPerSimulation;
      int steeringSessionId;
      unsigned int imagesPeriod;
      static const hemelb::LatticeTimeStep FORCE_FLUSH_PERIOD = 1000;
  };
}

#include "SimulationMaster.impl.h"
#endif /* HEMELB_SIMULATIONMASTER_H */
