/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "ngraph/json.hpp"
#include "ngraph/runtime/cpu/cpu_external_function.hpp"

namespace ngraph
{
    namespace runtime
    {
        namespace cpu
        {
            struct TraceEvent
            {
                // This should be a single character
                // but the JSON encoder nlohmann::json
                // is broken and doesn't handle character fields
                std::string m_phase;
                std::string m_category;
                const std::string& m_name;
                unsigned int m_pid;
                unsigned int m_tid;
                int64_t m_timestamp;
                int64_t m_duration;
                const std::vector<std::string>& m_outputs;
                const std::vector<std::string>& m_inputs;

                TraceEvent(const std::string& ph,
                           const std::string& cat,
                           const std::string& name,
                           unsigned int pid,
                           unsigned int tid,
                           int64_t ts,
                           int64_t dur,
                           const std::vector<std::string>& outputs,
                           const std::vector<std::string>& inputs);
            };

            void to_json(nlohmann::json& json, const TraceEvent& event);

            void generate_timeline(const std::vector<OpAttributes>& op_attrs,
                                   int64_t* op_durations);
            bool is_tracing_enabled();
        }
    }
}