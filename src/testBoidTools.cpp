#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include "boidTools.hpp"

#define MIN_SEPARATION 1e-6 // For float comparison

bool testGetAvgPosition() {
    
    // Subtest 1
    bool test1_success = false;
    boidLocation p1, p2, p3, p4;
    p1.position = sf::Vector2f(1.f, 0.f);
    p2.position = sf::Vector2f(0.f, 1.f);
    p3.position = sf::Vector2f(-1.f, 0.f);
    p4.position = sf::Vector2f(0.f, -1.f);

    boidLocation boids[4] = {p1, p2, p3, p4};
    sf::Vector2f avgPosition = getAvgPosition(4, boids);
    if (abs(avgPosition.x - 0.0) < MIN_SEPARATION &&
        abs(avgPosition.y - 0.0) < MIN_SEPARATION) {
        printf("testGetAvgPosition subtest 1 success.\n");
        test1_success = true;
    } else {
        printf("testGetAvgPosition subtest 1 FAILED.\n");
    }

    return test1_success;
}

bool testGetNeighboringVector() {

    boidLocation mainBoid, n1, n2, n3;
    mainBoid.position = sf::Vector2f(1.f, 1.f);

    n1.position = sf::Vector2f(3.f, 1.f);
    n2.position = sf::Vector2f(4.f, -2.f);
    n3.position = sf::Vector2f(-10.f, 0.f);

    boidLocation neighbors[3] = {n1, n2, n3};

    sf::Vector2f neighborVector = getNeighboringVector(3, neighbors, mainBoid.position);
    printf("Vector: (%fi, %fj)\n", neighborVector.x, neighborVector.y);
}

void testAlignmentVector() {

    boidLocation b1, b2, b3;
    b1.rotation = 90;
    b2.rotation = 0;

    boidLocation neighbors[2] = {b1, b2};

    sf::Vector2f alignmentVector = getAlignmentVector(2, neighbors);
    printf("Alignment Vector: (%fi, %fj)\n", alignmentVector.x, alignmentVector.y);

}


int main() {

    testGetAvgPosition();
    testGetNeighboringVector();
    testAlignmentVector();
    return 0;
}