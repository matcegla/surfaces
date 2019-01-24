#ifndef SURFACES_TIME_HPP
#define SURFACES_TIME_HPP

struct Clock {
	float delta;
	float current;
};

struct Time {
	Clock camera;
	Clock physics;
	float last;
	Time();
	void handle(bool paused, bool slowmo, float global);
};

#endif //SURFACES_TIME_HPP
