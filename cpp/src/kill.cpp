#include "kill.hpp"

#include "assert.hpp"

namespace batprotocol
{

std::shared_ptr<KillProgress> KillProgress::make(const std::string & root_task_id)
{
    std::shared_ptr<KillProgress> kill_progress(new KillProgress());
    kill_progress->_root_task_id = root_task_id;

    return kill_progress;
}

KillProgress::~KillProgress()
{
    for (const auto & kv : _tasks_progress)
    {
        switch (kv.second->type)
        {
            case fb::KillProgress_KillProgressAtomicProfile: {
                delete static_cast<Atomic*>(kv.second->data);
            } break;
            case fb::KillProgress_KillProgressSequentialProfile: {
                delete static_cast<Sequential*>(kv.second->data);
            } break;
            case fb::KillProgress_KillProgressForkJoinProfile: {
                delete static_cast<ForkJoin*>(kv.second->data);
            } break;
            case fb::KillProgress_KillProgressParallelTaskMergeProfile: {
                delete static_cast<ParallelTaskMerge*>(kv.second->data);
            } break;
        }
        delete kv.second;
    }

    _tasks_progress.clear();
}

KillProgress & KillProgress::add_atomic(const std::string & task_id, const std::string & profile_id, double progress)
{
    BAT_ENFORCE(_tasks_progress.find(task_id) == _tasks_progress.end(), "task_id '%s' already exists", task_id.c_str());

    auto atomic = new Atomic;
    atomic->profile_id = profile_id;
    atomic->progress = progress;

    auto variant = new KillProgressVariant;
    variant->type = fb::KillProgress_KillProgressAtomicProfile;
    variant->data = static_cast<void*>(atomic);
    _tasks_progress[task_id] = variant;

    return *this;
}

KillProgress & KillProgress::add_sequential(const std::string & task_id, const std::string & profile_id, uint32_t current_repetition, uint32_t current_task_index, const std::string & current_subtask_id)
{
    BAT_ENFORCE(_tasks_progress.find(task_id) == _tasks_progress.end(), "task_id '%s' already exists", task_id.c_str());

    auto sequential = new Sequential;
    sequential->profile_id = profile_id;
    sequential->current_repetition = current_repetition;
    sequential->current_task_index = current_task_index;
    sequential->current_subtask_id = current_subtask_id;

    auto variant = new KillProgressVariant;
    variant->type = fb::KillProgress_KillProgressSequentialProfile;
    variant->data = static_cast<void*>(sequential);
    _tasks_progress[task_id] = variant;

    return *this;
}

KillProgress & KillProgress::add_forkjoin(const std::string & task_id, const std::string & profile_id, const std::vector<std::string> & subtasks_id)
{
    BAT_ENFORCE(_tasks_progress.find(task_id) == _tasks_progress.end(), "task_id '%s' already exists", task_id.c_str());

    auto forkjoin = new ForkJoin;
    forkjoin->profile_id = profile_id;
    forkjoin->subtasks_id = subtasks_id;

    auto variant = new KillProgressVariant;
    variant->type = fb::KillProgress_KillProgressForkJoinProfile;
    variant->data = static_cast<void*>(forkjoin);
    _tasks_progress[task_id] = variant;

    return *this;
}

KillProgress & KillProgress::add_parallel_task_merge(const std::string & task_id, const std::string & profile_id, const std::vector<std::string> & subtasks_id)
{
    BAT_ENFORCE(_tasks_progress.find(task_id) == _tasks_progress.end(), "task_id '%s' already exists", task_id.c_str());

    auto ptask_merge = new ParallelTaskMerge;
    ptask_merge->profile_id = profile_id;
    ptask_merge->subtasks_id = subtasks_id;

    auto variant = new KillProgressVariant;
    variant->type = fb::KillProgress_KillProgressParallelTaskMergeProfile;
    variant->data = static_cast<void*>(ptask_merge);
    _tasks_progress[task_id] = variant;

    return *this;
}

} // end of namespace batprotocol
