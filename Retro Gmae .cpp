#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>

// --- Constants ---
const float GRAVITY = 0.6f;
const float MOVE_SPEED = 0.6f;
const float MAX_SPEED = 5.0f;
const float JUMP_FORCE = -13.0f;
const float FRICTION = 0.85f;

// --- Structs ---
struct Token {
    sf::CircleShape shape;
    bool collected = false;

    Token(float x, float y) {
        shape.setRadius(10.f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color(212, 175, 55)); // Gold
        shape.setPosition(x, y);
    }
};

struct Platform {
    sf::RectangleShape shape;

    Platform(float x, float y, float w, float h) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(w, h));
        shape.setFillColor(sf::Color(107, 140, 66)); // Grass Green
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color(64, 84, 40));
    }
};

struct Player {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool isGrounded = false;

    Player() {
        shape.setSize(sf::Vector2f(30.f, 30.f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(50.f, 400.f);
        velocity = {0.f, 0.f};
    }

    void update() {
        // Apply Gravity
        velocity.y += GRAVITY;

        // Apply Friction (X-axis only)
        velocity.x *= FRICTION;

        // Move
        shape.move(velocity);

        // Screen Boundaries
        if (shape.getPosition().x < 0)
            shape.setPosition(0, shape.getPosition().y);
        if (shape.getPosition().x + shape.getSize().x > 800)
            shape.setPosition(800 - shape.getSize().x, shape.getPosition().y);

        // Reset if fell off screen
        if (shape.getPosition().y > 600) {
            shape.setPosition(50, 400);
            velocity = {0, 0};
        }
    }
};

// --- Collision Logic ---
int checkCollision(Player& p, const Platform& plat) {
    sf::FloatRect playerBounds = p.shape.getGlobalBounds();
    sf::FloatRect wallBounds = plat.shape.getGlobalBounds();

    if (playerBounds.intersects(wallBounds)) {
        float overlapLeft = (playerBounds.left + playerBounds.width) - wallBounds.left;
        float overlapRight = (wallBounds.left + wallBounds.width) - playerBounds.left;
        float overlapTop = (playerBounds.top + playerBounds.height) - wallBounds.top;
        float overlapBottom = (wallBounds.top + wallBounds.height) - playerBounds.top;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY) {
            if (overlapLeft < overlapRight) {
                p.shape.move(-overlapLeft, 0);
                p.velocity.x = 0;
            } else {
                p.shape.move(overlapRight, 0);
                p.velocity.x = 0;
            }
        } else {
            if (overlapTop < overlapBottom) {
                p.shape.move(0, -overlapTop);
                p.velocity.y = 0;
                p.isGrounded = true;
            } else {
                p.shape.move(0, overlapBottom);
                p.velocity.y = 0;
            }
        }
    }
    return 0;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Retro Platformer C++");
    window.setFramerateLimit(60);

    // --- Font Handling ---
    // We define these variables so the code compiles, but we set hasFont to false
    // so it doesn't try to load anything that might crash.
    sf::Font font;
    bool hasFont = false;

    // Uncomment the line below ONLY if you have "arial.ttf" in your debug folder
    // hasFont = font.loadFromFile("arial.ttf");

    sf::Text scoreText;
    if (hasFont) {
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(20, 20);
    }

    // --- Game Objects ---
    Player player;
    int score = 0;

    std::vector<Platform> platforms;
    platforms.emplace_back(0.f, 550.f, 800.f, 50.f);    // Ground
    platforms.emplace_back(200.f, 450.f, 100.f, 20.f);
    platforms.emplace_back(400.f, 350.f, 100.f, 20.f);
    platforms.emplace_back(600.f, 250.f, 100.f, 20.f);
    platforms.emplace_back(100.f, 200.f, 80.f, 20.f);

    std::vector<Token> tokens;
    tokens.emplace_back(230.f, 410.f);
    tokens.emplace_back(440.f, 310.f);
    tokens.emplace_back(640.f, 210.f);
    tokens.emplace_back(130.f, 160.f);
    tokens.emplace_back(500.f, 510.f);
    tokens.emplace_back(700.f, 510.f);

    // --- Game Loop ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- Input ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (player.velocity.x > -MAX_SPEED) player.velocity.x -= MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (player.velocity.x < MAX_SPEED) player.velocity.x += MOVE_SPEED;
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            && player.isGrounded) {
            player.velocity.y = JUMP_FORCE;
            player.isGrounded = false;
        }

        // --- Physics ---
        player.update();
        player.isGrounded = false;

        for (auto& plat : platforms) {
            checkCollision(player, plat);
        }

        for (auto& t : tokens) {
            if (!t.collected && player.shape.getGlobalBounds().intersects(t.shape.getGlobalBounds())) {
                t.collected = true;
                score += 10;
                std::cout << "Score: " << score << std::endl; // Print score to console instead
            }
        }

        if (hasFont) {
            scoreText.setString("Score: " + std::to_string(score));
        }

        // --- Drawing ---
        window.clear(sf::Color(92, 148, 252));

        for (const auto& plat : platforms) window.draw(plat.shape);
        for (const auto& t : tokens) if (!t.collected) window.draw(t.shape);

        window.draw(player.shape);

        if (hasFont) window.draw(scoreText);

        window.display();
    }

    return 0;
}