
#include "Colli2D.h"

namespace Utl
{
    using namespace cinder;

    bool Colli2D::rectPoint(const Vec2f& pos, const Vec2f& size, const Vec2f& otherPos)
    {
        if (pos.x < otherPos.x && otherPos.x < pos.x + size.x &&
            pos.y < otherPos.y && otherPos.y < pos.y + size.y)
            return true;
        else return false;
    }
    bool Colli2D::rectRect(const Vec2f& pos, const Vec2f& size, const Vec2f& otherPos, const Vec2f& otherSize)
    {
        if (pos.x < otherPos.x + otherSize.x && otherPos.x < pos.x + size.x &&
            pos.y < otherPos.y + otherSize.y && otherPos.y < pos.y + size.y)
            return true;
        else return false;
    }
    bool Colli2D::rectCircle(const Vec2f& pos, const Vec2f& size, const Vec2f& otherPos, const float& otherSize)
    {
        bool A, B, C, D, E, F;

        const Vec2f r1 = pos;
        const Vec2f r2 = pos + size;
        const Vec2f c = otherPos;
        const float r = otherSize;

        A = (r1.x < c.x) & (c.x < r2.x) & (r1.y - r < c.y) & (c.y < r2.y + r);
        B = (r1.x - r < c.x) & (c.x < r2.x + r) & (r1.y < c.y) & (c.y < r2.y);
        C = (r1.x - c.x) * (r1.x - c.x) + (r1.y - c.y) * (r1.y - c.y) < r * r;
        D = (r2.x - c.x) * (r2.x - c.x) + (r1.y - c.y) * (r1.y - c.y) < r * r;
        E = (r2.x - c.x) * (r2.x - c.x) + (r2.y - c.y) * (r2.y - c.y) < r * r;
        F = (r1.x - c.x) * (r1.x - c.x) + (r2.y - c.y) * (r2.y - c.y) < r * r;

        return A | B | C | D | E | F;
    }
    bool Colli2D::circlePoint(const Vec2f& pos, const float& size, const Vec2f& otherPos)
    {
        const Vec2f radius_Pos = otherPos - pos;
        const float radius = size;
        if (radius_Pos.x * radius_Pos.x + radius_Pos.y * radius_Pos.y
            < radius * radius)
            return true;
        else return false;
    }
    bool Colli2D::circleCircle(const Vec2f& pos, const float& size, const Vec2f& otherPos, const float& otherSize)
    {
        const Vec2f radius_Pos = otherPos - pos;
        const float radius = size + otherSize;
        if (radius_Pos.x * radius_Pos.x + radius_Pos.y * radius_Pos.y
            < radius * radius)
            return true;
        else return false;
    }
    bool Colli2D::ellipsePoint(const Vec2f& pos, const Vec2f& size, const float& angle, const Vec2f& otherPos)
    {
        float Ofs_x = otherPos.x - pos.x;
        float Ofs_y = otherPos.y - pos.y;
        float After_x = Ofs_x * cos(angle) + Ofs_y * sin(angle);
        float After_y = size.x / size.y * (-Ofs_x * sin(angle) + Ofs_y * cos(angle));

        if (After_x * After_x + After_y * After_y <= size.x * size.x)
            return true;
        return false;

    }
}