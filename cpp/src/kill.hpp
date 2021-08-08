#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "batprotocol_generated.h"

namespace batprotocol
{
    class MessageBuilder;

    class KillProgress
    {
    public:
        static std::shared_ptr<KillProgress> make(const std::string & root_task_id);
        ~KillProgress();

        KillProgress & add_atomic(
            const std::string & task_id,
            const std::string & profile_id,
            double progress
        );

        KillProgress & add_sequential(
            const std::string & task_id,
            const std::string & profile_id,
            uint32_t current_repetition,
            uint32_t current_task_index,
            const std::string & current_subtask_id
        );

        KillProgress & add_forkjoin(
            const std::string & task_id,
            const std::string & profile_id,
            const std::vector<std::string> & subtasks_id
        );

    private:
        friend class MessageBuilder;
        KillProgress() = default;

        struct Atomic
        {
            std::string profile_id;
            double progress;
        };

        struct Sequential
        {
            std::string profile_id;
            uint32_t current_repetition;
            uint32_t current_task_index;
            std::string current_subtask_id;
        };

        struct ForkJoin
        {
            std::string profile_id;
            std::vector<std::string> subtasks_id;
        };

        struct KillProgressVariant
        {
            fb::KillProgress type = fb::KillProgress_NONE;
            void * data = nullptr;
        };

        std::string _root_task_id;
        std::unordered_map<std::string, KillProgressVariant*> _tasks_progress;
    };
} // end of namespace batprotocol
