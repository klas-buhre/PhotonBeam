#ifndef Transform_hpp
#define Transform_hpp

#include "Vector.hpp"
#include "Matrix.hpp"

namespace Pht {
    class Transform {
    public:
        void Reset();
        void SetPosition(const Vec3& position);
        void SetScale(const Vec3& scale);
        void SetScale(float scale);
        void SetRotation(const Vec3& rotation);
        void Translate(const Vec3& translation);
        void Scale(const Vec3& scale);
        void Rotate(const Vec3& rotation);
        Mat4 ToMatrix() const;
        
        bool HasChanged() const {
            return mHasChanged;
        }
   
        void SetHasChanged(bool hasChanged) {
            mHasChanged = hasChanged;
        }
        
        const Vec3& GetPosition() const {
            return mPosition;
        }
        
        const Vec3& GetScale() const {
            return mScale;
        }
        
        const Vec3& GetRotation() const {
            return mRotation;
        }
        
    private:
        bool mHasChanged {false};
        Vec3 mPosition {0.0f, 0.0f, 0.0f};
        Vec3 mScale {1.0f, 1.0f, 1.0f};
        Vec3 mRotation {0.0f, 0.0f, 0.0f};
    };
}

#endif
