## Scene Location
I designed a scene for the turkish flag. It is in scenes/turkey.json
All my renderings are in the outputs directory.

## Bonus: Accelerating rasterization using bounding boxes.
I tried to render the following 
```
CircleScene hw1_2_scene_5 = {
    {1024, 1024}, // resolution
    {0.5, 0.5, 0.5}, // background
    { // center, radius, color
        {{512, 512}, 1024, {1.0, 0.0, 0.0}},
        {{512, 512}, 512, {0.8, 0.0, 0.0}},
        {{512, 512}, 256, {0.6, 0.0, 0.0}},
        {{512, 512}, 128, {0.4, 0.0, 0.0}},
        {{512, 512}, 64, {0.2, 0.0, 0.0}},
        {{512, 512}, 32, {0.0, 0.0, 0.0}},
        {{512, 512}, 16, {0.0, 1.0, 0.0}},
        {{512, 512}, 8, {0.0, 0.8, 0.0}},
        {{512, 512}, 4, {0.0, 0.6, 0.0}},
        {{512, 512}, 2, {0.0, 0.4, 0.0}},
        {{512, 512}, 1, {0.0, 0.2, 0.0}}
    }
};
```
Without bounding boxes it took 0.358383 seconds.
With bounding boxes it took 0.0897453 seconds.