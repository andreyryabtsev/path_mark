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
#include <gls/event/ShortestPathEvent.hpp>
#include <gls/selector/ForwardSelector.hpp>

#include "NLinkArm.h"
#include "Visualizer.h"
#include "World.h"

#include <cmath>

#define FRAMES 300
static unsigned int DIM = 4;

namespace po = boost::program_options;


std::shared_ptr<ompl::geometric::PathGeometric> animating_path;
World animating_world;
void displayPathAnimationFrame(Renderer& r, int i) {
  auto state = animating_path->getState(i);
  double* arr_state = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  std::vector<double> vec_state;
  // not pretty, if ompl types are commonly used PathMark should accept them directly
  vec_state.assign(arr_state, arr_state + DIM);
  NLinkArm arm(DIM, vec_state);

  r.draw(animating_world);
  r.draw(arm);
  r.flush();
}


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
  animating_path->interpolate(FRAMES);
  visualizer::animate(r, displayPathAnimationFrame, FRAMES, 16);

  std::cout << "anim started, frame count: "<< path->getStateCount() << std::endl;
  std::cin.get();
  visualizer::closeVisualizer();
}

bool isPointValid(World w, const ompl::base::State *state) {
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  std::vector<double> vector_state;
  vector_state.assign(values, values + DIM); // 2 dimensions
  // bool m = !NLinkArm::inCollision(vector_state, w); // debug output spam
  // std::cout << "checking (" << vector_state[0];
  // for (int i = 1; i < DIM; i++) std::cout << ", " << vector_state[i];
  // std::cout << ")" << " >> " << m << std::endl;
  return !NLinkArm::inCollision(vector_state, w);
}

/// Creates an OMPL state from state values.
/// \param[in] space The ompl space the robot is operating in.
/// \param[in] x The x-coordinate.
/// \param[in] y The y-coorindate.
ompl::base::ScopedState<ompl::base::RealVectorStateSpace>
make_state(const ompl::base::StateSpacePtr space, std::vector<double> coords)
{
  ompl::base::ScopedState<ompl::base::RealVectorStateSpace>state(space);
  double* values = state->as<ompl::base::RealVectorStateSpace::StateType>()->values;
  for (unsigned int i = 0; i < coords.size(); i++) values[i] = coords[i];
  return state;
}

/// The main function.
int main(int argc, char *argv[])
{
  po::options_description desc("2D Map Test Options");
  desc.add_options()
      ("dimension,d", po::value<int>()->default_value(3), "dimension count")
  ;

  // Read arguments
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  DIM = vm["dimension"].as<int>();


  std::stringstream ssw, ssg;
  ssw << "../resources/out/world" << DIM << ".world";
  ssg << "../resources/out/g" << DIM << "_graph_0.graphml";
  std::string worldLocation = ssw.str();
  std::string graphLocation = ssg.str();

  // Define the state space: R^2
  auto space = std::make_shared<ompl::base::RealVectorStateSpace>(DIM);
  space->as<ompl::base::RealVectorStateSpace>()->setBounds(-M_PI, M_PI);
  space->setLongestValidSegmentFraction(0.1 / space->getMaximumExtent());
  space->setup();

  // Space Information
  World world(worldLocation);
  NLinkArm arm(DIM);
  std::function<bool(const ompl::base::State*)> isStateValid = std::bind(isPointValid, world, std::placeholders::_1);
  ompl::base::SpaceInformationPtr si(new ompl::base::SpaceInformation(space));
  si->setStateValidityChecker(isStateValid);
  si->setup();

  // Problem Definition
  ompl::base::ProblemDefinitionPtr pdef(new ompl::base::ProblemDefinition(si));
  auto source = world.getStartPosition();
  auto target = world.getTargetPosition();
  pdef->addStartState(make_state(space, source));
  pdef->setGoalState(make_state(space, target));

  //std::cout << "FROM (" << source[0] << ", " << source[1] << ") TO (" << target[0] << ", " << target[1] << ")" << std::endl;

  // Setup planner
  gls::GLS planner(si);
  planner.setConnectionRadius(0.04);
  planner.setCollisionCheckResolution(0.01);
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
  } else {
    std::cout << "NO SOLUTION" << std::endl;
  }

  return -1;
}
