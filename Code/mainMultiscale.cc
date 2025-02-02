
// This file is part of HemeLB and is Copyright (C)
// the HemeLB team and/or their institutions, as detailed in the
// file AUTHORS. This software is provided under the terms of the
// license in the file LICENSE.

/* Specifying the use of 'real' MPWide */
#include <MPWide.h>

#include "configuration/CommandLine.h"
#include "multiscale/MultiscaleSimulationMaster.h"
#include "multiscale/mpwide/MPWideIntercommunicator.h"

int main(int argc, char *argv[])
{
  // Bring up MPI
  hemelb::net::MpiEnvironment mpi(argc, argv);
  hemelb::logging::Logger::Init();

  try
  {
    hemelb::net::MpiCommunicator commWorld = hemelb::net::MpiCommunicator::World();

    // Start the debugger (no-op if HEMELB_BUILD_DEBUGGER is OFF)
    hemelb::debug::Debugger::Init(argv[0], commWorld);

    hemelb::net::IOCommunicator hemelbCommunicator(commWorld);

    try
    {
      // Parse command line
      hemelb::configuration::CommandLine options = hemelb::configuration::CommandLine(argc, argv);

      // Prepare some multiscale/MPWide stuff

      // Work out the location of the input file.
      std::string inputFile = options.GetInputFile();
      int sl = inputFile.find_last_of("/");
      std::string mpwideConfigDir = inputFile.substr(0, sl + 1);

      std::cout << inputFile << " " << mpwideConfigDir << " " << sl << std::endl;

      // Create some necessary buffers.
      std::map<std::string, bool> lbOrchestration;
      lbOrchestration["boundary1_pressure"] = true;
      lbOrchestration["boundary2_pressure"] = true;
      lbOrchestration["boundary1_velocity"] = true;
      lbOrchestration["boundary2_velocity"] = true;

      std::map<std::string, double> sharedValueBuffer;

      // TODO The MPWide config file should be read from the HemeLB XML config file!
      // Create the intercommunicator
      hemelb::multiscale::MPWideIntercommunicator intercomms(hemelbCommunicator.OnIORank(),
                                                             sharedValueBuffer,
                                                             lbOrchestration,
                                                             mpwideConfigDir.append("MPWSettings.cfg"));

      //TODO: Add an IntercommunicatorImplementation?
      hemelb::logging::Logger::Log<hemelb::logging::Info, hemelb::logging::OnePerCore>("Constructing MultiscaleSimulationMaster()");
      hemelb::multiscale::MultiscaleSimulationMaster<hemelb::multiscale::MPWideIntercommunicator> lMaster(options,
                                                                                                          intercomms);

      hemelb::logging::Logger::Log<hemelb::logging::Info, hemelb::logging::OnePerCore>("Runing simulation()");
      lMaster.RunSimulation();
    }
    // Interpose this catch to print usage before propagating the error.
    catch (hemelb::configuration::CommandLine::OptionError& e)
    {
      hemelb::logging::Logger::Log<hemelb::logging::Critical, hemelb::logging::Singleton>(hemelb::configuration::CommandLine::GetUsage());
      throw;
    }
  }
  catch (std::exception& e)
  {
    hemelb::logging::Logger::Log<hemelb::logging::Critical, hemelb::logging::OnePerCore>(e.what());
    mpi.Abort(-1);
  }
  // MPI gets finalised by MpiEnv's d'tor.
  return (0);
}
