#include "Hitable.h"
#include "Util.h"
#include "Material.h"

bool Translate::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
    Ray moveRay(r.Origin() - offset, r.Direction(), r.Time());
    if (ptr->Hit(moveRay, tmin, tmax, rec))
    {
        rec.p += offset;
        return true;
    }
    return false;
}

bool Translate::BoundingBox(float t0, float t1, AABB& box) const
{
    if (ptr->BoundingBox(t0, t1, box))
    {
        box = AABB(box.Min() + offset, box.Max() + offset);
        return true;
    }
    return false;
}

RotateY::RotateY(Hitable* obj, float angle)
    : ptr(obj)
{
    float radians = (float)(PI / 180.f) * angle;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasBox = ptr->BoundingBox(0, 1, _box);
    Vec3 min(FLT_MIN, FLT_MIN, FLT_MIN);
    Vec3 max(FLT_MAX, FLT_MAX, FLT_MAX);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                float x = i * _box.Max().X() + (1 - i) * _box.Min().X();
                float y = j * _box.Max().Y() + (1 - j) * _box.Min().Y();
                float z = k * _box.Max().Z() + (1 - k) * _box.Min().Z();

                float newX = cosTheta * x + sinTheta * z;
                float newZ = -sinTheta * x + cosTheta * z;
                Vec3 tester(newX, y, newZ);
                for (int c = 0; c < 3; ++c)
                {
                    if (tester[c] > max[c])
                    {
                        max[c] = tester[c];
                    }
                    if (tester[c] < min[c])
                    {
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    _box = AABB(min, max);
}

bool RotateY::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
    Vec3 origin = r.Origin();
    Vec3 direction = r.Direction();
    origin[0] = cosTheta * r.Origin()[0] - sinTheta * r.Origin()[2];
    origin[2] = sinTheta * r.Origin()[0] + cosTheta * r.Origin()[2];

    direction[0] = cosTheta * r.Direction()[0] - sinTheta * r.Direction()[2];
    direction[2] = sinTheta * r.Direction()[0] + cosTheta * r.Direction()[2];

    Ray rotateRay(origin, direction, r.Time());

    if (ptr->Hit(rotateRay, tmin, tmax, rec))
    {
        Vec3 p = rec.p;
        Vec3 normal = rec.normal;

        p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
        p[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];

        normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
        normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

        rec.p = p;
        rec.normal = normal;
        return true;
    }

    return false;
}

bool RotateY::BoundingBox(float t0, float t1, AABB& box) const
{
    box = _box;
    return hasBox;
}

ConstantMedium::ConstantMedium(Hitable* obj, float d, Texture* a)
    : ptr(obj), density(d) 
{
    phaseFunc = new Isotropic(a);
}

bool ConstantMedium::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
    bool db = Random() < 0.00001f;
    db = false;
    HitRecord rec1, rec2;
    if (ptr->Hit(r, -FLT_MAX, FLT_MAX, rec1))
    {
        if (ptr->Hit(r, rec1.t + 0.001f, FLT_MAX, rec2))
        {
            if (db)
            {
                std::cerr << "t0 t2 " << rec1.t << " " << rec2.t << endl;
            }
            if (rec1.t < tmin)
            {
                rec1.t = tmin;
            }
            if (rec2.t > tmax)
            {
                rec2.t = tmax;
            }
            if (rec1.t >= rec2.t)
            {
                return false;
            }
            if (rec1.t < 0)
            {
                rec1.t = 0;
            }

            float distanceInsideBoundary = (rec2.t - rec1.t) * r.Direction().Length();
            float hitDistance = -(1 / density) * log(Random());
            if (hitDistance < distanceInsideBoundary)
            {
                if (db)
                {
                    std::cerr << "HitDistance = " << hitDistance << endl;
                }

                rec.t = rec1.t + hitDistance / r.Direction().Length();
                if (db)
                {
                    std::cerr << "rec.t = " << rec.t << endl;
                }

                rec.p = r.PointAtParameter(rec.t);
                if (db)
                {
                    std::cerr << "rec.p = " << rec.p << endl;
                }

                rec.normal = Vec3(1, 0, 0);
                rec.pMat = phaseFunc;
                return true;
            }
        }
    }
    return false;
}

bool ConstantMedium::BoundingBox(float t0, float t1, AABB& box) const
{
    return ptr->BoundingBox(t0, t1, box);
}
