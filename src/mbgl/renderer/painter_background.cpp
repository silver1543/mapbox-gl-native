#include <mbgl/renderer/painter.hpp>

#include <mbgl/layer/background_layer.hpp>
#include <mbgl/shader/pattern_shader.hpp>
#include <mbgl/shader/plain_shader.hpp>
#include <mbgl/sprite/sprite_atlas.hpp>
#include <mbgl/util/mat4.hpp>
#include <mbgl/util/tile_cover.hpp>

using namespace mbgl;

void Painter::renderBackground(const BackgroundLayer& layer) {
    // Note that for bottommost layers without a pattern, the background color is drawn with
    // glClear rather than this method.
    const BackgroundPaintProperties& properties = layer.paint;

    const bool isPatterned = !properties.backgroundPattern.value.to.empty();// && false;
    optional<SpriteAtlasPosition> imagePosA;
    optional<SpriteAtlasPosition> imagePosB;

    if (isPatterned) {
        imagePosA = spriteAtlas->getPosition(properties.backgroundPattern.value.from, true);
        imagePosB = spriteAtlas->getPosition(properties.backgroundPattern.value.to, true);

        if (!imagePosA || !imagePosB)
            return;

        config.program = patternShader->getID();
        patternShader->u_matrix = identityMatrix;
        patternShader->u_pattern_tl_a = (*imagePosA).tl;
        patternShader->u_pattern_br_a = (*imagePosA).br;
        patternShader->u_pattern_tl_b = (*imagePosB).tl;
        patternShader->u_pattern_br_b = (*imagePosB).br;
        patternShader->u_mix = properties.backgroundPattern.value.t;
        patternShader->u_opacity = properties.backgroundOpacity;

        spriteAtlas->bind(true, glObjectStore);
        backgroundPatternArray.bind(*patternShader, tileStencilBuffer, BUFFER_OFFSET(0), glObjectStore);

    } else {
        Color color = properties.backgroundColor;
        color[0] *= properties.backgroundOpacity;
        color[1] *= properties.backgroundOpacity;
        color[2] *= properties.backgroundOpacity;
        color[3] *= properties.backgroundOpacity;

        config.program = plainShader->getID();
        plainShader->u_color = color;
        backgroundArray.bind(*plainShader, tileStencilBuffer, BUFFER_OFFSET(0), glObjectStore);
    }

    config.stencilTest = GL_FALSE;
    config.depthFunc.reset();
    config.depthTest = GL_TRUE;
    config.depthMask = GL_FALSE;
    setDepthSublayer(0);

    auto tileIDs = util::tileCover(state, state.getIntegerZoom());

    for (auto& id : tileIDs) {
        mat4 vtxMatrix;
        state.matrixFor(vtxMatrix, id);
        matrix::multiply(vtxMatrix, projMatrix, vtxMatrix);

        if (isPatterned) {
            patternShader->u_matrix = vtxMatrix;

            std::array<int, 2> imageSizeScaledA = {{
                (int)((*imagePosA).size[0] * properties.backgroundPattern.value.fromScale),
                (int)((*imagePosA).size[1] * properties.backgroundPattern.value.fromScale)
            }};
            std::array<int, 2> imageSizeScaledB = {{
                (int)((*imagePosB).size[0] * properties.backgroundPattern.value.toScale),
                (int)((*imagePosB).size[1] * properties.backgroundPattern.value.toScale)
            }};

            patternShader->u_patternscale_a = {{
                1.0f / id.pixelsToTileUnits(imageSizeScaledA[0], state.getIntegerZoom()),
                1.0f / id.pixelsToTileUnits(imageSizeScaledA[1], state.getIntegerZoom())
            }};
            patternShader->u_patternscale_b = {{
                1.0f / id.pixelsToTileUnits(imageSizeScaledB[0], state.getIntegerZoom()),
                1.0f / id.pixelsToTileUnits(imageSizeScaledB[1], state.getIntegerZoom())
            }};

            float offsetAx = (std::fmod(util::tileSize, imageSizeScaledA[0]) * id.canonical.x) /
                             (float)imageSizeScaledA[0];
            float offsetAy = (std::fmod(util::tileSize, imageSizeScaledA[1]) * id.canonical.y) /
                             (float)imageSizeScaledA[1];

            float offsetBx = (std::fmod(util::tileSize, imageSizeScaledB[0]) * id.canonical.x) /
                             (float)imageSizeScaledB[0];
            float offsetBy = (std::fmod(util::tileSize, imageSizeScaledB[1]) * id.canonical.y) /
                             (float)imageSizeScaledB[1];

            patternShader->u_offset_a = std::array<float, 2>{{offsetAx, offsetAy}};
            patternShader->u_offset_b = std::array<float, 2>{{offsetBx, offsetBy}};


        } else {
            plainShader->u_matrix = vtxMatrix;
            Color color = properties.backgroundColor;
            plainShader->u_color = color;
        }

        MBGL_CHECK_ERROR(glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)tileStencilBuffer.index()));
    }

}
