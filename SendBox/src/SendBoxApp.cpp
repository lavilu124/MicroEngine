#include <iostream>
#include <iomanip>
#include <memory>
#include <ctime>

#include "LightInclude.h"
#include "Micro/SystemLogic/Light/Constants.hpp"

float clamp(float x) {
    return std::max(0.f, std::min(1.f, x));
}
struct App {
    /*
    * WINDOW
    */
    constexpr static const float WIDTH = 700.f;
    constexpr static const float HEIGHT = 700.f;
    constexpr static const float MENU_W = HEIGHT / 8.f;
    sf::RenderWindow w;
    sf::View sandboxView, menuView;

    /*
    * LIGHTING
    */
    Micro::LightingArea lighting;
    bool glow = true;
    bool persistent_fog = true;
    std::vector<std::shared_ptr<Micro::LightSource>> lights1; // all
    std::vector<std::shared_ptr<Micro::LightSource>> lights2; // glowing
    Micro::EdgeVector edgePool;
    sf::VertexArray edgeVertices;
    sf::Texture fogTex;

    /*
    * INTERACTIVITY - Brushes - Lights
    */
    enum Brush {
        NONE = -1,
        RADIAL = 0,
        DIRECTED = 1,
        BLOCK = 2,
        LINE = 3
    };
    Brush brush;
    bool lineStarted;

    sf::VertexArray mouseBlock;
    float blockSize;

    Micro::RadialLight radialLight;
    Micro::DirectedLight directedLight;
    bool control, shift, alt;

    /*
    * INTERACTIVITY - Menu
    */
    struct Button : sf::Drawable, sf::Transformable {
        static sf::Color buttonA1;
        static sf::Color buttonA2;
        static sf::Color buttonZ1;
        static sf::Color buttonZ2;
        static int BC;
        sf::RectangleShape rect;
        sf::Drawable* icon;
        void (*function)(App*);
        Button(sf::Drawable* d, void (*f)(App*)) {
            float bw = (MENU_W - 4);
            rect.setFillColor(buttonZ1);
            rect.setOutlineColor(buttonZ2);
            rect.setOutlineThickness(2);
            rect.setSize({ bw, bw });
            setPosition(2, 2 + (BC) * (bw + 4));
            icon = d;
            function = f;
            BC++;
        }
        ~Button() {
            // delete icon;
        }
        bool contains(const sf::Vector2f& p) const {
            return getTransform().transformRect(rect.getGlobalBounds()).contains(p);
        }
        void draw(sf::RenderTarget& t, sf::RenderStates st) const {
            st.transform = this->getTransform();
            t.draw(rect, st);
            if (icon != NULL)
                t.draw(*icon, st);
        }
    };
    std::vector<Button> buttons;

