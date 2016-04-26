#pragma once

#include <mbgl/tile/tile_data.hpp>
#include <mbgl/tile/tile_cache.hpp>
#include <mbgl/style/types.hpp>

#include <mbgl/util/mat4.hpp>
#include <mbgl/util/rapidjson.hpp>
#include <mbgl/util/feature.hpp>
#include <mbgl/util/tileset.hpp>

#include <forward_list>
#include <vector>
#include <map>

namespace mapbox {
namespace geojsonvt {
class GeoJSONVT;
} // namespace geojsonvt
} // namespace mapbox

namespace mbgl {

class Painter;
class FileSource;
class AsyncRequest;
class TransformState;
class Tile;
class TileCoordinate;
struct ClipID;
struct box;

namespace style {

class Style;
class UpdateParameters;

class Source : private util::noncopyable {
public:
    class Observer {
    public:
        virtual ~Observer() = default;

        virtual void onSourceLoaded(Source&) {};
        virtual void onSourceError(Source&, std::exception_ptr) {};

        virtual void onTileLoaded(Source&, const TileID&, bool /* isNewTile */) {};
        virtual void onTileError(Source&, const TileID&, std::exception_ptr) {};
        virtual void onPlacementRedone() {};
    };

    Source(SourceType,
           const std::string& id,
           const std::string& url,
           uint16_t tileSize,
           std::unique_ptr<Tileset>&&,
           std::unique_ptr<mapbox::geojsonvt::GeoJSONVT>&&);
    ~Source();

    bool loaded = false;
    void load(FileSource&);
    bool isLoading() const;
    bool isLoaded() const;

    const Tileset* getTileset() const { return tileset.get(); }

    // Request or parse all the tiles relevant for the "TransformState". This method
    // will return true if all the tiles were scheduled for updating of false if
    // they were not. shouldReparsePartialTiles must be set to "true" if there is
    // new data available that a tile in the "partial" state might be interested at.
    bool update(const UpdateParameters&);

    void updateMatrices(const mat4 &projMatrix, const TransformState &transform);
    void finishRender(Painter &painter);

    std::forward_list<Tile *> getLoadedTiles() const;
    const std::vector<Tile*>& getTiles() const;

    std::unordered_map<std::string, std::vector<Feature>> queryRenderedFeatures(
            const std::vector<TileCoordinate>& queryGeometry,
            const double zoom,
            const double bearing,
            const optional<std::vector<std::string>>& layerIDs);

    void setCacheSize(size_t);
    void onLowMemory();

    void setObserver(Observer* observer);
    void dumpDebugLogs() const;

    const SourceType type;
    const std::string id;
    const std::string url;
    uint16_t tileSize = util::tileSize;
    bool enabled = false;

private:
    void tileLoadingCallback(const TileID&,
                             std::exception_ptr,
                             bool isNewTile);
    bool handlePartialTile(const TileID&);
    bool findLoadedChildren(const TileID&, int32_t maxCoveringZoom, std::vector<TileID>& retain);
    void findLoadedParent(const TileID&, int32_t minCoveringZoom, std::vector<TileID>& retain, const UpdateParameters&);

    TileData::State addTile(const TileID&, const UpdateParameters&);
    TileData::State hasTile(const TileID&);
    void updateTilePtrs();

private:
    std::unique_ptr<const Tileset> tileset;

    std::unique_ptr<mapbox::geojsonvt::GeoJSONVT> geojsonvt;

    // Stores the time when this source was most recently updated.
    TimePoint updated = TimePoint::min();

    std::map<TileID, std::unique_ptr<Tile>> tiles;
    std::vector<Tile*> tilePtrs;
    std::map<TileID, std::weak_ptr<TileData>> tileDataMap;
    TileCache cache;

    std::unique_ptr<AsyncRequest> req;

    Observer nullObserver;
    Observer* observer = &nullObserver;
};

} // namespace style
} // namespace mbgl
