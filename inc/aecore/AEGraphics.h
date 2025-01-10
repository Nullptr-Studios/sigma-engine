// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEGraphics.h
// Author			:	Thomas Komair
// Last Update		:	Jan 2020
// Purpose			:	Graphics component header file
// ---------------------------------------------------------------------------

#ifndef AE_GRAPHICS_H
#define AE_GRAPHICS_H

#define AEGFX_NAME_SIZE 512

// forward declare AEGfx structures
struct AEGfxTriList;
struct AEGfxTexture;
struct AEGfxFont;

// ---------------------------------------------------------------------------
// Defines

// maximum number of triangles per mesh
#define AE_GFX_TRI_NUM_MAX 8192

enum AEGfxBlendMode
{
    AE_GFX_BM_NONE = 0,       // Replaces the frame buffer color with the triangle pixel
    AE_GFX_BM_BLEND,          // Blends the fb color with the tri color using the alpha of the tri: FinalColor = TriColor * TriAlpha + FBColor * (1-TriAlpha)
    AE_GFX_BM_ADD,            // Adds the two colors together : FinalColor = TriColor + FBColor
    AE_GFX_BM_ADD_BLENDALPHA, // Adds the two colors together and Multiplies the triangle color with its alpha: FinalColor = TriColor * TriAlpha + FBColor
    AE_GFX_BM_MULTIPLY,       // Multiplies the two colors together: FinalColor = TriColor * FBColor
    AE_GFX_BM_NUM             // keeps count of how many blend modes are implemented - also the value returned by AEGfxGetBlendMode if the arguments are set manually
};

// use those in order to have better control over the blending operation
enum AEGfxBlendArgument
{
    AE_GFX_BA_ONE = 0,      // 1
    AE_GFX_BA_ZERO,         // 0
    AE_GFX_BA_SRCCOLOR,     // RGB Color of the triangle pixel
    AE_GFX_BA_SRCALPHA,     // Alpha of the triangle pixel
    AE_GFX_BA_INVSRCALPHA,  // 1-Alpha of the triangle pixel
    AE_GFX_BA_DESTCOLOR,    // RGB color of the pixel currently in the frame buffer
    AE_GFX_BA_DESTALPHA,    // Alpha of the pixel currently in the frame buffer
    AE_GFX_BA_INVDESTALPHA, // 1-Alpha of the pixel currently in the frame buffer
    AE_GFX_BA_NUM
};

// Texture filters define the behavior of sampling a texture when the size on screen
// of the object to draw is greater or smaller than the texture size.
enum AEGfxTextureFilter
{
    AE_GFX_TF_NEAREST = 0, // uses the nearest pixel to the specified texture coordinates
    AE_GFX_TF_LINEAR,      // uses the weighted average of the texture elements that are closest to the specified texture coordinates.
    AE_GFX_TF_NUM
};

// ---------------------------------------------------------------------------
// Struct/Class definitions

struct AEGfxVertex
{
    float x, y, z;
    float r, g, b, a;
    float tu, tv;
};

struct AEGfxTriList
{
    unsigned mID;
    unsigned mVtxNum;
    char     mName[AEGFX_NAME_SIZE];
};

// ---------------------------------------------------------------------------
struct AEGfxTexture
{
    unsigned mID;
    unsigned mWidth;
    unsigned mHeight;
    char     mName[AEGFX_NAME_SIZE];
};

struct AEGfxFont
{
    char           mName[AEGFX_NAME_SIZE];
    unsigned       mSize;
    void *         mFontData;
    AEGfxTexture * mFontTex;
};

// ---------------------------------------------------------------------------
// System Functions

bool AEGfxInit(HWND hWin);
void AEGfxReset();
void AEGfxExit();
void AEGfxStart();
void AEGfxEnd();
void AEGfxPresent();

// ---------------------------------------------------------------------------
// Window size Functions

//-----------------------------------------------------------------------------
// function to set the current transformation matrix to use
void AEGfxSetTransform(AEMtx33 * pMtx);
void AEGfxSetTransform(AEMtx44 * pMtx);
void AEGfxSetTextureTransform(AEMtx33 * pTexMtx);

//-----------------------------------------------------------------------------
// functions to set the frame buffer's clear color
void     AEGfxSetClearColor(unsigned clearColor);
unsigned AEGfxGetClearColor();
void     AEGfxClearBuffer();
void     AEGfxClearColorBuffer();
void     AEGfxClearDepthBuffer();
void     AEGfxSetDepthBufferEnabled(bool enabled);
bool     AEGfxGetDepthBufferEnabled();
void     AEGfxSetVSyncEnabled(bool enabled);
bool     AEGfxGetVSyncEnabled();
//-----------------------------------------------------------------------------
// function to set the current modulation color and Texture
void     AEGfxSetModulationColor(unsigned modColor);
unsigned AEGfxGetModulationColor();
void     AEGfxEnableModulationColor(bool enabled);
bool     AEGfxGetModulationColorEnabled();

//-----------------------------------------------------------------------------
// Blending operations
void               AEGfxSetBlendMode(AEGfxBlendMode blendMode);
void               AEGfxSetSourceBlendArgument(AEGfxBlendArgument blendArg);
void               AEGfxSetDestBlendArgument(AEGfxBlendArgument blendArg);
AEGfxBlendMode     AEGfxGetBlendMode();
AEGfxBlendArgument AEGfxGetSourceBlendArgument();
AEGfxBlendArgument AEGfxGetDestBlendArgument();

//-----------------------------------------------------------------------------
// MESH
void AEGfxTriStart();
void AEGfxTriAdd(float x0, float y0, unsigned c0, float tu0, float tv0,
                 float x1, float y1, unsigned c1, float tu1, float tv1,
                 float x2, float y2, unsigned c2, float tu2, float tv2);

