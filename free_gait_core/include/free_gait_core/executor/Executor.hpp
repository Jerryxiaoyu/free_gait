/*
 * Executor.hpp
 *
 *  Created on: Oct 20, 2015
 *      Author: Péter Fankhauser
 *   Institute: ETH Zurich, Autonomous Systems Lab
 */

#pragma once

#include <free_gait_core/executor/AdapterBase.hpp>
#include "free_gait_core/TypeDefs.hpp"
#include "free_gait_core/executor/State.hpp"
#include "free_gait_core/step/StepQueue.hpp"
#include "free_gait_core/step/StepCompleter.hpp"

// STD
#include <memory>
#include <thread>

// Boost
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace free_gait {

class Executor
{
 public:
  Executor(std::shared_ptr<StepCompleter> completer, std::shared_ptr<AdapterBase> adapter,
           std::shared_ptr<State> state);
  virtual ~Executor();

  /*!
   * Type definitions.
   */
  typedef boost::recursive_mutex Mutex;
  typedef boost::recursive_mutex::scoped_lock Lock;

  bool initialize();
  bool isInitialized() const;
  Mutex& getMutex();

  /*!
   * Advance in time
   * @param dt the time step to advance [s].
   * @return true if successful, false otherwise.
   */
  bool advance(double dt);

  /*!
   * Checks status of robot to make sure if advancement is safe to continue.
   * @return true if true execution active, false if execution stopped
   *         because of unexpected event/state.
   */
  bool getExecutionStatus();

  void reset();

  const StepQueue& getQueue() const;

  /*!
   * Note: To make usage of this method thread-safe, use the lock on mutex
   * of this class provided by the getMutex() method.
   * @return
   */
  StepQueue& getQueue();

  const State& getState() const;
  const AdapterBase& getAdapter() const;

 private:
  bool completeCurrentStep();
  bool initializeStateWithRobot();
  bool updateStateWithMeasurements();
  void updateExecutionStatus();
  bool writeIgnoreContact();
  bool writeIgnoreForPoseAdaptation();
  bool writeSupportLegs();
  bool writeSurfaceNormals();
  bool writeLegMotion();
  bool writeTorsoMotion();

  Mutex mutex_;
  boost::atomic<bool> isInitialized_;
  boost::atomic<bool> executionStatus_;
  StepQueue queue_;
  std::shared_ptr<StepCompleter> completer_;
  std::shared_ptr<AdapterBase> adapter_;
  std::shared_ptr<State> state_;
};

} /* namespace free_gait */