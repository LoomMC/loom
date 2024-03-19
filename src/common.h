#ifdef _WIN32
#include <Windows.h>

#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>

#else
#include <gl/GL.h>
#endif

// Basically if you are on Windows, you use
// DirectX, if you are not then you use OpenGL.