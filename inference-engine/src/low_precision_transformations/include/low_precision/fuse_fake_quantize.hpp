// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <memory>
#include <ngraph/ngraph.hpp>
#include "low_precision/layer_transformation.hpp"

namespace ngraph {
namespace pass {
namespace low_precision {

class TRANSFORMATIONS_API FuseFakeQuantizeTransformation : public LayerTransformation {
public:
    FuseFakeQuantizeTransformation(const Params& params) : LayerTransformation(params) {}
    ~FuseFakeQuantizeTransformation() override {}
    void registerMatcherIn(GraphRewrite& pass, TransformationContext& context) const override;
    bool transform(TransformationContext& context, ngraph::pattern::Matcher &m) const override;
    bool isPrecisionPreserved(std::shared_ptr<Node> layer) const noexcept override;

private:
    std::shared_ptr<opset1::FakeQuantize> handle(
        TransformationContext& context,
        const std::shared_ptr<opset1::FakeQuantize>& fakeQuantize) const;
};

} // namespace low_precision
} // namespace pass
} // namespace ngraph
