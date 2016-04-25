// This file is generated. Do not edit.

#include <mbgl/layer/background_layer_properties.hpp>

namespace mbgl {

void BackgroundPaintProperties::parse(const JSValue& value) {
    backgroundColor.parse("background-color", value);
    backgroundPattern.parse("background-pattern", value);
    backgroundOpacity.parse("background-opacity", value);
}

void BackgroundPaintProperties::cascade(const StyleCascadeParameters& parameters) {
    backgroundColor.cascade(parameters);
    backgroundPattern.cascade(parameters);
    backgroundOpacity.cascade(parameters);
}

bool BackgroundPaintProperties::recalculate(const StyleCalculationParameters& parameters) {
    bool hasTransitions = false;

    hasTransitions |= backgroundColor.calculate(parameters);
    hasTransitions |= backgroundPattern.calculate(parameters);
    hasTransitions |= backgroundOpacity.calculate(parameters);

    return hasTransitions;
}

} // namespace mbgl
