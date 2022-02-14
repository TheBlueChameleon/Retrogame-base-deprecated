// ========================================================================== //
// dependencies

// STL
#include <exception>
#include <iostream>

#include <cstring>
#include <string>
using namespace std::string_literals;

// own
#include "xmlwrapper.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#define VERSIONTEXT(major_int, minor_int) (std::to_string(major_int) + "." + std::to_string(minor_int))

// ========================================================================== //
// namespace

namespace RetrogameBase
{
// ========================================================================== //
// proc

    pugi::xml_document XMLload(const std::string& filename, const std::string& expectedContent)
    {
        /* Loads the document specified by <filename>
         * Performs project and version check
         * Returns XML handle on success
         * Throws std::runtime_error otherwise
         */

        pugi::xml_document doc;

        // ........................................................................ //
        // load and check XML validity

        pugi::xml_parse_result parseResult = doc.load_file(filename.data());

        if (not parseResult)
        {
            throw std::runtime_error(THROWTEXT(
                                         "    Could not load '"s + filename + "'.\n"
                                         "    Load result : " + parseResult.description()
                                     ));
        }

        // ........................................................................ //
        // check whether this belongs to the project

        /* A project file is expected to have "project" as a root node.
         * project should have the attribute "name"
         * the value has to be the same as global PROJECT_NAME, as defined in
         * globals.hpp.
         */

        auto nodeProject = doc.child("project");

        if (not nodeProject)
        {
            throw std::runtime_error(THROWTEXT(
                                         "    Invalid file '"s + filename + "'.\n"
                                         "    Couldn't find project tag"
                                     ));
        }

        if ( std::strcmp(nodeProject.attribute("name").value(), projectName) )
        {
            throw std::runtime_error(THROWTEXT(
                                         "    Invalid file '"s + filename + "'.\n"
                                         "    Not part of project '" + projectName + "'"
                                     ));
        }

        // ........................................................................ //
        // check version

        /* Version number is stored in node version, directly under the node project
         * It has two elements, major and minor.
         * They in turn each have an attribute "value" which holds an int.
         *
         * Negative or empty values are not valid.
         * trailing non-numeric strings will be ignored. ("1text" evaluates to 1)
         * leading non-numeric strings are evaluated to zero. ("text1" evaluates to 0)
         * version number 0.0 is invalid.
         *
         * Supported major/minor version is defined in globals.hpp as
         * VERSION_MAJOR, VERSION_MINOR
         */

        auto nodeVersion = nodeProject.child("version");
        if (not nodeVersion)
        {
            throw std::runtime_error(THROWTEXT(
                                         "    Invalid file '"s + filename + "'.\n"
                                         "    Couldn't find version tag"
                                     ));
        }

        auto nodeMajor = nodeVersion.child("major");
        auto nodeMinor = nodeVersion.child("minor");

        auto fileVersion_major = nodeMajor.attribute("value").as_int(-1);
        auto fileVersion_minor = nodeMinor.attribute("value").as_int(-1);

        if (
            (fileVersion_major  < 0 || fileVersion_minor  < 0) ||
            (fileVersion_major == 0 && fileVersion_minor == 0)
        )
        {
            throw std::runtime_error(THROWTEXT(
                                         "    Invalid file '"s + filename + "'.\n"
                                         "    Version number " + std::to_string(fileVersion_major) + "." + std::to_string(fileVersion_minor) + " is invalid."
                                     ));
        }


        bool invalidVersion = false;
        if (fileVersion_major > codeVersion_major)
        {
            invalidVersion = true;
        }
        else if (fileVersion_major == codeVersion_major)
        {
            if (fileVersion_minor > codeVersion_minor)
            {
                invalidVersion = true;
            }
        }
        if (invalidVersion)
        {
            throw std::runtime_error(THROWTEXT(
                                         "    Invalid file '"s + filename + "'.\n"
                                         "    Version number "  + VERSIONTEXT(fileVersion_major, fileVersion_minor) + " is not supported.\n"
                                         "    This is version " + VERSIONTEXT(codeVersion_major, codeVersion_minor)
                                     ));
        }


        // ........................................................................ //
        // check content (if specified)

        /* content is a node directly under the project node. It has a single
         * attribute name.
         * If argument content is nonzero, this will be checked for equality.
         */

        if ( expectedContent.size() )
        {
            auto fileContent = nodeProject.child("content").attribute("value").value();
            if ( std::strcmp( fileContent, expectedContent.data() ) )
            {
                throw std::runtime_error(THROWTEXT(
                                             "    Invalid content\n"
                                             "    Expected: '"s + expectedContent + "'\n"
                                             "    Found   : '"s + fileContent + "'"
                                         ));
            }
        }

        return doc;
    }

    XmlSimpleGroup XmlExtractAttributeList(pugi::xml_node& node)
    {
        std::string                 tag, attrib, value;
        XmlTagElement               element;
        std::vector<XmlTagElement>  elements;
        XmlSimpleGroup              reVal;

        for (auto subNode = node.first_child(); subNode; subNode = subNode.next_sibling())
        {
            tag = subNode.name();
            elements.clear();

            for (auto attrIt : subNode.attributes())
            {
                attrib = attrIt.name();
                value  = attrIt.value();
                element = std::make_pair(attrib, value);
                elements.push_back(element);
            }

            reVal.push_back( std::make_pair(tag, elements) );
        }

        return reVal;
    }

// ========================================================================== //
// namespace
}
