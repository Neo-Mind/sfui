# Styles in SFUI #

Widget can have many properties (like background color, text color, size, etc...)<br />
Although you can modify them into source code (by accessors), there is an other way to custom widget appearance.

The benefit of styles is mainly about _the separation between the designer and developer's layers_.<br />
Moreover, you can modify your ui's designs greatly **without any recompilation** of your source code, or offer multiple designs to users...

A style is just a properties container, defined in a XML file like this :
```
<style name="StyleName" attribute="value" />
```

Each style _must be_ identified by **a name** in order to be used in source code.
Then you can declare needed properties with syntax _attribute="value"_

Basic attributes for sf::ui::Widget :
| **Attribute** | **Type** | **Example** |
|:--------------|:---------|:------------|
| x<br />y      | float    | 123.4 <br /> -45 |
| width<br />height | integer (>= 0) | 456         |
| focusable<br />enabled<br />visible | boolean  | 0 (false)<br />1 (true) |
| color         | hexadecimal<br />rgb<br />rgb + alpha | #FF0000 <br /> rgb: 255, 0, 0 <br /> rgba: 255, 0, 0, 123 |
| align         | string   | top\_left<br />center<br />bottom\_right<br />... |

## Loading styles ##
To preload these styles in memory, you should use the ResourceManager singleton :
```
ui::ResourceManager::Get()->AddStylesFromFile("my_custom_styles.xml"); // or other file extension...
```
The resources will be removed automatically when singleton is killed (internally by the renderer).

**Rendering of different styles**

First, we create our basic Label
```
ui::Label lblStyle("My stylized label");
lblStyle.LoadStyle("MyStyle1"); // name of the style to load
```

And then, we define two styles in the file `my_custom_styles.xml`

```
<style name="MyStyle1" color="rgba: 0, 0, 0, 0" textSize="20" textColor="#000000" focusable="1" x="10" y="25" />
<style name="MyStyle2" color="rgb: 200, 100, 123" textSize="12" textColor="#FFFF00" width="123" height="16" align="center" y="-34" />
```

`MyStyle1 rendering`

![http://lokk.u7n.org/sfui/sfui_styles1.png](http://lokk.u7n.org/sfui/sfui_styles1.png)

`MyStyle2 rendering`

![http://lokk.u7n.org/sfui/sfui_styles2.png](http://lokk.u7n.org/sfui/sfui_styles2.png)

As you can see, it is very easy to use styles with widgets.

## Loading resources ##

Resources are another part of styles. Indeed, you can load images or fonts dynamically with XML files.

### Fonts ###

There are two ways to load them.
```
<style name="MyStyle" font="data/myfont.ttf" textColor="#FF0000" />
```
In this case, font is loaded when `Widget::LoadStyle("MyStyle")` has been called.<br />
You can also preload it and then use an alias :
```
<resources>
    <font name="TitleFont" size="15" src="data/myfont.ttf" />
</resources>

<style name="MyStyle" font="TitleFont" textColor="#FF0000" />
```
The second method allows you to fix a default size, it's useful for having a great text rendering with `TrueType` fonts.

### Images ###

Images can be preloaded like fonts too.
```
<resources>
    <image name="ButtonImages" src="data/mybuttons.png" />
</resources>

<style name="MyStyle" image="ButtonImages" />
```

You can also set a subpart of the image with syntax `sub: ImageName, left, right, top, bottom`
```
<style name="MyStyle" image="sub: ButtonImages, 0, 0, 140, 354" />
```
This technique allows the designer to create **tileset** for a widget part.

## Manipulating styles in source code ##

Ok, now you should give attention to this part if you want to create your widgets, and especially, add some widget properties.

Widget class provides a virtual method `LoadStyle`. <br />
For example, the Label implementation looks like that : (see the comments below)
```
void    Label::LoadStyle(const std::string& nameTpl)
{
    /* First, we get the ResourceManager instance... */
    ResourceManager* rm = ResourceManager::Get();

    /* Then, we get the style from memory (StyleProperties is just an STL map) */
    StyleProperties& properties = rm->GetStyle(nameTpl);

    /* The property value is the string from the XML file, we need to parse it to be accepted as a color.
       The second argument is the default value - if parsing has failed */
    SetTextColor(rm->GetColorValue(properties["textColor"], GetTextColor()));

    /* Here, we need to get the font resource from memory.
       The second argument is the default font size. */
    Font* newFont = rm->GetFont(properties["font"], GetTextSize());

    if (newFont)
        SetFont(*newFont);
    }

    if (properties["textSize"] != "")
    {
        /* For other usage, you should use this method (string, boolean, and number values) */
        SetTextSize(rm->GetValue(properties["textSize"], GetTextSize()));
        ...
    }

    /* Oh ! Don't forget to call this method, otherwise, the basic widget properties won't be processed. */
    Widget::LoadStyle(nameTpl);
}
```

That's all you have to know about basic styles =)

For advanced usage, you shoud read the tutorial about widget creation.<br />
You will learn how to make a _style hierarchy_ for complex widgets.