#ifndef SURFACES_TIME_HPP
#define SURFACES_TIME_HPP

struct Time {
	struct Clocks {
		float camera;
		float physics;
	};
	Clocks delta;
	Clocks current;
	Clocks last;
	Time();
	void handle(bool paused, float glfwtime);
};

#endif //SURFACES_TIME_HPP
