#include "TravMapValidatorXYTHETA.hpp"

#include <ompl/base/SpaceInformation.h>

#include <base/Logging.hpp>

namespace motion_planning_libraries
{

TravMapValidatorXYTHETA::TravMapValidatorXYTHETA(const ompl::base::SpaceInformationPtr& si,
            envire::TraversabilityGrid* trav_grid,
            boost::shared_ptr<TravData> grid_data,
            Config config) : 
            ompl::base::StateValidityChecker(si),
            mpSpaceInformation(si),
            mpTravGrid(trav_grid),
            mpTravData(grid_data),
            mConfig(config), 
            mGridCalc() {
    mGridCalc.setTravGrid(trav_grid, grid_data);
}

TravMapValidatorXYTHETA::~TravMapValidatorXYTHETA() {
}
    
bool TravMapValidatorXYTHETA::isValid(const ompl::base::State* state) const
{  
    const ompl::base::SE2StateSpace::StateType* state_se2 = 
            state->as<ompl::base::SE2StateSpace::StateType>();
        
    // Get current state XYTHETA.    
    double x_grid = state_se2->as<ompl::base::RealVectorStateSpace::StateType>(0)->values[0];
    double y_grid = state_se2->as<ompl::base::RealVectorStateSpace::StateType>(0)->values[1];
    double yaw_grid = state_se2->as<ompl::base::SO2StateSpace::StateType>(1)->value;
    
    mGridCalc.setFootprint(x_grid, y_grid, yaw_grid, 
            ceil(std::max(mConfig.mRobotLengthMinMax.first, mConfig.mRobotLengthMinMax.second) / 
                    (double)mpTravGrid->getScaleX()), 
            ceil(std::max(mConfig.mRobotWidthMinMax.first, mConfig.mRobotWidthMinMax.second) / 
                    (double)mpTravGrid->getScaleY()));
            
    return mGridCalc.isValid();
}

} // end namespace motion_planning_libraries
