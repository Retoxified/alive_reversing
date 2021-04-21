#pragma once

#include "IRenderer.hpp"

#include <GL/glew.h>
#include <SDL_opengl.h>

#define GLM_FORCE_SILENT_WARNINGS

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "GLShader.hpp"

struct VertexData
{
    float x, y, z;
    float r, g, b;
    float u, v;
};

struct RGBAPixel
{
    unsigned char R, G, B, A;
};

struct TextureCache
{
    GLuint mTextureID;
    PSX_RECT mVramRect;
    PSX_Point mPalXY;
    IRenderer::BitDepth mBitDepth;
    int mPalNormMulti; // For special case when converted bitmap data is normalized.
    bool mIsFG1;
    bool mIgnoreColor;
    PSX_Point mUvOffset;
};

struct PaletteCache
{
    unsigned int mPalTextureID;
    PSX_Point mPalPoint;
    short mPalDepth;
    RGBAPixel mPalData[256];
};

class OpenGLRenderer : public IRenderer
{
public:
    void Destroy() override;
    bool Create(TWindowHandleType window) override;
    void Clear(BYTE r, BYTE g, BYTE b) override;
    void StartFrame(int xOff, int yOff) override;
    void EndFrame() override;
    void BltBackBuffer(const SDL_Rect* pCopyRect, const SDL_Rect* pDst) override;
    void OutputSize(int* w, int* h) override;
    bool UpdateBackBuffer(const void* pPixels, int pitch) override;
    void CreateBackBuffer(bool filter, int format, int w, int h) override;
    void PalFree(const PalRecord& record) override;
    bool PalAlloc(PalRecord& record) override;
    void PalSetData(const PalRecord& record, const BYTE* pPixels) override;
    void SetTPage(short tPage) override;
    void SetClip(Prim_PrimClipper& clipper) override;
    void SetScreenOffset(Prim_ScreenOffset& offset) override;
    void Draw(Prim_Sprt& sprt) override;
    void Draw(Prim_GasEffect& gasEffect) override;
    void Draw(Prim_Tile& tile) override;
    void Draw(Line_F2& line) override;
    void Draw(Line_G2& line) override;
    void Draw(Line_G4& line) override;
    void Draw(Poly_F3& poly) override;
    void Draw(Poly_G3& poly) override;
    void Draw(Poly_F4& poly) override;
    void Draw(Poly_FT4& poly) override;
    void Draw(Poly_G4& poly) override;

    void Upload(BitDepth bitDepth, const PSX_RECT& rect, const BYTE* pPixels) override;
private:
    SDL_Window* mWindow = nullptr;
    SDL_GLContext mContext = nullptr;
    GLShader mTextureShader = {};
    WORD mLastTPage = 0;
    bool mWireframe = false;

    glm::mat4 m_View = {};

    glm::ivec4 mLastClip = {};

    GLuint mVBO = 0;
    GLuint mIBO = 0;
    GLuint mVAO = 0;

    glm::mat4 GetMVP();
    glm::mat4 GetMVP(float x, float y, float width, float height);

    void SetClipDirect(int x, int y, int width, int height);

    void DebugWindow();

    void InitAttributes();
    void DrawTexture(GLuint pTexture, float x, float y, float width, float height);
    void DrawTriangles(const VertexData* pVertData, int vertSize, const GLuint* pIndData, int indSize);
    void DrawLines(const VertexData* pVertData, int vertSize, const GLuint* pIndData, int indSize);

    void RenderBackground();
};

void HackSetBackground(const char* path);