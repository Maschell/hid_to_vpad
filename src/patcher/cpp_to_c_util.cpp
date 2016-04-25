#include "cpp_to_c_util.h"
#include <string.h>
#include "common/common.h"
#include "video/shaders/ColorShader.h"

// TODO: not creating these stuff at every frame
void draw_Cursor_at(f32 x, f32 y) {
	f32 widthScaleFactor = 1.0f / (f32)1280;
    f32 heightScaleFactor = 1.0f / (f32)720;

    int width = 20;

    glm::vec3 positionOffsets = glm::vec3(0.0f);

    positionOffsets[0] = (x-((1280)/2)+(width/2)) * widthScaleFactor * 2.0f;
    positionOffsets[1] = -(y-((720)/2)+(width/2)) * heightScaleFactor * 2.0f;

    u8 colorVtxs[16];
    memset(colorVtxs,0xFF,16*sizeof(u8));

    GX2Invalidate(GX2_INVALIDATE_CPU_ATTRIB_BUFFER, colorVtxs, 16 * sizeof(u8));

    glm::vec4 colorIntensity = glm::vec4(1.0f);
    colorIntensity[3] = 255;

    glm::vec3 scale(width*widthScaleFactor,width*heightScaleFactor,1.0f);

    ColorShader::instance()->setShaders();
    ColorShader::instance()->setAttributeBuffer(colorVtxs, NULL, 4);
    ColorShader::instance()->setAngle(0);
    ColorShader::instance()->setOffset(positionOffsets);
    ColorShader::instance()->setScale(scale);
    ColorShader::instance()->setColorIntensity(colorIntensity);
    ColorShader::instance()->draw(GX2_PRIMITIVE_QUADS, 4);
}


void draw_Cursor_destroy() {
    //! destroy shaders
    ColorShader::destroyInstance();
}




