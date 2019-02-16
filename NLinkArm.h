#include "Robot.h"
#ifndef _N_LINK_ARM_H_
template <int N>
class NLinkArm : public Robot {
 public:
 private:
    double mState[N];
};
#endif