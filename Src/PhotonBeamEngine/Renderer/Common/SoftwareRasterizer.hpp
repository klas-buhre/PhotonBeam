#ifndef SoftwareRasterizer_hpp
#define SoftwareRasterizer_hpp

#include <memory>
#include <vector>

#include "Vector.hpp"
#include "Material.hpp"

namespace Pht {
    class IImage;
    
    enum class DrawOver {
        Yes,
        No
    };
    
    class SoftwareRasterizer {
    public:
        struct HorizontalGradientColors {
            const Vec4 mLeft;
            const Vec4 mRight;
        };

        struct VerticalGradientColors {
            const Vec4 mBottom;
            const Vec4 mTop;
        };

        SoftwareRasterizer(const Vec2& coordinateSystemSize, const IVec2& imageSize);
        
        void ClearBuffer();
        void SetStencilBufferFillMode();
        void ClearStencilBuffer();
        void EnableStencilTest();
        void SetBlend(Blend blend);
        void DrawRectangle(const Vec2& upperRight,
                           const Vec2& lowerLeft,
                           const Vec4& color,
                           DrawOver drawOver = DrawOver::No);
        void DrawGradientRectangle(const Vec2& upperRight,
                                   const Vec2& lowerLeft,
                                   const HorizontalGradientColors& colors,
                                   DrawOver drawOver = DrawOver::No);
        void DrawGradientRectangle(const Vec2& upperRight,
                                   const Vec2& lowerLeft,
                                   const VerticalGradientColors& colors,
                                   DrawOver drawOver = DrawOver::No);
        void DrawTiltedTrapezoid45(const Vec2& upperRight,
                                   const Vec2& lowerLeft,
                                   float width,
                                   const Vec4& color,
                                   DrawOver drawOver = DrawOver::No);
        void DrawTiltedTrapezoid135(const Vec2& upperLeft,
                                    const Vec2& lowerRight,
                                    float width,
                                    const Vec4& color,
                                    DrawOver drawOver = DrawOver::No);
        void DrawTiltedTrapezoid225(const Vec2& upperLeft,
                                    const Vec2& lowerRight,
                                    float width,
                                    const Vec4& color,
                                    DrawOver drawOver = DrawOver::No);
        void DrawTiltedTrapezoid315(const Vec2& upperRight,
                                    const Vec2& lowerLeft,
                                    float width,
                                    const Vec4& color,
                                    DrawOver drawOver = DrawOver::No);
        void DrawCircle(const Vec2& center,
                        float radius,
                        float width,
                        const Vec4& color,
                        DrawOver drawOver = DrawOver::No);
        void FillEnclosedArea(const Vec4& color);
        std::unique_ptr<IImage> ProduceImage() const;
        
    private:
        enum class ScanlineState {
            Outside,
            FirstBorder,
            Inside,
            SecondBorder
        };
        
        IVec2 ToPixelCoordinates(const Vec2& point) const;
        void SetPixel(int x, int y, const Vec4& color, DrawOver drawOver = DrawOver::No);
        void SetPixelInNormalDrawMode(const Vec4& color, DrawOver drawOver, int offset);
        void SetPixelNormalBlend(const Vec4& color, int offset);
        void SetPixelAdditiveBlend(const Vec4& color, int offset);
        void SetPixelInStencilBufferFillMode(const Vec4& color, DrawOver drawOver, int offset);
        const Vec4& GetPixel(int x, int y) const;
        bool ShouldSkipLine(int y) const;
        ScanlineState UpdateScanlineFsm(int x, int y, ScanlineState state) const;
        
        enum class DrawMode {
            Normal,
            StencilBufferFill,
            StencilTest
        };

        DrawMode mDrawMode {DrawMode::Normal};
        Blend mBlend {Blend::No};
        Vec2 mCoordSystemSize;
        IVec2 mImageSize;
        std::vector<Vec4> mBuffer;
        std::vector<Vec4> mStencilBuffer;
    };
}

#endif
