/*
    Copyright (c) 2009, Robin RUAUX
    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the University of California, Berkeley nor the
          names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/



#include <iostream>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFUI/ResourceManager.hpp>

#include <TinyXML/tinyxml.h>

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
            for (Fonts::iterator it = mFonts.begin(); it != mFonts.end(); ++it)
            {
                delete it->second;
            }

            for (Images::iterator it = mImages.begin(); it != mImages.end(); ++it)
            {
                delete it->second;
            }

            mFonts.clear();
            mImages.clear();
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

        StyleProperties&   ResourceManager::GetStyle(const std::string& name)
        {
            return mStyles[name];
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
            if (name == "")
                return 0;

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

        Sprite  ResourceManager::GetImage(const std::string& name)
        {
            Sprite spr;
            IntRect rect;
            std::vector<std::string> rectElems;
            std::string newName = name;


            // Is sub-image ?
            if (name.substr(0,4) == "sub:")
            {
                _GetElementsFromString(name.substr(4, name.length() - 4), ',', &rectElems);
                newName = GetValue(rectElems[0], name);
            }

            // Get the image from resource ID...
            Image*  image = 0;
            Images::iterator it = mImages.find(newName);

            if (it != mImages.end())
                image = it->second;
            else if (LoadImage(newName, newName))
                image = mImages[newName];

            if (image)
                spr.SetImage(*image);

            // Set the subrect
            if (rectElems.size() == 5)
            {
                rect.Left = GetValue(rectElems[1], 0);
                rect.Top = GetValue(rectElems[2], 0);
                rect.Right = GetValue(rectElems[3], 0);
                rect.Bottom = GetValue(rectElems[4], 0);
            }
            else
            {
                rect = spr.GetSubRect();
            }

            spr.SetSubRect(rect);

            return spr;
        }

        bool    ResourceManager::AddStylesFromFile(const std::string& filename)
        {
            TiXmlDocument file(filename);

            if (!file.LoadFile())
            {
                std::cerr << "Could not load \"" << filename << "\" style file" << std::endl;
                std::cerr << "Reason :" << file.ErrorDesc() << std::endl;
                return false;
            }

            TiXmlHandle hdlAll(&file);
            TiXmlElement *t = hdlAll.FirstChildElement().Element();

            if (!t)
            {
                std::cerr << "Unable to parse style file" << std::endl;
                return false;
            }

            while (t)
            {
                if (t->ValueStr() == "style")
                {
                    StyleProperties  properties;

                    const TiXmlAttribute* attr = t->FirstAttribute();
                    std::string nameStyle;

                    while (attr)
                    {
                        std::string nameAttr(attr->Name());

                        if (nameAttr == "name")
                            nameStyle = attr->ValueStr();
                        else
                            properties[nameAttr] = attr->ValueStr();
                        attr = attr->Next();
                    }

                    if (nameStyle.size() == 0)
                    {
                        std::cerr << "Unable to parse style file. One of styles has no name." << std::endl;
                        return false;
                    }
                    mStyles[nameStyle] = properties;
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
