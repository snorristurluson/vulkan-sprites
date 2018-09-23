//
// Created by Snorri Sturluson on 23/09/2018.
//

#include "AreaAllocator.h"

void AreaAllocator::Initialize(int width, int height)
{
    m_width = width;
    m_height = height;

    m_freeAreas.emplace_back(new Area{0, 0, width, height});
}

int AreaAllocator::GetTotalWidth()
{
    return m_width;
}

int AreaAllocator::GetTotalHeight()
{
    return m_height;
}

Area* AreaAllocator::Allocate(int width, int height)
{
    AreaPtrList_t::iterator foundIt = m_freeAreas.end();

    for(auto it = m_freeAreas.begin(); it != m_freeAreas.end(); ++it) {
        if((*it)->width >= width && (*it)->height >= height) {
            foundIt = it;
            break;
        }
    }

    auto newArea = new Area {(*foundIt)->x, (*foundIt)->y, width, height};
    m_allocatedAreas.emplace_front(newArea);

    Area* oldArea = *foundIt;
    m_freeAreas.erase(foundIt);

    if(oldArea->width > width) {
        // Add an area to the right of newly allocated area
        m_freeAreas.emplace_back(new Area {oldArea->x + width, oldArea->y, oldArea->width - width, height});
    }
    if(oldArea->height > height) {
        // Add an area below the newly allocated area
        m_freeAreas.emplace_back(new Area {oldArea->x, oldArea->y + height, width, oldArea->height - height});
    }
    if(oldArea->width > width && oldArea->height > height) {
        // Add an area diagonally to the right and below the newly allocated area
        m_freeAreas.emplace_back(new Area {oldArea->x + width, oldArea->y + height, oldArea->width - width, oldArea->height - height});
    }
    return newArea;
}

int AreaAllocator::GetFreeAreaCount()
{
    return m_freeAreas.size();
}

int AreaAllocator::GetFreeAreaSize()
{
    return accumulateAreaSize(m_freeAreas);
}

int AreaAllocator::accumulateAreaSize(const AreaPtrList_t &areaList) const
{
    int size = 0;
    for(const auto& area: areaList) {
        size += area->width*area->height;
    }
    return size;
}

int AreaAllocator::GetAllocatedAreaCount()
{
    return m_allocatedAreas.size();
}

int AreaAllocator::GetAllocatedAreaSize()
{
    return accumulateAreaSize(m_allocatedAreas);
}

void AreaAllocator::Free(Area *area)
{
    m_allocatedAreas.remove(area);
    m_freeAreas.emplace_back(area);
}
