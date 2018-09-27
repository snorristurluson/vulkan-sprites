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
    Area *oldArea = getFreeArea(width, height);

    if(!oldArea) {
        collapseFreeAreas();
        oldArea = getFreeArea(width, height);
    }

    if(!oldArea) {
        return nullptr;
    }

    auto newArea = new Area {oldArea->x, oldArea->y, width, height};
    m_allocatedAreas.emplace_front(newArea);


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

Area *AreaAllocator::getFreeArea(int width, int height)
{
    Area* oldArea = nullptr;
    auto foundIt = m_freeAreas.FindArea(width, height);
    if(foundIt != m_freeAreas.end()) {
        oldArea = *foundIt;
        m_freeAreas.erase(foundIt);
    }
    return oldArea;
}

int AreaAllocator::GetFreeAreaCount()
{
    return m_freeAreas.size();
}

int AreaAllocator::GetFreeAreaSize()
{
    return accumulateAreaSize(m_freeAreas);
}

int AreaAllocator::accumulateAreaSize(const AreaList &areaList) const
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

int AreaAllocator::GetTotalAreaSize()
{
    return m_width * m_height;
}

void AreaAllocator::collapseFreeAreas()
{
    if( m_freeAreas.size() < 2 )
    {
        return;
    }

    int collapsed = 0;
    do {
        collapsed = 0;
        AreaList collapsedAreas;
        while(!m_freeAreas.empty()) {
            auto first = m_freeAreas.front();
            m_freeAreas.pop_front();
            while(!m_freeAreas.empty()) {
                auto other = m_freeAreas.FindAdjacent(*first);
                if(other != m_freeAreas.end()) {
                    first->CombineWith(**other);
                    delete *other;
                    m_freeAreas.erase(other);
                    ++collapsed;
                } else {
                    break;
                }
            }
            collapsedAreas.emplace_back(first);
        }
        m_freeAreas = collapsedAreas;
    } while(collapsed > 0);
}

bool AreaAllocator::canCombineLeftRight(const Area *first, const Area *second) const
{
    bool canCombineLeftRight = false;
    if(first->y && second->y) {
        if(first->height == second->height) {
            if(first->x + first->width == second->x) {
                canCombineLeftRight = true;
            }
        }
    }
    return canCombineLeftRight;
}
