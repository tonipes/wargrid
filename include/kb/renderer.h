// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/alloc.h>
#include <kb/hash.h>
#include <kb/resource.h>
#include <kb/handle.h>
#include <kb/math.h>
#include <kb/rwops.h>
#include <kb/shader.h>
#include <kb/vertex.h>
#include <kb/texture.h>

KB_HANDLE(kb_index_buffer);
KB_HANDLE(kb_vertex_buffer);
KB_HANDLE(kb_pipeline);
KB_HANDLE(kb_encoder);

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kb_draw_mode {
  KB_DRAW_SINGLE              = 0,
  KB_CULL_INSTANCED           = 1,
} kb_draw_mode;

typedef enum kb_index_type {
  KB_INDEX_TYPE_16            = 0,
  KB_INDEX_TYPE_32            = 1,
} kb_index_type;

typedef enum kb_cull_mode {
  KB_CULL_NONE                = 0,
  KB_CULL_BACK                = 1,
  KB_CULL_FRONT               = 2,
  KB_CULL_BOTH                = 3,
} kb_cull_mode;

typedef enum kb_topology_type {
  KB_TOPOLOGY_TRIANGLE_STRIP  = 0,
  KB_TOPOLOGY_TRIANGLE_LIST   = 1,
  KB_TOPOLOGY_LINE_STRIP      = 2,
  KB_TOPOLOGY_LINE_LIST       = 3,
  KB_TOPOLOGY_POINT_LIST      = 4,
} kb_topology_type;

typedef struct kb_frame_stats {
  uint32_t                  encoder_count;
  uint32_t                  transient_memory_used;
  uint32_t                  draw_calls;
  uint32_t                  vertex_buffer_count;
  uint32_t                  index_buffer_count;
  uint32_t                  pipeline_count;
  uint32_t                  texture_count;
  float                     frametime_avg;
  float                     frametime_min;
  float                     frametime_max;
} kb_frame_stats;

typedef struct kb_texture_create_info {
  kb_rwops*                 rwops;
  kb_texture_info           texture;
  bool                      mipmaps;
  kb_filter                 filter;
} kb_texture_create_info;

typedef struct kb_index_buffer_create_info {
  kb_rwops*                 rwops;
  uint32_t                  index_size;
  uint32_t                  size;
  bool                      host_mapped;
} kb_index_buffer_create_info;

typedef struct kb_vertex_buffer_create_info {
  kb_rwops*                 rwops;
  kb_vertex_layout          layout;
  uint32_t                  size;
  bool                      host_mapped;
} kb_vertex_buffer_create_info;

typedef struct kb_pipeline_create_info {
  kb_rwops*                 vert_code_rwops;
  kb_rwops*                 frag_code_rwops;
  const char*               vert_entry;
  const char*               frag_entry;
  kb_cull_mode              cull;
  kb_draw_mode              mode;
  kb_topology_type          topology;
  bool                      depth_write;
  bool                      depth_test;
} kb_pipeline_create_info;

typedef struct kb_graphics_init_info {
  uint32_t                  msaa;
  bool                      vsync;
  Int2                      resolution;
} kb_graphics_init_info;

typedef struct kb_primitive_info {
  uint32_t                  first_index;
  uint32_t                  first_vertex;
  uint32_t                  index_count;
} kb_primitive_info;

typedef struct kb_vertex_buffer_binding {
  kb_vertex_buffer          buffer;
  uint64_t                  offset;
} kb_vertex_buffer_binding;

typedef struct kb_index_buffer_binding {
  kb_index_buffer           buffer;
  uint64_t                  offset;
  kb_index_type             index_type;
} kb_index_buffer_binding;

typedef struct kb_texture_binding {
  uint32_t                  index;
  kb_texture                texture;
} kb_texture_binding;

typedef struct kb_uniform_binding {
  uint32_t                  index;
  uint64_t                  size;
  uint64_t                  offset;
} kb_uniform_binding;

typedef struct kb_draw_call_info {
  uint32_t                  first_vertex;
  uint32_t                  first_index;
  uint32_t                  index_count;
} kb_draw_call_info;

typedef struct kb_draw_call {
  kb_pipeline               pipeline;
  kb_vertex_buffer_binding  vertex_buffer;
  kb_index_buffer_binding   index_buffer;
  kb_texture_binding        vert_texture_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_uniform_binding        vert_uniform_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_texture_binding        frag_texture_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_uniform_binding        frag_uniform_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_draw_call_info         single;
} kb_draw_call;

typedef struct kb_encoder_frame {
  kb_pipeline               pipeline;
  kb_vertex_buffer_binding  vertex_buffer;
  kb_index_buffer_binding   index_buffer;
  kb_texture_binding        vert_texture_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_uniform_binding        vert_uniform_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_texture_binding        frag_texture_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
  kb_uniform_binding        frag_uniform_bindings[KB_CONFIG_MAX_SHADER_BINDINGS];
} kb_encoder_frame;

