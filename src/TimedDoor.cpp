// Copyright 2024 Podyachikh Mikhail
#include <thread> // NOLINT [build/c++11]
#include <stdexcept>
#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor &door) : door(door) {
}

void DoorTimerAdapter::Timeout() {
  door.throwState();
}

TimedDoor::TimedDoor(int timeout) : isOpened(false), iTimeout(timeout) {
  adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
  return isOpened;
}

void TimedDoor::unlock() {
  if (isOpened) {
    throw std::logic_error("Door already unlocked");
  }
  isOpened = true;
}

void TimedDoor::lock() {
  if (!isOpened) {
    throw std::logic_error("Door already locked");
  }
  isOpened = false;
}

int TimedDoor::getTimeOut() const {
  return iTimeout;
}

void TimedDoor::throwState() {
  if (isOpened) {
    throw std::runtime_error("Door was open for too long");
  }
}

void Timer::sleep(int time) {
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void Timer::tregister(int time, TimerClient *timerClient) {
  client = timerClient;
  sleep(time);
  client->Timeout();
  client = nullptr;
}
