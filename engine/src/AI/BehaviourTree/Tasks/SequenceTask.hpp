#pragma once

#include "Task.hpp"
#include <vector>

namespace Engine {

class SequenceTask : public Task {
  private:
    int taskIndex = 0;
    std::vector<Task *> m_Tasks;

  public:
    SequenceTask(const std::vector<Task *> tasks) : m_Tasks(tasks) {}

    TaskStatus update(std::shared_ptr<Blackboard> blackboard) override {
        auto task = m_Tasks[taskIndex];

        TaskStatus taskStatus = task->update(blackboard);
        if (taskStatus == TaskStatus::Fail) {
            return TaskStatus::Fail;
        }

        if (taskStatus == TaskStatus::Running) {
            return TaskStatus::Running;
        }

        if (taskIndex < m_Tasks.size() - 1) {
            taskIndex++;
            return TaskStatus::Running;
        }

        taskIndex = 0;
        return TaskStatus::Success;
    }
};

} // namespace Engine
