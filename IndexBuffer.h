#ifndef _H_INDEXBUFFER_
#define _H_INDEXBUFFER_

#include <vector>
class IndexBuffer
{
public:
	unsigned int mHandle;
	unsigned int mCount;
public:
	IndexBuffer();
	~IndexBuffer();
	void Set(unsigned int* rr, unsigned int len);
	void Set(std::vector<unsigned int>& input);

	unsigned int Count();
	unsigned int GetHandle();
private:
	IndexBuffer(const IndexBuffer& other);
	IndexBuffer& operator= (const IndexBuffer& other);

};
#endif
//IndexBuffer::IndexBuffer()
//{
//}
//
//IndexBuffer::~IndexBuffer()
//{
//}