//
// Created by Snorri Sturluson on 23/09/2018.
//

#ifndef VULKAN_SPRITES_AREAALLOCATOR_H
#define VULKAN_SPRITES_AREAALLOCATOR_H

#include <list>
#include "Area.h"
#include "AreaList.h"

// AreaAllocator allocates areas from a two-dimensional field. This serves as the basis for a texture
// atlas - this class focuses on the allocation and de-allocation of the areas, not concerning itself
// with the details of Vulkan buffers.
class AreaAllocator
{
public:
    AreaAllocator();

    void Initialize(int width, int height);

    int GetTotalWidth();
    int GetTotalHeight();
    int GetTotalAreaSize();
    int GetFreeAreaCount();
    int GetFreeAreaSize();
    int GetAllocatedAreaCount();
    int GetAllocatedAreaSize();

    Area* Allocate(int width, int height);

    void Free(Area *area);

protected:
    int m_width;
    int m_height;

    AreaList m_freeAreas;
    AreaList m_allocatedAreas;

    int accumulateAreaSize(const AreaList &areaList) const;

    Area *getFreeArea(int width, int height);

    void collapseFreeAreas();
};


#endif //VULKAN_SPRITES_AREAALLOCATOR_H
