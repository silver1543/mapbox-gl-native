#ifndef MBGL_RASTER_LAYER
#define MBGL_RASTER_LAYER

#include <mbgl/layer/layer_impl.hpp>
#include <mbgl/layer/raster_layer.hpp>
#include <mbgl/layer/raster_layer_properties.hpp>

namespace mbgl {

class RasterLayer::Impl : public Layer::Impl {
public:
    void parseLayout(const JSValue&) override {};
    void parsePaints(const JSValue&) override;

    void cascade(const StyleCascadeParameters&) override;
    bool recalculate(const StyleCalculationParameters&) override;

    std::unique_ptr<Bucket> createBucket(StyleBucketParameters&) const override;

    RasterPaintProperties paint;
};

} // namespace mbgl

#endif
