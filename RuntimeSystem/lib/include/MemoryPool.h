/*
 * MemoryPool.h
 *
 *  Created on: Sep 13, 2018
 *      Author: khm31335
 */

#ifndef LIB_INCLUDE_MEMORYPOOL_H_
#define LIB_INCLUDE_MEMORYPOOL_H_

#define __SHARED_MEMORY_POOL

#include <cstdlib>
#include <set>
typedef unsigned char uchar;
class MemoryPool
	{ // Basic type define
		typedef unsigned int uint;

		uint m_numOfBlocks; // Num of blocks
		uint m_sizeOfEachBlock; // Size of each block
		uint m_numFreeBlocks; // Num of remaining blocks
		uint m_numInitialized; // Num of initialized blocks
		uchar* m_memStart; // Beginning of memory pool
		uchar* m_next; // Num of next free block
	public:
#ifndef __SHARED_MEMORY_POOL
		static std::set<MemoryPool*> pools;
		static MemoryPool* findMemoryPool(void* p);
#endif
		MemoryPool();
		void createPool(size_t sizeOfEachBlock, uint numOfBlocks);
		bool safeDestroyPool();
		uchar* AddrFromIndex(uint i) const;
		uint IndexFromAddr(const uchar* p) const;
		void* Allocate();
		void DeAllocate(void* p);
		~MemoryPool();
};

#endif /* LIB_INCLUDE_MEMORYPOOL_H_ */
