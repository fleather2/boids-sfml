#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>

struct boidLocation {
    sf::Vector2f position;
    float rotation;
};

float toDegrees(float radian) {
    return (radian * (180.0f / M_PI));
}

float toRadians(float degrees) {
    return (degrees * (M_PI / 180.0f));
}

sf::Vector2f getAvgPosition(int numBoids, boidLocation boids[]) {
    /*
        Return the average position of all boids
    */
    sf::Vector2f avgPosition(0.f, 0.f);
    for (int i = 0; i < numBoids; i++) {
        avgPosition.x += boids[i].position.x/numBoids;
        avgPosition.y += boids[i].position.y/numBoids;
    }
    return avgPosition;
}

sf::Vector2f getNeighboringVector(int numFlockmateNeighbors, boidLocation flockmateNeighbors[], sf::Vector2f boidPosition) {
    // Get normalizaed vector from boidPosition to the flockmates
    if (numFlockmateNeighbors > 0) {
        sf::Vector2f localAvgPosition = getAvgPosition(numFlockmateNeighbors, flockmateNeighbors);
        sf::Vector2f cohesionVector(localAvgPosition.x - boidPosition.x, localAvgPosition.y - boidPosition.y);
        float normalizationFactor = sqrt(pow(cohesionVector.x, 2) + pow(cohesionVector.y, 2));
        cohesionVector.x = cohesionVector.x / normalizationFactor;
        cohesionVector.y = cohesionVector.y / normalizationFactor;
        return cohesionVector;
    } else {
        return sf::Vector2f(0.f, 0.f);
    }
}

sf::Vector2f getSeparationVector(int numFlockmatesInProtected, boidLocation flockmatesInProtected[], sf::Vector2f boidPosition) {
    /*
    Returns the separation vector for the boid at boidLocation.
    */

    sf::Vector2f separationVector = getNeighboringVector(numFlockmatesInProtected, flockmatesInProtected, boidPosition);
    separationVector.x *= -1.f;
    separationVector.y *= -1.f;

    return separationVector;
}

sf::Vector2f getCohesionVector(int numFlockmatesInLocal, boidLocation flockmatesInLocal[], sf::Vector2f boidPosition) {
    return getNeighboringVector(numFlockmatesInLocal, flockmatesInLocal, boidPosition);
}

sf::Vector2f getAlignmentVector(int numFlockmatesInLocal, boidLocation flockmatesInLocal[]) {
    // Convert all rotations to a vector. Then take the average of all the vectors
    sf::Vector2f avgAlignmentVector(0.f, 0.f);
    if (numFlockmatesInLocal > 0) {
    for (int i = 0; i < numFlockmatesInLocal; i++) {
            avgAlignmentVector.x += cos(toRadians(flockmatesInLocal[i].rotation))/numFlockmatesInLocal;
            avgAlignmentVector.y += sin(toRadians(flockmatesInLocal[i].rotation))/numFlockmatesInLocal;
        }
        float normalizationFactor = sqrt(pow(avgAlignmentVector.x, 2) + pow(avgAlignmentVector.y, 2));
        avgAlignmentVector.x = avgAlignmentVector.x / normalizationFactor;
        avgAlignmentVector.y = avgAlignmentVector.y / normalizationFactor;
    }
    return avgAlignmentVector;
}

// Just sum up all the vectors with their weights and 
// add the vector of the current heading (with a relatively large weight)
// and then normalize the vector, face that way,
// scale down with some factor (speed), and move according to that vector.