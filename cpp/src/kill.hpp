#pragma once

namespace batprotocol
{
    class KillProgress
    {
    public:
        KillProgress() = default;
        KillProgress & add_atomic(const std::string & task_id, const std::string & profile_id, double progress);
        KillProgress & add_sequential(const std::string & task_id, const std::string & profile_id, uint32_t current_repetition, uint32_t current_task_index, const std::string & current_subtask_id);
        KillProgress & add_forkjoin(const std::string & task_id, const std::string & profile_id, const std::vector<std::string> & subtasks_id);
        KillProgress & set_root_task(const std::string & task_id);
    };
} // end of namespace batprotocol
