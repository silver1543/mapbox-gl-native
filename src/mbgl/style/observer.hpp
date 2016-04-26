#pragma once

#include <mbgl/style/source.hpp>
#include <mbgl/text/glyph_store.hpp>
#include <mbgl/sprite/sprite_store.hpp>

namespace mbgl {
namespace style {

class Observer : public GlyphStore::Observer,
                 public SpriteStore::Observer,
                 public Source::Observer {
public:
    /**
     * In addition to the individual glyph, sprite, and source events, the
     * following "rollup" events are provided for convenience. They are
     * strictly additive; e.g. when a source is loaded, both `onSourceLoaded`
     * and `onResourceLoaded` will be called.
     */
     virtual void onResourceLoaded() {};
     virtual void onResourceError(std::exception_ptr) {};
};

} // namespace style
} // namespace mbgl
