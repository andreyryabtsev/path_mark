// Standard C++ libraries
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

// Boost libraries
#include <boost/shared_ptr.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/function.hpp>
#include <boost/program_options.hpp>

// OMPL base libraries
#include <ompl/base/Planner.h>
#include <ompl/base/ProblemDefinition.h>
#include <ompl/base/ScopedState.h>
#include <ompl/base/SpaceInformation.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/geometric/PathGeometric.h>

// OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Custom header files
#include <gls/GLS.hpp>

#include "NLinkArm.h"
#include "Animator.h"
#include "World.h"

#include <cmath>

#define FRAMES 300
static unsigned int DIM;

namespace po = boost::program_options;


// Wrapper around PathMark's inCollisions
bool isPointValid(std::shared_ptr<World> w, const ompl::base::State *state) {
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  std::vector<double> vector_state;
  vector_state.assign(values, values + DIM);
  return !NLinkArm::inCollision(vector_state, *w);
}

// Make an OMPL state in given OMPL space from provided doubles array
ompl::base::ScopedState<ompl::base::RealVectorStateSpace>
make_state(const ompl::base::StateSpacePtr space, std::vector<double> coords)
{
  ompl::base::ScopedState<ompl::base::RealVectorStateSpace>state(space);
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  for (unsigned int i = 0; i < coords.size(); i++) values[i] = coords[i];
  return state;
}

// Plan a {DIM}-link arm in specified world, using provided roadmap path
std::shared_ptr<ompl::geometric::PathGeometric> glsPlan(
    std::shared_ptr<World> world,
    std::string roadmap) {
  auto space = std::make_shared<ompl::base::RealVectorStateSpace>(DIM); // define space
  space->as<ompl::base::RealVectorStateSpace>()->setBounds(-M_PI, M_PI);
  space->setLongestValidSegmentFraction(0.1 / space->getMaximumExtent());
  space->setup();
  std::function<bool(const ompl::base::State*)> isStateValid = std::bind(isPointValid, world, std::placeholders::_1);
  ompl::base::SpaceInformationPtr si(new ompl::base::SpaceInformation(space));
  si->setStateValidityChecker(isStateValid);
  si->setup();
  ompl::base::ProblemDefinitionPtr pdef(new ompl::base::ProblemDefinition(si));
  auto source = world->getStartPosition();
  auto target = world->getTargetPosition();
  pdef->addStartState(make_state(space, source));
  pdef->setGoalState(make_state(space, target));
  gls::GLS planner(si);
  planner.setConnectionRadius(0.2);
  planner.setCollisionCheckResolution(0.01);
  planner.setRoadmap(roadmap);
  auto event = std::make_shared<gls::event::ShortestPathEvent>();
  auto selector = std::make_shared<gls::selector::ForwardSelector>();
  planner.setEvent(event);
  planner.setSelector(selector);
  planner.setup();
  planner.setProblemDefinition(pdef);
  ompl::base::PlannerStatus status;
  status = planner.solve(ompl::base::plannerNonTerminatingCondition());
  if (status == ompl::base::PlannerStatus::EXACT_SOLUTION) {
    std::cout << "Solution Path Cost: " << planner.getBestPathCost() << std::endl;
    return std::dynamic_pointer_cast<ompl::geometric::PathGeometric>(pdef->getSolutionPath());
  }
  else return nullptr;
}

/// The main function.
int main(int argc, char *argv[])
{
  // Read arguments; locate the world and graph that matches requested dimension
  po::options_description desc("2D Map Test Options");
  desc.add_options()("dimension,d", po::value<int>()->default_value(3), "dimension count");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  DIM = vm["dimension"].as<int>();
  std::string worldLocation = "../resources/out/world" + std::to_string(DIM) + ".world";
  std::string graphLocation = "../resources/out/g" + std::to_string(DIM) + "_graph_0.graphml";

  auto world = std::make_shared<World>(worldLocation);
  auto arm = std::make_shared<NLinkArm>(DIM);
  auto path = glsPlan(world, graphLocation);

  if (path == nullptr) {
    std::cout << "No solution found, exiting." << std::endl;
    return 0;
  }
  Animator animator(world, arm, path);
  animator.play(150);
  std::cout << "Animating solution path, press any key to exit...";
  std::cin.get();
  return 0;
}
