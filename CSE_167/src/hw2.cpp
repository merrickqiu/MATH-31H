#include "hw2.h"
#include "hw2_scenes.h"

using namespace hw2;

// Checks if p is in the triangle defined by v0, v1, v2
bool inTriangle(Vector2 p, Vector2 q0, Vector2 q1, Vector2 q2) {
    Vector2 v0 = q1 - q0;
    Vector2 v1 = q2 - q1;
    Vector2 v2 = q0 - q2;
    Vector2 n0{ v0.y, -v0.x };
    Vector2 n1{ v1.y, -v1.x };
    Vector2 n2{ v2.y, -v2.x };
    // overfill a little bit to avoid floating point triangle crease gaps
    bool b0 = dot(p - q0, n0) > -150;
    bool b1 = dot(p - q1, n1) > -150;
    bool b2 = dot(p - q2, n2) > -150;
    bool notb0 = dot(p - q0, n0) < 150;
    bool notb1 = dot(p - q1, n1) < 150;
    bool notb2 = dot(p - q2, n2) < 150;
    return (b0 && b1 && b2) || (notb0 && notb1 && notb2);
}

// divide by -z to project
Vector2 worldToCamera(Vector3 p) {
    return  { -p.x / p.z, -p.y / p.z };
}

// x'' = w(sa+x')/(2sa)
// y'' = h(s-y')/(2s)
Vector2 cameraToScreen(Vector2 p, Real s, Real w, Real h) {
    Real a = w / h;
    return Vector2{ w * (p.x + s * a) / (2 * s * a), h * (s - p.y) / (2 * s) };
}

// x' = (2sa/w)x''- sa
// y' = s - (2s/h)y''
Vector2 screenToCamera(Vector2 p, Real s, Real w, Real h) {
    Real a = w / h;
    return Vector2{ 2 * s * a * p.x / w - s * a, s - 2 * s * p.y / h };
}

// triangle area is half of parallelogram area
Real area(Vector2 p, Vector2 p0, Vector2 p1) {
    Vector3 u{ p0.x - p.x, p0.y - p.y, 0.0 };
    Vector3 v{ p1.x - p.x, p1.y - p.y, 0.0 };
    return length(cross(u, v))/ 2.0;
}

