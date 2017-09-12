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

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "ngraph/ngraph.hpp"
#include "ngraph/pass/topological_sort.hpp"
#include "ngraph/visualize.hpp"
#include "util.hpp"
#include "log.hpp"
#include "test_tools.hpp"

using namespace std;
using namespace ngraph;

TEST(topological_sort, basic)
{
    vector<shared_ptr<op::Parameter>> args;
    for (int i = 0; i < 10; i++)
    {
        auto arg = make_shared<op::Parameter>(element::Float32::element_type(), Shape{1});
        ASSERT_NE(nullptr, arg);
        args.push_back(arg);
    }

    auto t0 = make_shared<op::Add>(args[0], args[1]);
    ASSERT_NE(nullptr, t0);
    auto t1 = make_shared<op::Dot>(t0, args[2]);
    ASSERT_NE(nullptr, t1);
    auto t2 = make_shared<op::Multiply>(t0, args[3]);
    ASSERT_NE(nullptr, t2);

    auto t3 = make_shared<op::Add>(t1, args[4]);
    ASSERT_NE(nullptr, t2);
    auto t4 = make_shared<op::Add>(t2, args[5]);
    ASSERT_NE(nullptr, t3);

    auto r0 = make_shared<op::Add>(t3, t4);
    ASSERT_NE(nullptr, r0);

    auto f0 = make_shared<Function>(r0, args);
    ASSERT_NE(nullptr, f0);

    ASSERT_EQ(2, r0->get_arguments().size());

    // Visualize vz;
    // vz.add(r0);
    // vz.save_dot("test.png");
    pass::TopologicalSort ts;
    ts.run_on_tree(r0);
    auto sorted_list = ts.get_call_graph();

    size_t node_count = get_node_count(r0);

    EXPECT_EQ(node_count, sorted_list.size());
    EXPECT_TRUE(validate_list(sorted_list));
}

// TEST(topological_sort, cycle)
// {
//     vector<shared_ptr<op::Parameter>> args;
//     for (int i = 0; i < 10; i++)
//     {
//         auto arg = make_shared<op::Parameter>(element::Float32::element_type(), Shape{1});
//         ASSERT_NE(nullptr, arg);
//         args.push_back(arg);
//     }

//     auto add_0 = make_shared<op::Add>(args[0], args[1]);
//     auto add_1 = make_shared<op::Add>(args[0], args[1]);
// }

shared_ptr<Node> make_cell(shared_ptr<Node> in_0, shared_ptr<Node> in_1, shared_ptr<Node> in_2)
{
    auto t0 = make_shared<op::Dot>(in_0, in_1);
    auto t1 = make_shared<op::Add>(t0, in_2);
    auto t2 = make_shared<op::Negative>(t1); // no tanh yet, this will do
    return static_pointer_cast<Node>(t2);
}

TEST(benchmark, topological_sort)
{
    stopwatch timer;
    // x[i+1] = tanh(dot(W,x[i])+b)
    shared_ptr<Node> result;
    result = make_shared<op::Parameter>(element::Float32::element_type(), Shape{1});
    for (int i=0; i<1000000; i++)
    {
        auto in_1 = make_shared<op::Parameter>(element::Float32::element_type(), Shape{1});
        auto in_2 = make_shared<op::Parameter>(element::Float32::element_type(), Shape{1});
        result = make_cell(result, in_1, in_2);
    }

    auto op_r0 = static_pointer_cast<Op>(result);

    timer.start();
    pass::TopologicalSort ts;
    ts.run_on_tree(op_r0);
    auto sorted_list = ts.get_call_graph();
    timer.stop();
    INFO << "topological sort took " << timer.get_milliseconds() << "ms";

    size_t node_count = 0;
    traverse_nodes(op_r0, [&](const Node* node) {
        node_count++;
    });

    INFO << "node count " << node_count;

    timer.start();
    ngraph::free_nodes(result);
    timer.stop();
    INFO << "delete nodes took " << timer.get_milliseconds() << "ms";
}