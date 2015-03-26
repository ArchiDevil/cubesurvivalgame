#pragma once

#include "ISceneNode.h"

namespace ShiftEngine
{
    enum LightNodeType
    {
        LNT_Point,
        LNT_Directional,
        //LNT_Omni,
    };

    class LightNode : public ISceneNode
    {
    public:
        LightNode(LightNodeType type, const MathLib::Vector3F & color = Vector3F(1.0f, 1.0f, 1.0f));
        ~LightNode();

        virtual void PushToRQ(RenderQueue & rq) override;
        void SetColor(const MathLib::Vector3F & color);
        MathLib::Vector3F GetColor() const;

        LightNodeType GetType() const;

        float GetRadius() const;
        void SetRadius(float val);

        MathLib::Vector3F GetDirection() const;
        void SetDirection(const MathLib::Vector3F & val);

        void SetActive(bool active);
        bool IsActive() const;

        virtual void KillSelf();
        virtual MathLib::AABB GetBBox() const override;

        LightNode(const LightNode & ref) = delete;
        LightNode(LightNode && ref) = delete;
        LightNode& operator=(const LightNode & ref) = delete;
        LightNode& operator=(LightNode && ref) = delete;

    private:
        MathLib::Vector3F color;
        const LightNodeType type;

        //for directional light source type
        MathLib::Vector3F direction;

        //for point light source type
        float radius;

        bool active;
    };
}
