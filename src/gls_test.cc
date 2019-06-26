#include <iostream>

// OMPL base libraries
#include <ompl/base/Planner.h>
#include <ompl/base/ProblemDefinition.h>
#include <ompl/base/ScopedState.h>
#include <ompl/base/SpaceInformation.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include <ompl/geometric/PathGeometric.h>

// GLS and PathMark headers
#include <gls/GLS.hpp>
#include "NLinkArm.h"
#include "Animator.h"
#include "World.h"

// Option parsing for main()
#include <boost/program_options.hpp>
namespace po = boost::program_options;


// Assign supplied robot to given state; return true iff no collision with given world
bool isPointValid(std::shared_ptr<World> w, std::shared_ptr<Robot> robot, const ompl::base::State *state) {
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  std::vector<double> vector_state;
  vector_state.assign(values, values + robot->dimension());
  robot->setState(vector_state);
  return !robot->inCollision(*w);
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

// Plan with a given robot in specified world, using provided roadmap path
std::shared_ptr<ompl::geometric::PathGeometric> glsPlan(
    std::shared_ptr<World> world,
    std::shared_ptr<Robot> robot,
    std::string roadmap) {
  auto space = std::make_shared<ompl::base::RealVectorStateSpace>(robot->dimension());
  space->as<ompl::base::RealVectorStateSpace>()->setBounds(-M_PI, M_PI);
  space->setLongestValidSegmentFraction(0.1 / space->getMaximumExtent());
  space->setup();
  std::function<bool(const ompl::base::State*)> isStateValid = std::bind(isPointValid, world, robot, std::placeholders::_1);
  ompl::base::SpaceInformationPtr si(new ompl::base::SpaceInformation(space));
  si->setStateValidityChecker(isStateValid);
  si->setup();
  ompl::base::ProblemDefinitionPtr pdef(new ompl::base::ProblemDefinition(si));
  pdef->addStartState(make_state(space, world->getStartPosition()));
  pdef->setGoalState(make_state(space, world->getTargetPosition()));
  gls::GLS planner(si);
  planner.setConnectionRadius(0.2);
  planner.setCollisionCheckResolution(0.01);
  planner.setRoadmap(roadmap);
  planner.setEvent(std::make_shared<gls::event::ShortestPathEvent>());
  planner.setSelector(std::make_shared<gls::selector::ForwardSelector>());
  planner.setup();
  planner.setProblemDefinition(pdef);
  ompl::base::PlannerStatus status = planner.solve(ompl::base::plannerNonTerminatingCondition());
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
  int dim = vm["dimension"].as<int>();
  std::string worldLocation = "../resources/out/world" + std::to_string(dim) + ".world";
  std::string graphLocation = "../resources/out/g" + std::to_string(dim) + "_graph_0.graphml";

  // Load the world, create an arm, get the path using GLS
  auto world = std::make_shared<World>(worldLocation);
  auto arm = std::make_shared<NLinkArm>(dim);
  auto path = glsPlan(world, arm, graphLocation);

  if (path == nullptr) {
    std::cout << "No solution found, exiting." << std::endl;
    return 0;
  }
  // Animate the arm in this world along the acquired path
  Animator animator(world, arm, path);
  // 120 frames at ~60fps
  animator.play(120);
  std::cout << "Animating solution path, press any key to exit...";
  std::cin.get();
  return 0;
}
