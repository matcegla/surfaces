#include "time.hpp"

Time::Time():delta{0.0f,0.0f},current{0.0f,0.0f},last{0.0f,0.0f}{}

void Time::handle(bool paused, bool slowmo, float currentCameraPrim) {
	last = current;
	current.camera = currentCameraPrim;
	delta.camera = current.camera - last.camera;
	delta.physics = paused ? 0.0f : (slowmo ? 0.025f : 1.0f) * delta.camera;
	current.physics += delta.physics;
}


