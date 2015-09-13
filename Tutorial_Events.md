# Widget events #

A widget can receive events for their own usage. They use an _Observer Pattern_ for it : `EventListeners`.

There are two listeners, corresponding to different events.
  * [sf::ui::MouseListener](http://lokk.u7n.org/sfui/doc/html/classsf_1_1ui_1_1MouseListener.html) : Receive pressed, released, entered, left, and moved events from mouse.
  * [sf::ui::KeyListener](http://lokk.u7n.org/sfui/doc/html/classsf_1_1ui_1_1KeyListener.html) : Receive pressed, released, and text-entered events from key.

To attach a new listener, you should use
```
ui::Widget::AddMouseListener(ui::MouseListener* listener);
ui::Widget::AddKeyListener(ui::KeyListener* listener);
```

or detach it with
```
ui::Widget::RemoveMouseListener(ui::MouseListener* listener);
ui::Widget::RemoveKeyListener(ui::KeyListener* listener);
```

## 1 - Listener usage ##

First, we create our listener

```
#include <SFUI.hpp>
#include <iostream>

using namespace sf;

class MyListener : public ui::MouseListener
{
    protected :
        void OnMousePressed(const Event::MouseButtonEvent& mouse)
        {
            std::cout << "Mouse pressed !" << std::endl;
        }
};
```

Then, we create a widget for testing it :
```
int main()
{
   ...
   ui::Label myLbl("label");
   myLbl.SetAlignment(ui::Align::CENTER);

   MyListener ourListener;
   myLbl.SetFocusable();
   myLbl.AddMouseListener(&ourListener);
   ...
}
```

Voilà !
You can click on the label, it should print the message on the console.
The process is similar to all other events.

## 2 - Listener inheritance ##

There is an other way to use listeners. Indeed, you can make a widget inherited from a listener.
This example show the usage of it with the previous label :

```
class MyLabel : public sf::ui::Label, public sf::ui::MouseListener
{
    public :
        MyLabel()
            : sf::ui::Label()
        {
            SetFocusable();
            SetAlignment(sf::ui::Align::CENTER);
            AddMouseListener(this);
        }

    protected :
        // Inherited from MouseListener
        virtual void OnMousePressed(const sf::Event::MouseButtonEvent& mouse)
        {
            std::cout << "Mouse pressed !" << std::endl;
        }
};
```

The basic widgets in SFUI are constructed like this.

For example, the [TextInput widget](http://lokk.u7n.org/sfui/doc/html/classsf_1_1ui_1_1TextInput.html) inherits from the two listeners, because of cursor and text needings.