Image3 hw_2_1(const std::vector<std::string> &params) {
    // Homework 2.1: render a single 3D triangle

    Image3 img(640 /* width */, 480 /* height */);

    Vector3 p0{0, 0, -1};
    Vector3 p1{1, 0, -1};
    Vector3 p2{0, 1, -1};
    Real s = 1; // scaling factor of the view frustrum
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    Real z_near = 1e-6; // distance of the near clipping plane
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-p0") {
            p0.x = std::stof(params[++i]);
            p0.y = std::stof(params[++i]);
            p0.z = std::stof(params[++i]);
        } else if (params[i] == "-p1") {
            p1.x = std::stof(params[++i]);
            p1.y = std::stof(params[++i]);
            p1.z = std::stof(params[++i]);
        } else if (params[i] == "-p2") {
            p2.x = std::stof(params[++i]);
            p2.y = std::stof(params[++i]);
            p2.z = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        }
    }
    // Set Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }

    // Reject triangle if it's clipped
    if (-p0.z < z_near || -p1.z < z_near || -p2.z < z_near) {
        return img;
    }

    // Get 2D screen coordinates
    Vector2 v0 = cameraToScreen(worldToCamera(p0), s, img.width, img.height);
    Vector2 v1 = cameraToScreen(worldToCamera(p1), s, img.width, img.height);
    Vector2 v2 = cameraToScreen(worldToCamera(p2), s, img.width, img.height);
    
    // Render triangle
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            // subsample 4x4 subpixels
            double count = 0;
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    Vector2 p{ x + (0.125 + 0.25 * i), y + (0.125 + 0.25 * j)};
                    if (inTriangle(p, v0, v1, v2)) {
                        count++;
                    }
                }
            }
            img(x, y) = (count * color + (16.0 - count) * img(x, y)) / 16.0;
        }
    }
    return img;
}
// parameters
// pixel_camera, camera triangle vertices, world triangle depths
Vector3 getBarycentric(Vector2 p, Vector2 p0, Vector2 p1, Vector2 p2, Real p0_z, Real p1_z, Real p2_z) {
    Real triangleArea = area(p0, p1, p2);
    Real b0_c = area(p, p1, p2) / triangleArea;
    Real b1_c = area(p0, p, p2) / triangleArea;
    Real b2_c = area(p0, p1, p) / triangleArea;

    // convert to original barycentric coordinates
    Real B = b0_c / p0_z + b1_c / p1_z + b2_c / p2_z;
    Real b0 = b0_c / (p0_z * B);
    Real b1 = b1_c / (p1_z * B);
    Real b2 = b2_c / (p2_z* B);
    return Vector3{b0, b1, b2};
}
Image3 hw_2_2(const std::vector<std::string> &params) {
    // Homework 2.2: render a triangle mesh
    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }
    TriangleMesh mesh = meshes[scene_id];
    int n = mesh.faces.size();
    
    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{0.5, 0.5, 0.5};
        }
    }
    // For each triangle...
    std::vector<Real> zBuffer(img.width * img.height, std::numeric_limits<Real>::infinity());
    for (int i = 0; i < n; i++) {
        Vector3i face = mesh.faces[i];
        Vector3 color = mesh.face_colors[i];
        Vector3 triangle_w0 = mesh.vertices[face[0]];
        Vector3 triangle_w1 = mesh.vertices[face[1]];
        Vector3 triangle_w2 = mesh.vertices[face[2]];

        // Reject triangle if it's clipped
        if (-triangle_w0.z < z_near || -triangle_w1.z < z_near || -triangle_w2.z < z_near) {
            continue;
        }
        // Project world to camera space of triangle points
        Vector2 triangle_c0 = worldToCamera(triangle_w0);
        Vector2 triangle_c1 = worldToCamera(triangle_w1);
        Vector2 triangle_c2 = worldToCamera(triangle_w2);

        // Get 2D screen coordinates
        Vector2 triangle_s0 = cameraToScreen(triangle_c0, s, img.width, img.height);
        Vector2 triangle_s1 = cameraToScreen(triangle_c1, s, img.width, img.height);
        Vector2 triangle_s2 = cameraToScreen(triangle_c2, s, img.width, img.height);

        // For each pixel in the image space bounding box
        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                // Check if pixel in triangle
                Vector2 pixel_s{ x + 0.5, y + 0.5 };
                if (!inTriangle(pixel_s, triangle_s0, triangle_s1, triangle_s2)) {
                    continue;
                }

                // compute camera space barycentric coordinates
                Vector2 pixel_c = screenToCamera(pixel_s, s, img.width, img.height);
                
                Vector3 b = getBarycentric(pixel_c, triangle_c0, triangle_c1, triangle_c2,
                    triangle_w0.z, triangle_w1.z, triangle_w2.z);
                Real z = b.x * triangle_w0.z + b.y * triangle_w1.z + b.z * triangle_w2.z;
                // check if triangle is closer
                if (-z > zBuffer[y * img.width + x]) {
                    continue;
                }
                zBuffer[y * img.width + x] = -z;                

                // subsample 4x4 subpixels
                Real count = 0;
                for (int j = 0; j < 4; j++) {
                    for (int i = 0; i < 4; i++) {
                        Vector2 p{ x + (0.125 + 0.25 * i), y + (0.125 + 0.25 * j) };
                        if (inTriangle(p, triangle_s0, triangle_s1, triangle_s2)) {
                            count++;
                        }
                    }
                }
                img(x, y) = (count * color + (16.0 - count) * img(x, y)) / 16.0;
            }
        }
    }
    return img;
}