    /*
    * SUBROUTINES
    */
    sf::Vector2f getMousePosition() {
        sf::Vector2f mp = w.mapPixelToCoords(sf::Mouse::getPosition(w));
        return mp;
    }
    App()
        : lighting(Micro::LightingArea::FOG, { 0.f,0.f }, { WIDTH, HEIGHT })
    {
        w.create(sf::VideoMode(WIDTH + MENU_W, HEIGHT), "Candle - demo");
        w.setFramerateLimit(60);
        float totalWidth = WIDTH + MENU_W;
        edgeVertices.setPrimitiveType(sf::Lines);
        if (fogTex.loadFromFile("texture.png")) {
            lighting.setAreaTexture(&fogTex);
            lighting.scale(WIDTH / fogTex.getSize().x, HEIGHT / fogTex.getSize().y);
        }
        else {
            std::cout << "No texture detected" << std::endl;
            lighting.setAreaColor(sf::Color::Black);
        }
        lighting.clear();
        mouseBlock.setPrimitiveType(sf::Lines);
        mouseBlock.resize(8);
        sandboxView.setSize(WIDTH, HEIGHT);
        sandboxView.setCenter(WIDTH / 2.f, HEIGHT / 2.f);
        sandboxView.setViewport({ 0.f, 0.f, WIDTH / totalWidth, 1.f });
        menuView.setSize(MENU_W, HEIGHT);
        menuView.setCenter(MENU_W / 2, HEIGHT / 2);
        menuView.setViewport({ WIDTH / totalWidth, 0.f, MENU_W / totalWidth, 1.f });
        radialLight.setRange(100.f);
        directedLight.setRange(200.f);
        directedLight.setBeamWidth(200.f);
        static const sf::Color BG_COLORS[] = {
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Red
        };
        int colors = sizeof(BG_COLORS) / sizeof(*BG_COLORS);
        
        brush = NONE;
        lineStarted = false;
        control = false;
        shift = false;
        alt = false;
    }
    void capture() {
        sf::Texture tex;
        tex.create(w.getSize().x, w.getSize().y);
        tex.update(w);

        sf::RenderTexture rt;
        rt.create(WIDTH, HEIGHT);
        rt.setView(sf::View({ WIDTH / 2, HEIGHT / 2 }, { WIDTH, HEIGHT }));
        rt.draw(sf::Sprite(tex));
        rt.display();

        std::string name = "candle-capture-";
        char timestr[13];
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(timestr, 13, "%y%m%d%H%M%S", timeinfo);
        name += timestr;
        name += ".png";

        if (!rt.getTexture().copyToImage().saveToFile(name)) {
            exit(1);
        }
    }
    void setMouseBlockSize(float size) {
        if (size > 0) {
            mouseBlock[7].position =
                mouseBlock[0].position = { -size / 2, -size / 2 };
            mouseBlock[1].position =
                mouseBlock[2].position = { size / 2, -size / 2 };
            mouseBlock[3].position =
                mouseBlock[4].position = { size / 2, size / 2 };
            mouseBlock[5].position =
                mouseBlock[6].position = { -size / 2, size / 2 };
            blockSize = size;
            if (brush == BLOCK) {
                popBlock();
                pushBlock(getMousePosition());
                castAllLights();
            }
        }
    }
    void pushEdge(const sfu::Line& edge) {
        edgePool.push_back(edge);
        edgeVertices.append(sf::Vertex(edge.m_origin));
        edgeVertices.append(sf::Vertex(edge.point(1.f)));
    }
    void popEdge() {
        edgePool.pop_back();
        edgeVertices.resize(edgeVertices.getVertexCount() - 2);
    }
    void pushBlock(const sf::Vector2f& pos) {
        const sf::Vector2f points[] = {
            pos + mouseBlock[0].position,
            pos + mouseBlock[2].position,
            pos + mouseBlock[4].position,
            pos + mouseBlock[6].position,
        };
        sfu::Polygon p(points, 4);
        for (auto& l : p.lines) {
            pushEdge(l);
        }
    }
    void popBlock() {
        for (int i = 0; i < 4; i++) {
            popEdge();
        }
    }
    void drawBrush() {
        sf::Transform t;
        t.translate(getMousePosition());
        switch (brush) {
        case LINE:
            w.draw(sf::CircleShape(1.5f), sf::RenderStates(t));
            break;
        default:
            break;
        }
    }
    void setBrush(Brush b) {
        if (b != brush) {
            if (b == BLOCK) {
                pushBlock(getMousePosition());
            }
            if (brush == BLOCK) {
                popBlock();
                castAllLights();
            }
            if (lineStarted) {
                popEdge();
                castAllLights();
                lineStarted = false;
            }
            brush = b;
            updateOnMouseMove();
        }
    }
    void castAllLights() {
        for (auto& l : lights1) {
            l->castLight(edgePool.begin(), edgePool.end());
        }
    }
    void click() {
        sf::Vector2f mp = getMousePosition();
        if (mp.x > WIDTH) {
            mp.x -= WIDTH;
            bool press = false;
            for (auto& button : buttons) {
                if (button.contains(mp)) {
                    button.rect.setFillColor(sf::Color(Button::buttonA1));
                    button.rect.setOutlineColor(sf::Color(Button::buttonA2));
                    button.function(this);
                    press = true;
                }
                else {
                    button.rect.setFillColor(sf::Color(Button::buttonZ1));
                    button.rect.setOutlineColor(sf::Color(Button::buttonZ2));
                }
            }
            if (!press) setBrush(NONE);
        }
        else {
            switch (brush) {
            case RADIAL: {
                std::shared_ptr<Micro::LightSource> nl(new Micro::RadialLight(radialLight));
                lights1.push_back(nl);
                if (glow) {
                    lights2.push_back(nl);
                }
            }
                       break;
            case DIRECTED: {
                std::shared_ptr<Micro::LightSource> nl(new Micro::DirectedLight(directedLight));
                lights1.push_back(nl);
                if (glow) {
                    lights2.push_back(nl);
                }
            }
                         break;
            case LINE:
                pushEdge(sfu::Line(mp, 0.f));
                lineStarted = true;
                break;
            case BLOCK:
                pushBlock(mp);
                break;
            default:
                break;
            }
        }
    }
    void updateOnMouseMove() {
        sf::Vector2f mp = getMousePosition();
        if (mp.x < WIDTH) {
            switch (brush) {
            case BLOCK:
                popBlock();
                pushBlock(mp);
                castAllLights();
                break;
            case RADIAL:
                radialLight.setPosition(mp);
                radialLight.castLight(edgePool.begin(), edgePool.end());
                break;
            case DIRECTED:
                directedLight.setPosition(mp);
                directedLight.castLight(edgePool.begin(), edgePool.end());
                break;
            case LINE:
                if (lineStarted) {
                    int n = edgePool.size();
                    sf::Vector2f orig = edgePool[n - 1].m_origin;
                    popEdge();
                    pushEdge(sfu::Line(orig, mp));
                    castAllLights();
                }
            default:
                break;
            }
        }
    }
    void updateOnPressKey(sf::Keyboard::Key k) {
        switch (k) {
        case sf::Keyboard::M: {
            bool textured = lighting.getAreaTexture() != nullptr;
            if (lighting.getMode() == Micro::LightingArea::FOG) {
                lighting.setMode(Micro::LightingArea::AMBIENT);
                lighting.setAreaColor(textured ? sf::Color::White : sf::Color::Yellow);
            }
            else {
                lighting.setMode(Micro::LightingArea::FOG);
                lighting.setAreaColor(textured ? sf::Color::White : sf::Color::Black);
            }
        }
                            break;
        case sf::Keyboard::T:
            persistent_fog = !persistent_fog;
            break;
        case sf::Keyboard::P:
            capture();
            break;
        case sf::Keyboard::Q:
        case sf::Keyboard::Escape:
            w.close();
            break;
        case sf::Keyboard::LControl:
            control = true;
            break;
        case sf::Keyboard::LAlt:
            alt = true;
            break;
        case sf::Keyboard::LShift:
            shift = true;
            break;
        case sf::Keyboard::R:
            setBrush(RADIAL);
            break;
        case sf::Keyboard::D:
            setBrush(DIRECTED);
            break;
        case sf::Keyboard::B:
            setBrush(BLOCK);
            break;
        case sf::Keyboard::L:
            setBrush(LINE);
            break;
        case sf::Keyboard::A:
            lighting.setAreaOpacity(clamp(lighting.getAreaOpacity() + 0.1));
            break;
        case sf::Keyboard::Z:
            lighting.setAreaOpacity(clamp(lighting.getAreaOpacity() - 0.1));
            break;
        case sf::Keyboard::S:
            if (brush == RADIAL || brush == DIRECTED) {
                radialLight.setIntensity(clamp(radialLight.getIntensity() + 0.1));
                directedLight.setIntensity(clamp(directedLight.getIntensity() + 0.1));
            }
            break;
        case sf::Keyboard::X:
            if (brush == RADIAL || brush == DIRECTED) {
                radialLight.setIntensity(clamp(radialLight.getIntensity() - 0.1));
                directedLight.setIntensity(clamp(directedLight.getIntensity() - 0.1));
            }
            break;
        case sf::Keyboard::G:
            if (brush == RADIAL || brush == DIRECTED) {
                glow = !glow;
            }
            break;
        case sf::Keyboard::F:
            if (brush == RADIAL || brush == DIRECTED) {
                radialLight.setFade(!radialLight.getFade());
                directedLight.setFade(!directedLight.getFade());
            }
            break;
        case sf::Keyboard::C:
            if (brush == RADIAL || brush == DIRECTED) {
                static const sf::Color L_COLORS[] = {
                    sf::Color::White,
                    sf::Color::Magenta,
                    sf::Color::Cyan,
                    sf::Color::Yellow
                };
                static int color_i = 0;
                int n = sizeof(L_COLORS) / sizeof(*L_COLORS);
                color_i = (color_i + 1) % n;
                radialLight.setColor(L_COLORS[color_i]);
                directedLight.setColor(L_COLORS[color_i]);
            }
            break;
        case sf::Keyboard::Space:
            lineStarted = false;
            if (alt) {
                clearEdges();
            }
            else if (shift) {
                clearLights();
            }
            else {
                clearAll();
            }
            castAllLights();
            break;
        default:
            break;
        }
    }
    void updateOnReleaseKey(sf::Keyboard::Key k) {
        switch (k) {
        case sf::Keyboard::LControl:
            control = false;
            break;
        case sf::Keyboard::LAlt:
            alt = false;
            break;
        case sf::Keyboard::LShift:
            shift = false;
            break;
        default:
            break;
        }
    }
    void clearLights() {
        lights1.clear();
        lights2.clear();
    }
    void clearEdges() {
        edgePool.clear();
        edgeVertices.clear();
        lineStarted = false;
        if (brush == BLOCK) pushBlock(getMousePosition());
    }
    void clearAll() {
        clearLights();
        clearEdges();
    }
    void mainLoop() {
        //Micro::initializeTextures();
        sf::Clock clock;
        while (w.isOpen()) {
            sf::Event e;
            while (w.pollEvent(e)) {
                switch (e.type) {
                case sf::Event::Closed:
                    w.close();
                    break;
                case sf::Event::MouseMoved:
                    updateOnMouseMove();
                    break;
                case sf::Event::KeyPressed:
                    updateOnPressKey(e.key.code);
                    break;
                case sf::Event::KeyReleased:
                    updateOnReleaseKey(e.key.code);
                    break;
                case sf::Event::MouseButtonPressed:
                    if (e.mouseButton.button == sf::Mouse::Left) {
                        click();
                    }
                    else {
                        setBrush(NONE);
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if (e.mouseButton.button == sf::Mouse::Left) {
                        lineStarted = false;
                        for (auto& b : buttons) {
                            b.rect.setFillColor(Button::buttonZ1);
                            b.rect.setOutlineColor(Button::buttonZ2);
                        }
                    }
                    break;
                default:
                    break;
                }
            }

            if (persistent_fog) {
                lighting.clear();
            }
            for (auto& l : lights1) {
                lighting.draw(*l);
            }
            if (brush == RADIAL) {
                lighting.draw(radialLight);
            }
            else if (brush == DIRECTED) {
                lighting.draw(directedLight);
            }
            lighting.display();


            w.clear();

            w.setView(menuView);
            for (auto& b : buttons) {
                w.draw(b);
            }

            w.setView(sandboxView);
            //w.draw(background);
            w.draw(lighting);
            for (auto& l : lights2) {
                w.draw(*l);
            }
            if (glow) {
                if (brush == RADIAL) {
                    w.draw(radialLight);
                }
                else if (brush == DIRECTED) {
                    w.draw(directedLight);
                }
            }
            w.draw(edgeVertices);
            drawBrush();

            w.display();

            sf::Time dt = clock.restart();
            int fps = int(std::round(1.f / dt.asSeconds()));
            w.setTitle("Candle demo ["
                + std::to_string(fps)
                + " fps: "
                + std::to_string(dt.asMilliseconds())
                + " ms] ("
                + std::to_string(lights1.size() + (brush == RADIAL || brush == DIRECTED))
                + " Light/s  "
                + std::to_string(edgePool.size())
                + " Edge/s)");
        }
    }
};
int App::Button::BC = 0;
sf::Color App::Button::buttonA1({ 50,50,250,255 });
sf::Color App::Button::buttonA2({ 40,40,40,255 });
sf::Color App::Button::buttonZ1({ 50,50,50,255 });
sf::Color App::Button::buttonZ2({ 20,20,20,255 });
/*
 * MAIN
 */
int main() {
    App app;
    app.mainLoop();
    return 0;
}
