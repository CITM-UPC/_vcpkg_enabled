#include "BufferObject.h"
#include <GL/glew.h>

BufferObject::BufferObject(BufferObject&& other) noexcept : _id(other._id), _target(other._target)
{
	other._id = 0;
	other._target = 0;
}

void BufferObject::unload()
{
	if (_id != 0)
	{
		glDeleteBuffers(1, &_id);
	}
	_id = 0;
}

BufferObject::~BufferObject()
{
	unload();
}

void BufferObject::bind() const
{
	glBindBuffer(_target, _id);
}

void BufferObject::loadData(const void* data, size_t size)
{
	_target = GL_ARRAY_BUFFER;
	if (_id == 0)
	{
		glGenBuffers(1, &_id);
	}
	glBindBuffer(_target, _id);
	glBufferData(_target, size, data, GL_STATIC_DRAW);
}

void BufferObject::loadIndices(const unsigned int* indices, size_t num_indices)
{
	_target = GL_ELEMENT_ARRAY_BUFFER;
	if (_id == 0)
	{
		glGenBuffers(1, &_id);
	}
	glBindBuffer(_target, _id);
	glBufferData(_target, num_indices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}