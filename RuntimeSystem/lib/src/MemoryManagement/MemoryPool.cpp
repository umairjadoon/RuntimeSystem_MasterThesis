/*
 * MemoryPool.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: khm31335
 */

#include <stdio.h>
#include <pthread.h>
#include "FContext.h"
#include "MemoryPool.h"

pthread_mutex_t mem_lock = PTHREAD_MUTEX_INITIALIZER;

#ifndef __SHARED_MEMORY_POOL
std::set<MemoryPool*> MemoryPool::pools;
#endif

MemoryPool::MemoryPool()
{
	m_numOfBlocks = 0;
	m_sizeOfEachBlock = 0;
	m_numFreeBlocks = 0;
	m_numInitialized = 0;
	m_memStart = NULL;
	m_next = 0;
	createPool(0x10000, 500);
#ifndef __SHARED_MEMORY_POOL
	pools.insert(this);
#endif
}

MemoryPool::~MemoryPool()
{
	safeDestroyPool();
#ifndef __SHARED_MEMORY_POOL
	pools.erase(this);
#endif
}

void MemoryPool::createPool(size_t sizeOfEachBlock, uint numOfBlocks)
{
	m_numOfBlocks = numOfBlocks;
	m_sizeOfEachBlock = sizeOfEachBlock;
	m_memStart = new uchar[ m_sizeOfEachBlock * m_numOfBlocks ];
	m_numFreeBlocks = numOfBlocks;
	m_next = m_memStart;
}

bool MemoryPool::safeDestroyPool()
{
	pthread_mutex_lock(&mem_lock);
	bool ret = (m_numFreeBlocks == m_numOfBlocks);
	if (ret)
	{
		delete[] m_memStart;
		m_memStart = NULL;
	}
	pthread_mutex_unlock(&mem_lock);
	return ret;
}

uchar* MemoryPool::AddrFromIndex(uint i) const
{
	return m_memStart + ( i * m_sizeOfEachBlock );
}

uint MemoryPool::IndexFromAddr(const uchar* p) const
{
	return (((uint)(p - m_memStart)) / m_sizeOfEachBlock);
}

#ifndef __SHARED_MEMORY_POOL
bool inRange(void* b, void* p, int n)
{
	if ((uchar*)b<=(uchar*)p && (uchar*)p<(uchar*)b+n)
		return true;
	return false;
}

MemoryPool* MemoryPool::findMemoryPool(void* p)
{
	for(auto it = pools.begin(); it != pools.end(); it++)
	{
		if (inRange((*it)->m_memStart, p, (*it)->m_numOfBlocks * (*it)->m_sizeOfEachBlock))
			return (*it);
	}
	return nullptr;
}
#endif

void* MemoryPool::Allocate()
{
	pthread_mutex_lock(&mem_lock);
	if (m_numInitialized < m_numOfBlocks )
	{
		uint* p = (uint*)AddrFromIndex( m_numInitialized );
		*p = m_numInitialized + 1;
		m_numInitialized++;
	}
	void* ret = NULL;
	if ( m_numFreeBlocks > 0 )
	{
		ret = (void*)m_next;
		--m_numFreeBlocks;
		if (m_numFreeBlocks!=0)
		{
			m_next = AddrFromIndex( *((uint*)m_next) );
		}
		else
		{
			m_next = NULL;
		}
	}
	pthread_mutex_unlock(&mem_lock);
	return ret;
}

void MemoryPool::DeAllocate(void* p)
{
#ifndef __SHARED_MEMORY_POOL
	MemoryPool* mp = findMemoryPool(p);
	assert(mp != nullptr)
#else
#define mp this
#endif
	pthread_mutex_lock(&mem_lock);
	if (mp->m_next != NULL)
	{
		(*(uint*)p) = IndexFromAddr( mp->m_next );
		mp->m_next = (uchar*)p;
	}
	else
	{
		*((uint*)p) = mp->m_numOfBlocks;
		mp->m_next = (uchar*)p;
	}
	mp->m_numFreeBlocks++;
	pthread_mutex_unlock(&mem_lock);
#ifdef __SHARED_MEMORY_POOL
#undef mp
#endif
}
