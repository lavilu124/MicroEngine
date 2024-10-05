#include <SFML/Graphics.hpp>
#include <cmath>

// Function to calculate attenuation based on distance
float calculateAttenuation(float distance, float maxDistance) {
    if (distance > maxDistance) {
        return 0.0f;  // Light is out of range
    }
    return 1.0f - (distance / maxDistance);  // Linear attenuation
}

// Function to apply point light
void applyPointLight(sf::RenderTexture& texture, sf::Vector2f lightPosition, sf::Color lightColor, float radius) {
    sf::VertexArray vertices(sf::Points, texture.getSize().x * texture.getSize().y);

    for (unsigned int x = 0; x < texture.getSize().x; ++x) {
        for (unsigned int y = 0; y < texture.getSize().y; ++y) {
            sf::Vector2f pixelPosition(x, y);
            float distance = std::hypot(pixelPosition.x - lightPosition.x, pixelPosition.y - lightPosition.y);

            // Calculate attenuation
            float attenuation = calculateAttenuation(distance, radius);

            // Mix light color with the background
            sf::Color newColor = lightColor;
            newColor.r = static_cast<sf::Uint8>(lightColor.r * attenuation);
            newColor.g = static_cast<sf::Uint8>(lightColor.g * attenuation);
            newColor.b = static_cast<sf::Uint8>(lightColor.b * attenuation);
            newColor.a = 255;  // Full opacity

            // Set the vertex at this position with the new color
            vertices[x + y * texture.getSize().x].position = pixelPosition;
            vertices[x + y * texture.getSize().x].color = newColor;
        }
    }

    texture.draw(vertices);
}

// Function to apply directional light
void applyDirectionalLight(sf::RenderTexture& texture, sf::Vector2f direction, sf::Color lightColor, float intensity) {
    sf::VertexArray vertices(sf::Points, texture.getSize().x * texture.getSize().y);

    for (unsigned int x = 0; x < texture.getSize().x; ++x) {
        for (unsigned int y = 0; y < texture.getSize().y; ++y) {
            sf::Vector2f pixelPosition(x, y);

            // Simple directional light based on dot product
            float dotProduct = direction.x * (pixelPosition.x / texture.getSize().x) + direction.y * (pixelPosition.y / texture.getSize().y);
            dotProduct = std::max(dotProduct, 0.0f);  // Clamp negative values

            // Apply light color and intensity
            sf::Color newColor = lightColor;
            newColor.r = static_cast<sf::Uint8>(lightColor.r * dotProduct * intensity);
            newColor.g = static_cast<sf::Uint8>(lightColor.g * dotProduct * intensity);
            newColor.b = static_cast<sf::Uint8>(lightColor.b * dotProduct * intensity);
            newColor.a = 255;  // Full opacity

            // Set the vertex at this position with the new color
            vertices[x + y * texture.getSize().x].position = pixelPosition;
            vertices[x + y * texture.getSize().x].color = newColor;
        }
    }

    texture.draw(vertices);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simple Light System");

    // Create a render texture for the lighting effects
    sf::RenderTexture lightTexture;
    lightTexture.create(800, 600);

    sf::Vector2f lightPosition(400, 300);
    sf::Vector2f lightDirection(-0.5f, -0.5f);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the light texture each frame
        lightTexture.clear(sf::Color::Black);

        // Update light position based on mouse
        lightPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

        // Apply point light and directional light
        applyPointLight(lightTexture, lightPosition, sf::Color(255, 255, 100), 300.0f);
        applyDirectionalLight(lightTexture, lightDirection, sf::Color(200, 200, 255), 0.5f);

        // Draw to the main window
        window.clear();
        lightTexture.display();
        sf::Sprite lightSprite(lightTexture.getTexture());
        window.draw(lightSprite);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <cmath>

// Function to calculate attenuation based on distance
float calculateAttenuation(float distance, float maxDistance) {
    if (distance > maxDistance) {
        return 0.0f;  // Light is out of range
    }
    return 1.0f - (distance / maxDistance);  // Linear attenuation
}

// Function to apply point light
void applyPointLight(sf::RenderTexture& texture, sf::Vector2f lightPosition, sf::Color lightColor, float radius) {
    sf::VertexArray vertices(sf::Points, texture.getSize().x * texture.getSize().y);

    for (unsigned int x = 0; x < texture.getSize().x; ++x) {
        for (unsigned int y = 0; y < texture.getSize().y; ++y) {
            sf::Vector2f pixelPosition(x, y);
            float distance = std::hypot(pixelPosition.x - lightPosition.x, pixelPosition.y - lightPosition.y);

            // Calculate attenuation
            float attenuation = calculateAttenuation(distance, radius);

            // Mix light color with the background
            sf::Color newColor = lightColor;
            newColor.r = static_cast<sf::Uint8>(lightColor.r * attenuation);
            newColor.g = static_cast<sf::Uint8>(lightColor.g * attenuation);
            newColor.b = static_cast<sf::Uint8>(lightColor.b * attenuation);
            newColor.a = 255;  // Full opacity

            // Set the vertex at this position with the new color
            vertices[x + y * texture.getSize().x].position = pixelPosition;
            vertices[x + y * texture.getSize().x].color = newColor;
        }
    }

    texture.draw(vertices);
}

// Function to apply directional light
void applyDirectionalLight(sf::RenderTexture& texture, sf::Vector2f direction, sf::Color lightColor, float intensity) {
    sf::VertexArray vertices(sf::Points, texture.getSize().x * texture.getSize().y);

    for (unsigned int x = 0; x < texture.getSize().x; ++x) {
        for (unsigned int y = 0; y < texture.getSize().y; ++y) {
            sf::Vector2f pixelPosition(x, y);

            // Simple directional light based on dot product
            float dotProduct = direction.x * (pixelPosition.x / texture.getSize().x) + direction.y * (pixelPosition.y / texture.getSize().y);
            dotProduct = std::max(dotProduct, 0.0f);  // Clamp negative values

            // Apply light color and intensity
            sf::Color newColor = lightColor;
            newColor.r = static_cast<sf::Uint8>(lightColor.r * dotProduct * intensity);
            newColor.g = static_cast<sf::Uint8>(lightColor.g * dotProduct * intensity);
            newColor.b = static_cast<sf::Uint8>(lightColor.b * dotProduct * intensity);
            newColor.a = 255;  // Full opacity

            // Set the vertex at this position with the new color
            vertices[x + y * texture.getSize().x].position = pixelPosition;
            vertices[x + y * texture.getSize().x].color = newColor;
        }
    }

    texture.draw(vertices);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simple Light System");

    // Create a render texture for the lighting effects
    sf::RenderTexture lightTexture;
    lightTexture.create(800, 600);

    sf::Vector2f lightPosition(400, 300);
    sf::Vector2f lightDirection(-0.5f, -0.5f);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the light texture each frame
        lightTexture.clear(sf::Color::Black);

        // Update light position based on mouse
        lightPosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

        // Apply point light and directional light
        applyPointLight(lightTexture, lightPosition, sf::Color(255, 255, 100), 300.0f);
        applyDirectionalLight(lightTexture, lightDirection, sf::Color(200, 200, 255), 0.5f);

        // Draw to the main window
        window.clear();
        lightTexture.display();
        sf::Sprite lightSprite(lightTexture.getTexture());
        window.draw(lightSprite);
        window.display();
    }

    return 0;
}
