// ----------------------------------------------------------------------------
// Copyright 2017 Nervana Systems Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// ----------------------------------------------------------------------------

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stddef.h>
#include <stdio.h>

#include "cuda.h"
#include "cuda_runtime.h"

#include "ngraph/runtime/gpu/gpu_util.hpp"

using namespace ngraph;
using namespace std;

void runtime::gpu::print_gpu_f32_tensor(void* p, size_t element_count, size_t element_size)
{
    float* local;
    size_t size_in_bytes = element_size * element_count;
    local = static_cast<float*>(malloc(size_in_bytes));
    cudaMemcpy(local, p, size_in_bytes, cudaMemcpyDeviceToHost);
    for (size_t i = 0; i < element_count; i++)
    {
        std::cout << local[i] << "\n";
    }
}

void runtime::gpu::check_cuda_errors(CUresult err)
{
    assert(err == CUDA_SUCCESS);
}