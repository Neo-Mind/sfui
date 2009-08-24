/*
    This file is part of SFUI (by Robin RUAUX).

    SFUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SFUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SFUI.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ResourceManager.hpp"

#include <iostream>
#include <vector>

#include <tinyxml.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    namespace ui
    {
        ResourceManager* ResourceManager::mInstance = 0;

        ResourceManager* ResourceManager::Get()
        {
            if (!mInstance)
            {
                mInstance = new ResourceManager();
            }
            return mInstance;
        }

        ResourceManager::ResourceManager()
        {

        }

        ResourceManager::~ResourceManager()
        {
            // Delete all resources

            for (Fonts::iterator it = mFonts.begin(); it != mFonts.end(); ++it)
            {
                delete it->second;
                mFonts.erase(it);
            }

            for (Images::iterator it = mImages.begin(); it != mImages.end(); ++it)
            {
                delete it->second;
                mImages.erase(it);
            }
        }

        void ResourceManager::Kill()
        {
            delete mInstance;
            mInstance = 0;
        }

        static int    _GetValueFromHex(char hex)
        {
            if (hex >= '0' && hex <= '9')
                return hex - '0';
            else if (hex >= 'A' && hex <= 'F')
                return hex - 'A' + 10;
            else if (hex >= 'a' && hex <= 'f')
                return hex - 'f' + 10;
            return 0;
        }

        static Uint8   _GetElementFromHex(const std::string& str)
        {
            if (str.length() != 2)
                return 0;
            return static_cast<Uint8>(_GetValueFromHex(str[0]) * 16 + _GetValueFromHex(str[1]));
        }

        static void    _GetElementsFromString(std::string str, char separator, std::vector<std::string>* results)
        {
            size_t found = str.find_first_of(separator);

            while (found != std::string::npos)
            {
                if(found > 0)
                {
                    results->push_back(str.substr(0, found));
                }
                str = str.substr(found + 1);
                found = str.find_first_of(separator);
            }
            if (str.length() > 0)
            {
                results->push_back(str);
            }
        }

        Color   ResourceManager::GetColorValue(const std::string& value, const Color& defaultValue)
        {
            Color   retValue;

            if (value.length() == 0)
                return defaultValue;

            if (value.substr(0, 1) == "#" && value.length() == 7)
            {
                retValue.r = _GetElementFromHex(value.substr(1, 2));
                retValue.g = _GetElementFromHex(value.substr(3, 2));
                retValue.b = _GetElementFromHex(value.substr(5, 2));
                retValue.a = 255;

                return retValue;
            }
            else if (value.substr(0, 4) == "rgb:")
            {
                std::vector<std::string> vectEls;
                _GetElementsFromString(value.substr(4, value.length() - 4), ',', &vectEls);

                if (vectEls.size() == 3)
                {
                    retValue.r = GetValue(vectEls[0], 255);
                    retValue.g = GetValue(vectEls[1], 255);
                    retValue.b = GetValue(vectEls[2], 255);
                    retValue.a = 255;

                    return retValue;
                }
            }
            else if (value.substr(0, 5) == "rgba:")
            {
                std::vector<std::string> vectEls;
                _GetElementsFromString(value.substr(5, value.length() - 5), ',', &vectEls);

                if (vectEls.size() == 4)
                {
                    retValue.r = GetValue(vectEls[0], 255);
                    retValue.g = GetValue(vectEls[1], 255);
                    retValue.b = GetValue(vectEls[2], 255);
                    retValue.a = GetValue(vectEls[3], 255);

                    return retValue;
                }
            }
            return (retValue = defaultValue);
        }

        TemplateProperties&   ResourceManager::GetTemplate(const std::string& name)
        {
            return mTemplates[name];
        }

        bool      ResourceManager::LoadFont(const std::string& resId, const std::string& filename, float size)
        {
            Font* font = new Font();
            if (font->LoadFromFile(filename, size))
            {
                mFonts[resId] = font;
                return true;
            }
            return false;
        }

        bool    ResourceManager::LoadImage(const std::string& resId, const std::string& filename)
        {
            Image* image = new Image();
            if (image->LoadFromFile(filename))
            {
                mImages[resId] = image;
                return true;
            }
            return false;
        }

        Font*     ResourceManager::GetFont(const std::string& name, float size)
        {
            Font*   font = 0;
            Fonts::iterator it = mFonts.find(name);

            if (it != mFonts.end())
                font = it->second;
            else
            {
                if (LoadFont(name, name, size))
                    return mFonts[name];
            }
            return font;
        }

        Image*  ResourceManager::GetImage(const std::string& name)
        {
            Image*  image = 0;
            Images::iterator it = mImages.find(name);

            if (it != mImages.end())
                image = it->second;
            else
            {
                if (LoadImage(name, name))
                    return mImages[name];
            }
            return image;
        }

        Sprite  ResourceManager::GetSubImage(const std::string& imageName, const IntRect& subRect)
        {
            Sprite spr;

            spr.SetImage(*GetImage(imageName));
            spr.SetSubRect(subRect);
            return spr;
        }

        bool    ResourceManager::AddTemplatesFromFile(const std::string& filename)
        {
            TiXmlDocument file(filename);

            if (!file.LoadFile())
            {
                std::cerr << "Could not load \"" << filename << "\" template file" << std::endl;
                std::cerr << "Reason :" << file.ErrorDesc() << std::endl;
                return false;
            }

            TiXmlHandle hdlAll(&file);
            TiXmlElement *t = hdlAll.FirstChildElement().Element();

            if (!t)
            {
                std::cerr << "Unable to parse template file" << std::endl;
                return false;
            }

            while (t)
            {
                if (t->ValueStr() == "template")
                {
                    TemplateProperties  properties;

                    const TiXmlAttribute* attr = t->FirstAttribute();
                    std::string nameTpl;

                    while (attr)
                    {
                        std::string nameAttr(attr->Name());

                        if (nameAttr == "name")
                            nameTpl = attr->ValueStr();
                        else
                            properties[nameAttr] = attr->ValueStr();
                        attr = attr->Next();
                    }

                    if (nameTpl.size() == 0)
                    {
                        std::cerr << "Unable to parse template file. A template has no name." << std::endl;
                        return false;
                    }
                    mTemplates[nameTpl] = properties;
                }
                else if (t->ValueStr() == "resources")
                {
                    const TiXmlElement* cRes = t->FirstChildElement();

                    while (cRes)
                    {
                        if (cRes->ValueStr() == "font")
                        {
                            LoadFont(cRes->Attribute("name"), cRes->Attribute("src"), GetValue(cRes->Attribute("size"), 30.f));
                        }
                        else if (cRes->ValueStr() == "image")
                        {
                            LoadImage(cRes->Attribute("name"), cRes->Attribute("src"));
                        }
                        cRes = cRes->NextSiblingElement();
                    }
                }
                t = t->NextSiblingElement();
            }

            return true;

        }

    }
}