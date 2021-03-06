#version 460
#extension GL_NV_ray_tracing : require

// TODO: necessary?
// #extension GL_EXT_nonuniform_qualifier : require

layout(binding = 0, set = 0) uniform accelerationStructureNV topLevelAS;
layout(binding = 3, set = 0) buffer Vertices { vec4 v[]; } vertices;
layout(binding = 4, set = 0) buffer Indices { uint i[]; } indices;

layout(location = 0) rayPayloadInNV vec3 hitValue;
layout(location = 2) rayPayloadNV bool isShadowed;  // TODO: why not location 1?

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texCoords;
    uint materialIndex;
    vec3 color;
};

// Size of Vertex structure in number of vec4.
const uint VertexSize = 3;

hitAttributeNV vec3 attribs;

Vertex readVertex(const uint index)
{
    const vec4 d0 = vertices.v[index * VertexSize + 0];
    const vec4 d1 = vertices.v[index * VertexSize + 1];
    const vec4 d2 = vertices.v[index * VertexSize + 2];

    Vertex v;
    v.position = d0.xyz;
    v.normal = vec3(d0.w, d1.xy);
    v.texCoords = d1.zw;
    v.materialIndex = floatBitsToInt(d2.x);
    v.color = d2.yzw;

    return v;
}

void main()
{
    const uint i0 = indices.i[3 * gl_PrimitiveID + 0];
    const uint i1 = indices.i[3 * gl_PrimitiveID + 1];
    const uint i2 = indices.i[3 * gl_PrimitiveID + 2];

    const Vertex v0 = readVertex(i0);
    const Vertex v1 = readVertex(i1);
    const Vertex v2 = readVertex(i2);

    const vec3 bary = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);
    const vec3 normal = normalize(v0.normal * bary.x + v1.normal * bary.y + v2.normal * bary.z);

    const vec3 LightVector = normalize(vec3(5.0, 4.0, 3.0));
    const vec3 radiance = vec3(max(dot(normal, LightVector), 0.2));

    const vec3 shadowRayOrigin = gl_WorldRayOriginNV + gl_HitTNV * gl_WorldRayDirectionNV;
    isShadowed = true;
    traceNV(
        topLevelAS,
        gl_RayFlagsOpaqueNV | gl_RayFlagsTerminateOnFirstHitNV | gl_RayFlagsSkipClosestHitShaderNV,
        0xFF,           // cullMask
        1,              // sbtRecordOffset: use second hit group
        0,              // sbtRecordStride
        1,              // missIndex: use second miss shader
        shadowRayOrigin,
        1.0e-4f,        // Tmin
        LightVector,
        1.0e+4f,        // Tmax
        2);             // payload: isShadowed

    hitValue = isShadowed ? radiance * 0.2 : radiance;
}
