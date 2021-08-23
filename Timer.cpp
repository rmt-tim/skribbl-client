#include "Timer.h"

Timer::Timer() : frameTime(0) {

}


void Timer::setFrameTime(Uint32 frameTime) {
    this->frameTime = frameTime;
}

Uint32 Timer::getFrameTime() {
    return frameTime;
}
