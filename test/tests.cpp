// Copyright 2024 Podyachikh Mikhail

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
 public:
  MOCK_METHOD(void, Timeout, (), (override));
};

class MyTimedDoorTest : public ::testing::Test {
 protected:
  TimedDoor door;
  Timer timer;
  MockTimerClient *timerClient;
 public:
  MyTimedDoorTest() : door(1000), timer() {}
 protected:
  void SetUp() override {
    timerClient = new MockTimerClient();
  }
  void TearDown() override {
    delete timerClient;
  }
};

TEST_F(MyTimedDoorTest, MyTimedDoorStartsLocked) {
  EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(MyTimedDoorTest, MyTimedDoorLocksProperly) {
  door.unlock();
  door.lock();
  EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(MyTimedDoorTest, MyTimedDoorUnlocksProperly) {
  door.unlock();
  EXPECT_TRUE(door.isDoorOpened());
}

TEST_F(MyTimedDoorTest, MyTimedDoorThrowsErrorOnThrowStateWhenOpen) {
  door.unlock();
  EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST_F(MyTimedDoorTest, MyTimedDoorDoesNotThrowWhenLocked) {
  EXPECT_NO_THROW(door.throwState());
}

TEST_F(MyTimedDoorTest, MyTimedDoorThrowsErrorOnUnlockTwice) {
  door.unlock();
  EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST_F(MyTimedDoorTest, MyTimedDoorThrowsErrorOnLockTwice) {
  EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(MyTimedDoorTest, TimerCallsOnTimeout) {
  EXPECT_CALL(*timerClient, Timeout()).Times(1);
  timer.tregister(2000, timerClient);
}

TEST_F(MyTimedDoorTest, DoorTimerAdapterThrowsErrorOnUnlock) {
  DoorTimerAdapter adapter(door);
  door.unlock();
  EXPECT_THROW(adapter.Timeout(), std::runtime_error);
}

TEST_F(MyTimedDoorTest, DoorTimerAdapterDoesNotThrowOnLock) {
  DoorTimerAdapter adapter(door);
  EXPECT_NO_THROW(adapter.Timeout());
}
