//
// Created by Snorri Sturluson on 24/09/2018.
//

#ifndef VULKAN_SPRITES_AREA_H
#define VULKAN_SPRITES_AREA_H

struct Area {
    int x;
    int y;
    int width;
    int height;

    bool IsAdjacent(const Area &other) const;
    bool CombineWith(const Area& other);
};

#endif //VULKAN_SPRITES_AREA_H
