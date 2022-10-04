#pragma once
#include <vector>
#include <delaunator-cpp/delaunator.hpp>

std::vector<std::size_t> calculateCircumcenters(delaunator::Delaunator d) {
    for (std::size_t i = 0; i < d.triangles.size(); i += 3) {
        delaunator::circumcenter(
            d.coords[2 * d.triangles[i]], //ax
            d.coords[2 * d.triangles[i] + 1], //ay
            d.coords[2 * d.triangles[i + 1]], //bx
            d.coords[2 * d.triangles[i + 1] + 1],//by
            d.coords[2 * d.triangles[i + 2]],//cx
            d.coords[2 * d.triangles[i + 2] + 1]);//cy;
    }
}


class Map {
public:
    std::vector<std::array<float, 2>> points;
    delaunator::Delaunator delaunay;
    size_t numRegions = points.size();
    size_t numTriangles = delaunay.halfedges.size()/3;
    size_t numEdges = delaunay.halfedges.size();
    std::vector<std::size_t> halfedges = delaunay.halfedges;
    std::vector<std::size_t> triangles = delaunay.triangles;
    std::vector<std::size_t> circumcenters = calculateCircumcenters(delaunay);
};