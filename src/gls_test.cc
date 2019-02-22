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
#include "gls/GLS.hpp"
#include "gls/event/ShortestPathEvent.hpp"
#include "gls/selector/ForwardSelector.hpp"

namespace po = boost::program_options;

void displayPathAnimationFrame(Renderer& r, int i) {
  auto state = path->getState(i);
  double* arr_state = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  std::vector<double> vec_state;
  // not pretty, if ompl types are commonly used PathMark should accept them directly
  vec_state.assign(arr_state, arr_state + 2);
  NLinkArm<2> arm(vec_state);

  r.draw(world);
  r.draw(arm);
  r.flush();
}
std::shared_ptr<ompl::geometric::PathGeometric> animating_path;
World animating_world;


/// Displays path
/// \param[in] world the World object containing obstacles
/// \param[in] path OMPL path
void displayPath(World world,
                 std::shared_ptr<ompl::geometric::PathGeometric> path)
{
  // Get rendering capability from PathMark and open window
  Renderer r = visualizer::openVisualizer();
  animating_path = path;
  animating_world = world;
  visualizer::animate(r, displayPathAnimationFrame, path->getStateCount(), 16);

  std::cin.get();
  visualizer::closeVisualizer();
}

bool isPointValid(World w, const ompl::base::State *state) {
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  std::vector<double> vector_state;
  vector_state.assign(values + 2); // 2 dimensions
  return NLinkArm<2>.inCollision(vector_state, w);
}

/// Creates an OMPL state from state values.
/// \param[in] space The ompl space the robot is operating in.
/// \param[in] x The x-coordinate.
/// \param[in] y The y-coorindate.
ompl::base::ScopedState<ompl::base::RealVectorStateSpace>
make_state(const ompl::base::StateSpacePtr space, double x, double y)
{
  ompl::base::ScopedState<ompl::base::RealVectorStateSpace>state(space);
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  values[0] = x;
  values[1] = y;
  return state;
}

/// The main function.
int main(int argc, char *argv[])
{
  std::string graphLocation = "../resources/graphs/graph_400.graphml";
  std::string worldLocation = "../resources/worlds/test.world";

  // Define the state space: R^2
  auto space = std::make_shared<ompl::base::RealVectorStateSpace>(2);
  space->as<ompl::base::RealVectorStateSpace>()->setBounds(0.0, 1.0);
  space->setLongestValidSegmentFraction(0.1 / space->getMaximumExtent());
  space->setup();

  // Space Information
  World world(worldLocation);
  NLinkArm<2> arm();
  std::function<bool(const ompl::base::State*)> isStateValid = std::bind(isPointValid, world, std::placeholders::_1);
  ompl::base::SpaceInformationPtr si(new ompl::base::SpaceInformation(space));
  si->setStateValidityChecker(isStateValid);
  si->setup();

  // Problem Definition
  ompl::base::ProblemDefinitionPtr pdef(new ompl::base::ProblemDefinition(si));
  pdef->addStartState(make_state(space, source[0], source[1])); // TO DO: HOW TO GET START LOCATION FROM GRAPH?
  pdef->setGoalState(make_state(space, target[0], target[1]));

  // Setup planner
  gls::GLS planner(si);
  planner.setConnectionRadius(0.04);
  planner.setRoadmapFilename(graphLocation);

  auto event = std::make_shared<gls::event::ShortestPathEvent>();
  auto selector = std::make_shared<gls::selector::ForwardSelector>();
  planner.setEvent(event);
  planner.setSelector(selector);

  planner.setup();
  planner.setProblemDefinition(pdef);

  // Solve the motion planning problem
  ompl::base::PlannerStatus status;
  status = planner.solve(ompl::base::plannerNonTerminatingCondition());

  // Obtain required data if plan was successful
  if (status == ompl::base::PlannerStatus::EXACT_SOLUTION)
  {
    // Display path and specify path size
    auto path = std::dynamic_pointer_cast<ompl::geometric::PathGeometric>(pdef->getSolutionPath());
    std::cout << "Solution Path Cost: " << planner.getBestPathCost() << std::endl;
    displayPath(world, path);
    return 0;
  }

  return -1;
}
