#pragma once

#include <kb/alloc.h>
#include <kb/hash.h>
#include <kb/resource.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>

#include <kb/geometry.h>
#include <kb/texture.h>
#include <kb/font.h>

KB_HANDLE(kb_command_buffer);
KB_HANDLE(kb_index_buffer);
KB_HANDLE(kb_vertex_buffer);
KB_HANDLE(kb_mesh);
KB_HANDLE(kb_pipeline);
KB_HANDLE(kb_material);

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_draw_mode {
  KB_DRAW_SINGLE    = 0,
  KB_CULL_INSTANCED = 1,
} kb_draw_mode;

typedef enum kb_index_type {
  KB_INDEX_TYPE_16 = 0,
  KB_INDEX_TYPE_32 = 1,
} kb_index_type;

typedef enum kb_bind_type {
  KB_BIND_TYPE_UNSUPPORTED    = 0,
  KB_BIND_TYPE_UNIFORM_BUFFER = 1,
  KB_BIND_TYPE_PUSH_CONSTANT  = 2,
  KB_BIND_TYPE_SAMPLER        = 3,
  KB_BIND_TYPE_STORAGE_BUFFER = 4,
} kb_bind_type;

typedef enum kb_cull_mode {
  KB_CULL_NONE  = 0,
  KB_CULL_BACK  = 1,
  KB_CULL_FRONT = 2,
  KB_CULL_BOTH  = 3,
} kb_cull_mode;

typedef enum kb_topology_type {
  KB_TOPOLOGY_TRIANGLE_STRIP  = 0,
  KB_TOPOLOGY_TRIANGLE_LIST   = 1,
  KB_TOPOLOGY_LINE_STRIP      = 2,
  KB_TOPOLOGY_LINE_LIST       = 3,
  KB_TOPOLOGY_POINT_LIST      = 4,
} kb_topology_type;

typedef struct kb_simple_vertex {
  Float3              pos;
  Float4              color;
  Float2              texcoord;
} kb_simple_vertex;

typedef struct kb_frame_stats {
  uint32_t            primitive_count;
  uint32_t            command_buffer_count;
  uint32_t            transient_memory_use;
  uint32_t            draw_calls;
  uint32_t            vertex_buffer_count;
  uint32_t            index_buffer_count;
  uint32_t            pipeline_count;
  uint32_t            texture_count;
  float               frametime_avg;
  float               frametime_min;
  float               frametime_max;
} kb_frame_stats;

typedef struct kb_geometry_create_info {
  kb_vertex_buffer    vertex_buffer;
  kb_index_buffer     index_buffer;
  uint32_t            material_count;
  kb_material*        materials;
  uint32_t            mesh_count;
  kb_mesh*            meshes;
} kb_geometry_create_info;

typedef struct kb_command_buffer_create_info {
  int                 param;
} kb_command_buffer_create_info;

typedef struct kb_sampler_binding {
  kb_hash             hash;
  kb_texture          texture;
} kb_sampler_binding;

typedef struct kb_uniform_binding {
  kb_hash             hash;
  Float4              data;
} kb_uniform_binding;

typedef struct kb_material_create_info {
  kb_pipeline         pipeline;
  uint32_t            uniform_count;
  kb_uniform_binding* uniforms;
  uint32_t            sampler_count;
  kb_sampler_binding* samplers;
} kb_material_create_info;

typedef struct kb_texture_create_info {
  kb_rwops*           rwops;
  kb_texture_info     texture;
  kb_sampler_info     sampler;
} kb_texture_create_info;

typedef struct kb_index_buffer_create_info {
  kb_rwops*           rwops;
  uint32_t            index_size;
  uint32_t            size;
  bool                host_mapped;
} kb_index_buffer_create_info;

typedef struct {
  kb_rwops*           rwops;
  kb_vertex_layout    layout;
  uint32_t            size;
  bool                host_mapped;
} kb_vertex_buffer_create_info;

typedef struct {
  kb_vertex_buffer    vertex_buffer;
  kb_index_buffer     index_buffer;
  uint32_t            primitive_count;
  kb_primitive_data*  primitives;
  uint32_t            material_count;
  kb_material*        materials;
} kb_mesh_create_info;

typedef struct {
  kb_font_info        info;
  kb_pipeline         pipeline;
  kb_texture          atlas;
  uint32_t            atlas_height;
  uint32_t            atlas_width;
} kb_font_create_info;

typedef struct {
  kb_rwops*           vert_code_rwops;
  kb_rwops*           frag_code_rwops;
  const char*         vert_entry;
  const char*         frag_entry;
  kb_cull_mode        cull;
  kb_draw_mode        mode;
  kb_topology_type    topology;
  bool                depth_write;
  bool                depth_test;
} kb_pipeline_create_info;

typedef struct {
  uint32_t            msaa;
  bool                vsync;
  Int2                resolution;
} kb_graphics_init_info;

typedef struct {
  kb_bind_type        type;
  uint32_t            set;
  uint32_t            binding;
  uint64_t            size;
  uint64_t            offset;
  uint64_t            block_size;
} kb_bind_slot;

KB_RESOURCE_HASHED_FUNC_DECLS (index_buffer   , kb_index_buffer   , kb_index_buffer_create_info   )
KB_RESOURCE_HASHED_FUNC_DECLS (mesh           , kb_mesh           , kb_mesh_create_info           )
KB_RESOURCE_HASHED_FUNC_DECLS (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , kb_texture        , kb_texture_create_info        )
KB_RESOURCE_HASHED_FUNC_DECLS (vertex_buffer  , kb_vertex_buffer  , kb_vertex_buffer_create_info  )
KB_RESOURCE_HASHED_FUNC_DECLS (font           , kb_font           , kb_font_create_info           )
KB_RESOURCE_HASHED_FUNC_DECLS (geometry       , kb_geometry       , kb_geometry_create_info       )
KB_RESOURCE_HASHED_FUNC_DECLS (material       , kb_material       , kb_material_create_info       )