Image3 hw_2_3(const std::vector<std::string> &params) {
    // Homework 2.3: render a triangle mesh with vertex colors

    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }
    TriangleMesh mesh = meshes[scene_id];
    int n = mesh.faces.size();

    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{ 0.5, 0.5, 0.5 };
        }
    }
    // For each triangle...
    std::vector<Real> zBuffer(img.width * img.height, std::numeric_limits<Real>::infinity());
    for (int i = 0; i < n; i++) {
        Vector3i face = mesh.faces[i];
        Vector3 triangle_w0 = mesh.vertices[face[0]];
        Vector3 triangle_w1 = mesh.vertices[face[1]];
        Vector3 triangle_w2 = mesh.vertices[face[2]];
        Vector3 color0 = mesh.vertex_colors[face[0]];
        Vector3 color1 = mesh.vertex_colors[face[1]];
        Vector3 color2 = mesh.vertex_colors[face[2]];

        // Reject triangle if it's clipped
        if (-triangle_w0.z < z_near || -triangle_w1.z < z_near || -triangle_w2.z < z_near) {
            continue;
        }
        // Project world to camera space of triangle points
        Vector2 triangle_c0 = worldToCamera(triangle_w0);
        Vector2 triangle_c1 = worldToCamera(triangle_w1);
        Vector2 triangle_c2 = worldToCamera(triangle_w2);

        // Get 2D screen coordinates
        Vector2 triangle_s0 = cameraToScreen(triangle_c0, s, img.width, img.height);
        Vector2 triangle_s1 = cameraToScreen(triangle_c1, s, img.width, img.height);
        Vector2 triangle_s2 = cameraToScreen(triangle_c2, s, img.width, img.height);

        // For each pixel in the image space bounding box
        for (int y = 0; y < img.height; y++) {
            for (int x = 0; x < img.width; x++) {
                // Check if pixel in triangle
                Vector2 pixel_s{ x + 0.5, y + 0.5 };
                if (!inTriangle(pixel_s, triangle_s0, triangle_s1, triangle_s2)) {
                    continue;
                }

                // compute camera space barycentric coordinates
                Vector2 pixel_c = screenToCamera(pixel_s, s, img.width, img.height);

                Vector3 b = getBarycentric(pixel_c, triangle_c0, triangle_c1, triangle_c2,
                    triangle_w0.z, triangle_w1.z, triangle_w2.z);
                Real z = b.x * triangle_w0.z + b.y * triangle_w1.z + b.z * triangle_w2.z;
                // check if triangle is closer
                if (-z > zBuffer[y * img.width + x]) {
                    continue;
                }
                zBuffer[y * img.width + x] = -z;

                // subsample 4x4 subpixels
                Real count = 0;
                for (int j = 0; j < 4; j++) {
                    for (int i = 0; i < 4; i++) {
                        Vector2 p{ x + (0.125 + 0.25 * i), y + (0.125 + 0.25 * j) };
                        if (inTriangle(p, triangle_s0, triangle_s1, triangle_s2)) {
                            count++;
                        }
                    }
                }
                Vector3 color = b.x * color0 + b.y * color1 + b.z * color2;
                img(x, y) = (count * color + (16.0 - count) * img(x, y)) / 16.0;
            }
        }
    }
    return img;
}

