#ifndef ANIMATIONLAYER_HPP
#define ANIMATIONLAYER_HPP

// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <vector>
#include <tuple>

// Pugi
#include "../xmlSystem/pugixml.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;
    class Animation;
    class AnimationStore;

    class AnimationLayer
    {
        public:
            using Coordinate = std::tuple<int, int, double>;        // x, y, angle [rad]
            using Element    = std::pair<int, Coordinate>;          // storeID, Coordinate

        private:
            Window& window;
            AnimationStore& animationStore;

            std::vector<Element> elements;

        public:
            // -------------------------------------------------------------- //
            // CTor, DTor
            AnimationLayer(RetrogameBase::Window& window);

            // -------------------------------------------------------------- //
            // getters

            size_t size() const;

            Element    getElement    (const int index) const;
            int        getAnimationID(const int index) const;
            Animation& getAnimation  (const int index) const;
            Coordinate getCoordinate (const int index) const;

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();

            void addElement    (                 const Element& element);
            void replaceElement(const int index, const Element& element);
            void replaceElement(const int index, const int      animationID);
            void removeElement (const int index);
            void moveElement   (const int index, const Coordinate& coordinate);

            void loadXML (const std::string& filename);

        private:
            using ElementDescriptor = std::pair<int, int>;          // paletteID, angle [deg]

            static constexpr auto              INVALID_TAG         = "<-*-invalid-*->";
            static constexpr auto              VOID_REPRESENTATION = "#";
            static constexpr ElementDescriptor INVALID_GRIDELEMENT = {-1, -1};
            static constexpr ElementDescriptor VOID_GRIDELEMENT    = {-1,  0};

            std::vector<std::string> getPaletteEntries(pugi::xml_node node) const;
            std::vector<Element>     parseGridNode(pugi::xml_node node, const std::string& filename) const;
            ElementDescriptor        parseGridElement(const std::string_view& elementDescriptor) const;

        public:

            // -------------------------------------------------------------- //
            // display
    };

}
#endif // ANIMATIONLAYER_HPP
