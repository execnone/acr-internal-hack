#pragma once
#include <corecrt_math.h>

#define UFINVSQRT(x)  union { int d; float f; } u; u.f = x; u.d = 0x5f3759df - (u.d >> 1)
inline float ufInvSqrt(float x) { UFINVSQRT(x); return u.f; } // about 3.5% of error
inline float fInvSqrt(float x) { UFINVSQRT(x); return 0.5f * u.f * (3.00175f - x * u.f * u.f); } // about 0.1% of error
inline float fSqrt(float x) { return x * fInvSqrt(x); }
inline float ufSqrt(float x) { return x * ufInvSqrt(x); }
inline float fACos(float x)
{
    int s = 1;
    float y, r = 0;
    if (x < 0)
    {
        s = -1;
        r = 2.0f;
        y = 1.0f + x;
    }
    else y = 1.0f - x;
    UFINVSQRT(y);
    u.f = 0.5f * u.f * (3.0f - y * u.f * u.f);
    u.f = y * 0.5f * u.f * (3.0f - y * u.f * u.f);
    return 1.57079632f * (r + s * u.f * (0.9003163f + y * (0.07782684f + y * (0.006777598f + y * 0.015079262f))));
}
#undef UFINVSQRT

struct vec
{
    union
    {
        struct { float x, y, z; };
        float v[3];
        int i[3];
    };

    vec() { x = y = z = 0; }
    vec(float a, float b, float c) : x(a), y(b), z(c) {}
    vec(float* v) : x(v[0]), y(v[1]), z(v[2]) {}

    float& operator[](int i) { return v[i]; }
    float  operator[](int i) const { return v[i]; }

    bool iszero() const { return x == 0 && y == 0 && z == 0; }

    bool operator==(const vec& o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const vec& o) const { return x != o.x || y != o.y || z != o.z; }
    vec operator-() const { return vec(-x, -y, -z); }

    vec& mul(float f) { x *= f; y *= f; z *= f; return *this; }
    vec& div(float f) { x /= f; y /= f; z /= f; return *this; }
    vec& add(float f) { x += f; y += f; z += f; return *this; }
    vec& sub(float f) { x -= f; y -= f; z -= f; return *this; }

    vec& add(const vec& o) { x += o.x; y += o.y; z += o.z; return *this; }
    vec& sub(const vec& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }

    float squaredlen() const { return x * x + y * y + z * z; }
    float sqrxy() const { return x * x + y * y; }

    float dot(const vec& o) const { return x * o.x + y * o.y + z * o.z; }
    float dotxy(const vec& o) const { return x * o.x + y * o.y; }

    float magnitude() const { return sqrtf(squaredlen()); }
    vec& normalize() { if (magnitude()) div(magnitude()); return *this; }

    // should NOT be used
    float fmag() const { return fSqrt(squaredlen()); }
    float ufmag() const { return ufSqrt(squaredlen()); }
    float fmagxy() const { return fSqrt(x * x + y * y); }
    float ufmagxy() const { return ufSqrt(x * x + y * y); }

    float dist(const vec& e) const { vec t; return dist(e, t); }
    float dist(const vec& e, vec& t) const { t = *this; t.sub(e); return t.magnitude(); }

    float distxy(const vec& e) const { float dx = e.x - x, dy = e.y - y; return sqrtf(dx * dx + dy * dy); }
    float magnitudexy() const { return sqrtf(x * x + y * y); }

    bool reject(const vec& o, float max) const { return x > o.x + max || x<o.x - max || y>o.y + max || y < o.y - max; }

    vec& cross(const vec& a, const vec& b) { x = a.y * b.z - a.z * b.y; y = a.z * b.x - a.x * b.z; z = a.x * b.y - a.y * b.x; return *this; }
    float cxy(const vec& a) { return x * a.y - y * a.x; }

    vec& rotate_around_z(float angle) { *this = vec(cosf(angle) * x - sinf(angle) * y, cosf(angle) * y + sinf(angle) * x, z); return *this; }
    vec& rotate_around_x(float angle) { *this = vec(x, cosf(angle) * y - sinf(angle) * z, cosf(angle) * z + sinf(angle) * y); return *this; }
    vec& rotate_around_y(float angle) { *this = vec(cosf(angle) * x - sinf(angle) * z, y, cosf(angle) * z + sinf(angle) * x); return *this; }

    vec& rotate(float angle, const vec& d)
    {
        float c = cosf(angle), s = sinf(angle);
        return rotate(c, s, d);
    }