Image3 hw_2_4(const std::vector<std::string> &params) {
    // Homework 2.4: render a scene with transformation
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    Real z_near = scene.camera.z_near;
    Real s = scene.camera.s;
    Matrix4x4 viewMatrix = inverse(scene.camera.cam_to_world);
    std::cout << scene << std::endl;

    Image3 img(scene.camera.resolution.x,
               scene.camera.resolution.y);
    // Background
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }
    
    for (TriangleMesh mesh : scene.meshes) {
        // For each triangle...
        int n = mesh.faces.size();
        Matrix4x4 M = mesh.model_matrix;
        
        std::vector<Real> zBuffer(img.width * img.height, std::numeric_limits<Real>::infinity());
        for (int i = 0; i < n; i++) {
            Vector3i face = mesh.faces[i];
            Vector3 triangle_o0 = mesh.vertices[face[0]];
            Vector3 triangle_o1 = mesh.vertices[face[1]];
            Vector3 triangle_o2 = mesh.vertices[face[2]];
            Vector4 triangle_o0_4D{ triangle_o0.x,  triangle_o0.y, triangle_o0.z, 1.0 };
            Vector4 triangle_o1_4D{ triangle_o1.x,  triangle_o1.y, triangle_o1.z, 1.0 };
            Vector4 triangle_o2_4D{ triangle_o2.x,  triangle_o2.y, triangle_o2.z, 1.0 };
            Vector3 color0 = mesh.vertex_colors[face[0]];
            Vector3 color1 = mesh.vertex_colors[face[1]];
            Vector3 color2 = mesh.vertex_colors[face[2]];
            
            // Transform object to world
            Vector4 triangle_w0_4D = M * triangle_o0_4D;
            Vector4 triangle_w1_4D = M * triangle_o1_4D;
            Vector4 triangle_w2_4D = M * triangle_o2_4D;
            Vector3 triangle_w0{ triangle_w0_4D.x, triangle_w0_4D.y, triangle_w0_4D.z };
            Vector3 triangle_w1{ triangle_w1_4D.x, triangle_w1_4D.y, triangle_w1_4D.z };
            Vector3 triangle_w2{ triangle_w2_4D.x, triangle_w2_4D.y, triangle_w2_4D.z };

            // Transform world to unprojected camera space
            Vector4 triangle_uc0_4D = viewMatrix * triangle_w0_4D;
            Vector4 triangle_uc1_4D = viewMatrix * triangle_w1_4D;
            Vector4 triangle_uc2_4D = viewMatrix * triangle_w2_4D;
            Vector3 triangle_uc0{ triangle_uc0_4D.x, triangle_uc0_4D.y, triangle_uc0_4D.z };
            Vector3 triangle_uc1{ triangle_uc1_4D.x, triangle_uc1_4D.y, triangle_uc1_4D.z };
            Vector3 triangle_uc2{ triangle_uc2_4D.x, triangle_uc2_4D.y, triangle_uc2_4D.z };

            // Reject triangle if it's clipped
            if (-triangle_uc0.z < z_near || -triangle_uc1.z < z_near || -triangle_uc2.z < z_near) {
                continue;
            }

            // Project world to camera space of triangle points
            // function should be called "cameraToProjectedCamera" but name stuck from previous parts
            Vector2 triangle_c0 = worldToCamera(triangle_uc0);
            Vector2 triangle_c1 = worldToCamera(triangle_uc1);
            Vector2 triangle_c2 = worldToCamera(triangle_uc2);

            // Get 2D screen coordinates
            Vector2 triangle_s0 = cameraToScreen(triangle_c0, s, img.width, img.height);
            Vector2 triangle_s1 = cameraToScreen(triangle_c1, s, img.width, img.height);
            Vector2 triangle_s2 = cameraToScreen(triangle_c2, s, img.width, img.height);

            // For each pixel in the image space bounding box
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    // Check if pixel in triangle
                    Vector2 pixel_s{ x + 0.5, y + 0.5 };
                    if (!inTriangle(pixel_s, triangle_s0, triangle_s1, triangle_s2)) {
                        continue;
                    }
                    //std::cout << "Rendering triangle" << std::endl;

                    // compute camera space barycentric coordinates
                    Vector2 pixel_c = screenToCamera(pixel_s, s, img.width, img.height);

                    Vector3 b = getBarycentric(pixel_c, triangle_c0, triangle_c1, triangle_c2,
                        triangle_w0.z, triangle_w1.z, triangle_w2.z);
                    Real z = b.x * triangle_w0.z + b.y * triangle_w1.z + b.z * triangle_w2.z;
                    // check if triangle is closer
                    if (-z > zBuffer[y * img.width + x]) {
                        continue;
                    }
                    zBuffer[y * img.width + x] = -z;

                    // subsample 4x4 subpixels
                    Real count = 0;
                    for (int j = 0; j < 4; j++) {
                        for (int i = 0; i < 4; i++) {
                            Vector2 p{ x + (0.125 + 0.25 * i), y + (0.125 + 0.25 * j) };
                            if (inTriangle(p, triangle_s0, triangle_s1, triangle_s2)) {
                                count++;
                            }
                        }
                    }
                    Vector3 color = b.x * color0 + b.y * color1 + b.z * color2;
                    img(x, y) = (count * color + (16.0 - count) * img(x, y)) / 16.0;
                }
            }
        }
    }
    return img; 
}

