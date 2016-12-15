#include <string>
using std::string;

#ifndef __MYCRYPTO__
#define __MYCRYPTO__

typedef unsigned char BYTE;
typedef unsigned short WORD;

class ByteBlock {
	BYTE * pBlocks;
	size_t amount_of_bytes;
public:
	ByteBlock(size_t size_ = 0, BYTE init_value = 0);
    ByteBlock(BYTE * pBlocks_, size_t size_);
    ByteBlock(ByteBlock && rhs);
    ~ByteBlock();
	void operator = (ByteBlock && rhs);
    BYTE * byte_ptr();
    const BYTE * byte_ptr() const;
    BYTE & operator [] (size_t index);
    BYTE operator [] (size_t index) const;

	bool operator == (const ByteBlock & lhs) const;
	bool operator != (const ByteBlock & lhs) const;
    void reset(const BYTE * pBlocks_, size_t size_);
    size_t size() const;
    ByteBlock deep_copy() const;
    ByteBlock operator () (size_t begin, size_t length) const;
	friend void swap(ByteBlock & lhs, ByteBlock & rhs);
};
std::vector<ByteBlock> split_blocks(const ByteBlock & src, size_t length);
ByteBlock join_blocks(const std::vector<ByteBlock> & blocks);
void xor_blocks(ByteBlock & to_assign, const ByteBlock & lhs, const ByteBlock & rhs);
string hex_representation(const ByteBlock & bb);
ByteBlock hex_to_bytes(const string & s);

#endif
