#include "hw1.h"
#include "hw1_scenes.h"
#include <algorithm>
#include <functional>

using namespace hw1;

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            center = Vector2{x, y};
        } else if (params[i] == "-radius") {
            radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }
    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{ 0.5, 0.5, 0.5 };
        }
    }

    // Render Circle
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Vector2 p = Vector2{ x + Real(0.5), y + Real(0.5) };
            if (length(p-center) < radius) {
                img(x, y) = color;
            }
        }
    }

    
    return img;
}

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: render multiple circles
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    int scene_id = std::stoi(params[0]);
    const CircleScene &scene = hw1_2_scenes[scene_id];

    Image3 img(scene.resolution.x, scene.resolution.y);

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Rasterization with bounding boxes
    const bool USE_BOUNDING_BOXES = true;
    for (Circle circle : scene.objects) {
        if (USE_BOUNDING_BOXES) {
            Vector2 p_min = circle.center - circle.radius;
            Vector2 p_max = circle.center + circle.radius;
            for (int y = std::max((int)p_min.y, 0); y < std::min((int)p_max.y, img.height); y++) {
                for (int x = std::max((int)p_min.x, 0); x < std::min((int)p_max.x, img.width); x++) {
                    Vector2 p = Vector2{ x + Real(0.5), y + Real(0.5) };
                    if (length(p - circle.center) < circle.radius) {
                        img(x, y) = circle.color;
                    }
                }
            }
        }
        else {
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    Vector2 p = Vector2{ x + Real(0.5), y + Real(0.5) };
                    if (length(p - circle.center) < circle.radius) {
                        img(x, y) = circle.color;
                    }
                }
            }
        }
        
    }
    
    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: render multiple shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Render shapes
    for (Shape shape : scene.shapes) {
        if (Circle* circle = std::get_if<Circle>(&shape)) {
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    Vector2 p = Vector2{ x + Real(0.5), y + Real(0.5) };
                    if (length(p - circle->center) < circle->radius) {
                        img(x, y) = circle->color;
                    }
                }
            }
        }
        else if (Rectangle* rectangle = std::get_if<Rectangle>(&shape)) {
            for (int y = std::max((int)rectangle->p_min.y, 0); y < std::min((int)rectangle->p_max.y, img.height); y++) {
                for (int x = std::max((int)rectangle->p_min.x, 0); x < std::min((int)rectangle->p_max.x, img.width); x++) {
                    img(x, y) = rectangle->color;
                }
            }
        }
        else if (Triangle* triangle = std::get_if<Triangle>(&shape)) {
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    Vector2 q = Vector2{ x + Real(0.5), y + Real(0.5) };
                    Vector2 v0 = triangle->p1 - triangle->p0;
                    Vector2 v1 = triangle->p2 - triangle->p1;
                    Vector2 v2 = triangle->p0 - triangle->p2;
                    Vector2 n0 = Vector2{ v0.y, -v0.x };
                    Vector2 n1 = Vector2{ v1.y, -v1.x };
                    Vector2 n2 = Vector2{ v2.y, -v2.x };
                    bool b0 = dot(q - triangle->p0, n0) > 0;
                    bool b1 = dot(q - triangle->p1, n1) > 0;
                    bool b2 = dot(q - triangle->p2, n2) > 0;
                    if (b0 && b1 && b2 || !b0 && !b1 && !b2) {
                        img(x, y) = triangle->color;
                    }
                }
            }
        }
    }
    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.4: render transformed shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Render Shapes
    for (Shape shape : scene.shapes) {
        if (Circle* circle = std::get_if<Circle>(&shape)) {
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    Vector3 objSpaceP = Vector3{ x + Real(0.5), y + Real(0.5), 1.0 };
                    objSpaceP = inverse(circle->transform) * objSpaceP;
                    Vector2 p = Vector2{ objSpaceP.x, objSpaceP.y };

                    if (length(p - circle->center) < (circle->radius)) {
                        img(x, y) = circle->color;
                    }
                }
            }
        }
        else if (Rectangle* rectangle = std::get_if<Rectangle>(&shape)) {
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    Vector3 objSpaceP = Vector3{ x + Real(0.5), y + Real(0.5), 1.0 };
                    objSpaceP = inverse(rectangle->transform) * objSpaceP;
                    Vector2 p = Vector2{ objSpaceP.x, objSpaceP.y };

                    bool abovePMin = (p.x >= (rectangle->p_min.x)) && (p.y >= (rectangle->p_min.y));
                    bool belowPMax = (p.x <= (rectangle->p_max.x)) && (p.y <= (rectangle->p_max.y));
                    if (abovePMin && belowPMax) {
                        img(x, y) = rectangle->color;
                    }
                }
            }
        }
        else if (Triangle* triangle = std::get_if<Triangle>(&shape)) {
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    Vector3 objSpaceQ = Vector3{ x + Real(0.5), y + Real(0.5), 1.0 };
                    objSpaceQ = inverse(triangle->transform) * objSpaceQ;
                    Vector2 q = Vector2{ objSpaceQ.x, objSpaceQ.y };
                    
                    Vector2 v0 = triangle->p1 - triangle->p0;
                    Vector2 v1 = triangle->p2 - triangle->p1;
                    Vector2 v2 = triangle->p0 - triangle->p2;
                    Vector2 n0 = Vector2{ v0.y, -v0.x };
                    Vector2 n1 = Vector2{ v1.y, -v1.x };
                    Vector2 n2 = Vector2{ v2.y, -v2.x };
                    bool b0 = dot(q - triangle->p0, n0) > 0;
                    bool b1 = dot(q - triangle->p1, n1) > 0;
                    bool b2 = dot(q - triangle->p2, n2) > 0;
                    if ((b0 && b1 && b2) || (!b0 && !b1 && !b2)) {
                        img(x, y) = triangle->color;
                    }
                }
            }
        }
    }
    return img;
}

