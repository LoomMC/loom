#pragma once

// DirectX SDK
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1.h>
#include <dxgi1_4.h>
#include "DirectXMath.h"

// OpenGL SDK
#include <GL/gl.h>
#include <GL/glu.h>

namespace loom::gfx::device {
    enum class DeviceType {
        D3D11,
        D3D12,
        OpenGL
    };

    DeviceType type;

    class Color {
    public:
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

        float r, g, b, a;

        DirectX::XMFLOAT4 ToDirectXColor() const {
            return DirectX::XMFLOAT4(r, g, b, a);
        }

        void ToOpenGLColor(float& outR, float& outG, float& outB, float& outA) const {
            outR = r;
            outG = g;
            outB = b;
            outA = a;
        }
    };

    void drawRectangle(float x, float y, float width, float height, const Color& color, float strokeWidth, const Color& strokeColor, float radius)
    {
        if (type == DeviceType::D3D11 || type == DeviceType::D3D12) {
            
        }
        else if (type == DeviceType::OpenGL) {
            
        }
    };

};