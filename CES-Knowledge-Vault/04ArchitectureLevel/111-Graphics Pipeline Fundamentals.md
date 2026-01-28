---
Theme: Graphics Pipeline Fundamentals
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, GPU, Graphics]
---

## 📚 Idea/Concept

The graphics pipeline is a sequence of processing stages that transforms 3D scene data into 2D images displayed on screen, implemented in modern GPUs as a combination of fixed-function and programmable shader stages.

### Pipeline Overview

```
Application → Geometry Processing → Rasterization → Pixel Processing → Display
     │               │                   │                │
   (CPU)           (GPU)              (GPU)            (GPU)
```

### Detailed Pipeline Stages

```
┌─────────────────────────────────────────────────────────────┐
│                    Application (CPU)                         │
│  - Scene management, animation, AI, physics                 │
│  - Generate draw calls with vertex/index data               │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│              Vertex Specification (Input Assembly)           │
│  - Read vertex data from buffers                            │
│  - Assemble vertices into primitives                        │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                   Vertex Shader ★                           │
│  - Transform vertices (model → world → view → clip space)   │
│  - Per-vertex lighting, texcoord generation                 │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│              Tessellation Shaders ★ (Optional)              │
│  - Subdivide primitives for detail                          │
│  - Hull shader → Tessellator → Domain shader                │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                Geometry Shader ★ (Optional)                 │
│  - Process entire primitives                                │
│  - Can generate/destroy primitives                          │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│            Primitive Assembly & Clipping                     │
│  - Assemble vertices into primitives                        │
│  - Clip against view frustum                                │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                     Rasterization                            │
│  - Convert primitives to fragments                          │
│  - Interpolate vertex attributes                            │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                  Fragment Shader ★                          │
│  - Compute pixel color                                      │
│  - Texturing, lighting, effects                             │
└──────────────────────────┬──────────────────────────────────┘
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                 Per-Sample Operations                        │
│  - Depth test, stencil test, blending                       │
│  - Write to framebuffer                                     │
└─────────────────────────────────────────────────────────────┘

★ = Programmable shader stage
```

### Coordinate Transformations

```
Model Space → World Space → View Space → Clip Space → NDC → Screen Space
     │             │            │            │          │
  Model Matrix  View Matrix  Projection   Perspective  Viewport
                              Matrix      Division    Transform
```

Transformation matrices (4×4):
- Model: Position/orient object in world
- View: Camera position and orientation
- Projection: 3D to 2D projection (perspective/orthographic)

### Vertex Shader

Input: Vertex attributes (position, normal, texcoord, color)
Output: Transformed position, passed attributes

```glsl
// GLSL Vertex Shader Example
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    fragNormal = mat3(model) * normal;
}
```

### Rasterization

Converts primitives (triangles) to fragments:
1. Determine which pixels triangle covers
2. Generate fragment for each covered pixel
3. Interpolate vertex attributes across triangle

Interpolation uses barycentric coordinates:
$$P = \alpha \cdot V_0 + \beta \cdot V_1 + \gamma \cdot V_2$$

where α + β + γ = 1

### Fragment Shader

Input: Interpolated attributes from rasterizer
Output: Final pixel color (and optionally depth)

```glsl
// GLSL Fragment Shader Example
#version 330 core
in vec3 fragNormal;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 fragColor;

void main() {
    float diff = max(dot(normalize(fragNormal), -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    fragColor = vec4(diffuse * objectColor, 1.0);
}
```

### Per-Fragment Operations

Depth Test:
- Compare fragment depth to depth buffer
- Discard if behind existing geometry
- Enables correct occlusion

Stencil Test:
- Compare against stencil buffer value
- Used for masking, shadows, special effects

Blending:
- Combine fragment color with existing color
- Enables transparency: Final = α·Source + (1-α)·Dest

### Modern Rendering Techniques

Deferred Rendering:
1. Render geometry attributes to G-buffer
2. Apply lighting in screen space
3. Decouples geometry from lighting complexity

Forward+ Rendering:
- Tile-based light culling
- Efficient for many lights

Ray Tracing (RTX):
- Programmable ray intersection
- Realistic reflections, shadows, global illumination
- Hybrid with rasterization

### GPU Shader Units

Modern GPUs use unified shaders:
- Same hardware runs all shader types
- Dynamic allocation based on workload
- Enables compute shaders (GPGPU)

Specialized units:
- Texture units: Filtering, sampling
- ROPs (Render Output Units): Blending, depth test
- RT cores: Ray-triangle intersection
- Tensor cores: Matrix operations (AI)

### Performance Considerations

Bottleneck identification:
- Vertex-bound: Too many vertices, complex vertex shader
- Rasterization-bound: Many small triangles
- Fragment-bound: Complex pixel shader, overdraw
- Memory-bound: Large textures, bandwidth limits

Optimization:
- Level of Detail (LOD) for distant objects
- Frustum and occlusion culling
- Texture compression
- Batching draw calls

## 📌 Key Points (Optional)
- The graphics pipeline balances fixed-function efficiency with programmable flexibility.
- Understanding pipeline stages helps identify and fix rendering bottlenecks.

## 🖼️ Recommended Image
- Flowchart of graphics pipeline stages from vertices to pixels.

## 🔗 Connections
- [[110-GPU Architecture and SIMT Model]]
- [[112-CUDA Programming Model]]
- [[107-SIMD and Vector Architecture Fundamentals]]