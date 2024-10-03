#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include "boidTools.hpp"

#define MAX_X 1700
#define MAX_Y 1000

#define SPEED 0.30
#define NUM_BOIDS 100
#define RADIUS_PROTECTED_RANGE 100.0
#define RADIUS_LOCAL_RANGE 500.0

#define SEPARATION_STEER_FACTOR .005   // Separation: steer to avoid crowding boids in protected range
#define ALIGNMENT_STEER_FACTOR .0005   // Alignment: steer towards the average heading in local range
#define COHESION_STEER_FACTOR .001     // Cohesion: steer to move towards the average position in local range
#define STEER_SPEED .003

int main()
{
    sf::RenderWindow window(sf::VideoMode(MAX_X, MAX_Y), "Boids");

    // TODO anti-aliasing
    sf::ConvexShape boids[NUM_BOIDS];

    for (int i=0; i < NUM_BOIDS; i++) {
        // Define boid shape
        sf::ConvexShape boidShape;
        boidShape.setPointCount(4);
        boidShape.setOrigin(sf::Vector2f(100.f, 100.f)); // Set origin to center of "cutout" in boid shape
        boidShape.setPoint(0, sf::Vector2f(100.f, 100.f));  // Middle
        boidShape.setPoint(1, sf::Vector2f(95.f, 95.f));    // Bottom-left
        boidShape.setPoint(2, sf::Vector2f(110.f, 100.f));  // Right
        boidShape.setPoint(3, sf::Vector2f(95.f, 105.f));   // Top-left
        boidShape.setPosition(sf::Vector2f(rand()%MAX_X, rand()%MAX_Y)); // Move away from edge of screen
        boidShape.setRotation(rand()%360);
        boids[i] = boidShape;
    }

    boidLocation localFlockmates[NUM_BOIDS]; // Array used to calculate local flockmates
    boidLocation protectedFlockmates[NUM_BOIDS]; //Array used to calculate protected flockamtes
    sf::Vector2f targetVectors[NUM_BOIDS];

    sf::Vector2f separationTargetVector(0.f, 0.f);
    sf::Vector2f alignmentTargetVector(0.f, 0.f);
    sf::Vector2f cohesionTargetVector(0.f, 0.f);
    sf::Vector2f totalTargetVector(0.f, 0.f);
    sf::Vector2f currentVector(0.f, 0.f);

    float distance;
    int idxLocal, idxProtected;

    // Main loop
    while (window.isOpen())
    {
        // Event loop: Check the window events that were triggered since the last iteration of the loop
        // Required so that the window can process its internal events (window can be moved/resized)
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    std::cout << "Mouse clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
                    break;
                default:
                    break;
            }
        }

        for (int i = 0; i < NUM_BOIDS; i++) {
            idxLocal = 0;
            idxProtected = 0;
            for (int j = 0; j < NUM_BOIDS, j != i; j++) {
                distance = sqrt(pow((boids[j].getPosition().x - boids[i].getPosition().x), 2) + pow(boids[j].getPosition().y - boids[i].getPosition().y, 2));
                if (distance <= RADIUS_LOCAL_RANGE) {
                    localFlockmates[idxLocal].position = boids[j].getPosition();
                    localFlockmates[idxLocal].rotation = boids[j].getRotation();
                    idxLocal++;
                }
                if (distance <= RADIUS_PROTECTED_RANGE) {
                    protectedFlockmates[idxProtected].position = boids[j].getPosition();
                    protectedFlockmates[idxProtected].rotation = boids[j].getRotation();
                    idxProtected++;
                }
            }

            // Get vectors based on neighbors
            separationTargetVector = getSeparationVector(idxProtected, protectedFlockmates, boids[i].getPosition());
            cohesionTargetVector = getCohesionVector(idxLocal, localFlockmates, boids[i].getPosition());
            alignmentTargetVector = getAlignmentVector(idxLocal, localFlockmates);
            
            currentVector.x = cos(toRadians(boids[i].getRotation()));
            currentVector.y = sin(toRadians(boids[i].getRotation()));

            totalTargetVector.x = currentVector.x + separationTargetVector.x*SEPARATION_STEER_FACTOR + cohesionTargetVector.x*COHESION_STEER_FACTOR + alignmentTargetVector.x*ALIGNMENT_STEER_FACTOR;
            totalTargetVector.y = currentVector.y + separationTargetVector.y*SEPARATION_STEER_FACTOR + cohesionTargetVector.y*COHESION_STEER_FACTOR + alignmentTargetVector.y*ALIGNMENT_STEER_FACTOR;
            // Normalize target vector
            float normalizationFactor = sqrt(pow(totalTargetVector.x, 2) + pow(totalTargetVector.y, 2));
            totalTargetVector.x = totalTargetVector.x / normalizationFactor;
            totalTargetVector.y = totalTargetVector.y / normalizationFactor;
            targetVectors[i] = totalTargetVector;
        }
        
        for (int i = 0; i < NUM_BOIDS; i++) {
            
            boids[i].setRotation(toDegrees(atan2(targetVectors[i].y, targetVectors[i].x)));
            boids[i].move(targetVectors[i].x*SPEED, targetVectors[i].y*SPEED);

            // Wrap the world around
            if (boids[i].getPosition().x >= MAX_X) {
                boids[i].setPosition(0, boids[i].getPosition().y);
            } else if (boids[i].getPosition().x <= 0.0) {
                boids[i].setPosition(MAX_X, boids[i].getPosition().y);
            }
            if (boids[i].getPosition().y >= MAX_Y) {
                boids[i].setPosition(boids[i].getPosition().x, 0);
            } else if (boids[i].getPosition().y <= 0) {
                boids[i].setPosition(boids[i].getPosition().x, MAX_Y);
            }
        }

        // Display everything
        window.clear();
        for (int i=0; i < NUM_BOIDS; i++) {
            window.draw(boids[i]);
        }
        window.display();
        
    }

    return 0;
}