#include "Box.h"

Box::Box(const Vec3& p0, const Vec3& p1, Material* mat)
{
    pMin = p0;
    pMax = p1;
    assemblyBox = new HitableList();

    assemblyBox->AddObject(new Rect(p0.X(), p1.X(), p0.Y(), p1.Y(), p1.Z(), mat, RectType::XY));
    assemblyBox->AddObject(new FlipNormal(new Rect(p0.X(), p1.X(), p0.Y(), p1.Y(), p0.Z(), mat, RectType::XY)));

    assemblyBox->AddObject(new Rect(p0.X(), p1.X(), p0.Z(), p1.Z(), p1.Y(), mat, RectType::XZ));
    assemblyBox->AddObject(new FlipNormal(new Rect(p0.X(), p1.X(), p0.Z(), p1.Z(), p0.Y(), mat, RectType::XZ)));

    assemblyBox->AddObject(new Rect(p0.Y(), p1.Y(), p0.Z(), p1.Z(), p1.X(), mat, RectType::YZ));
    assemblyBox->AddObject(new FlipNormal(new Rect(p0.Y(), p1.Y(), p0.Z(), p1.Z(), p0.X(), mat, RectType::YZ)));
}

bool Box::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
    return assemblyBox->Hit(r, tmin, tmax, rec);
}

bool Box::BoundingBox(float t0, float t1, AABB& box) const
{
    box = AABB(pMin, pMax);
    return true;
}
