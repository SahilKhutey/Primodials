#include <catch2/catch_all.hpp>
#include "ShapeEngine/Math/Quaternion.hpp"
#include "ShapeEngine/Math/Transform.hpp"

using namespace Shape::Math;
using namespace Shape::Math::Literals;

TEST_CASE("Quaternion core operations", "[Math][Quaternion]") {
    SECTION("Identity") {
        Quaternionf id = Quaternionf::Identity();
        REQUIRE(NearEquals(id.w, 1.0f));
        REQUIRE(NearEquals(id.x, 0.0f));
        REQUIRE(id.NearIdentity());
    }

    SECTION("AxisAngle construction") {
        Quaternionf q = Quaternionf::AxisAngle(Vector3f::UnitY(), 90.0_deg);
        // Rotate UnitX by 90deg around Y => should become -UnitZ
        Vector3f v = q.Rotate(Vector3f::UnitX());
        REQUIRE(NearEquals(v.x,  0.0f));
        REQUIRE(NearEquals(v.y,  0.0f));
        REQUIRE(NearEquals(v.z, -1.0f));
    }

    SECTION("Normalize") {
        Quaternionf q(1.0f, 2.0f, 3.0f, 4.0f);
        Quaternionf n = q.Normalized();
        REQUIRE(NearEquals(n.Length(), 1.0f));
    }

    SECTION("Conjugate and Inverse") {
        Quaternionf q = Quaternionf::AxisAngle(Vector3f::UnitZ(), 45.0_deg);
        Quaternionf conj = q.Conjugate();
        Quaternionf prod = q * conj;
        REQUIRE(NearEquals(prod.w, 1.0f));
        REQUIRE(NearEquals(prod.x, 0.0f));
    }

    SECTION("Quaternion multiplication order") {
        Quaternionf qx = Quaternionf::AxisAngle(Vector3f::UnitX(), 90.0_deg);
        Quaternionf qy = Quaternionf::AxisAngle(Vector3f::UnitY(), 90.0_deg);
        Quaternionf combined = qy * qx;
        // Not commutative
        Quaternionf combined2 = qx * qy;
        REQUIRE(combined != combined2);
    }

    SECTION("SLERP interpolation") {
        Quaternionf a = Quaternionf::Identity();
        Quaternionf b = Quaternionf::AxisAngle(Vector3f::UnitY(), 90.0_deg);

        Quaternionf mid = Quaternionf::SLerp(a, b, 0.5f);
        REQUIRE(NearEquals(mid.Length(), 1.0f));

        Quaternionf atStart = Quaternionf::SLerp(a, b, 0.0f);
        REQUIRE(NearEquals(atStart.w, a.w));

        Quaternionf atEnd = Quaternionf::SLerp(a, b, 1.0f);
        REQUIRE(NearEquals(atEnd.w, b.w));
    }

    SECTION("NLERP interpolation") {
        Quaternionf a = Quaternionf::Identity();
        Quaternionf b = Quaternionf::AxisAngle(Vector3f::UnitZ(), 180.0_deg);
        Quaternionf mid = Quaternionf::NLerp(a, b, 0.5f);
        REQUIRE(NearEquals(mid.Length(), 1.0f));
    }

    SECTION("Difference and FromToRotation") {
        Quaternionf q = Quaternionf::AxisAngle(Vector3f::UnitX(), 45.0_deg);
        Quaternionf id = Quaternionf::Identity();
        Quaternionf diff = Quaternionf::Difference(id, q);
        REQUIRE(NearEquals(diff.w, q.w));
    }

    SECTION("EulerXYZ round-trip") {
        Quaternionf q = Quaternionf::EulerXYZ(30.0_deg, 45.0_deg, Radians(0.0f));
        // Should be a unit quaternion
        REQUIRE(NearEquals(q.Length(), 1.0f));
    }
}

TEST_CASE("Transform core operations", "[Math][Transform]") {
    SECTION("Identity transform") {
        Transformf t = Transformf::Identity();
        Vector3f p(5.0f, 3.0f, 1.0f);
        Vector3f result = t.TransformPoint(p);
        REQUIRE(NearEquals(result.x, p.x));
        REQUIRE(NearEquals(result.y, p.y));
        REQUIRE(NearEquals(result.z, p.z));
    }

    SECTION("Translation only") {
        Transformf t;
        t.position = Vector3f(10.0f, 0.0f, 0.0f);
        Vector3f p(0.0f, 0.0f, 0.0f);
        Vector3f result = t.TransformPoint(p);
        REQUIRE(NearEquals(result.x, 10.0f));
    }

    SECTION("Composition is hierarchical") {
        Transformf parent;
        parent.position = Vector3f(5.0f, 0.0f, 0.0f);

        Transformf child;
        child.position = Vector3f(1.0f, 0.0f, 0.0f);

        Transformf world = Transformf::Compose(parent, child);
        REQUIRE(NearEquals(world.position.x, 6.0f));
    }

    SECTION("Lerp interpolation") {
        Transformf a = Transformf::Identity();
        Transformf b;
        b.position = Vector3f(10.0f, 0.0f, 0.0f);
        Transformf mid = Transformf::Lerp(a, b, 0.5f);
        REQUIRE(NearEquals(mid.position.x, 5.0f));
    }
}
