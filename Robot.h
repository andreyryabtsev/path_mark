#ifndef _ROBOT_H_
namespace robot {
	class Robot {
     public:
        void assignWorld(const World &world);
     protected:
        const World world_;
	}
}
#endif