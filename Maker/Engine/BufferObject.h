#pragma once
class BufferObject
{
	unsigned int _id = 0;
	int _target = 0;

public:
	unsigned int id() const { return _id; }
	int target() const { return _target; }
	void loadData(const void* data, size_t size);
	void loadIndices(const unsigned int* indices, size_t num_indices);
	void unload();
	void bind() const;

	BufferObject() = default;

	BufferObject(BufferObject&& other) noexcept;
	BufferObject& operator=(BufferObject&& other) noexcept = delete;

	BufferObject(const BufferObject&) = delete;
	BufferObject& operator=(const BufferObject&) = delete;
	~BufferObject();
};

