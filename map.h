#pragma once
#include <vector>
#include <array>
#include<string>

/*Point generation*/
#include"thinks/poisson_disk_sampling/poisson_disk_sampling.h"
/*Delaunay trianglulation*/
#include<delaunator-cpp/delaunator.hpp>



class Map {
   
public:
    
    std::vector<std::array<float, 2>> points;
    std::vector<double> vertices;
    size_t numRegions;
    size_t numTriangles;
    size_t numEdges;
    std::vector<std::size_t> halfedges;
    std::vector<std::size_t> triangles;
    std::vector<std::pair<double, double>> circumcenters;
    std::vector<double> voronoi_edges;


    std::vector<double> construct_voronoi_edges(const delaunator::Delaunator& delaunay);
    std::vector<std::pair<double, double>> calculateCircumcenters(delaunator::Delaunator d);
    std::vector<std::array<float, 2>> generate_points();
    std::vector<double> flatten_vertices(std::vector<std::array<float, 2>> points);

    Map(){
        std::vector<std::array<float, 2>> points = generate_points();
        std::vector<double> vertices = flatten_vertices(points);
        delaunator::Delaunator delaunay(vertices);
        size_t numRegions = points.size();
        size_t numTriangles = delaunay.halfedges.size() / 3;
        size_t numEdges = delaunay.halfedges.size();
        std::vector<std::size_t> halfedges = delaunay.halfedges;
        std::vector<std::size_t> triangles = delaunay.triangles;
        std::vector<std::pair<double, double>> circumcenters = calculateCircumcenters(delaunay);
        std::vector<double> voronoi_edges = construct_voronoi_edges(delaunay);

    }



};


std::vector<double> Map::construct_voronoi_edges(const delaunator::Delaunator& delaunay) {
    /*
    Calculates the circumcenters of each of delaunay the triangles at the beginning and end of a half edge and
    returns all the lines between them in a vector = [x1,y1,x1,y2.....] and so on
    */
    std::vector<double> line_edges;

    for (size_t e = 0; e < delaunay.triangles.size(); e += 3) {
        if (e < delaunay.halfedges[e/3]) {
            const auto p = delaunator::circumcenter(delaunay.coords[2 * delaunay.triangles[e]], delaunay.coords[2 * delaunay.triangles[e] + 1],
                delaunay.coords[2 * delaunay.triangles[e + 1]], delaunay.coords[2 * delaunay.triangles[e + 1] + 1], 2 * delaunay.coords[delaunay.triangles[e + 2]], delaunay.coords[2 * delaunay.triangles[e + 2] + 1]);

            const auto q = delaunator::circumcenter(delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e]]], delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e/3]] + 1], delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e/3 + 1]]], delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e/3 + 1]] + 1], delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e/3 + 2]]], delaunay.coords[2 * delaunay.triangles[delaunay.halfedges[e/3 + 2]] + 1]);

            line_edges.push_back(p.first);
            line_edges.push_back(p.second);
            line_edges.push_back(q.first);
            line_edges.push_back(q.second);

        }
    }
    unsigned int index = 0;
    int count = 0;
    std::cout << "Beep Boop, Printing....." << '\n';
    for (auto i : line_edges)
    {

        index++;
        if ((index % 4) != 0) {
            std::cout << i << ',';
        }
        else
        {

            std::cout << i << ',' << '\n';
        };
        count++;
    }
    std::cout << count/4 <<  " voronoi edges created from:" << '\n';
    std::cout << delaunay.triangles.size() << " triangles" << '\n';

    return line_edges;
}



std::vector<std::pair<double,double>> Map::calculateCircumcenters(delaunator::Delaunator d) {
    std::vector<std::pair<double, double>> c_centers;
    for (std::size_t i = 0; i < d.triangles.size(); i += 3) {
        
       c_centers.push_back(delaunator::circumcenter(
            d.coords[2 * d.triangles[i]], //ax
            d.coords[2 * d.triangles[i] + 1], //ay
            d.coords[2 * d.triangles[i + 1]], //bx
            d.coords[2 * d.triangles[i + 1] + 1],//by
            d.coords[2 * d.triangles[i + 2]],//cx
            d.coords[2 * d.triangles[i + 2] + 1]));//cy;
    }
    return c_centers;
}

std::vector<std::array<float, 2>> Map::generate_points()
{

    // Input parameters.
    constexpr float max_val = 0.900f;
    constexpr float kRadius = 0.088f; //0.008 seems good
    constexpr auto kXMin = std::array<float, 2>{ {-max_val, -max_val}};
    constexpr auto kXMax = std::array<float, 2>{ {max_val, max_val}};
    const std::uint32_t max_sample_attempts = 30;
    std::vector<std::array<float, 2>> point_data = thinks::PoissonDiskSampling(kRadius, kXMin, kXMax, max_sample_attempts);
    // Samples returned as std::vector<std::array<float, 2>>.
    // Default seed and max sample attempts.
    return point_data;
}

std::vector<double> Map::flatten_vertices(std::vector<std::array<float, 2>> points) {
    std::vector<double> coords;
    for (auto point : points) {
        for (auto i : point) {
            coords.push_back((double)i);
            //std::cout << i << '\n';
        }
    }
    return coords;

}


// formatting reminder
//for (std::size_t i = 0; i < d.triangles.size(); i += 3) {
//    printf(
//        "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
//        d.coords[2 * d.triangles[i]],        //tx0
//        d.coords[2 * d.triangles[i] + 1],    //ty0
//        d.coords[2 * d.triangles[i + 1]],    //tx1
//        d.coords[2 * d.triangles[i + 1] + 1],//ty1
//        d.coords[2 * d.triangles[i + 2]],    //tx2
//        d.coords[2 * d.triangles[i + 2] + 1] //ty2
//    );