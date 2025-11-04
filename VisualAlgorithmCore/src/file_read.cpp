#include "file_read.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "exception.h"
#include "util.h"

namespace
{
enum class ObjKeyword
{
    Vertex,
    Texture,
    Normal,
    Face,
    Comment,
    Line,
    Object,
    Group
};

std::unordered_map<std::string, ObjKeyword> obj_file_map = {
    {"v", ObjKeyword::Vertex},  {"vt", ObjKeyword::Texture},
    {"vn", ObjKeyword::Normal}, {"f", ObjKeyword::Face},
    {"#", ObjKeyword::Comment}, {"l", ObjKeyword::Line},
    {"o", ObjKeyword::Object},  {"g", ObjKeyword::Group}};
}  // namespace

void OBJFileReadStream::read(std::string& file_name, Model* out_model)
{
    this->fs_ = std::ifstream{file_name};

    if (out_model == nullptr)
    {
        throw NullPointerError();
    }

    if (!fs_.is_open())
    {
        throw FileError("can't open file ");
        return;
    }

    std::string line;

    std::vector<Vector3> positions;
    std::vector<Vector2> texture_coors;
    std::vector<Vector3> normals;

    std::vector<Vertex>& final_vertices = out_model->vertex;
    std::vector<uint32_t>& final_indices = out_model->meshes;

    std::unordered_map<int64_t, uint32_t> history;

    while (std::getline(fs_, line))
    {
        // 한 줄씩 파싱
        std::vector<std::string> words = utils::split_string(line, ' ');

        if (!obj_file_map.contains(words[0])) continue;

        ObjKeyword head = obj_file_map[words[0]];  // 가장 앞 글자

        switch (head)
        {
            case ObjKeyword::Vertex:
            {
                positions.emplace_back(std::stof(words[1]), std::stof(words[2]),
                                       std::stof(words[3]));
                if (positions.size() >= MAX_NUMBER_OF_VERTEX_SIZE)
                    throw FileError(
                        "The total number of vertices must be less than "
                        "4,194,304");
                break;
            }
            case ObjKeyword::Texture:
            {
                texture_coors.emplace_back(std::stof(words[1]),
                                           std::stof(words[2]));
                if (texture_coors.size() >= MAX_NUMBER_OF_VERTEX_SIZE)
                    throw FileError(
                        "The total number of texture_coordinate must be less "
                        "than 4,194,304");
                break;
            }
            case ObjKeyword::Normal:
            {
                normals.emplace_back(std::stof(words[1]), std::stof(words[2]));
                if (normals.size() >= MAX_NUMBER_OF_VERTEX_SIZE)
                    throw FileError(
                        "The total number of vertices must be less "
                        "than 4,194,304");
                break;
            }
            case ObjKeyword::Face:
            {
                /* 면일 경우 정점 하나씩 파싱
                 * 만약 정점의 개수가*/
                for (auto iter = words.begin() + 1; iter != words.end(); ++iter)
                {
                    std::vector<std::string> vertex =
                        utils::split_string(*iter, '/');

                    int v = std::stoi(vertex[0]), vt = -1, vn = -1;

                    int64_t key = (v << 42);

                    if (vertex.size() > 1 && vertex[1] != "")
                    {
                        vt = std::stoi(vertex[1]);
                        key |= (vt << 21);
                    }
                    if (vertex.size() == 3)
                    {
                        vn = std::stoi(vertex[2]);
                        key |= vn;
                    }

                    if (history.contains(key))
                    {
                        final_indices.push_back(history[key]);
                        continue;
                    }

                    Vertex new_vertex{};

                    new_vertex.position = positions[v - 1];

                    if (vt >= 0)
                        new_vertex.texture_coor = texture_coors[vt - 1];
                    else
                        new_vertex.texture_coor = {0.0f, 0.0f};

                    if (vn >= 0)
                        new_vertex.normal = normals[vn - 1];
                    else
                    {
                        new_vertex.normal = {0.0f, 0.0f, 0.0f};
                    }

                    final_vertices.emplace_back(new_vertex);

                    uint32_t new_idx = final_vertices.size() - 1;
                    final_indices.push_back(new_idx);

                    history[key] = new_idx;
                }
            }
        }

        /* 법선 벡터 계산 (vn이 없을 경우 대비) */
        for (int i = 0; i < final_indices.size(); i += 3)
        {
            Vector3 p1 = final_vertices[i].position;
            Vector3 p2 = final_vertices[i + 1].position;
            Vector3 p3 = final_vertices[i + 2].position;

            Vector3 face_normal = cross(p3 - p1, p2 - p1);

            final_vertices[i].normal += face_normal;
        }

        /* 법선 정규화 */
        for (auto& vertex : final_vertices)
        {
            vertex.normal /= norm(vertex.normal);
        }
    }

    out_model->vertex = final_vertices;
}