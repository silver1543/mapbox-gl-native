#pragma once

#include <mbgl/style/layer.hpp>

namespace mbgl {

class CustomLayer : public Layer {
public:
    CustomLayer(const std::string& id,
                CustomLayerInitializeFunction,
                CustomLayerRenderFunction,
                CustomLayerDeinitializeFunction,
                void* context);
    CustomLayer(const CustomLayer&);
    ~CustomLayer() final;

    // Private implementation

    std::unique_ptr<Layer> clone() const final;

    class Impl;
    Impl* impl;
};

template <>
inline bool Layer::is<CustomLayer>() const {
    return type == Type::Custom;
}

} // namespace mbgl
