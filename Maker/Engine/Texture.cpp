#include "Texture.h"
#include <GL/glew.h>

static auto GLWrapMode(Texture::WrapModes mode) {
	switch (mode) {
	case Texture::Repeat: return GL_REPEAT;
	case Texture::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case Texture::Clamp: return GL_CLAMP_TO_EDGE;
	default: return GL_REPEAT;
	}
}

static auto GLMagFilter(Texture::Filters filter) {
	switch (filter) {
	case Texture::Nearest: return GL_NEAREST;
	case Texture::Linear: return GL_LINEAR;
	default: return GL_NEAREST;
	}
}

static auto GLMinFilter(Texture::Filters filter) {
	switch (filter) {
	case Texture::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
	case Texture::Linear: return GL_LINEAR_MIPMAP_LINEAR;
	default: return GL_NEAREST_MIPMAP_LINEAR;
	}
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, _img_ptr->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLWrapMode(wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLWrapMode(wrapMode));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLMinFilter(filter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLMagFilter(filter));
}



