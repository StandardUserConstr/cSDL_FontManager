<H1 align="center">cSDL_FontManager</H1>
<div >
    <H4 align="center" >this is font manager for horizontal UNICODE text;<br/> 
    it can be used to create tools like textboxes,notepads and things like that;<br/> 
    the purpose of this class is to be easy to use,<br/>customizable and flexible to various optymalizations font manager;<br/> 
    this class is flexible to various logical optymalization methods so if u use this class correctly,<br/>ur code can be very fast;<br/> 
    it's using freetype library for loading fonts from files so freetype library is needed;<br/> 
    it can be used hybridly with and without SDL2 library so SDL2 library is not needed;<br/> 
    class doesn't support vertical text so u have to use this class in more manual way to get the desire effect;<br/> 
    class needs extreme testing for finding potential bugs that may occur with correct use of class;</H4>
  
---
  
### additional libraries to include:
- Freetype **(required)**
- SDL/SDL2 **(not required)**

---

<H2 align="center">Examples of use:</H2>

<details>
<summary align="center">example 1</summary>

~~~~cpp
------------------------------------------------------------------------------------------------------------------
cSDL_FontManager font_manager(render,"SomeFont.ttf",0,64,20);                                   //  required to work;
int32_t error = font_manager.geterror_constructor();                                            //  highly recommended;
if(error!=0) destroy_object_and_print_error(error);                                             //
font_manager.load_characters(8,cSDL_FontManager::LoadCharactersRange::ALL);                     //  required to work otherwise u cannot draw any text (u have to run this funcion at least once);

font_manager.new_text("some text to draw\ntext in new line);                                    //  required to work (just to have some text u can work with);
uint32_t w,h;                                                                                   //
SDL_Texture* texture = font_manager.get_pixels_in_texture(&w,&h);                               //  now u have image of text u can work with;
------------------------------------------------------------------------------------------------------------------
~~~~

</details>

<details>
<summary align="center">example 2 (without SDL)</summary>

~~~~cpp
------------------------------------------------------------------------------------------------------------------
cSDL_FontManager font_manager("SomeFont.ttf",0,640,480);                                        //  required to work;
int32_t error = font_manager.geterror_constructor();                                            //  highly recommended;
if(error!=0) destroy_object_and_print_error(error);                                             //
font_manager.change_tab_width(8,1);                                                             //  not needed additional function;
font_manager.load_characters(16,cSDL_FontManager::LoadCharactersRange::NUMBERS_LETTERS_EXTRA);  //  required to work otherwise u cannot draw any text (u have to run this funcion at least once);

font_manager.change_bitmap_size(256,64);                                                        //  not needed additional function;
font_manager.switch_color_modes(1);                                                             //  not needed additional function (thanks to this u can coloring text);
using TextColor = cSDL_FontManager::TextColor;                                                  //  not needed (it's just for "change_default_text_color" so i can use "TextColor" without "cSDL_FontManager::");
font_manager.change_default_text_color(TextColor{255,168,168,0},TextColor{128,0,0,168});        //  not needed additional function;
font_manager.change_viewpoint(20,20);                                                           //  not needed additional function;

font_manager.new_text("some text to draw\n\ttext in new line);                                  //  required to work (just to have some text u can work with);
uint32_t w,h,pitch;                                                                             //
uint32_t* pointer_to_bitmap = font_manager.get_pixels(&w,&h,&pitch);                            //  required to work 'cause now u have drawed image of text;
------------------------------------------------------------------------------------------------------------------
~~~~

</details>

<details>
<summary align="center">example 3</summary>

~~~~cpp
------------------------------------------------------------------------------------------------------------------
cSDL_FontManager font_manager(render,"SomeFont.ttf",0,640,480);                                 //  required to work;
int32_t error = font_manager.geterror_constructor();                                            //  highly recommended;
if(error!=0) destroy_object_and_print_error(error);                                             //
font_manager.load_characters(16,cSDL_FontManager::LoadCharactersRange::NUMBERS);                //  required to work otherwise u cannot draw any text (u have to run this funcion at least once);

font_manager.switch_color_modes(1);                                                             //  not needed additional function (thanks to this u can coloring text);
cSDL_FontManager::TextColor letters_color = {255,168,168,0};                                    //
cSDL_FontManager::TextColor background_color = {0,0,0,0};                                       //
font_manager.new_text("123456789\n987654321);                                                   //  required to work 'cause now u have drawed image of text;
font_manager.change_text_color(0,3,letters_color,background_color);                             //  not needed additional function;

  // example of loop0:
        for(;;)
        {
            static variable0 = 4;
            variable0++;
            font_manager.change_text_color(variable0,variable0,letters_color,background_color);
            font_manager.add_text("text",font_manager.get_count_of_chars_in_text(),1); // it's redrawing main text,its position and its colors;
            uint32_t w,h;
            SDL_Texture* texture = font_manager.get_pixels_in_texture(&w,&h);
            ...
        }

// example of loop1:
        for(;;)
        {
            static variable0 = 4;
            variable0++;
            font_manager.change_text_color(variable0,variable0,letters_color,background_color);
            font_manager.colorize_text();   // for updating color of specific letters in text; it's only redrawing letters with new colors; it's a heavy function but not as heavy as "new_text" or "replace_text" or "redraw_text" and etc.;
            uint32_t w,h;
            SDL_Texture* texture = font_manager.get_pixels_in_texture(&w,&h);
            ...
        }
------------------------------------------------------------------------------------------------------------------
~~~~

</details>
<br/>

> [!NOTE]
> detailed documentation about each function in this class are inside header file "cSDL_FontManager.h";





