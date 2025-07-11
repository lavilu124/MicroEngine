#include <SFML/Graphics.hpp>
#include <map>
#include <array>
#include "Collision.hpp"

namespace Micro{
    namespace Collision
    {
        sf::Vector2i pixelPositionObject1;
        sf::Vector2i pixelPositionObject2;

        class BitmaskManager
        {
        public:
            ~BitmaskManager() {
                std::map<const sf::Texture*, sf::Uint8*>::const_iterator end = Bitmasks.end();
                for (std::map<const sf::Texture*, sf::Uint8*>::const_iterator iter = Bitmasks.begin(); iter != end; iter++)
                    delete[] iter->second;
            }

            sf::Uint8 GetPixel(const sf::Uint8* mask, const sf::Texture* tex, unsigned int x, unsigned int y) {
                if (x > tex->getSize().x || y > tex->getSize().y)
                    return 0;

                return mask[x + y * tex->getSize().x];
            }

            sf::Uint8* GetMask(const sf::Texture* tex) {
                sf::Uint8* mask;
                std::map<const sf::Texture*, sf::Uint8*>::iterator pair = Bitmasks.find(tex);
                if (pair == Bitmasks.end())
                {
                    sf::Image img = tex->copyToImage();
                    mask = CreateMask(tex, img);
                }
                else
                    mask = pair->second;

                return mask;
            }

            sf::Uint8* CreateMask(const sf::Texture* tex, const sf::Image& img) {
                sf::Uint8* mask = new sf::Uint8[tex->getSize().y * tex->getSize().x];

                for (unsigned int y = 0; y < tex->getSize().y; y++)
                {
                    for (unsigned int x = 0; x < tex->getSize().x; x++)
                        mask[x + y * tex->getSize().x] = img.getPixel(x, y).a;
                }

                Bitmasks.insert(std::pair<const sf::Texture*, sf::Uint8*>(tex, mask));

                return mask;
            }
        private:
            std::map<const sf::Texture*, sf::Uint8*> Bitmasks;
        };

        BitmaskManager Bitmasks;

        bool PixelPerfectCollision(const sf::Sprite& object1, const sf::Sprite& object2, sf::Uint8 alphaLimit) {
            sf::FloatRect Intersection;
            if (object1.getGlobalBounds().intersects(object2.getGlobalBounds(), Intersection)) {
                sf::IntRect O1SubRect = object1.getTextureRect();
                sf::IntRect O2SubRect = object2.getTextureRect();

                sf::Uint8* mask1 = Bitmasks.GetMask(object1.getTexture());
                sf::Uint8* mask2 = Bitmasks.GetMask(object2.getTexture());

                // Loop through our pixels
                for (int i = Intersection.left; i < Intersection.left + Intersection.width; i++) {
                    for (int j = Intersection.top; j < Intersection.top + Intersection.height; j++) {

                        sf::Vector2f o1v = object1.getInverseTransform().transformPoint(i, j);
                        sf::Vector2f o2v = object2.getInverseTransform().transformPoint(i, j);

                        // Make sure pixels fall within the sprite's subrect
                        if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
                            o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
                            o2v.x < O2SubRect.width && o2v.y < O2SubRect.height) {

                            if (Bitmasks.GetPixel(mask1, object1.getTexture(), (int)(o1v.x) + O1SubRect.left, (int)(o1v.y) + O1SubRect.top) > alphaLimit &&
                                Bitmasks.GetPixel(mask2, object2.getTexture(), (int)(o2v.x) + O2SubRect.left, (int)(o2v.y) + O2SubRect.top) > alphaLimit) {
                                pixelPositionObject1 = sf::Vector2i((int)(o1v.x) + O1SubRect.left, (int)(o1v.y) + O1SubRect.top);
                                pixelPositionObject2 = sf::Vector2i((int)(o2v.x) + O2SubRect.left, (int)(o2v.y) + O2SubRect.top);
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }

        bool CreateTextureAndBitmask(sf::Texture& loadInto, const std::string& filename)
        {
            sf::Image img;
            if (!img.loadFromFile(filename))
                return false;
            if (!loadInto.loadFromImage(img))
                return false;

            Bitmasks.CreateMask(&loadInto, img);
            return true;
        }

        sf::Vector2f GetSpriteCenter(const sf::Sprite& object)
        {
            sf::FloatRect AABB = object.getGlobalBounds();
            return sf::Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f);
        }

        sf::Vector2f GetSpriteSize(const sf::Sprite& object)
        {
            sf::IntRect OriginalSize = object.getTextureRect();
            sf::Vector2f Scale = object.getScale();
            return sf::Vector2f(OriginalSize.width * Scale.x, OriginalSize.height * Scale.y);
        }

        bool CircleTest(const sf::Sprite& object1, const sf::Sprite& object2) {
            sf::Vector2f Obj1Size = GetSpriteSize(object1);
            sf::Vector2f Obj2Size = GetSpriteSize(object2);
            float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
            float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

            sf::Vector2f Distance = GetSpriteCenter(object1) - GetSpriteCenter(object2);

            return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
        }

        class OrientedBoundingBox // Used in the BoundingBoxTest
        {
        public:
            OrientedBoundingBox(const sf::Sprite& object) // Calculate the four points of the OBB from a transformed (scaled, rotated...) sprite
            {
                sf::Transform trans = object.getTransform();
                sf::IntRect local = object.getTextureRect();
                Points[0] = trans.transformPoint(0.f, 0.f);
                Points[1] = trans.transformPoint(local.width, 0.f);
                Points[2] = trans.transformPoint(local.width, local.height);
                Points[3] = trans.transformPoint(0.f, local.height);
            }

            sf::Vector2f Points[4];

            void ProjectOntoAxis(const sf::Vector2f& axis, float& min, float& max) // Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
            {
                min = (Points[0].x * axis.x + Points[0].y * axis.y);
                max = sf::BlendMode::Min;
                for (int j = 1; j < 4; j++)
                {
                    float Projection = (Points[j].x * axis.x + Points[j].y * axis.y);

                    if (Projection < min)
                        min = Projection;
                    if (Projection > max)
                        max = Projection;
                }
            }
        };

        bool BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2) {
            OrientedBoundingBox OBB1(Object1);
            OrientedBoundingBox OBB2(Object2);

            // Create the four distinct axes that are perpendicular to the edges of the two rectangles
            sf::Vector2f Axes[4] = {
                sf::Vector2f(OBB1.Points[1].x - OBB1.Points[0].x,
                              OBB1.Points[1].y - OBB1.Points[0].y),
                sf::Vector2f(OBB1.Points[1].x - OBB1.Points[2].x,
                              OBB1.Points[1].y - OBB1.Points[2].y),
                sf::Vector2f(OBB2.Points[0].x - OBB2.Points[3].x,
                              OBB2.Points[0].y - OBB2.Points[3].y),
                sf::Vector2f(OBB2.Points[0].x - OBB2.Points[1].x,
                              OBB2.Points[0].y - OBB2.Points[1].y)
            };

            for (int i = 0; i < 4; i++) // For each axis...
            {
                float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

                // ... project the points of both OBBs onto the axis ...
                OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
                OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

                // ... and check whether the outermost projected points of both OBBs overlap.
                // If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
                if (!((MinOBB2 <= MaxOBB1) && (MaxOBB2 >= MinOBB1)))
                    return false;
            }
            return true;
        }

        std::array<int, 4> GetcollisonPosition() {
            std::array<int, 4> collisionArray = { pixelPositionObject1.x,pixelPositionObject2.x,pixelPositionObject1.y,pixelPositionObject2.y };
            return collisionArray;
        }
    }
}
