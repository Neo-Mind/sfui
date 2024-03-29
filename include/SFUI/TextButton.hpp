#ifndef TEXTBUTTON_HPP_INCLUDED
#define TEXTBUTTON_HPP_INCLUDED

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

/** \file TextButton.hpp
 * \brief A basic text button.
 * \author Robin Ruaux
 */

#include <SFUI/GridDecorator.hpp>
#include <SFUI/Label.hpp>
#include <SFUI/ButtonPolicy.hpp>

namespace sf
{
    namespace ui
    {
        /** \class TextButton
         *
         * \brief A basic TextButton widget.
         */
        class TextButton : public Widget, public ButtonPolicy
        {
            public :

                /** \brief Constructor
                 *
                 * Constructor of TextButton class.
                 * \param caption The textbutton caption.
                 */
                TextButton(const Unicode::Text& caption);


                /** \brief Get the button text color.
                 *
                 * \return The button text color.
                 */
                const Color&    GetTextColor() const;


                /** \brief Get the button text size.
                 *
                 * \return The button text size.
                 */
                float           GetTextSize() const;


                virtual void    LoadStyle(const std::string& nameStyle);


                /** \brief Set the button text color.
                 *
                 * \param color The new button text color.
                 */
                void            SetTextColor(const Color& color);


                /** \brief Set the button text size.
                 *
                 * \param size The new button text size.
                 */
                void            SetTextSize(float size);


            protected :

                virtual void    OnChange(Widget::Property property);

                Label           mCaption;

                GridDecorator   mDecorator;

        };

    }
}

#endif // TEXTBUTTON_HPP_INCLUDED
