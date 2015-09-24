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
      m_allocations = 0;
   }
   void* MainAllocator::allocateRaw(u32 size, u32 align)
   {
      auto alignment = align - (u32(m_byteAddress + m_allocatedBytes) % align);
      size += alignment;
      if( m_allocatedBytes + size <= m_sizeBytes )
      {
         void* result = m_byteAddress + m_allocatedBytes + alignment;
         m_allocatedBytes += size;
         ++m_allocations;
         return result;
      }
      return nullptr;
   }
   void MainAllocator::deallocateRaw(void* ptr, u32 size)
   {
      if( !ptr )
      {
         return;
      }
      // #todo
      CORE_ASSERT_DBGWRN(false, "Not implemented yet!");
      --m_allocations;
   }
   void MainAllocator::outputToStream(std::ostream& stream)
   {
      stream << "Allocated memory: " << byteSizes(m_allocatedBytes) << logLine;
      stream << "Allocation count: " << m_allocations << logLine;
   }
   void MainAllocator::shutdown()
   {
      CORE_ASSERT_DBGERR(m_allocations == 0, "Missing ", m_allocations, " deallocations for main allocator.");
   }

   /************************************************************************
    *              FrameAllocator functions
    ************************************************************************/
   void FrameAllocator::init(Allocator& parent, u32 sizeBytes)
   {
      CORE_ASSERT_DBGWRN(m_voidAddress == nullptr, "Attempting to initialize a frame allocator that has already been initialized.");
      m_parent = &parent;
      m_sizeBytes = sizeBytes;
      m_voidAddress = parent.allocateRaw(sizeBytes, 16);
      m_allocatedBytes = 0;
      m_allocations = 0;
   }
   void* FrameAllocator::allocateRaw(u32 size, u32 align)
   {
      u8* result = m_byteAddress + m_allocatedBytes;
      auto alignment = align - (u32(result) % align);
      size += alignment;
      if( m_allocatedBytes + size <= m_sizeBytes )
      {
         result += alignment;
         m_allocatedBytes += size;
         ++m_allocations;
         return result;
      }
      return nullptr;
   }
   void FrameAllocator::deallocateRaw(void* ptr, u32 size)
   {
      if( !ptr )
      {
         return;
      }
      // #todo
      CORE_ASSERT_DBGWRN(false, "Not implemented yet!");
      --m_allocations;
   }
   void FrameAllocator::outputToStream(std::ostream& stream)
   {
      stream << "Allocated memory: " << byteSizes(m_allocatedBytes) << logLine;
      stream << "Allocation count: " << m_allocations << logLine;
   }
   void FrameAllocator::shutdown()
   {
      CORE_ASSERT_DBGERR(m_allocations == 0, "Missing ", m_allocations, " deallocations for frame allocator.");
      m_parent->deallocateRaw(m_voidAddress, m_sizeBytes);
   }

   /************************************************************************
    *              PoolAllocator
    ************************************************************************/
   void PoolAllocator::init(Allocator& parent, u32 slotCount, u32 slotSize)
   {
      CORE_ASSERT_DBGWRN(m_head.next == nullptr, "Attempting to initialize a pool allocator that has already been initialized.");
      CORE_ASSERT_DBGWRN(sizeof(Node) <= slotSize, "Creating pool for type smaller than internal Node, increasing allocation size");
      m_parent = &parent;

      m_slotSize = sizeof(Node) > slotSize ? (u32)sizeof(Node) : slotSize;
      union
      {
         void* voidAddress;
         u8* byteAddress;
      };
      m_sizeBytes = slotCount*m_slotSize;
      m_voidAddress = parent.allocateRaw(m_sizeBytes, 16);
        
      CORE_ASSERT_DBGERR(voidAddress != nullptr, "Failed to allocate memory for pool!");

      Node* runner = &m_head;
      for( u32 i = 0; i < slotCount; ++i )
      {
         runner->next = (Node*)(byteAddress + i*m_slotSize);
         runner = runner->next;
      }
      runner->next = nullptr;
      m_allocations = 0;
      m_allocatedBytes = 0;
   }
   void* PoolAllocator::allocateRaw(u32 size, u32 align)
   {
      CORE_ASSERT_DBGERR(size == m_slotSize, "Attempting to pool allocate a size different than the initialized size!");
      CORE_ASSERT_DBGERR(size%align == 0, "Attempting to pool allocate memory with different alignment requirement than the initialized alignment!");
      void* result = nullptr;
      if( m_head.next )
      {
         result = m_head.next;
         m_head.next = m_head.next->next;
         ++m_allocations;
      }
      return result;
   }
   void PoolAllocator::deallocateRaw(void* ptr, u32 size)
   {
      if( !ptr )
      {
         return;
      }
      CORE_ASSERT_DBGERR(size == m_slotSize, "Attempting to pool deallocate a size different than the initialized size!");
      union
      {
         void* voidAddress;
         Node* node;
      };
      voidAddress = ptr;
      node->next = m_head.next;
      m_head.next = node;
      --m_allocations;
   }
   void PoolAllocator::outputToStream(std::ostream& stream)
   {
      stream << "Allocated memory: " << byteSizes(m_allocatedBytes) << logLine;
      stream << "Allocation count: " << m_allocations << logLine;
   }
   void PoolAllocator::shutdown()
   {
      CORE_ASSERT_DBGERR(m_allocations == 0, "Missing ", m_allocations, " deallocations for pool allocator.");
      m_parent->deallocateRaw(m_voidAddress, m_sizeBytes);
   }

   /************************************************************************
    *              HeapAllocator functions
    ************************************************************************/
   void HeapAllocator::init(Allocator& parent, u32 sizeBytes)
   {
      CORE_ASSERT_DBGWRN(m_head.voidAddress == nullptr, "Attempting to initialize a heap allocator that has already been initialized.");

      void* memory = parent.allocateRaw(sizeBytes, 16);
      
      CORE_ASSERT_DBGERR(memory != nullptr, "Failed to allocate memory for pool!");
      
      m_head.voidAddress = memory;
      m_head.size = sizeBytes;
      m_head.next->voidAddress = nullptr;
      m_head.next->size = 0;
   }
