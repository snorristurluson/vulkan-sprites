//
// Created by Snorri Sturluson on 24/09/2018.
//

#include "AreaList.h"

AreaList::const_iterator AreaList::FindAdjacent(const Area &area)
{
    for(auto it = cbegin(); it != cend(); ++it) {
        if(area.IsAdjacent(**it)) {
            return it;
        }
    }
    return cend();
}

AreaList::const_iterator AreaList::FindArea(int width, int height)
{
    auto foundIt = cend();

    for(auto it = cbegin(); it != cend(); ++it) {
        if((*it)->width >= width && (*it)->height >= height) {
            foundIt = it;
            break;
        }
    }

    return foundIt;
}
