#include "time.hpp"

Time::Time():camera{0.0f,0.0f},physics{0.0f,0.0f},last(0.0f){}

void Time::handle(bool paused, bool slowmo, float global) {
	camera.delta = global - camera.current;
	camera.current = global;
	physics.delta = paused ? 0.0f : (slowmo ? 0.025f : 1.0f) * camera.delta;
	physics.current += physics.delta;
}


