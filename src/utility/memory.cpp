//headers should be ordered alphabetically!
/******* precompiled header *******/
#include "stdafx.h"
/******* personal header *******/
#include "utility/memory.h"
/******* c++ headers *******/
/******* extra headers *******/
/******* end headers *******/

namespace core
{
   std::ostream& operator<<(std::ostream& stream, byteSizes bs)
   {
      u64 B = bs.m_B;
      f32 kB = B / 1024.0f;
      f32 MB = kB / 1024;
      stream << B << " B / " << std::fixed << kB << " kB / " << MB << " MB" << std::defaultfloat;
      return stream;
   }

   std::ostream& operator<<(std::ostream& stream, memoryRequirement mr)
   {
      stream << byteSizes(u32((u8*)mr.m_end - (u8*)mr.m_start));
      return stream;
   }

   std::ostream& operator<<(std::ostream& stream, memoryAddress ma)
   {
      stream << "0x" << ma.m_address;
      return stream;
   }


   /************************************************************************
    *              MainAllocator functions
    ************************************************************************/
   void MainAllocator::init(void* memory, u64 size)
   {
      CORE_ASSERT_DBGERR(m_voidAddress == nullptr, "Attempting to initialize a large allocator that has already been initialized.");
      m_voidAddress = memory;
      m_sizeBytes = size;
      m_allocatedBytes = 0;
      m_allocationsCount = 0;
   }
   void* MainAllocator::allocateRaw(u32 size, u32 align)
   {
      auto alignment = align - (u32(m_byteAddress + m_allocatedBytes) % align);
      size += alignment;
      if( m_allocatedBytes + size <= m_sizeBytes )
      {
         void* result = m_byteAddress + m_allocatedBytes + alignment;
         m_allocatedBytes += size;
         ++m_allocationsCount;
         return result;
      }
      return nullptr;
   }
   void MainAllocator::deallocateRaw(void* ptr, u32 size, u32 align)
   {
      //noop
      --m_allocationsCount;
   }
   void MainAllocator::outputToStream(std::ostream& stream)
   {
      stream << "Allocated memory: " << byteSizes(m_allocatedBytes) << logLine;
      stream << "Allocation count: " << m_allocationsCount << logLine;
   }
   MainAllocator::~MainAllocator()
   {
      CORE_ASSERT_DBGERR(m_allocationsCount == 0, "Missing ", m_allocationsCount, " deallocations for main allocator.");
   }

   /************************************************************************
    *              LinearAllocator functions
    ************************************************************************/
   void LinearAllocator::init(Allocator& parent, u32 sizeBytes)
   {
      CORE_ASSERT_DBGWRN(m_memory.voidAddress == nullptr, "Attempting to initialize a linear allocator that has already been initialized.");
      m_memory.voidAddress = parent.allocateRaw(sizeBytes, 16);
      m_memory.size = sizeBytes;
      m_allocatedBytes = 0;
      m_allocationsCount = 0;
   }
   void* LinearAllocator::allocateRaw(u32 size, u32 align)
   {
      u8* result = m_memory.byteAddress + m_allocatedBytes;
      auto alignment = align - (u32(result) % align);
      size += alignment;
      if( m_allocatedBytes + size <= m_memory.size )
      {
         result += alignment;
         m_allocatedBytes += size;
         ++m_allocationsCount;
         return result;
      }
      return nullptr;
   }
   void LinearAllocator::deallocateRaw(void* ptr, u32 size, u32 align)
   {
      //noop
      --m_allocationsCount;
   }
   void LinearAllocator::outputToStream(std::ostream& stream)
   {
      stream << "Allocated memory: " << byteSizes(m_allocatedBytes) << logLine;
      stream << "Allocation count: " << m_allocationsCount << logLine;
   }
   LinearAllocator::~LinearAllocator()
   {
      CORE_ASSERT_DBGERR(m_allocationsCount == 0, "Missing ", m_allocationsCount, " deallocations for linear allocator.");
   }

   /************************************************************************
    *              HeapAllocator functions
    ************************************************************************/
}
