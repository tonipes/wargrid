// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#define KB_TOOL_ONLY

#include <kb/foundation/core.h>
#include <kb/foundation/time.h>

#include <kbextra/cliargs.h>

#include "kb/foundation.cpp"

#include "kbextra/texture.cpp"
#include "kbextra/cliargs.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <platform/platform_rwops_stdio.cpp>

#define EXIT_FAIL     1
#define EXIT_SUCCESS  0

int std_io_rwops_read(void* user, char* data, int size) {
  kb_stream* io = (kb_stream*) user;
  int ret = kb_stream_read(io, data, 1, size);
  return (int) ret * sizeof(char);
}

void std_io_rwops_skip(void* user, int n) {
	kb_stream* io = (kb_stream*) user;

	kb_stream_seek(io, n, KB_RWOPS_SEEK_CUR);
}

int std_io_rwops_eof(void* user) {
	kb_stream* io = (kb_stream*) user;
  
  uint32_t d;
  uint64_t c = kb_stream_read(io, &d, 1, 1);
  
  if (c != 0) {
    kb_stream_seek(io, -c, KB_RWOPS_SEEK_CUR);
    return 0;
  }

  return 1;
}

stbi_io_callbacks stb_io_callbacks {
  .read = std_io_rwops_read,
  .skip = std_io_rwops_skip,
  .eof = std_io_rwops_eof,
};

void print_help(const char* error = nullptr) {
  if (error != nullptr) printf("%s\n", error);

  printf(
    "Kimberlite texturec\n"
    "\tUsage: texturec --input <in> --output <out>\n"
  );
}

int main(int argc, const char* argv[]) {
  int             exit_val      = EXIT_FAIL;
  kb_stream*      rwops_in      = nullptr;
  kb_stream*      rwops_out     = nullptr;
  const char*     in_filepath   = nullptr;
  const char*     out_filepath  = nullptr;
  int             tex_width     = 0;
  int             tex_height    = 0;
  int             tex_channels  = 0;
  stbi_uc*        pixel_data    = nullptr;
  uint64_t        data_size     = 0;
  kb_texture_data texture       = {};
  
  kb_cli_args args {};
  kb_cliargs_init(&args, argc, argv);

  if (kb_cliargs_has(&args, "help")) {
    print_help();
    goto end;
  }

  kb_cliargs_get_str(&args, &in_filepath, "input");
  if (in_filepath == nullptr) {
    print_help("Please specify input file with --input");
    goto end;
  }

  kb_cliargs_get_str(&args, &out_filepath, "output");
  if (out_filepath == nullptr) {
    print_help("Please specify output file with --output");
    goto end;
  }

  rwops_in = kb_stream_open_file(in_filepath, KB_FILE_MODE_READ);
  if (!rwops_in) {
    print_help("Unable to open input file");
    goto end;
  }

  rwops_out = kb_stream_open_file(out_filepath, KB_FILE_MODE_WRITE);
  if (!rwops_out) {
    print_help("Unable to open output file");
    goto end;
  }
  
  // Load
  pixel_data = stbi_load_from_callbacks(&stb_io_callbacks, rwops_in, &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
  data_size = tex_width * tex_height * STBI_rgb_alpha;

  if (!pixel_data) {
    goto end;
  }
  
  // Fill info
  texture.header.format = KB_FORMAT_RGBA8_UNORM;
  texture.header.width  = tex_width;
  texture.header.height = tex_height;
  texture.data_size     = data_size;
  texture.data          = pixel_data;

  kb_texture_write(&texture, rwops_out);
    
  exit_val = EXIT_SUCCESS;

end:
  stbi_image_free(pixel_data);
  kb_stream_close(rwops_in);
  kb_stream_close(rwops_out);

  return exit_val;
}
