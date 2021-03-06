// Copyright (C) 2018 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <description_buffer.hpp>
#include "ie_built_in_impl.hpp"
#include <ie_layers.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace InferenceEngine {
namespace ShapeInfer {

/**
 *@brief Implementation of Shape inference for ReorgYolo layer
 */
class ReorgYoloShapeProp : public BuiltInShapeInferImpl {
public:
    explicit ReorgYoloShapeProp(const std::string& type) : BuiltInShapeInferImpl(type) {}

    void inferShapesImpl(const std::vector<SizeVector>& inShapes,
                         const std::map<std::string, std::string>& params,
                         const std::map<std::string, Blob::Ptr>& blobs,
                         std::vector<SizeVector>& outShapes) override {
        LayerParams lp{};
        CNNLayer cnnLayer(lp);
        cnnLayer.params = params;
        cnnLayer.type = _type;
        validate(&cnnLayer, inShapes, params, blobs);
        size_t stride = static_cast<size_t>(cnnLayer.GetParamAsInt("stride"));
        SizeVector outShape;
        for (size_t i = 0; i < inShapes[0].size(); i++) {
            outShape.push_back(inShapes[0][i]);
            if (i == 1) {
                outShape[outShape.size() - 1] *= stride * stride;
            } else if (i > 1) {
                outShape[outShape.size() - 1] /= stride;
            }
        }
        outShapes.push_back(outShape);
    }
};

}  // namespace ShapeInfer
}  // namespace InferenceEngine
