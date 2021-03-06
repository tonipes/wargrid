// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/material.h>

#include <kb/foundation.h>
#include <kb/graphics.h>

#ifndef KB_TOOL_ONLY

typedef struct kb_material_uniform_ref {
  kb_uniform_slot slot;
  void*           data;
  uint64_t        size;
} kb_material_uniform_ref;

struct kb_material_ref {
  kb_pipeline               pipeline;
  kb_material_uniform_ref   uniforms[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  kb_material_texture       textures[KB_CONFIG_MAX_UNIFORM_BINDINGS];
};

KB_RESOURCE_STORAGE_DEF     (material, kb_material, kb_material_ref, KB_CONFIG_MAX_MATERIALS);
KB_RESOURCE_ALLOC_FUNC_DEF  (material, kb_material, kb_material_create_info, KB_CONFIG_MAX_MATERIALS);
KB_RESOURCE_DATA_HASHED_DEF (material, kb_material);

void kb_material_construct(kb_material handle, const kb_material_create_info info) {  
  kb_material_ref* ref = material_ref(handle);

  ref->pipeline = info.pipeline;
    
  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    ref->textures[i] = info.textures[i];

    ref->uniforms[i].slot = info.uniforms[i].slot;
    ref->uniforms[i].size = info.uniforms[i].size;
    ref->uniforms[i].data = kb_memdup(info.uniforms[i].data, info.uniforms[i].size);
  }
  
  kb_material_set_initialized(handle, true);
}

void kb_material_destruct(kb_material handle) { 
  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    KB_DEFAULT_FREE(material_ref(handle)->uniforms[i].data);
  }
  kb_material_set_initialized(handle, false);
}

KB_API void kb_encoder_bind_material(kb_encoder encoder, kb_material material) {
  KB_ASSERT_VALID(encoder);
  KB_ASSERT_VALID(material);

  kb_encoder_bind_pipeline(encoder, material_ref(material)->pipeline);
  
  for (uint32_t i = 0; i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++i) {
    kb_material_uniform_ref& uniform = material_ref(material)->uniforms[i];
    if (uniform.data != NULL && uniform.size > 0 && uniform.slot.stage != 0) {
      kb_encoder_bind_uniform(encoder, uniform.slot, kb_graphics_transient_write(uniform.data, uniform.size, KB_BUFFER_USAGE_UNIFORM_BUFFER));
    }
    
    kb_material_texture& texture = material_ref(material)->textures[i];
    if (KB_IS_VALID(texture.texture) && texture.slot.stage != 0) {
      kb_encoder_bind_texture(encoder, texture.slot, texture.texture);
    }
  }
  
}
#endif
