//
// Created by Snorri Sturluson on 23/09/2018.
//

#ifndef VULKAN_SPRITES_AREAALLOCATOR_H
#define VULKAN_SPRITES_AREAALLOCATOR_H

#include <list>

struct Area {
    int x;
    int y;
    int width;
    int height;
};

typedef std::list<Area*> AreaPtrList_t;

// AreaAllocator allocates areas from a two-dimensional field. This serves as the basis for a texture
// atlas - this class focuses on the allocation and de-allocation of the areas, not concerning itself
// with the details of Vulkan buffers.
class AreaAllocator
{
public:
    void Initialize(int width, int height);

    int GetTotalWidth();
    int GetTotalHeight();
    int GetFreeAreaCount();
    int GetFreeAreaSize();
    int GetAllocatedAreaCount();
    int GetAllocatedAreaSize();

    Area* Allocate(int width, int height);

    void Free(Area *area);

protected:
    int m_width;
    int m_height;

    AreaPtrList_t m_freeAreas;
    AreaPtrList_t m_allocatedAreas;

    int accumulateAreaSize(const AreaPtrList_t &areaList) const;
};


#endif //VULKAN_SPRITES_AREAALLOCATOR_H