#define ALIGNMENT 16
   void* HeapAllocator::allocateRaw(u32 size, u32 align)
   {
      u32 sizeExpansionForFreelistPurposes = (ALIGNMENT - (size % ALIGNMENT));
      sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & (ALIGNMENT - 1);

      size += sizeExpansionForFreelistPurposes;
      CORE_ASSERT_DBGERR((size % ALIGNMENT) == 0, "The requested size should always be a multiple of ", ALIGNMENT, " after expansion.");
      CORE_ASSERT_DBGERR(size >= sizeof(HeapAllocator::Node), "The requested size should be at least ", sizeof(HeapAllocator::Node), " bytes after expansion.");

      HeapAllocator::Node* currentNode = &m_head;
      while( currentNode->voidAddress && currentNode->size < size )
      {
         currentNode = currentNode->next;
      }

      if( currentNode->voidAddress == nullptr )
      {
         //case where we traversed the entire freelist and found no slot big enough for the requested size
         return nullptr;
      }

      //case where we found a slot big enough for the requested size
      u8* result = currentNode->byteAddress;
      CORE_ASSERT_DBGERR(((u32)result % ALIGNMENT) == 0, "The returned address should always be aligned to ", ALIGNMENT, " bytes.");

      currentNode->size -= size;
      if( currentNode->size )
      {
         //case where there is more room in the slot after the requested size is removed
         HeapAllocator::Node* unmoved = currentNode->next;
         currentNode->byteAddress += size;
         currentNode->next->voidAddress = unmoved->voidAddress;
         currentNode->next->size = unmoved->size;
      }
      else
      {
         //case where the slot is exhausted by the requested size
         currentNode->size = currentNode->next->size;
         currentNode->voidAddress = currentNode->next->voidAddress;
      }
      m_allocatedBytes += size;
      memset(result, 0, size);
      ++m_allocations;
      return result;
   }
   void HeapAllocator::deallocateRaw(void* ptr, u32 size)
   {
      if( !ptr )
      {
         return;
      }
      u32 sizeExpansionForFreelistPurposes = (ALIGNMENT - (size % ALIGNMENT));
      sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & (ALIGNMENT - 1);
      size += sizeExpansionForFreelistPurposes;

      CORE_ASSERT_DBGERR((size % ALIGNMENT) == 0, "The size should always be a multiple of ", ALIGNMENT, " after expansion.");
      CORE_ASSERT_DBGERR(size >= sizeof(Node), "The size should be at least ", sizeof(Node), " bytes after expansion.");
      CORE_ASSERT_DBGERR(((u32)ptr % ALIGNMENT) == 0, "Every address returned should be aligned to ", ALIGNMENT, " bytes.");

      HeapAllocator::Node* previousNode = nullptr;
      HeapAllocator::Node* currentNode = &m_head;
      while( currentNode->voidAddress && currentNode->voidAddress < ptr )
      {
         previousNode = currentNode;
         currentNode = currentNode->next;
      }

      HeapAllocator::Node* newNode = (HeapAllocator::Node*)ptr;
      newNode->voidAddress = currentNode->voidAddress;
      newNode->size = currentNode->size;
      currentNode->next = newNode;
      currentNode->size = size;

      while( previousNode && previousNode->byteAddress + previousNode->size == currentNode->voidAddress )
      {
         previousNode->size += currentNode->size;
         currentNode->size = currentNode->next->size;
         currentNode->voidAddress = currentNode->next->voidAddress;
      }

      while( currentNode->voidAddress && currentNode->byteAddress + currentNode->size == newNode->voidAddress )
      {
         currentNode->size += newNode->size;
         newNode->size = newNode->next->size;
         newNode->voidAddress = newNode->next->voidAddress;
      }

      m_allocatedBytes -= size;
      --m_allocations;
   }
   void HeapAllocator::outputToStream(std::ostream& stream)
   {
      stream << "Allocated memory: " << byteSizes(m_allocatedBytes) << logLine;
      stream << "Allocation count: " << m_allocations << logLine;
   }
   void HeapAllocator::shutdown()
   {
      CORE_ASSERT_DBGERR(m_allocations == 0, "Missing ", m_allocations, " deallocations for pool allocator.");
      m_parent->deallocateRaw(m_voidAddress, m_sizeBytes);
   }

   void* HeapAllocator::reallocateRaw(void* ptr, u32 osize, u32 nsize)
   {
      void* result = nullptr;
      if( nsize < osize )
      {
         //if the new size is less than the old size, we can just return the original pointer, and release the extra size
         //first we need to modify the size to be multiple of 16
         u32 sizeExpansionForFreelistPurposes = (ALIGNMENT - (nsize % ALIGNMENT));
         sizeExpansionForFreelistPurposes = sizeExpansionForFreelistPurposes & (ALIGNMENT - 1);
         nsize += sizeExpansionForFreelistPurposes;

         void* ptrToDeallocate = (u8*)ptr + nsize;
         if( osize - nsize )
         {
            deallocateRaw(ptrToDeallocate, osize - nsize);
            ++m_allocations;
         }
         result = ptr;
      }
      else //nsize > osize
      {
         HeapAllocator::Node* currentNode = &m_head;
         union
         {
            u8* byte;
            void* address;
         };
         address = ptr;
         while( currentNode->byteAddress < (byte + osize) )
         {
            currentNode = currentNode->next;
         }
         if( currentNode->byteAddress == byte + osize && currentNode->size >= (nsize - osize) )
         {
            //this means there is enough room to expand, so we need to allocate that piece
            u32 size = (nsize - osize);
            currentNode->size -= size;
            if( currentNode->size )
            {
               //case where there is more room in the slot after the requested size is removed
               HeapAllocator::Node* unmoved = currentNode->next;
               currentNode->byteAddress += size;
               currentNode->next->voidAddress = unmoved->voidAddress;
               currentNode->next->size = unmoved->size;
            }
            else
            {
               //case where the slot is exhausted by the requested size
               currentNode->size = currentNode->next->size;
               currentNode->voidAddress = currentNode->next->voidAddress;
            }
            m_allocatedBytes += size;
            memset(byte + osize, 0, size);
            result = ptr;
         }
         else
         {
            void* newPtr = allocateRaw(nsize, 16);
            if( newPtr )
            {
               memcpy(newPtr, ptr, osize);
               deallocateRaw(ptr, osize);
            }
            result = newPtr;
         }
      }

      return result;
   }
}
