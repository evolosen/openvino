// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "gtest/gtest.h"
#include "ngraph/builder/reshape.hpp"
#include "ngraph/ngraph.hpp"
#include "ngraph/opsets/opset5.hpp"
#include "util/type_prop.hpp"

using namespace std;
using namespace ngraph;

TEST(type_prop, if_simple_test)
{
    // That which we iterate over
    auto X = make_shared<op::Parameter>(element::f32, Shape{32, 40, 10});
    auto Y = make_shared<op::Parameter>(element::f32, Shape{32, 40, 10});
    auto cond = std::make_shared<ngraph::opset5::Constant>(ngraph::element::boolean, ngraph::Shape{1}, true);

    // Set up the cell body, a function from (Xi, Yi) -> (Zo)
    // Body parameters
    auto Xt = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Yt = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Xe = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Ye = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    // Body
    auto then_op = std::make_shared<op::v1::Add>(Xt,Yt);
    auto then_body =
        make_shared<ngraph::Function>(OutputVector{then_op}, ParameterVector{Xt, Yt});

    auto else_op = std::make_shared<op::v1::Maximum>(Xe, Ye);
    auto else_body = make_shared<ngraph::Function>(OutputVector{else_op}, ParameterVector{Xe, Ye});
    auto if_op = make_shared<op::If>(cond);
    if_op->reserve_bodies(2);
    if_op->set_then_body(then_body);
    if_op->set_else_body(else_body);
    if_op->set_invariant_input(X, Xt, Xe);
    if_op->set_invariant_input(Y, Yt, Ye);
    auto res = if_op->set_output(then_op, else_op);
    auto result0 = make_shared<op::Result>(res);
    Shape out0_shape{32, 40, 10};
    auto sh = result0->get_output_shape(0);
    EXPECT_EQ(sh, out0_shape);
}

TEST(type_prop, if_non_const_condition_test)
{
    // That which we iterate over
    auto X = make_shared<op::Parameter>(element::f32, Shape{32, 40, 10});
    auto Y = make_shared<op::Parameter>(element::f32, Shape{32, 40, 10});
    auto cond = make_shared<op::Parameter>(element::boolean, Shape{1});

    // Set up the cell body, a function from (Xi, Yi) -> (Zo)
    // Body parameters
    auto Xt = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Yt = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Xe = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Ye = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    // Body
    auto then_op = std::make_shared<op::v1::Add>(Xt, Yt);
    auto then_body = make_shared<ngraph::Function>(OutputVector{then_op}, ParameterVector{Xt, Yt});

    auto else_op = std::make_shared<op::v1::Maximum>(Xe, Ye);
    auto else_body = make_shared<ngraph::Function>(OutputVector{else_op}, ParameterVector{Xe, Ye});
    auto if_op = make_shared<op::If>(cond);
    if_op->reserve_bodies(2);
    if_op->set_then_body(then_body);
    if_op->set_else_body(else_body);
    if_op->set_invariant_input(X, Xt, Xe);
    if_op->set_invariant_input(Y, Yt, Ye);
    auto res = if_op->set_output(then_op, else_op);
    auto result0 = make_shared<op::Result>(res);
    Shape out0_shape{32, 40, 10};
    auto sh = result0->get_output_shape(0);
    EXPECT_EQ(sh, out0_shape);
}

TEST(type_prop, if_clone_test)
{
    // That which we iterate over
    auto X = make_shared<op::Parameter>(element::f32, Shape{32, 40, 10});
    auto Y = make_shared<op::Parameter>(element::f32, Shape{32, 40, 10});
    auto cond = make_shared<op::Parameter>(element::boolean, Shape{1});

    // Set up the cell body, a function from (Xi, Yi) -> (Zo)
    // Body parameters
    auto Xt = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Yt = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Xe = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Ye = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Xnew = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    auto Ynew = make_shared<op::Parameter>(element::f32, PartialShape::dynamic());
    // Body
    auto then_op = std::make_shared<op::v1::Add>(Xt, Yt);
    auto then_body = make_shared<ngraph::Function>(OutputVector{then_op}, ParameterVector{Xt, Yt});

    auto else_op = std::make_shared<op::v1::Maximum>(Xe, Ye);
    auto else_body = make_shared<ngraph::Function>(OutputVector{else_op}, ParameterVector{Xe, Ye});
    auto if_op = make_shared<op::If>(cond);
    if_op->reserve_bodies(2);
    if_op->set_then_body(then_body);
    if_op->set_else_body(else_body);
    if_op->set_invariant_input(X, Xt, Xe);
    if_op->set_invariant_input(Y, Yt, Ye);
    auto res = if_op->set_output(then_op, else_op);

    auto new_if = if_op->clone_with_new_inputs(OutputVector{cond, Xnew, Ynew});
    auto result0 = make_shared<op::Result>(res);
    Shape out0_shape{32, 40, 10};
    auto sh = result0->get_output_shape(0);
    EXPECT_EQ(sh, out0_shape);
}