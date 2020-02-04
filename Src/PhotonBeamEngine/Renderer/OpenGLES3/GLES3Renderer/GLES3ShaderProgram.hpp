#ifndef GLES3ShaderProgram_hpp
#define GLES3ShaderProgram_hpp

#define GLES_SILENCE_DEPRECATION

#include <OpenGLES/ES3/gl.h>

#include "Matrix.hpp"
#include "VertexBuffer.hpp"
#include "Optional.hpp"
#include "Noncopyable.hpp"

namespace Pht {
    class GLES3ShaderProgram: public Noncopyable {
    public:
        struct UniformHandles {
            GLint mProjection {0};
            GLint mModelViewProjection {0};
            GLint mNormalMatrix {0};
            GLint mLightPosition {0};
            GLint mAmbientMaterial {0};
            GLint mDiffuseMaterial {0};
            GLint mSpecularMaterial {0};
            GLint mEmissiveMaterial {0};
            GLint mShininess {0};
            GLint mReflectivity {0};
            GLint mOpacity {0};
            GLint mTextColor {0};
            GLint mTextTopColorSubtraction {0};
            GLint mTextMidColorSubtraction {0};
            GLint mSampler {0};
            GLint mSecondSampler {0};
            GLint mModel {0};
            GLint mModel3x3 {0};
            GLint mCameraPosition {0};
        };
        
        struct AttributeHandles {
            GLint mPosition {0};
            GLint mNormal {0};
            GLint mTextureCoord {0};
            GLint mColor {0};
            GLint mTextCoords {0};
            GLint mTextGradientFunction {0};
            GLint mPointSize {0};
        };
        
        GLES3ShaderProgram(const VertexFlags& vertexFlags);
        
        void Build(const char* vertexShaderSource, const char* fragmentShaderSource);
        void SetProjection(const Mat4& projectionMatrix);
        void SetCameraPosition(const Vec3& cameraPosition);
        void SetLightPosition(const Vec3& lightPosition);
        void Use() const;
        
        const UniformHandles& GetUniforms() const {
            return mUniforms;
        }
        
        const AttributeHandles& GetAttributes() const {
            return mAttributes;
        }
        
        VertexFlags GetVertexFlags() const {
            return mVertexFlags;
        }
        
        void SetIsEnabled(bool isEnabled) {
            mIsEnabled = isEnabled;
        }
        
        bool IsEnabled() const {
            return mIsEnabled;
        }
    
    private:
        GLuint mProgram {0};
        UniformHandles mUniforms;
        AttributeHandles mAttributes;
        VertexFlags mVertexFlags;
        Optional<Vec3> mLightPosition;
        Optional<Vec3> mCameraPosition;
        bool mIsEnabled {true};
    };
}

#endif
