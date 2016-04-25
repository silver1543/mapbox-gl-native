// This file is generated. Do not edit.

#pragma once

#include <mbgl/style/layer.hpp>

namespace mbgl {

class BackgroundLayer : public Layer {
public:
    BackgroundLayer(const std::string& layerID);
    BackgroundLayer(const BackgroundLayer&);
    ~BackgroundLayer() final;

    // Paint properties

    Function<Color> getBackgroundColor() const;
    void setBackgroundColor(Function<Color>);

    Function<std::string> getBackgroundPattern() const;
    void setBackgroundPattern(Function<std::string>);

    Function<float> getBackgroundOpacity() const;
    void setBackgroundOpacity(Function<float>);

    // Private implementation

    std::unique_ptr<Layer> clone() const final;

    class Impl;
    Impl* impl;
};

template <>
inline bool Layer::is<BackgroundLayer>() const {
    return type == Type::Background;
}

} // namespace mbgl