    vec& rotate(float c, float s, const vec& d)
    {
        *this = vec(x * (d.x * d.x * (1 - c) + c) + y * (d.x * d.y * (1 - c) - d.z * s) + z * (d.x * d.z * (1 - c) + d.y * s),
            x * (d.y * d.x * (1 - c) + d.z * s) + y * (d.y * d.y * (1 - c) + c) + z * (d.y * d.z * (1 - c) - d.x * s),
            x * (d.x * d.z * (1 - c) - d.y * s) + y * (d.y * d.z * (1 - c) + d.x * s) + z * (d.z * d.z * (1 - c) + c));
        return *this;
    }

    void orthogonal(const vec& d)
    {
        int i = fabs(d.x) > fabs(d.y) ? (fabs(d.x) > fabs(d.z) ? 0 : 2) : (fabs(d.y) > fabs(d.z) ? 1 : 2);
        v[i] = d[(i + 1) % 3];
        v[(i + 1) % 3] = -d[i];
        v[(i + 2) % 3] = 0;
    }
};

#define loop(v,m) for(int v = 0; v<int(m); v++)
#define loopi(m) loop(i,m)
class worldobject
{
public:
    virtual ~worldobject() {};
};

class physent : public worldobject
{
public:
    vec o, vel, vel_t;                         // origin, velocity
    vec deltapos, newpos;                       // movement interpolation
    float yaw, pitch, roll;             // used as vec in one place
    float pitchvel, yawvel;
    float maxspeed;                     // cubes per second, 24 for player
    int timeinair;                      // used for fake gravity
    float radius, eyeheight, maxeyeheight, aboveeye;  // bounding box size
    bool inwater;
    bool onfloor, onladder, jumpnext, crouching, crouchedinair, trycrouch, sprinting, cancollide, stuck, shoot;
    int lastjump;
    float lastjumpheight;
    int lastsplash;
    char move, strafe;
    unsigned char state, type;
    float eyeheightvel;
    int last_pos;
    float zoomed;

    physent() : o(0, 0, 0), deltapos(0, 0, 0), newpos(0, 0, 0), yaw(270), pitch(0), roll(0), pitchvel(0), yawvel(0),
        crouching(false), crouchedinair(false), trycrouch(false), sprinting(false), cancollide(true), stuck(false), shoot(false), lastjump(0), lastjumpheight(0), lastsplash(0), state(0), last_pos(0)
    {
        reset();
    }
    virtual ~physent() {}

    void resetinterp()
    {
        newpos = o;
        newpos.z -= eyeheight;
        deltapos = vec(0, 0, 0);
    }

    void reset()
    {
        vel.x = vel.y = vel.z = eyeheightvel = vel_t.x = vel_t.y = vel_t.z = 0.0f;
        move = strafe = 0;
        timeinair = lastjump = lastsplash = 0;
        onfloor = onladder = inwater = jumpnext = crouching = crouchedinair = trycrouch = sprinting = stuck = false;
        last_pos = 0;
        zoomed = 0;
    }

    virtual void oncollision() {}
    virtual void onmoved(const vec& dist) {}
};

struct animstate                                // used for animation blending of animated characters
{
    int anim, frame, range, basetime;
    float speed;
    animstate() { reset(); }
    void reset() { anim = frame = range = basetime = 0; speed = 100.0f; };

    bool operator==(const animstate& o) const { return frame == o.frame && range == o.range && basetime == o.basetime && speed == o.speed; }
    bool operator!=(const animstate& o) const { return frame != o.frame || range != o.range || basetime != o.basetime || speed != o.speed; }
};


class dynent : public physent                 // animated ent
{
public:
    bool k_left, k_right, k_up, k_down;         // see input code

    animstate prev[2], current[2];              // md2's need only [0], md3's need both for the lower&upper model
    int lastanimswitchtime[2];
    void* lastmodel[2];
    int lastrendered;

    void stopmoving()
    {
        k_left = k_right = k_up = k_down = jumpnext = false;
        move = strafe = 0;
    }

    void resetanim()
    {
        loopi(2)
        {
            prev[i].reset();
            current[i].reset();
            lastanimswitchtime[i] = -1;
            lastmodel[i] = NULL;
        }
        lastrendered = 0;
    }

    void reset()
    {
        physent::reset();
        stopmoving();
    }

    dynent() { reset(); resetanim(); }
    virtual ~dynent() {}
};

struct traceresult_s
{
    vec end;
    bool collided;
};