bool inCircle(Circle* circle, Real x, Real y) {
    Vector3 objSpaceP = Vector3{ x, y, 1.0 };
    objSpaceP = inverse(circle->transform) * objSpaceP;
    Vector2 p = Vector2{ objSpaceP.x, objSpaceP.y };

    return length(p - circle->center) < (circle->radius);
}

bool inRectangle(Rectangle* rectangle, Real x, Real y) {
    Vector3 objSpaceP = Vector3{ x, y, 1.0 };
    objSpaceP = inverse(rectangle->transform) * objSpaceP;
    Vector2 p = Vector2{ objSpaceP.x, objSpaceP.y };

    bool abovePMin = (p.x > (rectangle->p_min.x)) && (p.y >= (rectangle->p_min.y));
    bool belowPMax = (p.x <= (rectangle->p_max.x)) && (p.y <= (rectangle->p_max.y));
    return abovePMin && belowPMax;
}

bool inTriangle(Triangle* triangle, Real x, Real y) {
    Vector3 objSpaceQ = Vector3{ x, y, 1.0 };
    objSpaceQ = inverse(triangle->transform) * objSpaceQ;
    Vector2 q = Vector2{ objSpaceQ.x, objSpaceQ.y };

    Vector2 v0 = triangle->p1 - triangle->p0;
    Vector2 v1 = triangle->p2 - triangle->p1;
    Vector2 v2 = triangle->p0 - triangle->p2;
    Vector2 n0 = Vector2{ v0.y, -v0.x };
    Vector2 n1 = Vector2{ v1.y, -v1.x };
    Vector2 n2 = Vector2{ v2.y, -v2.x };
    bool b0 = dot(q - triangle->p0, n0) > 0;
    bool b1 = dot(q - triangle->p1, n1) > 0;
    bool b2 = dot(q - triangle->p2, n2) > 0;
    return (b0 && b1 && b2) || (!b0 && !b1 && !b2);
}

bool inShape(Shape* shape, Real x, Real y) {
    if (Circle* circle = std::get_if<Circle>(shape)) {
        return inCircle(circle, x, y);
    }
    else if (Rectangle* rectangle = std::get_if<Rectangle>(shape)) {
        return inRectangle(rectangle, x, y);
    }
    else if (Triangle* triangle = std::get_if<Triangle>(shape)) {
        return  inTriangle(triangle, x, y);
    }
}
Vector3 getColor(Shape* shape) {
    if (Circle* circle = std::get_if<Circle>(shape)) {
        return circle->color;
    }
    else if (Rectangle* rectangle = std::get_if<Rectangle>(shape)) {
        return rectangle->color;
    }
    else if (Triangle* triangle = std::get_if<Triangle>(shape)) {
        return triangle->color;
    }
}
Real getAlpha(Shape* shape) {
    if (Circle* circle = std::get_if<Circle>(shape)) {
        return circle->alpha;
    }
    else if (Rectangle* rectangle = std::get_if<Rectangle>(shape)) {
        return rectangle->alpha;
    }
    else if (Triangle* triangle = std::get_if<Triangle>(shape)) {
        return triangle->alpha;
    }
}
Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Render Shapes
    for (Shape shape : scene.shapes) {        
        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                // subsample 4x4 subpixels
                double count = 0.0;
                for (int j = 0; j < 4; j++) {
                    for (int i = 0; i < 4; i++) {
                        if (inShape(&shape, x + (0.125+0.25*i), y + (0.125+0.25*j))) {
                            count++;
                        }
                    }
                }
                
                img(x, y) = (count * getColor(&shape) + (16.0 - count) * img(x, y)) / 16.0;  
            }
        }
    }
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }

    // Render Shapes
    for (Shape shape : scene.shapes) {
        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                // subsample 4x4 subpixels
                double count = 0;
                for (int j = 0; j < 4; j++) {
                    for (int i = 0; i < 4; i++) {
                        if (inShape(&shape, x + (0.125 + 0.25 * i), y + (0.125 + 0.25 * j))) {
                            count++;
                        }
                    }
                }
                Vector3 color = (count * getColor(&shape) + (16 - count) * img(x, y)) / 16.0;
                Real a = getAlpha(&shape);
                img(x, y) = a * color + (1.0 - a) * img(x, y);
            }
        }
    }
    return img;
}
