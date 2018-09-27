//
// Created by Snorri Sturluson on 24/09/2018.
//

#ifndef VULKAN_SPRITES_AREALIST_H
#define VULKAN_SPRITES_AREALIST_H


#include "Area.h"
#include <list>

typedef std::list<Area*> AreaPtrList_t;

class AreaList : public AreaPtrList_t
{
public:
    AreaList::const_iterator FindAdjacent(const Area &area);

    AreaList::const_iterator FindArea(int width, int height);
};


#endif //VULKAN_SPRITES_AREALIST_H
