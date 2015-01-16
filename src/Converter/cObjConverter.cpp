#include "cObjConverter.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <sstream>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool cObjConverter::Convert(const std::wstring & in, const std::wstring & out)
{
    std::ifstream input;
#ifdef _MSC_VER
    input.open(in.c_str());
#else
    std::string str(in.begin(), in.end());
    input.open(str.c_str());
#endif

    if (input.fail())
        return false;

    std::vector<Vector3F> positions;
    std::vector<Vector3F> normals;
    //unable to use UVW coordinates
    std::vector<Vector2F> texCoords;

    typedef std::tuple<long, long, long> key_t;
    struct key_hash : public std::unary_function < key_t, std::size_t >
    {
        std::size_t operator()(const key_t& k) const
        {
            return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
        }
    };

    //key is data from F .obj, value is index of vertex in M array
    std::unordered_map<key_t, unsigned long, key_hash> verticesMap;
    std::vector<unsigned long> indices;
    std::vector<Vertex> MeshData;

    std::string bufStr;

    while (getline(input, bufStr))
    {
        if (!bufStr.empty() && bufStr[0] == '#')
            continue;

        int DelimiterPosition = bufStr.find(' ');
        std::string cu = bufStr.substr(0, DelimiterPosition);

        if (cu == "v")
        {
            auto elems = split(bufStr, ' ');

            if (elems.size() != 4)
                return false;

            Vector3F pos = Vector3F(std::stof(elems[1]), std::stof(elems[2]), std::stof(elems[3]));
            positions.push_back(pos);
        }

        if (cu == "vt")
        {
            auto elems = split(bufStr, ' ');

            if (elems.size() != 3)
                return false;

            Vector2F tc = Vector2F(std::stof(elems[1]), std::stof(elems[2]));
            texCoords.push_back(tc);
        }

        if (cu == "vn")
        {
            auto elems = split(bufStr, ' ');

            if (elems.size() != 4)
                return false;

            Vector3F norm = Vector3F(std::stof(elems[1]), std::stof(elems[2]), std::stof(elems[3]));
            normals.push_back(norm);
        }

        if (cu == "f")
        {
            auto elems = split(bufStr, ' ');
            if (elems.size() != 4)
                return false;

            //check from 1 to 4 elem
            for (int i = 1; i < 4; i++)
            {
                auto subelems = split(elems[i], '/');
                key_t t;
                if (subelems.size() == 1)
                {
                    long index = std::stoi(subelems[0]) - 1;
                    t = std::make_tuple(index, index, index);
                }
                else if (subelems.size() == 2)
                {
                    long index1 = std::stoi(subelems[0]) - 1;
                    long index2 = std::stoi(subelems[1]) - 1;
                    t = std::make_tuple(index1, index1, index2);
                }
                else if (subelems.size() == 3)
                {
                    long index1 = std::stoi(subelems[0]) - 1;
                    long index2 = -1;
                    if (subelems[1].size() != 0)
                        index2 = std::stoi(subelems[1]) - 1;
                    long index3 = std::stoi(subelems[2]) - 1;
                    t = std::make_tuple(index1, index2, index3);
                }

                unsigned long index = 0;
                auto iter = verticesMap.find(t);
                if (iter != verticesMap.end())
                {
                    //key exists
                    index = iter->second;
                    indices.push_back(index);
                }
                else
                {
                    index = MeshData.size(); //must be equal but...
                    verticesMap[t] = index;
                    long posIndex = std::get<0>(t);
                    long texIndex = std::get<1>(t);
                    long normIndex = std::get<2>(t);
                    if (texIndex != -1)
                        MeshData.push_back(Vertex(positions[posIndex], normals[normIndex], texCoords[texIndex], {}));
                    else
                        MeshData.push_back(Vertex(positions[posIndex], normals[normIndex], {}, {}));
                    indices.push_back(index);
                }
            }
        }
    }

    MeshLIMHeader header;
    if (!normals.empty())
        header.hasNormals = true;
    if (!texCoords.empty())
        header.hasTexCoords = true;
    header.version = LIM_HEADER_VERSION;
    header.indicesCount = indices.size();
    header.verticesCount = MeshData.size();

    return LIMSaver::Save(out, MeshData.data(), indices.data(), header);
}