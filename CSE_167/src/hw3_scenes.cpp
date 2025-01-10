#include "hw3_scenes.h"
#include "3rdparty/json.hpp"
#include "3rdparty/tinyply.h"
#include "flexception.h"
#include <fstream>

using json = nlohmann::json;

namespace hw3 {

TriangleMesh parse_ply(const fs::path &filename) {
    std::ifstream ifs(filename, std::ios::binary);
    tinyply::PlyFile ply_file;
    ply_file.parse_header(ifs);

    std::shared_ptr<tinyply::PlyData> vertices, faces, vertex_colors, uvs, vertex_normals;
    try {
        vertices = ply_file.request_properties_from_element("vertex", { "x", "y", "z"}); 
    } catch (const std::exception &e) { 
        Error(std::string("Vertex positions not found in ") + filename.string());
    }
    try {
        faces = ply_file.request_properties_from_element("face", { "vertex_indices"}); 
    } catch (const std::exception &e) { 
        Error(std::string("Vertex indices not found in ") + filename.string());
    }
    try {
        vertex_colors = ply_file.request_properties_from_element("vertex", { "red", "green", "blue"});
    } catch (const std::exception &e) {
        // Some meshes may not have vertex colors
    }
    try {
        uvs = ply_file.request_properties_from_element("vertex", {"s", "t"});
    } catch (const std::exception &e) {
        // Some meshes may not have UVs
    }
    try {
        vertex_normals = ply_file.request_properties_from_element("vertex", {"nx", "ny", "nz"});
    } catch (const std::exception &e) {
        // Some meshes may not have vertex normals
    }

    ply_file.read(ifs);

    TriangleMesh mesh;
    mesh.vertices.resize(vertices->count);
    if (vertices->t == tinyply::Type::FLOAT32) {
        float *data = (float*)vertices->buffer.get();
        for (size_t i = 0; i < vertices->count; i++) {
            mesh.vertices[i] = Vector3f{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else if (vertices->t == tinyply::Type::FLOAT64) {
        double *data = (double*)vertices->buffer.get();
        for (size_t i = 0; i < vertices->count; i++) {
            mesh.vertices[i] = Vector3f{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else {
        Error(std::string("Unknown type of vertex positions in ") + filename.string());
    }

    if (vertex_colors && vertex_colors->count > 0) {
        mesh.vertex_colors.resize(vertex_colors->count);
        if (vertex_colors->t == tinyply::Type::FLOAT32) {
            float *data = (float*)vertex_colors->buffer.get();
            for (size_t i = 0; i < vertex_colors->count; i++) {
                mesh.vertex_colors[i] = Vector3f{
                    data[3 * i], data[3 * i + 1], data[3 * i + 2]};
            }
        } else if (vertex_colors->t == tinyply::Type::FLOAT64) {
            double *data = (double*)vertex_colors->buffer.get();
            for (size_t i = 0; i < vertex_colors->count; i++) {
                mesh.vertex_colors[i] = Vector3f{
                    data[3 * i], data[3 * i + 1], data[3 * i + 2]};
            }
        } else if (vertex_colors->t == tinyply::Type::UINT8) {
            uint8_t *data = (uint8_t*)vertex_colors->buffer.get();
            for (size_t i = 0; i < vertex_colors->count; i++) {
                mesh.vertex_colors[i] = Vector3f{
                    std::pow(float(data[3 * i]) / 255, 2.2f),
                    std::pow(float(data[3 * i + 1]) / 255, 2.2f),
                    std::pow(float(data[3 * i + 2]) / 255, 2.2f)};
            }
        } else {
            Error(std::string("Unknown type of vertex colors in ") + filename.string());
        }
    }

    if (uvs && uvs->count > 0) {
        mesh.uvs.resize(uvs->count);
        if (uvs->t == tinyply::Type::FLOAT32) {
            float *data = (float*)uvs->buffer.get();
            for (size_t i = 0; i < uvs->count; i++) {
                mesh.uvs[i] = Vector2f{
                    data[2 * i], data[2 * i + 1]};
            }
        } else if (uvs->t == tinyply::Type::FLOAT64) {
            double *data = (double*)uvs->buffer.get();
            for (size_t i = 0; i < uvs->count; i++) {
                mesh.uvs[i] = Vector2f{
                    data[2 * i], data[2 * i + 1]};
            }
        } else {
            Error(std::string("Unknown type of UV in ") + filename.string());
        }
    }

    if (vertex_normals && vertex_normals->count > 0) {
        mesh.vertex_normals.resize(vertex_normals->count);
        if (vertex_normals->t == tinyply::Type::FLOAT32) {
            float *data = (float*)vertex_normals->buffer.get();
            for (size_t i = 0; i < vertex_normals->count; i++) {
                mesh.vertex_normals[i] = Vector3f{
                    data[3 * i], data[3 * i + 1], data[3 * i + 2]};
            }
        } else if (vertex_normals->t == tinyply::Type::FLOAT64) {
            double *data = (double*)vertex_normals->buffer.get();
            for (size_t i = 0; i < vertex_normals->count; i++) {
                mesh.vertex_normals[i] = Vector3f{
                    data[3 * i], data[3 * i + 1], data[3 * i + 2]};
            }
        } else {
            Error(std::string("Unknown type of vertex normals in ") + filename.string());
        }
    }

    mesh.faces.resize(faces->count);
    if (faces->t == tinyply::Type::INT8) {
        int8_t *data = (int8_t*)faces->buffer.get();
        for (size_t i = 0; i < faces->count; i++) {
            mesh.faces[i] = Vector3i{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else if (faces->t == tinyply::Type::UINT8) {
        uint8_t *data = (uint8_t*)faces->buffer.get();
        for (size_t i = 0; i < faces->count; i++) {
            mesh.faces[i] = Vector3i{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else if (faces->t == tinyply::Type::INT16) {
        int16_t *data = (int16_t*)faces->buffer.get();
        for (size_t i = 0; i < faces->count; i++) {
            mesh.faces[i] = Vector3i{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else if (faces->t == tinyply::Type::UINT16) {
        uint16_t *data = (uint16_t*)faces->buffer.get();
        for (size_t i = 0; i < faces->count; i++) {
            mesh.faces[i] = Vector3i{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else if (faces->t == tinyply::Type::INT32) {
        int32_t *data = (int32_t*)faces->buffer.get();
        for (size_t i = 0; i < faces->count; i++) {
            mesh.faces[i] = Vector3i{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else if (faces->t == tinyply::Type::UINT32) {
        uint32_t *data = (uint32_t*)faces->buffer.get();
        for (size_t i = 0; i < faces->count; i++) {
            mesh.faces[i] = Vector3i{
                data[3 * i], data[3 * i + 1], data[3 * i + 2]};
        }
    } else {
        Error(std::string("Unknown type of faces in ") + filename.string());
    }

    return mesh;
}

Matrix4x4 parse_transformation(const json &node) {
    // Homework 2.4: parse a sequence of linear transformation and 
 // combine them into a 4x4 transformation matrix
    Matrix4x4 F = Matrix4x4::identity();
    auto transform_it = node.find("transform");
    if (transform_it == node.end()) {
        // Transformation not specified, return identity.
        return F;
    }

    for (auto it = transform_it->begin(); it != transform_it->end(); it++) {
        if (auto scale_it = it->find("scale"); scale_it != it->end()) {
            Vector3 scale = Vector3{
                (*scale_it)[0], (*scale_it)[1], (*scale_it)[2]
            };
            // TODO (HW2.4): construct a scale matrix and composite with F
            Matrix4x4 S = Matrix4x4::identity();
            S(0, 0) = scale[0];
            S(1, 1) = scale[1];
            S(2, 2) = scale[2];
            F = S * F;
        }
        else if (auto rotate_it = it->find("rotate"); rotate_it != it->end()) {
            Real angle = c_PI / 180 * (*rotate_it)[0];
            Vector3 axis = normalize(Vector3{
                (*rotate_it)[1], (*rotate_it)[2], (*rotate_it)[3]
                });
            // TODO (HW2.4): construct a rotation matrix and composite with F
            Matrix4x4 R = Matrix4x4::identity();
            Vector3 v_x{ 1.0, 0.0, 0.0 };
            Vector3 v_y{ 0.0, 1.0, 0.0 };
            Vector3 v_z{ 0.0, 0.0, 1.0 };
            Vector3 vc_x = axis.x * axis;
            Vector3 vc_y = axis.y * axis;
            Vector3 vc_z = axis.z * axis;
            Vector3 v1_x = v_x - vc_x;
            Vector3 v1_y = v_y - vc_y;
            Vector3 v1_z = v_z - vc_z;
            Vector3 v2_x = cross(axis, v1_x);
            Vector3 v2_y = cross(axis, v1_y);
            Vector3 v2_z = cross(axis, v1_z);
            Vector3 wx = vc_x + cos(angle) * v1_x + sin(angle) * v2_x;
            Vector3 wy = vc_y + cos(angle) * v1_y + sin(angle) * v2_y;
            Vector3 wz = vc_z + cos(angle) * v1_z + sin(angle) * v2_z;
            R(0, 0) = wx.x;
            R(1, 0) = wx.y;
            R(2, 0) = wx.z;
            R(0, 1) = wy.x;
            R(1, 1) = wy.y;
            R(2, 1) = wy.z;
            R(0, 2) = wz.x;
            R(1, 2) = wz.y;
            R(2, 2) = wz.z;
            F = R * F;
        }
        else if (auto translate_it = it->find("translate"); translate_it != it->end()) {
            Vector3 translate = Vector3{
                (*translate_it)[0], (*translate_it)[1], (*translate_it)[2]
            };
            // TODO (HW2.4): construct a translation matrix and composite with F
            Matrix4x4 T = Matrix4x4::identity();
            T(0, 3) = translate[0];
            T(1, 3) = translate[1];
            T(2, 3) = translate[2];
            F = T * F;
        }
        else if (auto lookat_it = it->find("lookat"); lookat_it != it->end()) {
            Vector3 position{ 0, 0, 0 };
            Vector3 target{ 0, 0, -1 };
            Vector3 up{ 0, 1, 0 };
            auto position_it = lookat_it->find("position");
            auto target_it = lookat_it->find("target");
            auto up_it = lookat_it->find("up");
            if (position_it != lookat_it->end()) {
                position = Vector3{
                    (*position_it)[0], (*position_it)[1], (*position_it)[2]
                };
            }
            if (target_it != lookat_it->end()) {
                target = Vector3{
                    (*target_it)[0], (*target_it)[1], (*target_it)[2]
                };
            }
            if (up_it != lookat_it->end()) {
                up = normalize(Vector3{
                    (*up_it)[0], (*up_it)[1], (*up_it)[2]
                    });
            }
            // TODO (HW2.4): construct a lookat matrix and composite with F
            Matrix4x4 L = Matrix4x4::identity();
            Vector3 d = normalize(target - position);
            Vector3 r = normalize(cross(d, up));
            Vector3 u = cross(r, d);
            L(0, 0) = r.x;
            L(1, 0) = r.y;
            L(2, 0) = r.z;
            L(0, 1) = u.x;
            L(1, 1) = u.y;
            L(2, 1) = u.z;
            L(0, 2) = -d.x;
            L(1, 2) = -d.y;
            L(2, 2) = -d.z;
            L(0, 3) = position.x;
            L(1, 3) = position.y;
            L(2, 3) = position.z;
            F = L * F;
        }
    }
    return F;
}

Scene parse_scene(const fs::path &filename) {
    std::ifstream f(filename.string().c_str());
    json data = json::parse(f);

    // back up the current working directory and switch to the parent folder of the file
    fs::path old_path = fs::current_path();
    fs::current_path(filename.parent_path());

    Scene scene;

    auto camera = data.find("camera");
    if (camera == data.end()) {
        Error("Scene does not contain the field \"camera\".");
        return scene;        
    }

    auto res = camera->find("resolution");
    if (res == camera->end()) {
        Error("Camera does not contain the field \"resolution\".");
        return scene;
    }
    scene.camera.resolution = Vector2i{(*res)[0], (*res)[1]};
    scene.camera.cam_to_world = parse_transformation(*camera);
    scene.camera.s = 1;
    if (auto s = camera->find("s"); s != camera->end()) {
        scene.camera.s = *s;
    }
    scene.camera.z_near = 1e-3f;
    if (auto z_near = camera->find("z_near"); z_near != camera->end()) {
        scene.camera.z_near = *z_near;
    }
    scene.camera.z_far = 1e3f;
    if (auto z_far = camera->find("z_far"); z_far != camera->end()) {
        scene.camera.z_far = *z_far;
    }

    auto background = data.find("background");
    scene.background = Vector3{1, 1, 1};
    if (background != data.end()) {
        scene.background = Vector3{
            (*background)[0], (*background)[1], (*background)[2]
        };
    }

    auto objects = data.find("objects");
    for (auto it = objects->begin(); it != objects->end(); it++) {
        TriangleMesh mesh;
        if (auto fn_it = it->find("filename"); fn_it != it->end()) {
            mesh = parse_ply(std::string(*fn_it));
        } else {
            auto vertices_it = it->find("vertices");
            if (vertices_it != it->end()) {
                int num_vertices = vertices_it->size() / 3;
                mesh.vertices.resize(num_vertices);
                for (int i = 0; i < num_vertices; i++) {
                    mesh.vertices[i] = Vector3f{
                        (*vertices_it)[3 * i + 0],
                        (*vertices_it)[3 * i + 1],
                        (*vertices_it)[3 * i + 2]
                    };
                }
            }
            auto faces_it = it->find("faces");
            if (faces_it != it->end()) {
                int num_triangles = faces_it->size() / 3;
                mesh.faces.resize(num_triangles);
                for (int i = 0; i < num_triangles; i++) {
                    mesh.faces[i] = Vector3i{
                        (*faces_it)[3 * i + 0],
                        (*faces_it)[3 * i + 1],
                        (*faces_it)[3 * i + 2]
                    };
                }
            }
            auto vertex_colors_it = it->find("vertex_colors");
            if (vertex_colors_it != it->end()) {
                int num_vertices = vertex_colors_it->size() / 3;
                mesh.vertex_colors.resize(num_vertices);
                if (mesh.vertex_colors.size() != mesh.vertices.size()) {
                    Error("Mesh has different number of vertices and number of colors.");
                    return scene;
                }
                for (int i = 0; i < num_vertices; i++) {
                    mesh.vertex_colors[i] = Vector3f{
                        (*vertex_colors_it)[3 * i + 0],
                        (*vertex_colors_it)[3 * i + 1],
                        (*vertex_colors_it)[3 * i + 2]
                    };
                }
            }
        }
        
        mesh.model_matrix = parse_transformation(*it);
        scene.meshes.push_back(mesh);
    }

    // switch back to the old current working directory
    fs::current_path(old_path);
    return scene;
}

std::ostream& operator<<(std::ostream &os, const Camera &camera) {
    os << "Camera[" << std::endl;
    os << "\tcam_to_world=" << std::endl << camera.cam_to_world << std::endl;
    os << "\tresolution=" << camera.resolution << std::endl;
    os << "\ts=" << camera.s << std::endl;
    os << "\tz_near=" << camera.z_near << std::endl;
    os << "\tz_far=" << camera.z_far << std::endl;
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream &os, const TriangleMesh &mesh) {
    os << "TriangleMesh[" << std::endl;
    os << "\tnum_vertices=" << mesh.vertices.size() << std::endl;
    os << "\tnum_faces=" << mesh.faces.size() << std::endl;
    os << "\ttransform=" << std::endl << mesh.model_matrix << std::endl;
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream &os, const Scene &scene) {
    os << "Scene[" << std::endl;
    os << "\t" << scene.camera << std::endl;
    os << "\tBackground:" << scene.background << std::endl;
    for (auto s : scene.meshes) {
        os << "\t" << s << std::endl;
    }
    os << "]";
    return os;
}

} // namespace hw3