KB_RESOURCE_CORE_FUNC_DECLS   (geometry       , kb_geometry       , kb_geometry_create_info       )
KB_RESOURCE_CORE_FUNC_DECLS   (font           , kb_font           , kb_font_create_info           )
KB_RESOURCE_CORE_FUNC_DECLS   (index_buffer   , kb_index_buffer   , kb_index_buffer_create_info   )
KB_RESOURCE_CORE_FUNC_DECLS   (mesh           , kb_mesh           , kb_mesh_create_info           )
KB_RESOURCE_CORE_FUNC_DECLS   (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_CORE_FUNC_DECLS   (texture        , kb_texture        , kb_texture_create_info        )
KB_RESOURCE_CORE_FUNC_DECLS   (vertex_buffer  , kb_vertex_buffer  , kb_vertex_buffer_create_info  )
KB_RESOURCE_CORE_FUNC_DECLS   (command_buffer , kb_command_buffer , kb_command_buffer_create_info )
KB_RESOURCE_CORE_FUNC_DECLS   (material       , kb_material       , kb_material_create_info       )

KB_API void                 kb_graphics_init                      (const kb_graphics_init_info info);
KB_API void                 kb_graphics_deinit                    ();
KB_API void                 kb_graphics_frame                     ();
KB_API Int2                 kb_graphics_get_extent                ();
KB_API void                 kb_graphics_wait_device_idle          ();
KB_API void                 kb_graphics_get_frame_stats           (kb_frame_stats* stats);

KB_API kb_allocator*        kb_graphics_transient_allocator       ();
KB_API void*                kb_graphics_transient_at              (uint64_t offset);
KB_API uint64_t             kb_graphics_transient_offset          (void* ptr);
KB_API void*                kb_graphics_transient_alloc           (uint64_t size, uint64_t align);

KB_API kb_command_buffer    kb_command_buffer_begin               ();
KB_API void                 kb_command_buffer_end                 (kb_command_buffer command_buffer);
KB_API void                 kb_command_buffer_set_mesh            (kb_command_buffer command_buffer, kb_mesh handle);
KB_API void                 kb_command_buffer_set_viewport        (kb_command_buffer command_buffer, Int2 size, Float2 depth_range);
KB_API void                 kb_command_buffer_set_scissors        (kb_command_buffer command_buffer, Int2 extent, Int2 offset);
KB_API void                 kb_command_buffer_submit_mesh         (kb_command_buffer command_buffer, kb_mesh mesh);
KB_API void                 kb_command_buffer_submit              (kb_command_buffer command_buffer, uint32_t index_offset, uint32_t vertex_offset, uint32_t index_count);
KB_API void                 kb_command_buffer_bind_pipeline       (kb_command_buffer command_buffer, kb_pipeline pipeline);
KB_API void                 kb_command_buffer_bind_vertex_buffer  (kb_command_buffer command_buffer, kb_vertex_buffer handle);
KB_API void                 kb_command_buffer_bind_index_buffer   (kb_command_buffer command_buffer, kb_index_buffer handle);
KB_API void                 kb_command_buffer_bind_data           (kb_command_buffer command_buffer, const kb_bind_slot* slot, const void* data, uint32_t size);
KB_API void                 kb_command_buffer_bind_texture        (kb_command_buffer command_buffer, const kb_bind_slot* slot, kb_texture texture);
KB_API void                 kb_command_buffer_bind_material       (kb_command_buffer command_buffer, kb_material material);

KB_API void                 kb_command_buffer_bind_vertex_buffer_transient  (kb_command_buffer command_buffer, uint64_t offset);
KB_API void                 kb_command_buffer_bind_index_buffer_transient   (kb_command_buffer command_buffer, uint64_t offset, kb_index_type type);

KB_API bool                 kb_pipeline_get_block_bind_slot       (kb_pipeline pipeline, const char* name, kb_bind_slot* bind_slot);
KB_API bool                 kb_pipeline_get_field_bind_slot       (kb_pipeline pipeline, const char* name, kb_bind_slot* bind_slot);
KB_API bool                 kb_pipeline_get_field_bind_slot_hash  (kb_pipeline pipeline, kb_hash hash, kb_bind_slot* bind_slot);
KB_API bool                 kb_pipeline_get_block_bind_slot_hash  (kb_pipeline pipeline, kb_hash hash, kb_bind_slot* bind_slot);

KB_API kb_texture           kb_font_get_atlas                     (kb_font handle);
// KB_API Real32               kb_font_get_line_height               (kb_font handle);
// KB_API Real32               kb_font_get_string_height             (kb_font handle, const char* str);
// KB_API Real32               kb_font_get_string_width              (kb_font handle, const char* str);
// KB_API Real32               kb_font_get_string_line_width         (kb_font handle, const char* str);

KB_API void                 kb_overlay_print                      (kb_command_buffer command_buffer, kb_font font, const char* str, Float2 pos, float font_height);

KB_API void                 kb_font_load                          (kb_font target, kb_rwops* rwops);
KB_API void                 kb_texture_load                       (kb_texture target, kb_rwops* rwops);
KB_API void                 kb_geometry_load                      (kb_geometry target, kb_rwops* rwops);

#ifdef __cplusplus
}
#endif
