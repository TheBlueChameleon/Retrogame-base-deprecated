#ifndef XMLWRAPPER_HPP
#define XMLWRAPPER_HPP

// ========================================================================== //
// dependencies

// STL
#include <vector>
#include <string>
#include <utility>

// Pugi
#include "pugixml.hpp"

// ========================================================================== //
// namespace

namespace RetrogameBase
{
// ========================================================================== //
// global vars

#ifdef PROJECT_NAME
    constexpr auto projectName = PROJECT_NAME;
#else
    constexpr auto projectName = "Retrogame-base";
#endif

#ifdef CODEVERSION_MAJOR
    constexpr auto codeVersion_major = CODEVERSION_MAJOR;
#else
    constexpr auto codeVersion_major = 0;
#endif

#ifdef CODEVERSION_MINOR
    constexpr auto codeVersion_minor = CODEVERSION_MINOR;
#else
    constexpr auto codeVersion_minor = 1;
#endif

// ========================================================================== //
// proc
    pugi::xml_document XmlLoad(const std::string& filename, const std::string& expectedContent = "");

    /* a simple xml tag has the following structure:
     * <tag attribute=value attribute=value ... />
     *
     * multiple simple tags can be grouped under a parent tag:
     * <group>
     *  <tag attribute=value attribute=value ... />
     *  <tag attribute=value attribute=value ... />
     *  ...
     * </group>
     *
     * The below definitions help to map these structures onto STL concepts.
     * There may not be any sub-groups under <group>
     */
    using XmlTagElement  = std::pair<std::string, std::string>;                  // (attribute, value)
    using XmlSimpleTag   = std::pair<std::string, std::vector<XmlTagElement>>;   // (tag, elements)
    using XmlSimpleGroup = std::vector<XmlSimpleTag>;

    XmlSimpleGroup XmlExtractSimpleGroup (const pugi::xml_node& node);

// ========================================================================== //
// namespace
}
#endif // XMLWRAPPER_HPP