AEGfxTriList * AEGfxTriEnd(const char * name = 0);
AEGfxTriList * AEGfxTriListCreate(AEGfxVertex * pVertexData, unsigned vtxCount, const char * name = 0);
void           AEGfxTriDraw(AEGfxTriList * pTriList);
void           AEGfxTriFree(AEGfxTriList * pTriList);

//-----------------------------------------------------------------------------
// TEXTURE

AEGfxTexture * AEGfxTextureLoad(const char * pFileName);                                                                    // from file
AEGfxTexture * AEGfxTextureLoad(unsigned width, unsigned height, unsigned char * pPixels = NULL, const char * name = NULL); // from 2D array of pixels
void           AEGfxTextureGetPixels(AEGfxTexture * pTexture, unsigned char ** outPixels);
void           AEGfxTextureUnload(AEGfxTexture * pTexture);
void           AEGfxTextureSet(AEGfxTexture * pTexture); // Use NULL for 'no texture'
void           AEGfxTextureSetFilters(AEGfxTexture * pTexture, AEGfxTextureFilter minFilter, AEGfxTextureFilter magFilter);
void           AEGfxTextureGetFilters(AEGfxTexture * pTexture, AEGfxTextureFilter * minFilter, AEGfxTextureFilter * magFilter);
bool           AEGfxLoadImagePNG(const char * filename, unsigned char *& outPixels, unsigned & outWidth, unsigned & outHeight);
bool           AEGfxSaveImagePNG(const char * filename, const unsigned char * pixelData, unsigned width, unsigned height);

//-----------------------------------------------------------------------------
// RENDER TEXTURE

AEGfxTexture * AEGfxRenderTexCreate(unsigned width, unsigned height, const char * name = NULL);
void           AEGfxRenderTexSet(AEGfxTexture * pRTex); // Note: if pRTex is NULL, the the window back buffer becomes the current render target
AEGfxTexture * AEGfxRenderTexGetCurrent();              // Note: If this function returns NULL, then the window back buffer is the current render target.

//-----------------------------------------------------------------------------
// functions to draw simple primitives
// * these are slow!!! use with caution!!
// * DO NOT call any of these function while building mesh using above functions
void AEGfxLine(float x0, float y0, float z0, unsigned c0, float x1, float y1, float z1, unsigned c1);
void AEGfxRect(float x, float y, float z, float w, float h, unsigned c);
void AEGfxRectMinMax(float minX, float minY, float maxX, float maxY, float z, unsigned c);
void AEGfxRectRot(float x, float y, float z, float w, float h, float angle_rad, unsigned c);
void AEGfxCircle(float x, float y, float rad, unsigned color, unsigned segments = 20);
void AEGfxFlush();

//-----------------------------------------------------------------------------
// Utility functions
// ----------------------------------------------------------------------------

unsigned AEGfxColor(unsigned r, unsigned g, unsigned b, unsigned a = 255);                       // Computes a 32-bit ARGB color
void     AEGfxColorComp(unsigned color, unsigned * r, unsigned * g, unsigned * b, unsigned * a); // Extracts the component of a 32-bit ARGB color
unsigned AEGfxColInterp(unsigned c0, unsigned c1, float t);                                      // Interpolates 2 color values.

// Prints text in screen coordinates (origin is top-left of client area)
void AEGfxPrint(int x, int y, unsigned color, const char * pStr);

//-----------------------------------------------------------------------------
// SIZE & VIEWPORT METHODS
void AEGfxSetFullscreen(bool fullscreen);
bool AEGfxGetFullscreen();
void AEGfxSetWindowsize(unsigned w, unsigned h);
void AEGfxGetScreenSize(unsigned * sw, unsigned * sh);
void AEGfxSetDefaultViewport();
void AEGfxSetViewport(float left, float bottom, float right, float top);
void AEGfxGetViewport(float * left, float * bottom, float * right, float * top);
void AEGfxSetViewRectangle(float width, float height);
void AEGfxGetViewRectangle(float * width, float * height);
//-----------------------------------------------------------------------------
// EXTRA TRANSFORM
void    AEGfxSetViewTransform(AEMtx44 * pMtx);
void    AEGfxSetProjTransform(AEMtx44 * pMtx);
AEMtx44 AEGfxGetViewTransform();
AEMtx44 AEGfxGetProjTransform();

//-----------------------------------------------------------------------------
// FONT SUPPORT

AEGfxFont *    AEGfxFontCreate(const char * fontName, unsigned size, float img_size = 1.0f);
void           AEGfxFontFree(AEGfxFont * pFont);
void           AEGfxFontDraw(const char * pText, AEGfxFont * pFont, unsigned color);
void           AEGfxFontTextRect(const char * pText, AEGfxFont * pFont, AEVec2 * minP, AEVec2 * maxP);
AEGfxTriList * AEGfxFontMesh(AEGfxFont * pFont, const char * text);
// ---------------------------------------------------------------------------
// DEBUGGING
std::list<AEGfxTexture *> AEGfxGetAllocatedTextures();
unsigned                  AEGfxGetAllocatedTexturesCount();
std::list<AEGfxTexture *> AEGfxGetAllocatedRenderTextures();
unsigned                  AEGfxGetAllocatedRenderTexturesCount();
std::list<AEGfxTriList *> AEGfxGetAllocatedTrilist();
unsigned                  AEGfxGetAllocatedTrilistCount();
std::list<AEGfxFont *>    AEGfxGetAllocatedFont();
unsigned                  AEGfxGetAllocatedFontCount();
#define AEGfxCheckGLError() _AEGfxCheckGLError(__FILE__, __LINE__)
void _AEGfxCheckGLError(const char * file, int line);
#endif // AE_GRAPHICS_H