typedef struct kb_encoder_state {
  uint32_t                  stack_pos;
  uint32_t                  draw_call_count;
  kb_encoder_frame          stack[KB_CONFIG_GIZMO_STACK_SIZE];  
  kb_draw_call              draw_calls[KB_CONFIG_MAX_DRAW_CALLS];
} kb_encoder_state;

typedef struct kb_encoder_pool {
  uint16_t                  count;
  kb_encoder_state          states[KB_CONFIG_MAX_ENCODERS];
} kb_encoder_pool;

KB_RESOURCE_HASHED_FUNC_DECLS (index_buffer   , kb_index_buffer   , kb_index_buffer_create_info   )
KB_RESOURCE_HASHED_FUNC_DECLS (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_HASHED_FUNC_DECLS (texture        , kb_texture        , kb_texture_create_info        )
KB_RESOURCE_HASHED_FUNC_DECLS (vertex_buffer  , kb_vertex_buffer  , kb_vertex_buffer_create_info  )

KB_RESOURCE_ALLOC_FUNC_DECLS  (index_buffer   , kb_index_buffer   , kb_index_buffer_create_info   )
KB_RESOURCE_ALLOC_FUNC_DECLS  (pipeline       , kb_pipeline       , kb_pipeline_create_info       )
KB_RESOURCE_ALLOC_FUNC_DECLS  (texture        , kb_texture        , kb_texture_create_info        )
KB_RESOURCE_ALLOC_FUNC_DECLS  (vertex_buffer  , kb_vertex_buffer  , kb_vertex_buffer_create_info  )

KB_API void                       kb_graphics_init                      (const kb_graphics_init_info info);
KB_API void                       kb_graphics_deinit                    ();
KB_API void                       kb_graphics_frame                     ();
KB_API void                       kb_graphics_run_encoders              ();
KB_API Int2                       kb_graphics_get_extent                ();
KB_API void                       kb_graphics_wait_device_idle          ();
KB_API uint32_t                   kb_graphics_get_current_resource_slot ();
KB_API void                       kb_graphics_get_frame_stats           (kb_frame_stats* stats);

KB_API void*                      kb_graphics_transient_at              (uint64_t offset);
KB_API uint64_t                   kb_graphics_transient_offset          (void* ptr);
KB_API void*                      kb_graphics_transient_alloc           (uint64_t size, uint64_t align);

KB_API kb_vertex_layout*          kb_graphics_vertex_buffer_layout      (kb_vertex_buffer buffer);

KB_API kb_vertex_layout*          kb_graphics_pipeline_vertex_layout    (kb_pipeline pipeline);
KB_API kb_shader_desc*            kb_graphics_pipeline_bindings         (kb_pipeline pipeline);
KB_API bool                       kb_pipeline_get_binding_slot          (kb_pipeline pipeline, const char* name, kb_shader_binding_type type, kb_shader_binding_slot* bind_slot);
KB_API bool                       kb_pipeline_get_binding_slot_hash     (kb_pipeline pipeline, kb_hash hash, kb_shader_binding_type type, kb_shader_binding_slot* bind_slot);

KB_API kb_shader_buffer_binding*  kb_pipeline_get_buffer_binding        (kb_pipeline pipeline, const char* name, uint32_t index);
KB_API kb_shader_texture_binding* kb_pipeline_get_texture_binding       (kb_pipeline pipeline, const char* name, uint32_t index);

KB_API kb_encoder                 kb_encoder_begin                      ();
KB_API void                       kb_encoder_end                        (kb_encoder encoder);
KB_API void                       kb_encoder_push                       (kb_encoder encoder);
KB_API void                       kb_encoder_pop                        (kb_encoder encoder);
KB_API void                       kb_encoder_bind_pipeline              (kb_encoder encoder, kb_pipeline pipeline);
KB_API void                       kb_encoder_bind_vertex_buffer         (kb_encoder encoder, kb_vertex_buffer vertex_buffer, uint64_t offset);
KB_API void                       kb_encoder_bind_index_buffer          (kb_encoder encoder, kb_index_buffer index_buffer, uint64_t offset, kb_index_type type);
KB_API void                       kb_encoder_bind_texture               (kb_encoder encoder, const kb_shader_binding_slot* slot, kb_texture texture);
KB_API void                       kb_encoder_bind_uniform               (kb_encoder encoder, const kb_shader_binding_slot* slot, const void* data, uint64_t size);
KB_API void                       kb_encoder_submit_single              (kb_encoder encoder, uint32_t first_vertex, uint32_t first_index, uint32_t index_count);
KB_API void                       kb_encoder_reset_frame                (kb_encoder encoder);

#ifdef __cplusplus
}
#endif
