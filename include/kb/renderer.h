#pragma once

#include <kb/config.h>
#include <kb/hash.h>
#include <kb/resource.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>

#include <kb/geometry.h>
#include <kb/texture.h>
#include <kb/font.h>

#ifdef __cplusplus
extern "C" {
#endif

//#####################################################################################################################
// Structs
//#####################################################################################################################

KB_HANDLE(CommandBufferHandle);
KB_HANDLE(IndexBufferHandle);
KB_HANDLE(VertexBufferHandle);
KB_HANDLE(MeshHandle);
KB_HANDLE(ProgramHandle);
KB_HANDLE(MaterialHandle);
KB_HANDLE(GizmoHandle);

//#####################################################################################################################
// Enums
//#####################################################################################################################

typedef enum {
  KB_DRAW_SINGLE    = 0,
  KB_CULL_INSTANCED = 1,
} DrawMode;

typedef enum {
  KB_BIND_TYPE_UNSUPPORTED    = 0,
  KB_BIND_TYPE_UNIFORM_BUFFER = 1,
  KB_BIND_TYPE_PUSH_CONSTANT  = 2,
  KB_BIND_TYPE_SAMPLER        = 3,
  KB_BIND_TYPE_STORAGE_BUFFER = 4,
} BindType;

typedef enum {
  KB_CULL_NONE  = 0,
  KB_CULL_BACK  = 1,
  KB_CULL_FRONT = 2,
  KB_CULL_BOTH  = 3,
} CullMode;

typedef enum {
  KB_TOPOLOGY_TRIANGLE_STRIP  = 0,
  KB_TOPOLOGY_TRIANGLE_LIST   = 1,
  KB_TOPOLOGY_LINE_STRIP      = 2,
  KB_TOPOLOGY_LINE_LIST       = 3,
  KB_TOPOLOGY_POINT_LIST      = 4,
} TopologyType;

//#####################################################################################################################
// Structs
//#####################################################################################################################

struct SimpleVertex {
  Float3 pos;
  Float4 color;
  Float2 texcoord;
};

typedef struct {
  uint32_t  primitive_count;
  uint32_t  command_buffer_count;
  uint32_t  transient_memory_use;
  uint32_t  draw_calls;
  uint32_t  vertex_buffer_count;
  uint32_t  index_buffer_count;
  uint32_t  program_count;
  uint32_t  texture_count;
  float     frametime_avg;
  float     frametime_min;
  float     frametime_max;
} FrameStats;

typedef struct {
  VertexBufferHandle  vertex_buffer;
  IndexBufferHandle   index_buffer;
  
  uint32_t            material_count;
  MaterialHandle*     materials;

  uint32_t            mesh_count;
  MeshHandle*         meshes;
} GeometryCreateInfo;

typedef struct {
  int param;
} CommandBufferCreateInfo;

typedef struct  {
  Hash    hash;
  TextureHandle texture;
} SamplerBinding;

typedef struct {
  Hash    hash;
  Float4  data;
} UniformBinding;

typedef struct {
  ProgramHandle   program;

  uint32_t        uniform_count;
  UniformBinding* uniforms;

  uint32_t        sampler_count;
  SamplerBinding* samplers;
} MaterialCreateInfo;

typedef struct {
  RWops*      rwops;
  TextureInfo texture;
  SamplerInfo sampler;
} TextureCreateInfo;

typedef struct {
  RWops*    rwops;
  uint32_t  index_size;
  uint32_t  size;
  bool      host_mapped;
} IndexBufferCreateInfo;

typedef struct {
  RWops*        rwops;
  VertexLayout  layout;
  uint32_t      size;
  bool          host_mapped;
} VertexBufferCreateInfo;

typedef struct {
  VertexBufferHandle  vertex_buffer;
  IndexBufferHandle   index_buffer;

  uint32_t            primitive_count;
  Primitive*          primitives;

  uint32_t            material_count;
  MaterialHandle*     materials;

} MeshCreateInfo;

typedef struct {
  FontBaseInfo  base_info;
  ProgramHandle program;
  
  TextureHandle atlas;
  
  uint32_t      atlas_height;
  uint32_t      atlas_width;
  uint32_t      char_count;
  FontChar*     chars;
} FontCreateInfo;

typedef struct {
  RWops* vert_code_rwops;
  RWops* frag_code_rwops;
  
  const char* vert_entry = "main";
  const char* frag_entry = "main";

  CullMode      cull        = KB_CULL_BACK;
  TopologyType  topology    = KB_TOPOLOGY_TRIANGLE_LIST;
  DrawMode      mode        = KB_DRAW_SINGLE;
  bool          depth_write = true;
  bool          depth_test  = true;
} ProgramCreateInfo;

typedef struct {
  uint32_t  msaa;
  bool      vsync;
  Int2      resolution;
} GraphicsInitInfo;

typedef struct {
  int param;
} BeginInfo;

typedef struct {
  BindType  type;
  uint32_t  set;
  uint32_t  binding;
  uint64_t  size;
  uint64_t  offset;
  uint64_t  block_size;
} BindSlot;

//#####################################################################################################################
// Object management
//#####################################################################################################################

KB_RESOURCE_HASHED_FUNC_DECLS (index_buffer   , IndexBufferHandle   , IndexBufferCreateInfo   )
KB_RESOURCE_HASHED_FUNC_DECLS (mesh           , MeshHandle          , MeshCreateInfo          )
KB_RESOURCE_HASHED_FUNC_DECLS (program        , ProgramHandle       , ProgramCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , TextureHandle       , TextureCreateInfo       )
KB_RESOURCE_HASHED_FUNC_DECLS (vertex_buffer  , VertexBufferHandle  , VertexBufferCreateInfo  )
KB_RESOURCE_HASHED_FUNC_DECLS (font           , FontHandle          , FontCreateInfo          )
KB_RESOURCE_HASHED_FUNC_DECLS (geometry       , GeometryHandle      , GeometryCreateInfo      )
KB_RESOURCE_HASHED_FUNC_DECLS (material       , MaterialHandle      , MaterialCreateInfo      )

KB_RESOURCE_CORE_FUNC_DECLS   (geometry       , GeometryHandle      , GeometryCreateInfo      )
KB_RESOURCE_CORE_FUNC_DECLS   (font           , FontHandle          , FontCreateInfo          )
KB_RESOURCE_CORE_FUNC_DECLS   (index_buffer   , IndexBufferHandle   , IndexBufferCreateInfo   )
KB_RESOURCE_CORE_FUNC_DECLS   (mesh           , MeshHandle          , MeshCreateInfo          )
KB_RESOURCE_CORE_FUNC_DECLS   (program        , ProgramHandle       , ProgramCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (texture        , TextureHandle       , TextureCreateInfo       )
KB_RESOURCE_CORE_FUNC_DECLS   (vertex_buffer  , VertexBufferHandle  , VertexBufferCreateInfo  )
KB_RESOURCE_CORE_FUNC_DECLS   (command_buffer , CommandBufferHandle , CommandBufferCreateInfo )
KB_RESOURCE_CORE_FUNC_DECLS   (material       , MaterialHandle      , MaterialCreateInfo      )

//#####################################################################################################################
// API functions
//#####################################################################################################################

KB_API CommandBufferHandle  kb_command_buffer_begin               ();
KB_API void                 kb_command_buffer_end                 (CommandBufferHandle command_buffer);

KB_API void*                kb_vertex_buffer_get_mapped           (VertexBufferHandle handle);
KB_API void*                kb_index_buffer_get_mapped            (IndexBufferHandle handle);

KB_API void                 kb_command_buffer_set_mesh            (CommandBufferHandle command_buffer, MeshHandle handle);
KB_API void                 kb_command_buffer_set_viewport        (CommandBufferHandle command_buffer, Int2 size, Float2 depth_range);
KB_API void                 kb_command_buffer_set_scissors        (CommandBufferHandle command_buffer, Int2 extent, Int2 offset);
KB_API void                 kb_command_buffer_submit_mesh         (CommandBufferHandle command_buffer, MeshHandle mesh);
KB_API void                 kb_command_buffer_submit              (CommandBufferHandle handle, uint32_t index_offset, uint32_t vertex_offset, uint32_t index_count);
KB_API void                 kb_command_buffer_bind_program        (CommandBufferHandle command_buffer, ProgramHandle handle);
KB_API void                 kb_command_buffer_bind_vertex_buffer  (CommandBufferHandle command_buffer, VertexBufferHandle handle);
KB_API void                 kb_command_buffer_bind_index_buffer   (CommandBufferHandle command_buffer, IndexBufferHandle handle);
KB_API void                 kb_command_buffer_bind_data           (CommandBufferHandle command_buffer, const BindSlot* slot, const void* data, uint32_t size);
KB_API void                 kb_command_buffer_bind_texture        (CommandBufferHandle command_buffer, const BindSlot* slot, TextureHandle texture);
KB_API void                 kb_command_buffer_bind_material       (CommandBufferHandle command_buffer, MaterialHandle material);

KB_API void                 kb_graphics_init                      (const GraphicsInitInfo info);
KB_API void                 kb_graphics_deinit                    ();
KB_API void                 kb_graphics_frame                     ();
KB_API void                 kb_graphics_frame_tmp_render          ();
KB_API Int2                 kb_graphics_get_extent                ();
KB_API void                 kb_graphics_wait_device_idle          ();
KB_API void                 kb_graphics_get_frame_stats           (FrameStats* stats);

KB_API bool                 kb_program_get_block_bind_slot        (ProgramHandle program, const char* name, BindSlot* bind_slot);
KB_API bool                 kb_program_get_field_bind_slot        (ProgramHandle program, const char* name, BindSlot* bind_slot);

KB_API bool                 kb_program_get_field_bind_slot_hash   (ProgramHandle program, Hash hash, BindSlot* bind_slot);
KB_API bool                 kb_program_get_block_bind_slot_hash   (ProgramHandle program, Hash hash, BindSlot* bind_slot);

KB_API VertexBufferHandle   kb_mesh_get_vertex_buffer             (MeshHandle handle);
KB_API IndexBufferHandle    kb_mesh_get_index_buffer              (MeshHandle handle);

KB_API TextureHandle        kb_font_get_atlas                     (FontHandle handle);
KB_API Real32               kb_font_get_line_height               (FontHandle handle);
KB_API Real32               kb_font_get_string_height             (FontHandle handle, const char* str);
KB_API Real32               kb_font_get_string_width              (FontHandle handle, const char* str);
KB_API Real32               kb_font_get_string_line_width         (FontHandle handle, const char* str);

KB_API void                 kb_overlay_print                      (CommandBufferHandle command_buffer, FontHandle font, const char* str, Float2 pos, float font_height);

KB_API GizmoHandle          kb_gizmo_begin                        (CommandBufferHandle command_buffer, const Float4x4 view, const Float4x4 proj);
KB_API void                 kb_gizmo_end                          (GizmoHandle gizmo);
KB_API void                 kb_gizmo_state_push                   (GizmoHandle gizmo);
KB_API void                 kb_gizmo_state_pop                    (GizmoHandle gizmo);
KB_API void                 kb_gizmo_state_push_transform         (GizmoHandle gizmo, const Float4x4 mtx, bool flush);
KB_API void                 kb_gizmo_state_pop_transform          (GizmoHandle gizmo, bool flush);
KB_API void                 kb_gizmo_state_set_color              (GizmoHandle gizmo, Float4 color);
KB_API void                 kb_gizmo_state_set_lod                (GizmoHandle gizmo, uint8_t lod);
KB_API void                 kb_gizmo_state_set_wireframe          (GizmoHandle gizmo, bool wireframe);
KB_API void                 kb_gizmo_shape_close                  (GizmoHandle gizmo);
KB_API void                 kb_gizmo_shape_move_to                (GizmoHandle gizmo, const Float3 pos);
KB_API void                 kb_gizmo_shape_line_to                (GizmoHandle gizmo, const Float3 pos);
KB_API void                 kb_gizmo_draw_arc                     (GizmoHandle gizmo, Axis axis, const Float3 pos, float radius, float degrees);
KB_API void                 kb_gizmo_draw_circle                  (GizmoHandle gizmo, const Float3 normal, const Float3 center, float radius, float weight);
KB_API void                 kb_gizmo_draw_cone                    (GizmoHandle gizmo, const Float3 from, const Float3 to, float radius);
KB_API void                 kb_gizmo_draw_cylinder                (GizmoHandle gizmo, const Float3 from, const Float3 to, float radius);
KB_API void                 kb_gizmo_draw_axis                    (GizmoHandle gizmo, const Float3 pos, float length);
KB_API void                 kb_gizmo_draw_grid                    (GizmoHandle gizmo, Axis axis, const Float3 center, uint32_t size, float step);
KB_API void                 kb_gizmo_draw_aabb                    (GizmoHandle gizmo, const Aabb aabb);
KB_API void                 kb_gizmo_flush                        (GizmoHandle gizmo);
KB_API void                 kb_gizmo_flush_quad                   (GizmoHandle gizmo);

KB_API void                 kb_font_load                          (FontHandle target, RWops* rwops);
KB_API void                 kb_texture_load                       (TextureHandle target, RWops* rwops);
KB_API void                 kb_geometry_load                      (GeometryHandle target, RWops* rwops);

#ifdef __cplusplus
}
#endif
