#ifndef CSDL_FONTMANAGER_SECURITY_H
#define CSDL_FONTMANAGER_SECURITY_H


#include <ft2build.h>
#include <freetype/freetype.h>
#include <stdint.h>         // for uint
#include <stdlib.h>         // for malloc/free
#include <string.h>         // for memcpy/memset
//#include <SDL2/SDL.h>     // optional

/***
    this is font manager for horizontal UNICODE text; it can be used to create tools like textboxes,notepads and things like that;
    the purpose of this class is to be easy to use,customizable and flexible to various optymalizations font manager;
    this class is flexible to various logical optymalization methods so if u use this class correctly,ur code can be very fast;
    it's using freetype library for loading fonts from files so freetype library is needed;
    it can be used with and withoud SDL2 library so SDL2 library is not needed;
    class doesn't support vertical text so u have to use this class in more manual way to get the desire effect;
    class needs extreme testing for finding potential bugs that may occur with correct use of class;
***/
/*

        example of use number 1:
------------------------------------------------------------------------------------------------------------------
    cSDL_FontManager font_manager(render,"SomeFont.ttf",0,64,20);                                   //  required to work;
    int32_t error = font_manager.geterror_constructor();                                            //  highly recommended;
    if(error!=0) destroy_object_and_print_error(error);                                             //
    font_manager.load_characters(8,cSDL_FontManager::LoadCharactersRange::ALL);                     //  required to work otherwise u cannot draw any text (u have to run this funcion at least once);

    font_manager.new_text("some text to draw\ntext in new line);                                    //  required to work (just to have some text u can work with);
    uint32_t w,h;                                                                                   //
    SDL_Texture* texture = font_manager.get_pixels_in_texture(&w,&h);                               //  now u have image of text u can work with;
------------------------------------------------------------------------------------------------------------------


        example of use number 2 (without SDL):
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


        example of use number 3:
------------------------------------------------------------------------------------------------------------------
    cSDL_FontManager font_manager(render,"SomeFont.ttf",0,640,480);                                 //  required to work;
    int32_t error = font_manager.geterror_constructor();                                            //  highly recommended;
    if(error!=0) destroy_object_and_print_error(error);                                             //
    font_manager.load_characters(16,cSDL_FontManager::LoadCharactersRange::NUMBERS);                //  required to work otherwise u cannot draw any text (u have to run this funcion at least once);

    font_manager.switch_color_modes(1);                                                             //  not needed additional function (thanks to this u can coloring text);
    cSDL_FontManager::TextColor letters_color = {255,168,168,0};                                    //
    cSDL_FontManager::TextColor background_color = {0,0,0,0};                                       //
    font_manager.new_text("123456789\n987654321);                                                   //  required to work 'cause now u have drawed image of text;
    font_manager.change_text_color(0,3,letters_color,background_color);                              //  not needed additional function;

            example of loop0:

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

                example of loop1:

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
*/
#ifdef SDL_h_
    #define  CSDL_FONTMANAGER_WITH_SDL  // if u don't want to use SDL with this class even if u include this library,then just delete this line or paste this class before including SDL;
#endif
class cSDL_FontManager
{
public:
// type of characters u want to load; if you load limited number of characters then the drawing text functions will be less heavy;
// ALL: all available characters in font; can be very heavy in some cases so watch out and inspect how much characters your font loads;
//          you can use function "get_loaded_characters()" to check how much of characters has been loaded from the font; larger number == heavier functions to draw text;
// LETTERS: chars in ASCII: 65-90,97-122;
// NUMBERS: chars in ASCII: 48-57;
// EXTRA: chars in ASCII: 9,10,13,27,32-47,58-64,91-96,123-127;
// LETTERS_EXTRA: chars in ASCII: LETTERS+EXTRA;
// NUMBERS_EXTRA: chars in ASCII: NUMBERS+EXTRA;
// NUMBERS_LETTERS: chars in ASCII: NUMBERS+LETTERS;
// NUMBERS_LETTERS_EXTRA: chars in ASCII: NUMBERS+LETTERS+EXTRA;
// CUSTOM: set it if u want using "custom_buffor" and define yourself characters u want to load in unicode; although this is not required but it is here for aesthetic reasons;
// chars '\n' and '\t' are loaded by default;
    enum LoadCharactersRange{ALL,LETTERS,NUMBERS,EXTRA,LETTERS_EXTRA,NUMBERS_EXTRA,
                                NUMBERS_LETTERS,NUMBERS_LETTERS_EXTRA,CUSTOM};

    struct Character
    {
        uint32_t* bitmap;   //  main letter pixel bitmap; has the format of ARGB in big endian; bitmap has no padding,just straight lines of pixels so pitch is bitmap_width*4;
        uint32_t bitmap_width,bitmap_height; // width and height of "bitmap";
        int32_t offset_right,offset_up;     // offset_right can be negative so watch out when rendering first character in main bitmap;
        int32_t advance;                    // number of pixels to the next character; can be negative;

        uint32_t unicode;   // unicode of character;
    };

    struct TextCharacter
    {
        bool hitbox_visible;// if variable is "0" then hitbox is not visible in text bitmap and it's not setted up; setted up is only "gliph_pointer" and eventually letter bitmap variables if "letter_visible" is 1;
        uint32_t start_x;   // first left pixel in "_main_bitmap"; width is "advance" from "gliph_pointer" in normal viewpoint; setted up when "hitbox_visible" is 1;
        uint32_t x_width;   // can be 0 so watch out; it's usefull for changing background calculation; setted up when "hitbox_visible" is 1;
        uint32_t start_y;   // first upper pixel in "_main_bitmap"; height is "_line_height" in normal viewpoint; setted up when "hitbox_visible" is 1;
        uint32_t y_height;  // can be 0 so watch out; it's usefull for changing background calculation; setted up when "hitbox_visible" is 1;
        uint32_t background_color; // background color in ARGB big endian format; setted up when "hitbox_visible" is 1;

        bool letter_visible; // visibility of bitmap; tells if variables "letter_x" & "letter_y" & "letter_width" & "letter_height" are setted up;
        uint32_t letter_x;  // graphical first left x of letter; setted up when "letter_visible" is 1;
        uint32_t letter_y;  // graphical first upper y of letter; setted up when "letter_visible" is 1;
        uint32_t letter_width;  // graphical width of letter; setted up when "letter_visible" is 1;
        uint32_t letter_height;  // graphical height of letter; setted up when "letter_visible" is 1;
        uint32_t letter_cut_off_left; // cutted of left side of letter; setted up when "letter_visible" is 1;
        uint32_t letter_cut_off_up; // cutted of upper side of letter; setted up when "letter_visible" is 1;
        uint32_t letter_color;  // letter color in ARGB big endian format; setted up when "letter_visible" is 1;

        uint32_t line;     // number of line this character is at; from 0 to up; this is only variable where can be setted when "letter_visible" or "visible" is setted to 1;

        Character* gliph_pointer; // setted up always; pointer to structure of original character; character '\n' is the only character where only "unicode" is setted up;
    };

    struct TextColor
    {
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    struct ViewPoint
    {
        int32_t x;
        int32_t y;
        bool interpret_y_as_line_height; // if 0 then "y" is calculated as pixels; if 1 then "y" is calculated as height of line;
    };

private:

    FT_Error _error_code;

    bool _external_library;  // for destructor only;
    bool _external_font;     // for destructor only;
    bool _destroy_class;     // for destructor only;
    const char* _filename_for_constructor; // for copying mechanism;
    uint32_t _style_index;                  // for copying mechanism;

    FT_Library _library;
    FT_Face _font;
#ifdef CSDL_FONTMANAGER_WITH_SDL
    SDL_Renderer* _render;
    SDL_Texture* _main_texture;
#endif
    uint32_t* _main_bitmap;         //  raw main bitmap
    uint32_t _bitmap_w,_bitmap_h;
    uint32_t _largest_bitmap_w,_largest_bitmap_h;
    uint32_t _default_letters_color,_default_background_color;
    int32_t _extra_offset_right,_extra_offset_up;

    uint32_t* _main_text_buffor;
    size_t _count_main_text_buffor;
    size_t _reserved_count_main_text_buffor; // for optymalization reasons;
    TextCharacter* _text_characters;         // for informations and calculations;
    size_t _count_text_characters;           // for informations and calculations;
    size_t _reserved_count_text_characters;  // for optymalization reasons;

// calculation dynamic variables of drawing functions:
    bool _word_wrap;
    uint32_t _total_updated_columns,_total_updated_rows;    // of "_main_bitmap" and "_final_bitmap"; it's total updated methrics that starts calculating and resets after func "load_characters()";
                                                                    //  u should use this variable each time u render new text; u don't need this variables when u updates only color of bitmap;
                                                                    //  columns == w; rows == y;
    ViewPoint _viewpoint;
    uint32_t _total_lines_in_text;              // only information variable;
    uint32_t _visible_bitmap_letters;           // visible bitmap letters in actual text;
    uint32_t _visible_hitbox_letters;           // visible hitboxes in actual text;
    uint32_t _first_index_of_visible_hitbox;    // first index of visible hitbox in structure "_text_characters";
    uint32_t _first_index_of_visible_letter;    // first index of visible letter in structure "_text_characters";
    bool _quality_draw;                         // for changing method of drawing text;
    bool _color_mode;                           // draw colored bitmap or not;
    uint32_t _tab_width;                         // tab width
    bool _treat_tab_width_as_spaces;             // treat tab width as pixels or as spaces;
    uint32_t _tab_index_inside_characters;      // this is form of optymalization;

//------------------------

    uint32_t _count_of_characters;
    Character* _characters;
    Character _extra_enter_character;   // setted up only "unicode" to '\n';

    uint32_t _max_height;       // for calculations max height of char;
    uint32_t _max_y;            // for calculating height of background of the chars; it's a first line max Y;
    uint32_t _line_height;     // for calculating new line \n;
                                    //  to draw background u have to draw it from ascender with height of "_line_height";


public:

//          main functions:
//-------------------------------------------------------------------------------------------------------------------------

// "reserverd_bitmap_w" and "reserverd_bitmap_h" cannot be 0; if u set up 0 in any of these variables,constructor will automatically change it to 1;
//      if u want functions "change_bitmap_size()" & "change_bitmap_size_dynamically()" to work fast,then u should set up
//      "reserverd_bitmap_w" & "reserverd_bitmap_h" to maximum variable u're gonna use in this class; then use "change_bitmap_size()" to set up default bitmap size u want to use;
//      this is kind of reservation of available maximum bitmap memory u're gonna be using in this class so class doesn't have to realloc bitmap and texture with new size;
// class will create "FT_Library" & "FT_Face" if u don't pass these variables created beforehand to the class constructor;
// "filename" should be path to font .ttf/.otf etc. if u don't pass your variable of "FT_Face" to the constructor of the class;
// "render" should be SDL render created beforehand; this variable will be used to create structure that will be returned from func "get_pixels_in_texture()";
#ifdef CSDL_FONTMANAGER_WITH_SDL
    cSDL_FontManager(SDL_Renderer* render,const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h);
    cSDL_FontManager(SDL_Renderer* render,const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library);
    cSDL_FontManager(SDL_Renderer* render,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library,FT_Face font);
#else
    cSDL_FontManager(const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h);
    cSDL_FontManager(const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library);
    cSDL_FontManager(uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library,FT_Face font);
#endif


// after creating object of this class or copying one object to another,this function should be executed;
//      if error occurs,the created class object created by u should be manually freed 'cause it's not usable;
// returns non-zero number if error occurs; returns 0 if object was successfuly created;
//      meaning of error number can be checked in "freetype" documentation;
//      freetype.org/freetype2/docs/reference/ft2-error_code_values.html
    FT_Error geterror_constructor();

// loads characters from font that can be rendered; this is one of the main functions and it's heavy and should be used only when needed;
//      this function must be executed at least once at the time when u created object of this class; you cannot use any of the other functions
//      in class if u didn't call this function first;
// "font_size": size of characters;
// "characters_type": type of characters to load;
// "custom_buffor": optional pointer to allocated beforehand buffor to array of characters to load in UNICODE from font;
//      if this variable is not-NULL then variable "characters_type" is ignored; u don't have to setup "characters_type" to "CUSTOM" but u can for aesthetic aspects;
// "count_custom_buffor": count of chars inside "custom_buffor"; optional aswell; must be setted when "custom_buffor" isn't NULL;
// returns freetype error if there is any; returns 0 if function successed; returns -1 if in font there's not even one char available to load;
//      returns -2 if argument/arguments are invalid;
// should be called BEFORE calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    int32_t load_characters(uint32_t font_size,cSDL_FontManager::LoadCharactersRange characters_type,uint32_t* custom_buffor = NULL,uint32_t count_custom_buffor = 0);

// it's one of the main functions to redrawing whole bitmap with text; it's a heavy function;
// "text" must ends with char \0 (not in uint32_t* version); if doesn't contains \0 then function may and should crash;
//      version with uint32_t* is a little faster 'cause there is no conversion from char* to uint32_t*;
//      "count_of_chars" must contain number of chars that u pass to the function in variable "text";
// if some char of the "text" doesn't has been loaded from font then the first char that has been loaded from font is rendered in his place;
// function destroys all of the informations for each of the previous letters about their color and background color if there was any;
// this function uses function "colorize_text()" if color mode is enabled by function "switch_color_modes()";
// returns 0 if functions ends with success; returns -1 if none of characters from font are available (probably didn't used function "load_characters()" before as u should);
    int32_t new_text(const char* const text);
    int32_t new_text(const uint32_t* const text,size_t count_of_chars);
// it's one of the main functions and has the same mechanism as "new_text" and it's similarly heavy as well but only deletes characters from actual text buffor;
// "delete_from" & "delete_to" are the indexes from 0 to MAX_UINT32 to actual text buffor that has been created by "add_text()" or "new_text()";
//      if in "delete_to" you will point at index beyond max index in actual text buffor then funtion will just deletes maximum number of characters that can;
//      if in "delete_from" you will point at too high index beyond max that is in actual text buffor then none of the characters will be deleted and function fails;
// "save_colored_bitmap" is to save information about colored text inside actual text buffor; i u're coloring text or backround and
//      want to save your previous progress then set "save_colored_bitmap" to 1; be aware that this option make this funcion a little more heavier;
// this function uses function "colorize_text()" if color mode is enabled by function "switch_color_modes()";
// returns 0 if functions ends with success; returns -1 if "delete_from" is larger than "delete_to" or there is no chars in bitmap or "delete_from" is larger than max index in actual text buffor;
    int32_t delete_text(uint32_t delete_from,uint32_t delete_to,bool save_colored_bitmap);
// it's one of the main functions and has the same mechanism as "new_text" and it's similarly heavy as well but only adds characters to actual text buffor;
// if "add_at_position" is larger than maximum available index+1 in main text bitmap then "add_at_position" is setted to maximum available index+1;
// "text" must ends with char \0 (not in uint32_t* version); if doesn't contains \0 then function may and should crash;
// "save_colored_bitmap" is to save information about colored text inside actual text buffor; i u're coloring text or backround and
//      want to save your previous progress then set "save_colored_bitmap" to 1; be aware that this option make this funcion a little more heavier;
// "count_of_chars" in "uint32_t*" version is the number of chars inside "text"; if u tell larger number than it is in "text" then program may and should crash;
// function with "uint32_t*" as "text" is a little faster 'cause there is no conversion from char* to uint32*;
// these functions uses function "colorize_text()" if color mode is enabled by function "switch_color_modes()";
// returns 0 if no errors; returns -1 if none of characters from font are available (probably didn't used function "load_characters()" before as u should);
    int32_t add_text(const char* const text,uint32_t add_at_position,bool save_colored_bitmap);
    int32_t add_text(const uint32_t* const text,size_t count_of_chars,uint32_t add_at_position,bool save_colored_bitmap);
// it's one of the main functions and has the same mechanism as "new_text" and it's similarly heavy as well but only replaces characters in actual text buffor;
// version with "char*" is a little heavier 'cause there is conversion from "char*" to "uint32_t*";
// "text" must ends with char \0 (not in uint32_t* version); if doesn't contains \0 then function may and should crash;
// "replace_from" & "replace_to" are the indexes from 0 to MAX_UINT32 to actual text buffor that has been created by "add_text()" or "new_text()";
//      if in "replace_to" you will point at index beyond max index in actual text buffor then funtion will just replaces maximum number of characters that can;
//      if in "replace_from" you will point at too high index beyond max that is in actual text buffor then none of the characters will be replaced and function fails;
// "save_colored_bitmap" is to save information about colored text inside actual text buffor; i u're coloring text or backround and
//      want to save your previous progress then set "save_colored_bitmap" to 1; be aware that this option make this funcion a little more heavier;
// these functions uses function "colorize_text()" if color mode is enabled by function "switch_color_modes()";
// returns 0 if no errors and function ends successively; returns -2 if "replace_from" is larger than maximum available index in actual text buffor or if actual text buffor has no text or if
//      "replace_from" is larger than "replace_to"; returns -1 if none of characters from font are available (probably didn't used function "load_characters()" before as u should);
    int32_t replace_text(const char* const text,uint32_t replace_from,uint32_t replace_to,bool save_colored_bitmap);
    int32_t replace_text(const uint32_t* const text,size_t count_of_chars,uint32_t replace_from,uint32_t replace_to,bool save_colored_bitmap);
// it's one of the main functions and has the same mechanism as "new_text" and it's similarly heavy as well but only redraws actual text that is inside actual text buffor if there is any;
//      the purpose of this function is to update text bitmap wihout changing content of the actual text buffor; you should run this func
//      after calling functions that changes attributes of the main bitmap/text like "change_bitmap_size()","words_wrap_up()" and etc.;
// "save_colored_bitmap" is to save information about colored text inside actual text buffor; i u're coloring text or backround and
//      want to save your previous progress then set "save_colored_bitmap" to 1; be aware that this option make this funcion a little more heavier;
// this function uses function "colorize_text()" if color mode is enabled by function "switch_color_modes()";
    void redraw_text(bool save_colored_bitmap);


//      SDL ONLY FUNCTION
// draws final bitmap to the main texture; after this,texture is updated; it's a heavy and one of the main functions;
// it's the main text texture from the class; texture shouldn't be deallocated and changed in any way; texture lives until class isn't destroyed or texture isn't reallocated due to changed size of bitmap;
// "out_texture_w" & "out_texture_h" are actual maximum metrics of texture that function returns; methrics may or may not change if u change size of the bitmap due to optymalization;
//      for example if u change bitmap "w" by function "change_bitmap_size()" by 5 and actual bitmap was 200 then this func can return in "w" for example
//      number 230; and another time "w" may return the same number u pass to "change_bitmap_size()" so watch out; can be NULL if u don't need them;
//      if u want to use variables "out_texture_w" & "out_texture_h" with mathematic calculations u should run function "change_bitmap_size()" (or just reserve sizes in constructor) once with maximum methrics of bitmap that
//      u want to use with this class and don't go beyond that number; then "out_texture_w" & "out_texture_h" will not change and will have the same size/maximum methrics that u used with
//      the function "change_bitmap_size()"; it's kind of reservation so texture and bitmap will not change their size;
// returns main texture of the text;
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
#ifdef CSDL_FONTMANAGER_WITH_SDL
    SDL_Texture* get_pixels_in_texture(uint32_t* out_texture_w,uint32_t* out_texture_h);
#endif
// returns pointer to the raw pixels bitmap in big endian ARGB format; returns bitmap directly from the class so u shouldn't deallocate memory from this pointer;
// "updated_w_out" & "updated_h_out" & "updated_pitch_out" should be pointers that will be filled with updated bitmap width and height and pitch; this function returns updated methrics from bitmap; can be NULL if u don't need them
//      but you should need them especially when u're updating text 'cause each time usable space of main text bitmap is different then different is "updated_w_out" & "updated_h_out" & "updated_pitch_out";
//      basically u don't need these variables only when u're just changing color of the text;
// the returned pitch of the bitmap in "updated_ptich_out" will be ready to go: actual_bitmap_w*4;
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    uint32_t* get_pixels(uint32_t* updated_w_out,uint32_t* updated_h_out,uint32_t* updated_pitch_out);


// it's a function only to color text and not drawing new letters; to use this function you should use enable color mode by func "switch_color_modes()";
// it's one of the main functions and it is heavy;
// you should either use functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()" whose automatically use this function
//      or combine this function with "change_text_color()" or "change_text_color_dynamically()" by calling them beforehand and then calling "get_pixels_in_texture()"
//      or "get_pixels()" if u want only to draw new colors to existing text drawed beforehand by one of function to draw text; this is heavy function but not as heavy as functions to redraw text;
    void colorize_text();

//-------------------------------------------------------------------------------------------------------------------------




//      additional functions:
//-------------------------------------------------------------------------------------------------------------------------

// changes width of tab '\t'; this character is loaded by "load_characters()" independed and if u want to use tab,then u should
//      make sure that space character ' ' is definied inside font because tab width is calculated based on that character;
//      but u shouldn't be worry too much 'cause basically in every font has space definied;
// "tab_width" new width of the tab;
// "interprete_width_as_spaces" is variable that defines how "tab_width" should be interpreted; if u set it to 0 then "tab_width" is
//      interpreted like pixels; if u set it to 1 then "tab_width" is calculated based on width of space definied inside font;
// by default "tab_width" is 4 and "interprete_width_as_spaces" is 1;
// should be called BEFORE "load_characters()";
    void change_tab_width(uint32_t tab_width,bool interprete_width_as_spaces);

// turn ON or OFF colored mode for letters and background; by default this option is OFF;
// by enabling this function,function like "change_text_render_quality_mode() doesn't work,drawing text is slower but for exchange
//      you can use functions like: "change_default_text_color()","change_text_color()","change_text_color_dynamically";
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void switch_color_modes(bool enable);

// changes maximum size of bitmaps and texture; changes size of texture and bitmaps only if max allocated bitmap size in class history is lower than actually changed size by this funtion;
//      in this case texture and bitmap is reallocated with new size;
//      that mechanism exists due to optymalization reasons; overall it's quiet a heavy function and kind of function for reservation memory for the bitmaps and texture;
// "new_w" & "new_h" should be higher than 0; if it is not,then automatically size is changed to 1;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_bitmap_size(uint32_t new_w,uint32_t new_h);

// it's the same function as "change_bitmap_size()" except it works dynamically by adds numbers to actual size of bitmap;
// numbers of arguments can be negative;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_bitmap_size_dynamically(int32_t new_w,int32_t new_h);

// turns ON/OFF words wrap up; if text is beyond available bitmap width then it's wrap up to new line automatically without char \n;
//      by default this option is turned OFF;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void words_wrap_up(bool enable);

// gets status of "words_wrap_up"; can be called any time;
    bool get_words_wrap_up() const;

// change height of the enter to custom variable (in pixels);
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_line_height(uint32_t new_line_height);

// gets actual viewpoint; can be called anytime;
    void get_actual_viewpoint(cSDL_FontManager::ViewPoint* viewpoint) const;

// change viewpoint of text;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_viewpoint(int32_t x,int32_t y);

// adds values to actual viewpoint of text (dynamic function);
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_viewpoint_dynamically(int32_t x,int32_t y);

// get lines from text;
// should be called after calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    uint32_t get_total_lines_in_text() const;

// changes method of drawing text; you shouldn't enable color mode by function "switch_color_modes()"; otherwise this function will has no effect;
// "quality_mode": if it's 0 then quality mode is "fast" and chars may overwrites some other chars that collapse with each other;
//      if it's 1 then quality mode is "precision" which means before rendering pixels in bitmap,alghorithm checks which pixel should be drawed and which shouldn't (it's slow method);
// by default "quality_mode" is 0;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_text_draw_quality_mode(bool quality_mode);

// gets actual method of quality mode; can be called any time; returned variable doesn't matter if u turn ON color mode by function "switch_color_modes()";
    bool get_text_draw_quality_mode() const;

// changes interpretation of the value "y" that has been passed to the function "change_viewpoint()" or "add_to_viewpoint";
// "interpret_as_line_height" changes interpretation of variable "y"; by setting "1" it will converts interpretation from pixels to height of line;
//      if setted to 1,then function "change_line_height()" influences behavior of this function;
//      by default it's 0;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_interpretation_of_viepointY(bool interpret_y_as_whole_lines);

// it changes default colors of rendered letters and background; to use this function you should beforechang enabling color mode by function "switch_color_modes()";
// this function affects functions like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()" and should be called before these functions;
    void change_default_text_color(cSDL_FontManager::TextColor letters_color,cSDL_FontManager::TextColor background_color);

// gets default text colors; can be called any time; variables that u don't need can be NULL;
    void get_default_text_color(cSDL_FontManager::TextColor* letters_color,cSDL_FontManager::TextColor* background_color) const;

// changes color of text that you want to draw; to use this function you should beforechang enabling color mode by function "switch_color_modes()";
// "change_from" & "change_to" are the indexes from 0 to MAX_UINT32 to actual text buffor that has been created by "add_text()" or "new_text()";
//      if in "change_to" you will point at index beyond max index in actual text buffor then funtion will just changes maximum number of characters that can;
//      if in "change_from" you will point at too high index beyond max that is in actual text buffor then none of the characters will be changed;
//      "change_from" shouldn't be larger than "change_to";
// "letters" & "background" are the colors u want to apply to the text;
//      default variables to "letters" are: a==255,r==0,g==0,b==0;
//      default variables to "background" are: a==0,r==0,g==0,b==0;
// should be called BEFORE function "colorize_text()" or BEFORE calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";;
    void change_text_color(uint32_t change_from,uint32_t change_to,cSDL_FontManager::TextColor letters,cSDL_FontManager::TextColor background);
// it's the same function as "change_text_color()" except it's adding/subtracting numbers from "letters" & "background" to actual numbers;
//      this function is heavier than "change_text_color()";
// "add_or_subtract" decide if u want add or subtract numbers to the actual variables; 0 means add,1 means subtract;
    void change_text_color_dynamically(uint32_t change_from,uint32_t change_to,cSDL_FontManager::TextColor letters,cSDL_FontManager::TextColor background,bool add_or_subtract);

// works like "get_information_chars_in_text()" but doesn't requires "cSDL_FontManager::TextCharacter" structure to pass it;
// returns number of characters drawed in main bitmap (also characters that aren't visible);
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    size_t get_count_of_chars_in_text() const;

// function to check how much of characters function "load_characters()" has been loaded and to getting access to the pointer of structure to each of character;
// "characters" should be not allocated pointer; this variable returns pointer to allocated inside class structure to all loaded characters;
//      WARNING: returned structure shouldn't be freed otherwise class will/should crash; u also shouldn't change this structure if u don't have good reason for it and
//      if u don't know what u doing;
// function returns number of characters/number of indexes in returned variable "characters"; if u're confused why this function returns larger number than u should load characters,
//      that is because some extra characters are created indepedent like tab '\t';
//      larger number of characters == heavier functions to draw text;
// function should be called after function "load_characters()";
    size_t get_loaded_characters(cSDL_FontManager::Character*& characters) const;


// returns information about chars in already rendered text (also about chars that aren't visible);
// "text_characters_pointer" should be not allocated pointer to which func returns address of allocated already structure inside claass;
//      WARNING: you shouldn't change this structure; you should only reads from it infromations,otherwise program may crash espacially if u don't know what u doing;
// "first_index_for_visible_letter" returns first index in "text_characters_pointer" to first visible letter in text (letter_visible is 1); can be NULL if u don't need this;
//      actual index can be 0 but it can also have 0 if no letter is visible in text; before this func run "get_visible_letters_count_from_text()" to check if there is any letter visible;
// "first_index_for_visible_hitbox" returns first index in "text_characters_pointer" to first visible hitbox in text (hitbox_visible is 1); can be NULL if u don't need this;
//      actual index can be 0 but it can also have 0 if no hitbox is visible in text; before this func run "get_visible_letters_count_from_text()" to check if there is any hitbox visible;
// returns count of structures returned inside pointer "text_characters_pointer";
// function works good with another function "get_visible_letters_count_from_text()";
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    size_t get_information_chars_in_text(cSDL_FontManager::TextCharacter*& text_characters_pointer,uint32_t* first_index_for_visible_letter,uint32_t* first_index_for_visible_hitbox);

// returns what maximum number of pixels to "x" should be passed to function "change_viewpoint()" for maximum edge (for hitbox) of horizontal scroll;
//      it's usefull when u're creating horizontal scrolling mechanizm without word wrap;
// don't call it every frame 'cause it's a little heavy function; call it only when you're adding or deleting some text;
// should returns negative number; otherwise if returns 0 then there was an error or your actual text is fitting in the main bitmap;
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
// actual "x" of text passed to "change_viewpoint()" doesn't change results returned by this function;
    int32_t find_viewpoint_x_for_max_horizontal_scroll() const;
// returns what maximum number of pixels to "y" should be passed to function "change_viewpoint()" for maximum edge (for hitbox) of horizontal scroll;
//      it's usefull when u're creating vertical scrolling mechanism;
// this function is light,not like "find_viewpoint_x_for_max_horizontal_scroll()";
// should returns negative number; otherwise if returns 0 then there was an error or your actual text is fitting in the main bitmap;
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
// actual "y" of text passed to "change_viewpoint()" doesn't change results returned by this function;
// variable "interpret_y_as_whole_lines" in "change_interpretation_of_viepointY()" should be set to 0 when changing "y" in "change_viewpoint()" by variable returned by this function;
    int32_t find_viewpoint_y_for_max_vertical_scroll() const;

// returns height of line in text; should be called after "load_characters()" 'cause this function changes line height;
    uint32_t get_line_height() const;

// get numbers of visible letters/hitboxes from the drawed text;
// function may help in iterating through structure "text_characters_pointer" from function "get_information_chars_in_text()"; "visible_hitboxes" or "visible_letters"
//      will not have larger number than size of the structure;
// "visible_hitboxes" will return number of hitboxes visible in text; hitboxes can be graphically seen if u draw text in color mode and draw background; background space is the hitbox of letter;
// "visible_letters" will return number of letters visible in text;
// variables that u don't need can be NULL;
// should be called AFTER calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void get_visible_letters_count_from_text(uint32_t* visible_hitboxes,uint32_t* visible_letters) const;


//      more advanced and experimental functions:

// experimental function,especially variable "offset_right"; class isn't fully ready for this function;
// adds these variables to actual offsets of each of individual bitmaps; it's usefull when u're coloring background and using function "change_line_height()" to correct the positions of the letters;
// numbers can be negative;
// should be called before calling functions to redrawing text like "new_text()" or "add_text()" or "delete_text()" or "replace_text()" or "redraw_text()";
    void change_offset_of_bitmaps(int32_t offset_right,int32_t offset_up);



//-------------------------------------------------------------------------------------------------------------------------




    ~cSDL_FontManager();

private:
    void _new_text_wraper(const uint32_t* const text,size_t lenght);
    void _cSDL_FontManager_reset_variables();
    void _free_class(cSDL_FontManager& object);
    void _copy_objects_of_class(const cSDL_FontManager& other_object);
    void _move_object_of_class(cSDL_FontManager& other_object);

public:

    //  implementation for "rule of the five" needed 'cause of manual memory management existing in class (new/malloc);

//  copying constructor; used like:
//      Class object0 = object1;
//      Class object0 = return_object();
//          [inside return_object]:
//          return object2;
    cSDL_FontManager(const cSDL_FontManager& other_object);

//  copying operator; used like:
//      object0 = object1;
//      object0 = return_object();
//          [inside return_object]:
//          return object2;
    cSDL_FontManager& operator=(const cSDL_FontManager& other_object);

//  moving constructor; used like:
//      Class object0 = Class();
//      Class object0 = std::move(object1);
//      Class object0 = return_object();
//          [inside return_object]:
//          return Class();
    cSDL_FontManager(cSDL_FontManager&& other_object);

//  moving operator; used like:
//      object0 = Class();
//      object0 = std::move(object1);
//      object0 = return_object();
//          [inside return_object]:
//          return Class();
    cSDL_FontManager& operator=(cSDL_FontManager&& other_object);

};
#ifdef CSDL_FONTMANAGER_WITH_SDL
cSDL_FontManager::cSDL_FontManager(SDL_Renderer* render,const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h)
#else
cSDL_FontManager::cSDL_FontManager(const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h)
#endif
{
    this->_filename_for_constructor = filename;
    this->_style_index = style_index;

    this->_external_library = 0;
    this->_external_font = 0;
    this->_cSDL_FontManager_reset_variables();

    this->_error_code = FT_Init_FreeType(&this->_library);
    if(this->_error_code!=0) return;

    this->_error_code = FT_New_Face(this->_library,filename,style_index,&this->_font);
    if(this->_error_code!=0) return;
//---------------------------------------------------------------------------------

    this->_bitmap_h = reserverd_bitmap_h; this->_bitmap_w = reserverd_bitmap_w;
    if(this->_bitmap_w==0) this->_bitmap_w = 1;
    if(this->_bitmap_h==0) this->_bitmap_h = 1;
    this->_largest_bitmap_w = this->_bitmap_w;
    this->_largest_bitmap_h = this->_bitmap_h;
#ifdef CSDL_FONTMANAGER_WITH_SDL
    this->_render = render;
    this->_main_texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,reserverd_bitmap_w,reserverd_bitmap_h);
    SDL_SetTextureBlendMode(this->_main_texture,SDL_BLENDMODE_BLEND);
#endif
    this->_main_bitmap = (uint32_t*)calloc(reserverd_bitmap_h*reserverd_bitmap_w,sizeof(uint32_t));

    return;
}

#ifdef CSDL_FONTMANAGER_WITH_SDL
cSDL_FontManager::cSDL_FontManager(SDL_Renderer* render,const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library)
#else
cSDL_FontManager::cSDL_FontManager(const char* filename,uint32_t style_index,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library)
#endif
{
    this->_filename_for_constructor = filename;
    this->_style_index = style_index;

    this->_external_library = 1;
    this->_external_font = 0;
    this->_cSDL_FontManager_reset_variables();

    this->_library = library;
    this->_error_code = FT_New_Face(this->_library,filename,style_index,&this->_font);
    if(this->_error_code!=0) return;
//--------------------------------------------------------------------------------

    this->_bitmap_h = reserverd_bitmap_h; this->_bitmap_w = reserverd_bitmap_w;
    if(this->_bitmap_w==0) this->_bitmap_w = 1;
    if(this->_bitmap_h==0) this->_bitmap_h = 1;
    this->_largest_bitmap_w = this->_bitmap_w;
    this->_largest_bitmap_h = this->_bitmap_h;
#ifdef CSDL_FONTMANAGER_WITH_SDL
    this->_render = render;
    this->_main_texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,reserverd_bitmap_w,reserverd_bitmap_h);
    SDL_SetTextureBlendMode(this->_main_texture,SDL_BLENDMODE_BLEND);
#endif
    this->_main_bitmap = (uint32_t*)calloc(reserverd_bitmap_h*reserverd_bitmap_w,sizeof(uint32_t));

    return;
}

#ifdef CSDL_FONTMANAGER_WITH_SDL
cSDL_FontManager::cSDL_FontManager(SDL_Renderer* render,uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library,FT_Face font)
#else
cSDL_FontManager::cSDL_FontManager(uint32_t reserverd_bitmap_w,uint32_t reserverd_bitmap_h,FT_Library library,FT_Face font)
#endif
{
    this->_filename_for_constructor = NULL;
    this->_style_index = 0;

    this->_external_library = 1;
    this->_external_font = 1;
    this->_cSDL_FontManager_reset_variables();

    this->_library = library;
    this->_font = font;
//--------------------------------------------------------------------------------

    this->_bitmap_h = reserverd_bitmap_h; this->_bitmap_w = reserverd_bitmap_w;
    if(this->_bitmap_w==0) this->_bitmap_w = 1;
    if(this->_bitmap_h==0) this->_bitmap_h = 1;
    this->_largest_bitmap_w = this->_bitmap_w;
    this->_largest_bitmap_h = this->_bitmap_h;
#ifdef CSDL_FONTMANAGER_WITH_SDL
    this->_render = render;
    this->_main_texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,reserverd_bitmap_w,reserverd_bitmap_h);
    SDL_SetTextureBlendMode(this->_main_texture,SDL_BLENDMODE_BLEND);
#endif
    this->_main_bitmap = (uint32_t*)calloc(reserverd_bitmap_h*reserverd_bitmap_w,sizeof(uint32_t));

    return;
}

void cSDL_FontManager::_cSDL_FontManager_reset_variables()
{
    this->_destroy_class = 1;
    this->_error_code = 0;
    this->_library = NULL;
    this->_font = NULL;
    this->_characters = NULL;
    //this->_main_bitmap = NULL;
    //this->_color_bitmap = NULL;
    //this->_main_texture = NULL;
    this->_main_text_buffor = NULL;
    this->_text_characters = NULL;
    this->_count_text_characters = 0;
    this->_reserved_count_text_characters = 0;
    this->_count_main_text_buffor = 0;
    this->_reserved_count_main_text_buffor = 0;
    this->_total_updated_columns = 1;
    this->_total_updated_rows = 1;
    this->_word_wrap = 0;
    this->_viewpoint.x = 0;
    this->_viewpoint.y = 0;
    this->_viewpoint.interpret_y_as_line_height = 0;
    this->_total_lines_in_text = 0;
    this->_quality_draw = 0;
    this->_default_letters_color = 0xFF000000;
    this->_default_background_color = 0x00000000;
    this->_line_height = 0;
    this->_extra_offset_right = 0;
    this->_extra_offset_up = 0;
    this->_visible_bitmap_letters = 0;
    this->_visible_hitbox_letters = 0;
    this->_first_index_of_visible_hitbox = 0;
    this->_first_index_of_visible_letter = 0;
    this->_color_mode = 0;
    this->_count_of_characters = 0;
    this->_extra_enter_character.unicode = (uint32_t)'\n';
    this->_tab_width = 4;
    this->_treat_tab_width_as_spaces = 1;
    this->_tab_index_inside_characters = 0;

    return;
}

FT_Error cSDL_FontManager::geterror_constructor()
{
    return this->_error_code;
}

int32_t  cSDL_FontManager::load_characters(uint32_t font_size,cSDL_FontManager::LoadCharactersRange characters_type,uint32_t* custom_buffor,uint32_t count_custom_buffor)
{

// ALL: all available characters in font; should be used only for experimetional reasons; can be very heavy in some cases;
// LETTERS: chars in ASCII: 65-90,97-122;
// NUMBERS: chars in ASCII: 48-57;
// EXTRA: chars in ASCII: 9,10,13,27,32-47,58-64,91-96,123-127;
// LETTERS_EXTRA: chars in ASCII: LETTERS+EXTRA;
// NUMBERS_EXTRA: chars in ASCII: NUMBERS+EXTRA;
// NUMBERS_LETTERS: chars in ASCII: NUMBERS+LETTERS;
// NUMBERS_LETTERS_EXTRA: chars in ASCII: NUMBERS+LETTERS+EXTRA;

    static uint8_t LETTERS[52] = {65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,
                                    97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,
                                        117,118,119,120,121,122};
    static uint8_t NUMBERS[10] = {48,49,50,51,52,53,54,55,56,57};
    static uint8_t EXTRA[38]   = {9,10,13,27,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,58,59,60,61,62,63,
                                    64,91,92,93,94,95,96,123,124,125,126,127};

    if(this->_characters!=NULL)
    {
        for(uint32_t i = 0; i!=this->_count_of_characters; i++) free(this->_characters[i].bitmap);
        free(this->_characters);
        this->_characters = NULL;
    }
    this->_count_of_characters = 0;

    /*for(uint32_t i = 0; i!=this->_total_updated_rows; i++)
    {
        memset(this->_main_bitmap+(i*this->_bitmap_w),0,this->_total_updated_columns*4);
    }*/
    //this->_total_updated_columns = 1;
    //this->_total_updated_rows = 1;
    //this->_total_lines_in_text = 0;


    int32_t error;
    error = FT_Select_Charmap(this->_font,FT_ENCODING_UNICODE);
    if(error!=0) return error;
    error = FT_Set_Pixel_Sizes(this->_font,0,font_size);
    if(error!=0) return error;

    this->_max_height = (this->_font->size->metrics.ascender-this->_font->size->metrics.descender)/64;
    this->_max_y = this->_font->size->metrics.ascender/64;
    this->_line_height = this->_font->size->metrics.height/64;

    uint32_t* selected_chars;
    uint32_t count_selected_chars;
    bool allocated_buffor;
    if(custom_buffor!=NULL)
    {
        allocated_buffor = 0;
        count_selected_chars = count_custom_buffor;
        selected_chars = custom_buffor;
        if(count_custom_buffor==0) return -2;
    }
    else
    {
        allocated_buffor = 1;

    // help function;
        auto copy_arrays = [](uint32_t* dst,uint32_t& dst_counter,uint8_t* src,uint32_t src_counter)
        {
            for(uint32_t i = 0; i!=src_counter; i++,dst_counter++)
            {
                dst[dst_counter] = src[i];
            }
            return;
        };

        switch(characters_type)
        {
            default: return -2; break;

            case LoadCharactersRange::ALL: allocated_buffor = 0; break;

            case LoadCharactersRange::CUSTOM:
            {
                if(custom_buffor==NULL) return -2;
            }break;

            case LoadCharactersRange::LETTERS:
            {
                count_selected_chars = sizeof(LETTERS);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,LETTERS,sizeof(LETTERS));

            }break;

            case LoadCharactersRange::NUMBERS:
            {
                count_selected_chars = sizeof(NUMBERS);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,NUMBERS,sizeof(NUMBERS));

            }break;

            case LoadCharactersRange::EXTRA:
            {
                count_selected_chars = sizeof(EXTRA);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,EXTRA,sizeof(EXTRA));
            }break;

            case LoadCharactersRange::LETTERS_EXTRA:
            {
                count_selected_chars = sizeof(EXTRA)+sizeof(LETTERS);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,LETTERS,sizeof(LETTERS));
                copy_arrays(selected_chars,main_counter,EXTRA,sizeof(EXTRA));
            }break;

            case LoadCharactersRange::NUMBERS_EXTRA:
            {
                count_selected_chars = sizeof(EXTRA)+sizeof(NUMBERS);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,NUMBERS,sizeof(NUMBERS));
                copy_arrays(selected_chars,main_counter,EXTRA,sizeof(EXTRA));
            }break;

            case LoadCharactersRange::NUMBERS_LETTERS:
            {
                count_selected_chars = sizeof(LETTERS)+sizeof(NUMBERS);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,NUMBERS,sizeof(NUMBERS));
                copy_arrays(selected_chars,main_counter,LETTERS,sizeof(LETTERS));
            }break;

            case LoadCharactersRange::NUMBERS_LETTERS_EXTRA:
            {
                count_selected_chars = sizeof(LETTERS)+sizeof(NUMBERS)+sizeof(EXTRA);
                selected_chars = (uint32_t*)malloc(count_selected_chars*4);

                uint32_t main_counter = 0;
                copy_arrays(selected_chars,main_counter,NUMBERS,sizeof(NUMBERS));
                copy_arrays(selected_chars,main_counter,LETTERS,sizeof(LETTERS));
                copy_arrays(selected_chars,main_counter,EXTRA,sizeof(EXTRA));
            }break;
        }
    }

    if(characters_type==LoadCharactersRange::ALL&&custom_buffor==NULL)
    {
        FT_ULong char_code;
        FT_UInt index;
        count_selected_chars = 0;

        char_code = FT_Get_First_Char(this->_font,&index);
        while(index!=0)
        {
            count_selected_chars++;
            char_code = FT_Get_Next_Char(this->_font,char_code,&index);
        }
        if(count_selected_chars==0)
        {
            this->_count_of_characters = 0;
            return -1;
        }
        this->_characters = (cSDL_FontManager::Character*)malloc(sizeof(cSDL_FontManager::Character)*(count_selected_chars+1)); // +1 'cause extra tab char that is calculated later;

        uint32_t counter = 0;
        char_code = FT_Get_First_Char(this->_font,&index);
        while(index!=0)
        {
            FT_Load_Glyph(this->_font,index,FT_LOAD_RENDER);
            if((this->_font->glyph->bitmap.rows+this->_font->glyph->bitmap.width)>0) this->_characters[counter].bitmap = (uint32_t*)malloc(this->_font->glyph->bitmap.width*this->_font->glyph->bitmap.rows*sizeof(uint32_t));
            else this->_characters[counter].bitmap = NULL;
            this->_characters[counter].bitmap_height = this->_font->glyph->bitmap.rows;
            this->_characters[counter].bitmap_width = this->_font->glyph->bitmap.width;
            this->_characters[counter].offset_right = this->_font->glyph->bitmap_left;
            this->_characters[counter].offset_up = this->_font->glyph->bitmap_top;
            this->_characters[counter].advance = this->_font->glyph->advance.x>>6;
            this->_characters[counter].unicode = char_code;

            for(uint32_t j = 0; j!=this->_font->glyph->bitmap.rows; j++)
            {
                for(uint32_t k = 0; k!=this->_font->glyph->bitmap.width; k++)
                {
                    this->_characters[counter].bitmap[(j*this->_font->glyph->bitmap.width)+k] = ((this->_font->glyph->bitmap.buffer[(this->_font->glyph->bitmap.pitch*j)+k]<<24) | (0<<16) | (0<<8) | 0);
                }
            }

            char_code = FT_Get_Next_Char(this->_font,char_code,&index);
            counter++;
        }

    }
    else
    {
        if(count_selected_chars==0)
        {
            if(allocated_buffor==1) free(selected_chars);
            this->_count_of_characters = 0;
            return -1;
        }
        this->_characters = (cSDL_FontManager::Character*)malloc(sizeof(cSDL_FontManager::Character)*(count_selected_chars+1)); // +1 'cause extra tab char that is calculated later;
        for(uint32_t i = 0; i!=count_selected_chars; i++)
        {
            FT_Load_Glyph(this->_font,FT_Get_Char_Index(this->_font,selected_chars[i]),FT_LOAD_RENDER);
            if((this->_font->glyph->bitmap.rows+this->_font->glyph->bitmap.width)>0) this->_characters[i].bitmap = (uint32_t*)malloc(this->_font->glyph->bitmap.width*this->_font->glyph->bitmap.rows*sizeof(uint32_t));
            else this->_characters[i].bitmap = NULL;
            this->_characters[i].bitmap = (uint32_t*)malloc(this->_font->glyph->bitmap.width*this->_font->glyph->bitmap.rows*sizeof(uint32_t));
            this->_characters[i].bitmap_height = this->_font->glyph->bitmap.rows;
            this->_characters[i].bitmap_width = this->_font->glyph->bitmap.width;
            this->_characters[i].offset_right = this->_font->glyph->bitmap_left;
            this->_characters[i].offset_up = this->_font->glyph->bitmap_top;
            this->_characters[i].advance = this->_font->glyph->advance.x>>6;
            this->_characters[i].unicode = selected_chars[i];


            for(uint32_t j = 0; j!=this->_font->glyph->bitmap.rows; j++)
            {
                for(uint32_t k = 0; k!=this->_font->glyph->bitmap.width; k++)
                {
                    this->_characters[i].bitmap[(j*this->_font->glyph->bitmap.width)+k] = ((this->_font->glyph->bitmap.buffer[(this->_font->glyph->bitmap.pitch*j)+k]<<24) | (0<<16) | (0<<8) | 0);
                }
            }
        }
    }
    this->_count_of_characters = count_selected_chars;

    if(allocated_buffor==1) free(selected_chars);

// calculating extra tab char;
    FT_Load_Glyph(this->_font,FT_Get_Char_Index(this->_font,(uint32_t)' '),FT_LOAD_RENDER);
    this->_characters[count_selected_chars].bitmap = NULL;
    this->_characters[count_selected_chars].bitmap_height = 0;
    this->_characters[count_selected_chars].bitmap_width = 0;
    this->_characters[count_selected_chars].offset_right = 0;
    this->_characters[count_selected_chars].offset_up = 0;
    if(this->_treat_tab_width_as_spaces==1) this->_characters[count_selected_chars].advance = (this->_font->glyph->advance.x>>6)*this->_tab_width;
    else this->_characters[count_selected_chars].advance = this->_tab_width;
    this->_characters[count_selected_chars].unicode = (uint32_t)'\t';
    this->_tab_index_inside_characters = count_selected_chars;
    this->_count_of_characters++;


    return 0;
}

void cSDL_FontManager::words_wrap_up(bool enable)
{
    this->_word_wrap = enable;
    return;
}

int32_t cSDL_FontManager::new_text(const char* const text)
{
    if(this->_count_of_characters==0) return -1;   // if none of a characters are available (probably didn't used func "load_characters()";

    size_t length = strlen(text);
    if(this->_reserved_count_main_text_buffor<length)
    {
        if(this->_main_text_buffor!=NULL) free(this->_main_text_buffor);
        this->_reserved_count_main_text_buffor = length*2;
        this->_main_text_buffor = (uint32_t*)malloc(this->_reserved_count_main_text_buffor*sizeof(uint32_t));
    }
    this->_count_main_text_buffor = length;

    for(size_t i = 0; i!=length; i++)
    {
        this->_main_text_buffor[i] = text[i];
    }

    this->_new_text_wraper(this->_main_text_buffor,this->_count_main_text_buffor);

    if(this->_color_mode==1) this->colorize_text();

    return 0;
}

int32_t cSDL_FontManager::new_text(const uint32_t* const text,size_t count_of_chars)
{
    if(this->_count_of_characters==0) return -1;   // if none of a characters are available (probably didn't used func "load_characters()";

    if(this->_reserved_count_main_text_buffor<count_of_chars)
    {
        if(this->_main_text_buffor!=NULL) free(this->_main_text_buffor);
        this->_reserved_count_main_text_buffor = count_of_chars*2;
        this->_main_text_buffor = (uint32_t*)malloc(this->_reserved_count_main_text_buffor*sizeof(uint32_t));
    }
    this->_count_main_text_buffor = count_of_chars;

    memcpy(this->_main_text_buffor,text,count_of_chars);

    this->_new_text_wraper(this->_main_text_buffor,this->_count_main_text_buffor);

    if(this->_color_mode==1) this->colorize_text();

    return 0;
}

int32_t cSDL_FontManager::delete_text(uint32_t delete_from,uint32_t delete_to,bool save_colored_bitmap)
{
    if(delete_from>delete_to) return -1;
    if(this->_count_main_text_buffor==0) return -1;
    const size_t max_main_text_index = this->_count_main_text_buffor-1;
    if(delete_from>max_main_text_index) return -1;
    if(delete_to>max_main_text_index) delete_to = max_main_text_index;

    const uint32_t chars_to_delete_count = (delete_to-delete_from)+1;
    const uint32_t new_length = this->_count_main_text_buffor-chars_to_delete_count;

    uint32_t* previous_letter_colors = NULL;
    uint32_t* previous_background_colors = NULL;

    size_t index_counter = 0;

    if(save_colored_bitmap==1)
    {
        if(new_length!=0)
        {
            previous_letter_colors = (uint32_t*)malloc(new_length*4);
            previous_background_colors = (uint32_t*)malloc(new_length*4);
        }



        for(; index_counter!=delete_from; index_counter++)
        {
            previous_letter_colors[index_counter] = this->_text_characters[index_counter].letter_color;
            previous_background_colors[index_counter] = this->_text_characters[index_counter].background_color;
        }

        const uint32_t max_end_of_deleting = (delete_to+1);
        /*for(uint32_t i = delete_from; i!=max_end_of_deleting; i++)
        {

        }*/

        for(uint32_t i = max_end_of_deleting,j = 0; i!=this->_count_main_text_buffor; i++,j++,index_counter++)
        {
            this->_main_text_buffor[delete_from+j] = this->_main_text_buffor[i];
            previous_letter_colors[index_counter] = this->_text_characters[i].letter_color;
            previous_background_colors[index_counter] = this->_text_characters[i].background_color;
        }
    }
    else
    {
        const uint32_t max_end_of_deleting = (delete_to+1);
        const uint32_t size_to_copy = this->_count_main_text_buffor-max_end_of_deleting;
        if(size_to_copy!=0) memcpy(&this->_main_text_buffor[delete_from],&this->_main_text_buffor[max_end_of_deleting],size_to_copy*4);

    }

    this->_count_main_text_buffor = new_length;

    this->_new_text_wraper(this->_main_text_buffor,this->_count_main_text_buffor);

    if(save_colored_bitmap==1)
    {
        for(size_t i = 0; i!=index_counter; i++)
        {
            this->_text_characters[i].letter_color = previous_letter_colors[i];
            this->_text_characters[i].background_color = previous_background_colors[i];
        }

        free(previous_letter_colors);
        free(previous_background_colors);
    }

    if(this->_color_mode==1) this->colorize_text();

    return 0;
}


int32_t cSDL_FontManager::add_text(const char* const text,uint32_t add_at_position,bool save_colored_bitmap)
{
    if(this->_count_of_characters==0) return -1;

    size_t length_of_extra_text = strlen(text);
    uint32_t* main_buffor = (uint32_t*)malloc(length_of_extra_text*4);

    for(size_t i = 0; i!=length_of_extra_text; i++)
    {
        main_buffor[i] = (uint32_t)text[i];
    }
    int32_t return_value = this->add_text(main_buffor,length_of_extra_text,add_at_position,save_colored_bitmap);

    free(main_buffor);

    return return_value;
}

int32_t cSDL_FontManager::add_text(const uint32_t* const text,size_t count_of_chars,uint32_t add_at_position,bool save_colored_bitmap)
{
    if(this->_count_of_characters==0) return -1;   // if none of a characters are available (probably didn't used func "load_characters()";


    const size_t max_available_position = this->_count_main_text_buffor;
    if(add_at_position>max_available_position) add_at_position = max_available_position;
    const size_t new_lenght_of_main_text = this->_count_main_text_buffor+count_of_chars;
    const size_t length_of_endpart_of_main_text = max_available_position-add_at_position;
    uint32_t* endpart_of_main_text = NULL;
    if(length_of_endpart_of_main_text!=0)
    {
        endpart_of_main_text = (uint32_t*)malloc(length_of_endpart_of_main_text*4);
        memcpy(endpart_of_main_text,&this->_main_text_buffor[add_at_position],length_of_endpart_of_main_text*4); // copy the end of the actual text buffor;
    }

    this->_count_main_text_buffor = new_lenght_of_main_text;    // change char counter number of the actual text bitmap;

    if(new_lenght_of_main_text>this->_reserved_count_main_text_buffor)   // reserve more memory if needed;
    {
        this->_reserved_count_main_text_buffor = new_lenght_of_main_text*2;
        uint32_t* new_buffor_main_text = (uint32_t*)malloc(this->_reserved_count_main_text_buffor*4);
        memcpy(new_buffor_main_text,this->_main_text_buffor,add_at_position*4);
        free(this->_main_text_buffor);
        this->_main_text_buffor = new_buffor_main_text;
    }

    memcpy(&this->_main_text_buffor[add_at_position],text,count_of_chars*4); // paste new text inside actual text buffor;
    if(length_of_endpart_of_main_text!=0) memcpy(&this->_main_text_buffor[add_at_position+count_of_chars],endpart_of_main_text,length_of_endpart_of_main_text*4); // paste the end of the actual text buffor;

    free(endpart_of_main_text);



    uint32_t* previous_letter_colors = NULL;
    uint32_t* previous_background_colors = NULL;



    if(save_colored_bitmap==1)
    {
        if(this->_count_text_characters!=0)
        {
            previous_letter_colors = (uint32_t*)malloc(this->_count_text_characters*4);
            previous_background_colors = (uint32_t*)malloc(this->_count_text_characters*4);

            for(size_t i = 0; i!=this->_count_text_characters; i++)
            {
                previous_letter_colors[i] = this->_text_characters[i].letter_color;
                previous_background_colors[i] = this->_text_characters[i].background_color;
            }
        }
    }



    this->_new_text_wraper(this->_main_text_buffor,this->_count_main_text_buffor);

    if(save_colored_bitmap==1)
    {
        size_t index;
        for(index = 0; index!=add_at_position; index++)
        {
            this->_text_characters[index].letter_color = previous_letter_colors[index];
            this->_text_characters[index].background_color = previous_background_colors[index];
        }

        const size_t beginning_of_loop = add_at_position+count_of_chars;
        for(size_t i = add_at_position; i!=beginning_of_loop; i++)
        {
            this->_text_characters[i].letter_color = this->_default_letters_color;
            this->_text_characters[i].background_color = this->_default_background_color;
        }

        for(size_t i = beginning_of_loop; i!=this->_count_text_characters; i++,index++)
        {
            this->_text_characters[i].letter_color = previous_letter_colors[index];
            this->_text_characters[i].background_color = previous_background_colors[index];
        }

        free(previous_letter_colors);   // it's save to use free() with NULL pointers;
        free(previous_background_colors); // it's save to use free() with NULL pointers;
    }

    if(this->_color_mode==1) this->colorize_text();


    return 0;
}

int32_t cSDL_FontManager::replace_text(const char* const text,uint32_t replace_from,uint32_t replace_to,bool save_colored_bitmap)
{
    size_t length_text = strlen(text);
    uint32_t* main_buffor = (uint32_t*)malloc(length_text*4);

    for(size_t i = 0; i!=length_text; i++)
    {
        main_buffor[i] = (uint32_t)text[i];
    }
    int32_t return_value = this->replace_text(main_buffor,length_text,replace_from,replace_to,save_colored_bitmap);

    free(main_buffor);

    return return_value;


    return 0;
}


int32_t cSDL_FontManager::replace_text(const uint32_t* const text,size_t count_of_chars,uint32_t replace_from,uint32_t replace_to,bool save_colored_bitmap)
{
    if(this->_count_of_characters==0) return -1;   // if none of a characters are available (probably didn't used func "load_characters()";
    if(replace_from>replace_to) return -2;
    size_t max_index_in_actual_text_buffor;
    if(this->_count_main_text_buffor==0) return -2;
    else max_index_in_actual_text_buffor = this->_count_main_text_buffor-1;
    if(replace_from>max_index_in_actual_text_buffor) return -2;
    if(replace_to>max_index_in_actual_text_buffor) replace_to = max_index_in_actual_text_buffor;

    const size_t chars_to_delete = (replace_to-replace_from)+1;
    const size_t new_text_size = (this->_count_main_text_buffor+count_of_chars)-chars_to_delete;

    size_t max_end_of_deleting = replace_to+1;
    size_t length_of_endpart_of_text = this->_count_main_text_buffor-max_end_of_deleting;
    uint32_t* endpart_of_text = NULL;
    if(length_of_endpart_of_text!=0)
    {
        endpart_of_text = (uint32_t*)malloc(length_of_endpart_of_text*4);
        memcpy(endpart_of_text,&this->_main_text_buffor[max_end_of_deleting],length_of_endpart_of_text*4);  // copy endpart of actual text buffor;
    }


    if(new_text_size>this->_reserved_count_main_text_buffor)    // if more space needs to be reserved;
    {
        this->_reserved_count_main_text_buffor = new_text_size*2;
        uint32_t* new_buffor_main_text = (uint32_t*)malloc(this->_reserved_count_main_text_buffor*4);
        memcpy(new_buffor_main_text,this->_main_text_buffor,replace_from*4);
        free(this->_main_text_buffor);
        this->_main_text_buffor = new_buffor_main_text;

    }

    size_t index_for_colored_bitmap = 0;

    uint32_t* previous_letter_colors = NULL;
    uint32_t* previous_background_colors = NULL;

    if(save_colored_bitmap==1)
    {
        previous_letter_colors = (uint32_t*)malloc(this->_count_main_text_buffor*4);
        previous_background_colors = (uint32_t*)malloc(this->_count_main_text_buffor*4);


        for(; index_for_colored_bitmap!=replace_from; index_for_colored_bitmap++)
        {
            previous_letter_colors[index_for_colored_bitmap] = this->_text_characters[index_for_colored_bitmap].letter_color;
            previous_background_colors[index_for_colored_bitmap] = this->_text_characters[index_for_colored_bitmap].background_color;
        }

        for(size_t i = max_end_of_deleting; i!=this->_count_main_text_buffor; i++,index_for_colored_bitmap++) //endpart;
        {
            previous_letter_colors[index_for_colored_bitmap] = this->_text_characters[i].letter_color;
            previous_background_colors[index_for_colored_bitmap] = this->_text_characters[i].background_color;
        }
    }

    memcpy(&this->_main_text_buffor[replace_from],text,count_of_chars*4);   // paste new text;
    if(length_of_endpart_of_text!=0) memcpy(&this->_main_text_buffor[replace_from+count_of_chars],endpart_of_text,length_of_endpart_of_text*4);  // paste endpart;



    this->_count_main_text_buffor = new_text_size;

    this->_new_text_wraper(this->_main_text_buffor,this->_count_main_text_buffor);

    if(save_colored_bitmap==1)
    {
        size_t additional_index = 0;
        for(; additional_index!=replace_from; additional_index++)
        {
            this->_text_characters[additional_index].letter_color = previous_letter_colors[additional_index];
            this->_text_characters[additional_index].background_color = previous_background_colors[additional_index];
        }
        size_t beginning_of_second_loop = replace_from+count_of_chars;
        for(size_t i = replace_from; i!=beginning_of_second_loop; i++)
        {
            this->_text_characters[i].letter_color = this->_default_letters_color;
            this->_text_characters[i].background_color = this->_default_background_color;
        }

        for(size_t i = beginning_of_second_loop; i!=this->_count_main_text_buffor; additional_index++,i++)
        {
            this->_text_characters[i].letter_color = previous_letter_colors[additional_index];
            this->_text_characters[i].background_color = previous_background_colors[additional_index];
        }

        free(previous_letter_colors);
        free(previous_background_colors);
    }

    free(endpart_of_text);

    if(this->_color_mode==1) this->colorize_text();

    return 0;
}

void cSDL_FontManager::redraw_text(bool save_colored_bitmap)
{
    //if(this->_count_main_text_buffor==0) return;

    uint32_t* previous_letter_colors = NULL;
    uint32_t* previous_background_colors = NULL;

    if(save_colored_bitmap==1)
    {
        if(this->_count_text_characters!=0)
        {
            previous_letter_colors = (uint32_t*)malloc(this->_count_text_characters*4);
            previous_background_colors = (uint32_t*)malloc(this->_count_text_characters*4);

            for(size_t i = 0; i!=this->_count_text_characters; i++)
            {
                previous_letter_colors[i] = this->_text_characters[i].letter_color;
                previous_background_colors[i] = this->_text_characters[i].background_color;
            }
        }
    }

    this->_new_text_wraper(this->_main_text_buffor,this->_count_main_text_buffor);

    if(save_colored_bitmap==1)
    {

        for(size_t i = 0; i!=this->_count_text_characters; i++)
        {
            this->_text_characters[i].letter_color = previous_letter_colors[i];
            this->_text_characters[i].background_color = previous_background_colors[i];
        }

        free(previous_background_colors);
        free(previous_letter_colors);
    }

    if(this->_color_mode==1) this->colorize_text();

    return;
}



#ifdef CSDL_FONTMANAGER_WITH_SDL
SDL_Texture* cSDL_FontManager::get_pixels_in_texture(uint32_t* out_texture_w,uint32_t* out_texture_h)
{
    if(out_texture_w!=NULL) *out_texture_w = this->_largest_bitmap_w;
    if(out_texture_h!=NULL) *out_texture_h = this->_largest_bitmap_h;

    SDL_Rect texture_rect = {0,0,(int32_t)this->_total_updated_columns,(int32_t)this->_total_updated_rows};
    SDL_UpdateTexture(this->_main_texture,&texture_rect,this->_main_bitmap,this->_bitmap_w*4);

    return this->_main_texture;
}
#endif

uint32_t* cSDL_FontManager::get_pixels(uint32_t* updated_w_out,uint32_t* updated_h_out,uint32_t* updated_pitch_out)
{
    if(updated_w_out!=NULL) *updated_w_out = this->_total_updated_columns;
    if(updated_h_out!=NULL) *updated_h_out = this->_total_updated_rows;
    if(updated_pitch_out!=NULL) *updated_pitch_out = this->_bitmap_w*4;
    return this->_main_bitmap;
}


void cSDL_FontManager::colorize_text()
{
    if(this->_color_mode==0) return;

    for(size_t i = 0; i!=this->_total_updated_rows; i++)
    {
        memset(this->_main_bitmap+(i*this->_bitmap_w),0,this->_total_updated_columns*4);
    }
    if(this->_count_main_text_buffor==0) return;

    auto colorize_text_memset32 = [](uint32_t* dest,uint32_t value,size_t count)
    {
    #if defined(__GNUC__) || defined(__clang__)
        __asm__ volatile
        (
            "rep stosl"
            : "+c"(count),"+D"(dest)
            : "a"(value)
            : "memory"
        );
    #elif defined(_MSC_VER) && defined(_M_IX86)  // MSVC 32-bit
        __asm
        {
            mov edi,dest
            mov eax,value
            mov ecx,count
            rep stosl
        }
    #else
        for (size_t i = 0; i<count; i++) dest[i] = value;
    #endif
    };

    uint32_t visible_chars_counter = 0;
    for(size_t i = this->_first_index_of_visible_hitbox; visible_chars_counter!=this->_visible_hitbox_letters; i++)
    {
        if(this->_text_characters[i].hitbox_visible==1)    // background;
        {
            visible_chars_counter++;
            const size_t end_of_loop = this->_text_characters[i].start_y+this->_text_characters[i].y_height;
            for(size_t j = this->_text_characters[i].start_y; j!=end_of_loop; j++) colorize_text_memset32(&this->_main_bitmap[(j*this->_bitmap_w)+this->_text_characters[i].start_x],this->_text_characters[i].background_color,this->_text_characters[i].x_width);
        }
    }

    visible_chars_counter = 0;
    for(size_t i = this->_first_index_of_visible_letter; visible_chars_counter!=this->_visible_bitmap_letters; i++)
    {

        if(this->_text_characters[i].letter_visible==1) // letter;
        {
            visible_chars_counter++;
            const size_t end_of_loop_y = this->_text_characters[i].letter_y+this->_text_characters[i].letter_height;
            const size_t end_of_loop_x = this->_text_characters[i].letter_x+this->_text_characters[i].letter_width;
            const size_t width_of_original_bitmap = this->_text_characters[i].gliph_pointer->bitmap_width;

            const uint8_t letter_a = (uint8_t)(this->_text_characters[i].letter_color>>24);
            const uint8_t letter_r = (uint8_t)(this->_text_characters[i].letter_color>>16);
            const uint8_t letter_g = (uint8_t)(this->_text_characters[i].letter_color>>8);
            const uint8_t letter_b = (uint8_t)(this->_text_characters[i].letter_color);
            for(size_t j = this->_text_characters[i].letter_y,extra_y = 0; j!=end_of_loop_y; j++,extra_y++)
            {
                for(size_t k = this->_text_characters[i].letter_x,extra_x = 0; k!=end_of_loop_x; k++,extra_x++)
                {
                    const size_t index_in_original_bitmap = (width_of_original_bitmap*(extra_y+this->_text_characters[i].letter_cut_off_up))+extra_x+this->_text_characters[i].letter_cut_off_left;
                    const uint8_t original_visibility = (uint8_t)(this->_text_characters[i].gliph_pointer->bitmap[index_in_original_bitmap]>>24);
                    if(original_visibility!=0)
                    {
                        const size_t index_in_color_bitmap = (j*this->_bitmap_w)+k;

                        const uint8_t background_a = (uint8_t)(this->_main_bitmap[index_in_color_bitmap]>>24);
                        const uint8_t background_r = (uint8_t)(this->_main_bitmap[index_in_color_bitmap]>>16);
                        const uint8_t background_g = (uint8_t)(this->_main_bitmap[index_in_color_bitmap]>>8);
                        const uint8_t background_b = (uint8_t)(this->_main_bitmap[index_in_color_bitmap]);
/*

uint8_t A_letter = (letter_a * original_visibility + 127) / 255;  // 127 to zaokraglenie;
uint8_t A_background = 255 - A_letter;

uint8_t new_letter_r = (uint8_t)((letter_r * A_letter + background_r * A_background + 127) / 255);
uint8_t new_letter_g = (uint8_t)((letter_g * A_letter + background_g * A_background + 127) / 255);
uint8_t new_letter_b = (uint8_t)((letter_b * A_letter + background_b * A_background + 127) / 255);

uint8_t new_letter_a = (uint8_t)(A_letter + ((background_a * A_background + 127) / 255));*/


                        const uint8_t original_a_letter = (uint8_t)(((letter_a*original_visibility)+255)>>8);
                        const uint8_t original_a_background = (uint8_t)((background_a*(255-original_a_letter)+255)>>8);

                        const uint8_t new_letter_r = (uint8_t)(((letter_r*original_a_letter)+(background_r*original_a_background)+255)>>8);
                        const uint8_t new_letter_g = (uint8_t)(((letter_g*original_a_letter)+(background_g*original_a_background)+255)>>8);
                        const uint8_t new_letter_b = (uint8_t)(((letter_b*original_a_letter)+(background_b*original_a_background)+255)>>8);

                        const uint8_t new_letter_a = (uint8_t)(original_a_letter+original_a_background);


                        this->_main_bitmap[index_in_color_bitmap] = (new_letter_a<<24) | (new_letter_r<<16) | (new_letter_g<<8) | (new_letter_b);

                    }
                }
            }
        }


    }


    return;
}

void cSDL_FontManager::change_tab_width(uint32_t tab_width,bool interprete_width_as_spaces)
{
    this->_tab_width = tab_width;
    this->_treat_tab_width_as_spaces = interprete_width_as_spaces;
    return;
}

void cSDL_FontManager::switch_color_modes(bool enable)
{
    this->_color_mode = enable;
    return;
}


void cSDL_FontManager::change_bitmap_size(uint32_t new_w,uint32_t new_h)
{
    if(new_w==0) new_w = 1;
    if(new_h==0) new_h = 1;

    bool change_size = 0;

    if(this->_largest_bitmap_w<new_w)
    {
        this->_largest_bitmap_w = new_w;
        change_size = 1;
    }

    if(this->_largest_bitmap_h<new_h)
    {
        this->_largest_bitmap_h = new_h;
        change_size = 1;
    }

    const uint32_t old_bitmap_w = this->_bitmap_w;
    const uint32_t old_bitmap_h = this->_bitmap_h;

    this->_bitmap_w = new_w;
    this->_bitmap_h = new_h;



    if(change_size==1)
    {
        free(this->_main_bitmap);

    #ifdef CSDL_FONTMANAGER_WITH_SDL
        SDL_DestroyTexture(this->_main_texture);
        this->_main_texture = SDL_CreateTexture(this->_render,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,this->_largest_bitmap_w,this->_largest_bitmap_h);
        SDL_SetTextureBlendMode(this->_main_texture,SDL_BLENDMODE_BLEND);
    #endif

        this->_main_bitmap = (uint32_t*)calloc(this->_largest_bitmap_h*this->_largest_bitmap_w,sizeof(uint32_t));

        this->_total_updated_columns = 1;
        this->_total_updated_rows = 1;
    }
    else
    {
        //bool lower_w = 0;
        //bool lower_h = 0;
        //if(old_bitmap_w>this->_bitmap_w) lower_w = 1;
        //if(old_bitmap_h>this->_bitmap_h) lower_h = 1;
        /*if(lower_h==1||lower_w==1)
        {
            for(size_t i = 0; i!=old_bitmap_h; i++)
            {
                memset(this->_main_bitmap+(i*old_bitmap_w),0,old_bitmap_w*4);
            }
            SDL_Rect texture_rect = {0,0,(int32_t)old_bitmap_w,(int32_t)old_bitmap_h};
            SDL_UpdateTexture(this->_main_texture,&texture_rect,this->_main_bitmap,old_bitmap_w*4);
        }*/
        if(old_bitmap_w>this->_bitmap_w)
        {
            uint32_t difference_w = old_bitmap_w-this->_bitmap_w;
            for(size_t i = 0; i!=old_bitmap_h; i++)
            {
                memset(this->_main_bitmap+(i*old_bitmap_w)+this->_bitmap_w,0,difference_w*4);
            }
        #ifdef CSDL_FONTMANAGER_WITH_SDL
            SDL_Rect texture_rect = {(int32_t)this->_bitmap_w,0,(int32_t)difference_w,(int32_t)old_bitmap_h};
            SDL_UpdateTexture(this->_main_texture,&texture_rect,this->_main_bitmap+this->_bitmap_w,old_bitmap_w*4);
        #endif
        }
        if(old_bitmap_h>this->_bitmap_h)
        {
            uint32_t difference_h = old_bitmap_h-this->_bitmap_h;
            for(size_t i = this->_bitmap_h; i!=old_bitmap_h; i++)
            {
                memset(this->_main_bitmap+(i*old_bitmap_w),0,old_bitmap_w*4);
            }
        #ifdef CSDL_FONTMANAGER_WITH_SDL
            SDL_Rect texture_rect = {0,(int32_t)this->_bitmap_h,(int32_t)old_bitmap_w,(int32_t)difference_h};
            SDL_UpdateTexture(this->_main_texture,&texture_rect,this->_main_bitmap+(this->_bitmap_h*old_bitmap_w),old_bitmap_w*4);
        #endif

        }

        if(this->_bitmap_w>old_bitmap_w) this->_total_updated_columns = this->_bitmap_w;
        else if(this->_bitmap_w<this->_total_updated_columns) this->_total_updated_columns = this->_bitmap_w;

        if(this->_bitmap_h>old_bitmap_h) this->_total_updated_rows = this->_bitmap_h;
        else if(this->_bitmap_h<this->_total_updated_rows) this->_total_updated_rows = this->_bitmap_h;

    }


    return;
}

void cSDL_FontManager::change_bitmap_size_dynamically(int32_t new_w,int32_t new_h)
{
    int32_t bitmap_w = (int32_t)this->_bitmap_w;
    int32_t bitmap_h = (int32_t)this->_bitmap_h;
    bitmap_w+=new_w;
    bitmap_h+=new_h;
    if(bitmap_w<=0) bitmap_w = 1;
    if(bitmap_h<=0) bitmap_h = 1;

    this->change_bitmap_size(bitmap_w,bitmap_h);

    return;
}

bool cSDL_FontManager::get_words_wrap_up() const
{
    return this->_word_wrap;
}

void cSDL_FontManager::change_line_height(uint32_t new_line_height)
{
    this->_line_height = new_line_height;
    return;
}

void cSDL_FontManager::get_actual_viewpoint(cSDL_FontManager::ViewPoint* viewpoint) const
{
    if(viewpoint==NULL) return;
    viewpoint->interpret_y_as_line_height = this->_viewpoint.interpret_y_as_line_height;
    viewpoint->x = this->_viewpoint.x;
    viewpoint->y = this->_viewpoint.y;
    return;
}

void cSDL_FontManager::change_viewpoint(int32_t x,int32_t y)
{
    this->_viewpoint.x = x;
    this->_viewpoint.y = y;

    return;
}

void cSDL_FontManager::change_viewpoint_dynamically(int32_t x,int32_t y)
{
    this->_viewpoint.x+=x;
    this->_viewpoint.y+=y;

    return;
}

uint32_t cSDL_FontManager::get_total_lines_in_text() const
{
    return this->_total_lines_in_text;
}

bool cSDL_FontManager::get_text_draw_quality_mode() const
{
    return this->_quality_draw;
}

void cSDL_FontManager::change_text_draw_quality_mode(bool quality_mode)
{
    this->_quality_draw = quality_mode;
    return;
}

void cSDL_FontManager::change_interpretation_of_viepointY(bool interpret_y_as_whole_lines)
{
    this->_viewpoint.interpret_y_as_line_height = interpret_y_as_whole_lines;
    return;
}

void cSDL_FontManager::change_default_text_color(cSDL_FontManager::TextColor letters_color,cSDL_FontManager::TextColor background_color)
{
    this->_default_letters_color = (letters_color.a<<24) | (letters_color.r<<16) | (letters_color.g<<8) | (letters_color.b);
    this->_default_background_color = (background_color.a<<24) | (background_color.r<<16) | (background_color.g<<8) | (background_color.b);
    return;
}

void cSDL_FontManager::get_default_text_color(cSDL_FontManager::TextColor* letters_color,cSDL_FontManager::TextColor* background_color) const
{
    if(letters_color!=NULL)
    {
        letters_color->a = (uint8_t)(this->_default_letters_color>>24);
        letters_color->r = (uint8_t)(this->_default_letters_color>>16);
        letters_color->g = (uint8_t)(this->_default_letters_color>>8);
        letters_color->b = (uint8_t)(this->_default_letters_color);
    }
    if(background_color!=NULL)
    {
        background_color->a = (uint8_t)(this->_default_background_color>>24);
        background_color->r = (uint8_t)(this->_default_background_color>>16);
        background_color->g = (uint8_t)(this->_default_background_color>>8);
        background_color->b = (uint8_t)(this->_default_background_color);
    }
    return;
}

void cSDL_FontManager::change_text_color(uint32_t change_from,uint32_t change_to,cSDL_FontManager::TextColor letters,cSDL_FontManager::TextColor background)
{
    if(change_from>change_to) return;
    if(this->_count_text_characters==0) return;
    const size_t max_main_text_index = this->_count_text_characters-1;
    if(change_from>max_main_text_index) return;
    if(change_to>max_main_text_index) change_to = max_main_text_index;

    change_to++;
    const uint32_t color_letter = ((uint32_t)letters.a<<24) | ((uint32_t)letters.r<<16) | ((uint32_t)letters.g<<8) | ((uint32_t)letters.b);
    const uint32_t color_background = ((uint32_t)background.a<<24) | ((uint32_t)background.r<<16) | ((uint32_t)background.g<<8) | ((uint32_t)background.b);

    for(uint32_t i = change_from; i!=change_to; i++)
    {
        this->_text_characters[i].letter_color = color_letter;
        this->_text_characters[i].background_color = color_background;
    }

    return;
}

void cSDL_FontManager::change_text_color_dynamically(uint32_t change_from,uint32_t change_to,cSDL_FontManager::TextColor letters,cSDL_FontManager::TextColor background,bool add_or_subtract)
{
    if(change_from>change_to) return;
    if(this->_count_text_characters==0) return;
    const size_t max_main_text_index = this->_count_text_characters-1;
    if(change_from>max_main_text_index) return;
    if(change_to>max_main_text_index) change_to = max_main_text_index;

    change_to++;
    const uint32_t color_letter = ((uint32_t)letters.a<<24) | ((uint32_t)letters.r<<16) | ((uint32_t)letters.g<<8) | ((uint32_t)letters.b);
    const uint32_t color_background = ((uint32_t)background.a<<24) | ((uint32_t)background.r<<16) | ((uint32_t)background.g<<8) | ((uint32_t)background.b);

    for(uint32_t i = change_from; i!=change_to; i++)
    {
        if(add_or_subtract==0)
        {
            uint8_t a = (uint8_t)(this->_text_characters[i].letter_color>>24);
            uint8_t r = (uint8_t)(this->_text_characters[i].letter_color>>16);
            uint8_t g = (uint8_t)(this->_text_characters[i].letter_color>>8);
            uint8_t b = (uint8_t)(this->_text_characters[i].letter_color);
            uint32_t tmp = (uint32_t)a+(uint32_t)letters.a;
            a = (tmp>255) ? 255 : (uint8_t)tmp;
            tmp = (uint32_t)r+(uint32_t)letters.r;
            r = (tmp>255) ? 255 : (uint8_t)tmp;
            tmp = (uint32_t)g+(uint32_t)letters.g;
            g = (tmp>255) ? 255 : (uint8_t)tmp;
            tmp = (uint32_t)b+(uint32_t)letters.b;
            b = (tmp>255) ? 255 : (uint8_t)tmp;
            this->_text_characters[i].letter_color = ((uint32_t)a<<24) | ((uint32_t)r<<16) | ((uint32_t)g<<8) | ((uint32_t)b);

            a = (uint8_t)(this->_text_characters[i].background_color>>24);
            r = (uint8_t)(this->_text_characters[i].background_color>>16);
            g = (uint8_t)(this->_text_characters[i].background_color>>8);
            b = (uint8_t)(this->_text_characters[i].background_color);
            tmp = (uint32_t)a+(uint32_t)background.a;
            a = (tmp>255) ? 255 : (uint8_t)tmp;
            tmp = (uint32_t)r+(uint32_t)background.r;
            r = (tmp>255) ? 255 : (uint8_t)tmp;
            tmp = (uint32_t)g+(uint32_t)background.g;
            g = (tmp>255) ? 255 : (uint8_t)tmp;
            tmp = (uint32_t)b+(uint32_t)background.b;
            b = (tmp>255) ? 255 : (uint8_t)tmp;
            this->_text_characters[i].background_color = ((uint32_t)a<<24) | ((uint32_t)r<<16) | ((uint32_t)g<<8) | ((uint32_t)b);

        }
        else
        {
            uint8_t a = (uint8_t)(this->_text_characters[i].letter_color>>24);
            uint8_t r = (uint8_t)(this->_text_characters[i].letter_color>>16);
            uint8_t g = (uint8_t)(this->_text_characters[i].letter_color>>8);
            uint8_t b = (uint8_t)(this->_text_characters[i].letter_color);
            int32_t tmp = (int32_t)a-(int32_t)letters.a;
            a = (tmp<0) ? 0 : (uint8_t)tmp;
            tmp = (int32_t)r-(int32_t)letters.r;
            r = (tmp<0) ? 0 : (uint8_t)tmp;
            tmp = (int32_t)g-(int32_t)letters.g;
            g = (tmp<0) ? 0 : (uint8_t)tmp;
            tmp = (int32_t)b-(int32_t)letters.b;
            b = (tmp<0) ? 0 : (uint8_t)tmp;
            this->_text_characters[i].letter_color = ((uint32_t)a<<24) | ((uint32_t)r<<16) | ((uint32_t)g<<8) | ((uint32_t)b);

            a = (uint8_t)(this->_text_characters[i].background_color>>24);
            r = (uint8_t)(this->_text_characters[i].background_color>>16);
            g = (uint8_t)(this->_text_characters[i].background_color>>8);
            b = (uint8_t)(this->_text_characters[i].background_color);
            tmp = (int32_t)a-(int32_t)background.a;
            a = (tmp<0) ? 0 : (uint8_t)tmp;
            tmp = (int32_t)r-(int32_t)background.r;
            r = (tmp<0) ? 0 : (uint8_t)tmp;
            tmp = (int32_t)g-(int32_t)background.g;
            g = (tmp<0) ? 0 : (uint8_t)tmp;
            tmp = (int32_t)b-(int32_t)background.b;
            b = (tmp<0) ? 0 : (uint8_t)tmp;
            this->_text_characters[i].background_color = ((uint32_t)a<<24) | ((uint32_t)r<<16) | ((uint32_t)g<<8) | ((uint32_t)b);
        }
    }

    return;
}

size_t cSDL_FontManager::get_count_of_chars_in_text() const
{
    return this->_count_main_text_buffor;
}

size_t cSDL_FontManager::get_loaded_characters(cSDL_FontManager::Character*& characters) const
{
    characters = this->_characters;
    return this->_count_of_characters;
}

size_t cSDL_FontManager::get_information_chars_in_text(cSDL_FontManager::TextCharacter*& text_characters_pointer,uint32_t* first_index_for_visible_letter,uint32_t* first_index_for_visible_hitbox)
{
    text_characters_pointer = this->_text_characters;
    if(first_index_for_visible_letter!=NULL) *first_index_for_visible_letter = this->_first_index_of_visible_letter;
    if(first_index_for_visible_hitbox!=NULL) *first_index_for_visible_hitbox = this->_first_index_of_visible_hitbox;
    return this->_count_text_characters;
}

int32_t cSDL_FontManager::find_viewpoint_x_for_max_horizontal_scroll() const
{
    int32_t result = 0;

    int32_t right_side = 0;
    int32_t max_right_site = 0;

    for(size_t i = 0; i!=this->_count_text_characters; i++)
    {
        if(this->_text_characters[i].gliph_pointer->unicode==(uint32_t)'\n')
        {
            right_side = 0;
            continue;
        }

        right_side+=this->_text_characters[i].gliph_pointer->advance;
        if(right_side<0) right_side = 0;    // it's needed for compability reasons;
        if(right_side>max_right_site)
        {
            max_right_site = right_side;
        }

    }

    if(max_right_site<=(int32_t)this->_bitmap_w) return 0;

    result = (max_right_site-this->_bitmap_w)*-1;

    return result;
}

int32_t cSDL_FontManager::find_viewpoint_y_for_max_vertical_scroll() const
{
    int32_t result = 0;

    uint32_t total_height = this->_line_height*this->_total_lines_in_text;

    if(total_height<=this->_bitmap_h) return 0;

    result = (total_height-this->_bitmap_h)*-1;

    return result;
}

uint32_t cSDL_FontManager::get_line_height() const
{
    return this->_line_height;
}

void cSDL_FontManager::get_visible_letters_count_from_text(uint32_t* visible_hitboxes,uint32_t* visible_letters) const
{
    if(visible_hitboxes!=NULL) *visible_hitboxes = this->_visible_hitbox_letters;
    if(visible_letters!=NULL) *visible_letters = this->_visible_bitmap_letters;
    return;
}

void cSDL_FontManager::change_offset_of_bitmaps(int32_t offset_right,int32_t offset_up)
{
    this->_extra_offset_right = offset_right;
    this->_extra_offset_up = offset_up;
    return;
}


cSDL_FontManager::~cSDL_FontManager()
{
    this->_free_class(*this);

    return;
}

void cSDL_FontManager::_new_text_wraper(const uint32_t* const text,size_t lenght)
{
    if(this->_reserved_count_text_characters<lenght)
    {
        free(this->_text_characters);   // _text_characters can be NULL; it's legal for function free();
        this->_reserved_count_text_characters = lenght*2;
        this->_text_characters = (TextCharacter*)malloc(this->_reserved_count_text_characters*sizeof(TextCharacter));
    }
    this->_count_text_characters = lenght;

    for(size_t i = 0; i!=this->_total_updated_rows; i++)
    {
        memset(this->_main_bitmap+(i*this->_bitmap_w),0,this->_total_updated_columns*4);
    }

    uint32_t left_x = 0;
    int32_t right_x = 0;   // can be negative so watch out;
    uint32_t up_y = 0;
    uint32_t max_update_x = 0;          //
    uint32_t max_update_y = 0;          // those variables doesn't need to be precise; but needs to be a little larger than a little shorter;
    //if(this->_max_height>this->_line_height) max_update_y = this->_max_height;  // if custom _line_height was used;
    //else max_update_y = this->_line_height;
    const uint32_t max_full_height = (this->_max_height>this->_line_height) ? this->_max_height : this->_line_height; // if custom _line_height was used;
    uint32_t line = 0;
    bool skip_drawing = 0;

    bool viewpoint_custom = 0;
    uint32_t viewpoint_y = 0;
    uint32_t viewpoint_x = 0;
    if(this->_viewpoint.y!=0)
    {
        if(this->_viewpoint.interpret_y_as_line_height==1) this->_viewpoint.y*=this->_line_height;
        if(this->_viewpoint.y<0) viewpoint_y = this->_viewpoint.y*-1;
        else viewpoint_y = this->_viewpoint.y;
        viewpoint_custom = 1;
    }
    if(this->_viewpoint.x!=0)
    {
        if(this->_viewpoint.x<0) viewpoint_x = this->_viewpoint.x*-1;
        else viewpoint_x = this->_viewpoint.x;
        viewpoint_custom = 1;
    }
    uint32_t viewpoint_bitmap_height_cut_off = 0;
    uint32_t viewpoint_up_y_height_cut_off = 0;
    uint32_t original_y_up_for_viewpoint = 0;
    uint32_t viewpoint_bitmap_width_cut_off = 0;
    int32_t viewpoint_left_x_width_cut_off = 0;
    int32_t original_x_right_for_viewpoint = 0;

    uint32_t positive_extra_offset_right = 0;
    if(this->_extra_offset_right>0) positive_extra_offset_right = this->_extra_offset_right;

    this->_visible_hitbox_letters = 0;
    this->_visible_bitmap_letters = 0;
    this->_first_index_of_visible_hitbox = 0;
    this->_first_index_of_visible_letter = 0;
    bool is_first_index_of_visible_letter_taken = 0;
    bool is_first_index_of_visible_hitbox_taken = 0;

//printf("|");
    for(size_t i = 0; i!=lenght; i++)
    {
// load char:
//--------------------------------------------------------------------------------------------------------------
        if(text[i]==(uint32_t)'\n') this->_text_characters[i].gliph_pointer = &this->_extra_enter_character;
        else if(text[i]==(uint32_t)'\t') this->_text_characters[i].gliph_pointer = &this->_characters[this->_tab_index_inside_characters];  // this is form of optymalization 'cause '\t' is always at the end of "_characters";
        else
        {
            size_t j;
            for(j = 0; j!=this->_count_of_characters; j++)
            {
                if(this->_characters[j].unicode==text[i])
                {
                    this->_text_characters[i].gliph_pointer = &this->_characters[j];
                    break;
                }
            }
            if(j==this->_count_of_characters) this->_text_characters[i].gliph_pointer = &this->_characters[0];
        }

        this->_text_characters[i].hitbox_visible = 0;
        this->_text_characters[i].letter_visible = 0;
        this->_text_characters[i].background_color = this->_default_background_color;   // this variable must be reseted after this function so that's why it's here;
        this->_text_characters[i].letter_color = this->_default_letters_color;          // this variable must be reseted after this function so that's why it's here;
//--------------------------------------------------------------------------------------------------------------


// draw char:
//-------------------------------------------------------------------------------------------------------------
        if(this->_text_characters[i].gliph_pointer->unicode==(uint32_t)'\n')
        {
            up_y+=this->_line_height;
            original_y_up_for_viewpoint+=this->_line_height;
            //if(this->_max_height>this->_line_height) max_update_y+=this->_max_height; // if custom _line_height was used;
            //else max_update_y+=this->_line_height;
            left_x = 0; original_x_right_for_viewpoint = 0;
            line++;

            if(up_y<this->_bitmap_h) skip_drawing = 0;  // optymalization;
            else if(this->_viewpoint.y<0)   // for fixing bugs reason;
            {
                if(viewpoint_y<=original_y_up_for_viewpoint)
                {
                    if((original_y_up_for_viewpoint-viewpoint_y)<this->_bitmap_h) skip_drawing = 0;
                }
                else if((viewpoint_y-original_y_up_for_viewpoint)<this->_line_height) skip_drawing = 0;
            }
            continue;
        }
        if(skip_drawing==1) continue;


// calculating bitmap_y & bitmap_x & right_x & max_update_x;
        int32_t bitmap_y = (up_y+this->_max_y)-this->_text_characters[i].gliph_pointer->offset_up+this->_extra_offset_up;
        if(bitmap_y<0) bitmap_y = 0; // "offset_up" may be bigger than "_max_y" so to be sure;

        int32_t bitmap_x;
        if(left_x==0) bitmap_x = 0+positive_extra_offset_right;
        else
        {
            bitmap_x = left_x+this->_text_characters[i].gliph_pointer->offset_right+this->_extra_offset_right;
            if(bitmap_x<0) bitmap_x = 0; // can be negative 'cause of "offset_right" so watch out;
        }
        right_x = left_x+this->_text_characters[i].gliph_pointer->advance;
        if(right_x<0) right_x = 0;  //  can be negative so to be sure;
        original_x_right_for_viewpoint+=this->_text_characters[i].gliph_pointer->advance;
        if(original_x_right_for_viewpoint<0) original_x_right_for_viewpoint = 0;

        const uint32_t max_bitmap_right =  bitmap_x+this->_text_characters[i].gliph_pointer->bitmap_width;
        const uint32_t add_to_max_update = ((int32_t)max_bitmap_right>right_x) ? max_bitmap_right-right_x : 0;
        if(max_update_x<(right_x+add_to_max_update)) max_update_x = right_x+add_to_max_update;









// if pixels + free space is beyond available bitmap width and word_wrap is turned ON then move char to line below;
        if((right_x>(int32_t)this->_bitmap_w)&&this->_word_wrap==1)
        {
            left_x = 0;
            right_x = this->_text_characters[i].gliph_pointer->advance;
            if(right_x<0) right_x = 0;  //  can be negative so to be sure;
            original_x_right_for_viewpoint = right_x;

            up_y+=this->_line_height;
            original_y_up_for_viewpoint+=this->_line_height;
            //if(this->_max_height>this->_line_height) max_update_y+=this->_max_height; // if custom _line_height was used;
            //else max_update_y+=this->_line_height;
            bitmap_y = (up_y+this->_max_y)-this->_text_characters[i].gliph_pointer->offset_up+this->_extra_offset_up;
            if(bitmap_y<0) bitmap_y = 0; // "offset_up" may be larger than "_max_y" so to be sure;
            bitmap_x = 0+positive_extra_offset_right;
            line++;
        }

        max_update_y = up_y+max_full_height;


        if(viewpoint_custom!=0)
        {
            if(this->_viewpoint.y<0)
            {

            // calculate if bitmap or part of it should be cut off and bitmap y;
                bitmap_y = (original_y_up_for_viewpoint+this->_max_y)-this->_text_characters[i].gliph_pointer->offset_up+this->_extra_offset_up;
                if(bitmap_y<0) bitmap_y = 0; // "offset_up" may be bigger than "_max_y" so to be sure;
                const uint32_t max_bitmap_y = bitmap_y+this->_text_characters[i].gliph_pointer->bitmap_height;
                if((int32_t)viewpoint_y>bitmap_y)
                {
                    if(viewpoint_y>=max_bitmap_y) viewpoint_bitmap_height_cut_off = this->_text_characters[i].gliph_pointer->bitmap_height;
                    else viewpoint_bitmap_height_cut_off = viewpoint_y-bitmap_y;
                    bitmap_y = 0;
                }
                else
                {
                    viewpoint_bitmap_height_cut_off = 0;
                    bitmap_y-=viewpoint_y;

                }

            // calculate "up_y" and height of "up_y";
                if(viewpoint_y>original_y_up_for_viewpoint)
                {
                    viewpoint_up_y_height_cut_off = viewpoint_y-original_y_up_for_viewpoint;
                    up_y = 0;
                    if(viewpoint_up_y_height_cut_off>this->_line_height) viewpoint_up_y_height_cut_off = this->_line_height;
                }
                else
                {
                    viewpoint_up_y_height_cut_off = 0;
                    up_y = original_y_up_for_viewpoint-viewpoint_y;
                }

                max_update_y = up_y+max_full_height;

            // if text is beyond visible part;
                if(viewpoint_up_y_height_cut_off==this->_line_height&&viewpoint_bitmap_height_cut_off==this->_text_characters[i].gliph_pointer->bitmap_height)
                {
                    if(this->_viewpoint.x==0)
                    {
                        left_x = right_x;   // for checking if "_word_wrap" forces new line;
                        continue;
                    }

                }
            }
            else if(this->_viewpoint.y>0)
            {
                up_y = viewpoint_y+original_y_up_for_viewpoint;
                bitmap_y = (up_y+this->_max_y)-this->_text_characters[i].gliph_pointer->offset_up+this->_extra_offset_up;
                if(bitmap_y<0) bitmap_y = 0; // "offset_up" may be bigger than "_max_y" so to be sure;
                max_update_y = up_y*max_full_height;

            }

            if(this->_viewpoint.x<0)
            {
                int32_t original_left_x = original_x_right_for_viewpoint-this->_text_characters[i].gliph_pointer->advance;
                if(original_left_x<0)
                {
                    //if((original_left_x*-1)>(int32_t)positive_extra_offset_right) positive_extra_offset_right = 0;
                    //else positive_extra_offset_right-=(original_left_x*-1);
                    original_left_x = 0;
                }

            // calculate if bitmap or part of it should be cut off and bitmap x;
                if(original_left_x==0) bitmap_x = 0+positive_extra_offset_right;
                else
                {
                    bitmap_x = original_left_x+this->_text_characters[i].gliph_pointer->offset_right+this->_extra_offset_right;
                    if(bitmap_x<0) bitmap_x = 0; // can be negative 'cause of "offset_right" so watch out;
                }

                const uint32_t max_bitmap_x = bitmap_x+this->_text_characters[i].gliph_pointer->bitmap_width;
                if((int32_t)viewpoint_x>bitmap_x)
                {
                    if(viewpoint_x>=max_bitmap_x) viewpoint_bitmap_width_cut_off = this->_text_characters[i].gliph_pointer->bitmap_width;
                    else viewpoint_bitmap_width_cut_off = viewpoint_x-bitmap_x;
                    bitmap_x = 0;
                }
                else
                {

                    viewpoint_bitmap_width_cut_off = 0;
                    bitmap_x-=viewpoint_x;
                }
                // viewpoint_bitmap_width_cut_off is setted up now;

            // calculate "left_x" and width of "right_x";
                if((int32_t)viewpoint_x>original_left_x)
                {
                    left_x = 0;
                    viewpoint_left_x_width_cut_off = viewpoint_x-original_left_x;
                    if(viewpoint_left_x_width_cut_off>this->_text_characters[i].gliph_pointer->advance)
                    {
                        viewpoint_left_x_width_cut_off = this->_text_characters[i].gliph_pointer->advance;
                        if(viewpoint_left_x_width_cut_off<0) viewpoint_left_x_width_cut_off = 0;
                        right_x = 0;
                    }
                    else
                    {
                        if(this->_text_characters[i].gliph_pointer->advance<0) right_x = 0;
                        else right_x = this->_text_characters[i].gliph_pointer->advance-viewpoint_left_x_width_cut_off;
                    }


                }
                else
                {
                    viewpoint_left_x_width_cut_off = 0;
                    left_x = original_left_x-viewpoint_x;
                    right_x = left_x+this->_text_characters[i].gliph_pointer->advance;
                    if(right_x<0) right_x = 0;  // advance can be negative so to be sure;
                }

            // if text is beyond visible part;
                if(right_x==0&&viewpoint_bitmap_width_cut_off==this->_text_characters[i].gliph_pointer->bitmap_width)
                {
                    left_x = right_x;
                    continue;
                }
            }
            else if(this->_viewpoint.x>0)
            {
                int32_t original_left_x = original_x_right_for_viewpoint-this->_text_characters[i].gliph_pointer->advance;
                if(original_left_x<0) original_left_x = 0;

                left_x = original_left_x+viewpoint_x;
                right_x = original_x_right_for_viewpoint+viewpoint_x;
                if(original_left_x==0) bitmap_x = 0+viewpoint_x+positive_extra_offset_right;
                else
                {
                    bitmap_x = original_left_x+this->_text_characters[i].gliph_pointer->offset_right+viewpoint_x+this->_extra_offset_right;
                    if(bitmap_x<0) bitmap_x = 0; // can be negative 'cause of "offset_right" so watch out;
                }

                const uint32_t _max_bitmap_right =  bitmap_x+this->_text_characters[i].gliph_pointer->bitmap_width;
                const uint32_t _add_to_max_update = ((int32_t)_max_bitmap_right>right_x) ? _max_bitmap_right-right_x : 0;
                if(max_update_x<(right_x+_add_to_max_update)) max_update_x = right_x+_add_to_max_update;
            }
        }


// if there is no pixels to draw & not space for backround operations then skip;
        bool hitbox_visibility = 1;
        if(left_x>=this->_bitmap_w)
        {
            if((uint32_t)bitmap_x>=_bitmap_w)
            {
                skip_drawing = 1;   // optymalization;
                continue;
            }
            else    // if bitmap is visible but hitbox not;
            {
                hitbox_visibility = 0;
            }

        }
        if(up_y>=this->_bitmap_h)
        {
            if((uint32_t)bitmap_y>=_bitmap_h)
            {
                skip_drawing = 1;   // optymalization;
                continue;
            }
            else    // if bitmap is visible but hitbox not;
            {
                hitbox_visibility = 0;
            }
        }


        if(hitbox_visibility==1)    // this section is for backround operations;
        {
        // setting up structure for background operations:
            this->_text_characters[i].hitbox_visible = 1;
            this->_text_characters[i].start_x = left_x;
            if(this->_text_characters[i].gliph_pointer->advance<0) this->_text_characters[i].x_width = 0;  // advance can be negative so to be sure;
            else this->_text_characters[i].x_width = this->_text_characters[i].gliph_pointer->advance-viewpoint_left_x_width_cut_off;
            if(left_x+this->_text_characters[i].x_width>this->_bitmap_w)
            {
                uint32_t difference = (left_x+this->_text_characters[i].x_width)-this->_bitmap_w;
                this->_text_characters[i].x_width-=difference;
            }
            this->_text_characters[i].start_y = up_y;
            this->_text_characters[i].y_height = this->_line_height-viewpoint_up_y_height_cut_off;
            if(up_y+this->_text_characters[i].y_height>this->_bitmap_h)
            {
                uint32_t difference = (up_y+this->_text_characters[i].y_height)-this->_bitmap_h;
                this->_text_characters[i].y_height-=difference;
            }
            this->_text_characters[i].line = line;
            //this->_text_characters[i].background_color = this->_default_background_color;
            this->_visible_hitbox_letters++;
            if(is_first_index_of_visible_hitbox_taken==0)
            {
                this->_first_index_of_visible_hitbox = i;
                is_first_index_of_visible_hitbox_taken = 1;
            }
        }



// if there is no pixels to draw then skip;
        if((uint32_t)bitmap_y>=this->_bitmap_h||(uint32_t)bitmap_x>=this->_bitmap_w)
        {
            left_x = right_x;
            continue;
        }



// draw bitmap;
        uint32_t draw_to_max_y = bitmap_y+(this->_text_characters[i].gliph_pointer->bitmap_height-viewpoint_bitmap_height_cut_off);
        if(draw_to_max_y>this->_bitmap_h) draw_to_max_y = this->_bitmap_h;
        //uint32_t draw_to_max_x = bitmap_x+this->_text_characters[i].gliph_pointer->bitmap_width;
        //if(draw_to_max_x>this->_bitmap_w) draw_to_max_x = this->_bitmap_w;

        uint32_t copy_width;
        const uint32_t character_bitmap_width = this->_text_characters[i].gliph_pointer->bitmap_width-viewpoint_bitmap_width_cut_off;
        if((bitmap_x+character_bitmap_width)>this->_bitmap_w)
        {
            const uint32_t difference = this->_bitmap_w-bitmap_x; // bitmap_x can't be larger than left_x so it's safe;
            copy_width = difference*4;

        }
        else copy_width = character_bitmap_width*4;


        this->_text_characters[i].letter_visible = 1;
        //this->_text_characters[i].letter_color = this->_default_letters_color;
        this->_text_characters[i].letter_x = (uint32_t)bitmap_x;
        this->_text_characters[i].letter_y = (uint32_t)bitmap_y;
        this->_text_characters[i].letter_width = copy_width/4;
        this->_text_characters[i].letter_height = (uint32_t)draw_to_max_y-bitmap_y;
        this->_text_characters[i].letter_cut_off_left = viewpoint_bitmap_width_cut_off;
        this->_text_characters[i].letter_cut_off_up = viewpoint_bitmap_height_cut_off;
        this->_text_characters[i].line = line;
        this->_visible_bitmap_letters++;
        if(is_first_index_of_visible_letter_taken==0)
        {
            this->_first_index_of_visible_letter = i;
            is_first_index_of_visible_letter_taken = 1;
        }

        if(this->_color_mode==0)
        {
            if(this->_quality_draw==0)
            {
                for(uint32_t k = bitmap_y,l = viewpoint_bitmap_height_cut_off; k!=draw_to_max_y; k++,l++) memcpy(this->_main_bitmap+(k*this->_bitmap_w)+bitmap_x,this->_text_characters[i].gliph_pointer->bitmap+(this->_text_characters[i].gliph_pointer->bitmap_width*l)+viewpoint_bitmap_width_cut_off,copy_width);
            }
            else
            {
                copy_width/=4;
                for(uint32_t k = bitmap_y,l = viewpoint_bitmap_height_cut_off; k!=draw_to_max_y; k++,l++)
                {
                    for(uint32_t m = 0; m!=copy_width; m++)
                    {
                        const uint32_t copy_from_index = (this->_text_characters[i].gliph_pointer->bitmap_width*l)+m+viewpoint_bitmap_width_cut_off;
                        const uint32_t char_from_index = this->_text_characters[i].gliph_pointer->bitmap[copy_from_index];
                        const uint32_t copy_to_index = (k*this->_bitmap_w)+bitmap_x+m;
                        const uint32_t char_to_index = this->_main_bitmap[copy_to_index];

                        if((char_from_index&0xFF000000)>(char_to_index&0xFF000000))
                        {
                            this->_main_bitmap[copy_to_index] = char_from_index;
                        }

                    }
                }
            }
        }




        left_x = right_x;
//--------------------------------------------------------------------------------------------------------------
    }

    if(max_update_x>(this->_total_updated_columns-1))
    {
        if(max_update_x>(this->_bitmap_w-1)) this->_total_updated_columns = this->_bitmap_w;
        else this->_total_updated_columns = max_update_x+1;
    }
    if(max_update_y>(this->_total_updated_rows-1))
    {
        if(max_update_y>(this->_bitmap_h-1)) this->_total_updated_rows = this->_bitmap_h;
        else this->_total_updated_rows = max_update_y+1;
    }
    this->_total_lines_in_text = line+1;

    return;
}

void cSDL_FontManager::_free_class(cSDL_FontManager& object)
{
    if(object._destroy_class==1)
    {
        if(object._text_characters!=NULL) free(object._text_characters);
        if(object._main_text_buffor!=NULL) free(object._main_text_buffor);
        if(object._main_bitmap!=NULL) free(object._main_bitmap);
    #ifdef CSDL_FONTMANAGER_WITH_SDL
        if(object._main_texture!=NULL) SDL_DestroyTexture(object._main_texture);
    #endif
        if(object._characters!=NULL)
        {
            for(uint32_t i = 0; i!=object._count_of_characters; i++) free(object._characters[i].bitmap);  // it's safe to call free() at NULL pointers;
            free(object._characters);
        }
        if(object._external_font==0)
        {
            if(object._font!=NULL) FT_Done_Face(object._font);
        }
        if(object._external_library==0)
        {
            if(object._library!=NULL) FT_Done_FreeType(object._library);
        }
        object._destroy_class = 0;
    }

    return;
}

void cSDL_FontManager::_copy_objects_of_class(const cSDL_FontManager& other_object)
{
    this->_error_code = other_object._error_code;
    if(this->_error_code!=0) {this->_destroy_class = 0; return;}
    this->_destroy_class = 1;
    this->_library = NULL;
    this->_font = NULL;

    if(other_object._external_library==0)
    {
        this->_external_library = 0;
        this->_error_code = FT_Init_FreeType(&this->_library);
        if(this->_error_code!=0) {this->_destroy_class = 0; return;}
    }
    else
    {
        this->_external_library = 1;
        this->_library = other_object._library;
    }

    if(other_object._external_font==0)
    {
        this->_external_font = 0;
        this->_filename_for_constructor = other_object._filename_for_constructor;
        this->_style_index = other_object._style_index;
        this->_error_code = FT_New_Face(this->_library,this->_filename_for_constructor,this->_style_index,&this->_font);
        if(this->_error_code!=0)
        {
            if(this->_external_library==0) FT_Done_FreeType(this->_library);
            this->_destroy_class = 0;
            return;
        }
    }
    else
    {
        this->_external_font = 1;
        this->_font = other_object._font;
        this->_filename_for_constructor = NULL;
        this->_style_index = 0;
    }

// if sdl is setted up:
//----------------------------------------------------
 #ifdef CSDL_FONTMANAGER_WITH_SDL
    this->_render = other_object._render;
    this->_main_texture = SDL_CreateTexture(this->_render,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,other_object._largest_bitmap_w,other_object._largest_bitmap_h);
    SDL_SetTextureBlendMode(this->_main_texture,SDL_BLENDMODE_BLEND);
    SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = other_object._total_updated_columns; rect.h = other_object._total_updated_rows;
    SDL_UpdateTexture(this->_main_texture,&rect,other_object._main_bitmap,other_object._bitmap_w*4);
#endif
//----------------------------------------------------
    this->_main_bitmap = (uint32_t*)calloc(other_object._largest_bitmap_h*other_object._largest_bitmap_w,sizeof(uint32_t));
    for(size_t i = 0; i!=other_object._total_updated_rows; i++)
    {
        memcpy(this->_main_bitmap+(i*other_object._bitmap_w),other_object._main_bitmap+(i*other_object._bitmap_w),other_object._total_updated_columns*4);
    }
    this->_bitmap_w = other_object._bitmap_w;
    this->_bitmap_h = other_object._bitmap_h;
    this->_largest_bitmap_w = other_object._largest_bitmap_w;
    this->_largest_bitmap_h = other_object._largest_bitmap_h;
    this->_default_letters_color = other_object._default_letters_color;
    this->_default_background_color = other_object._default_background_color;
    this->_extra_offset_right = other_object._extra_offset_right;
    this->_extra_offset_up = other_object._extra_offset_up;

    if(other_object._reserved_count_main_text_buffor!=0)
    {
        this->_main_text_buffor = (uint32_t*)malloc(other_object._reserved_count_main_text_buffor*4);
        memcpy(this->_main_text_buffor,other_object._main_text_buffor,other_object._count_main_text_buffor*4);
    }
    else this->_main_text_buffor = NULL;
    this->_count_main_text_buffor = other_object._count_main_text_buffor;
    this->_reserved_count_main_text_buffor = other_object._reserved_count_main_text_buffor;
    if(other_object._reserved_count_text_characters!=0)
    {
        this->_text_characters = (TextCharacter*)malloc(other_object._reserved_count_text_characters*sizeof(TextCharacter));
        memcpy(this->_text_characters,other_object._text_characters,other_object._count_text_characters*sizeof(TextCharacter));
    }
    else this->_text_characters = NULL;
    this->_count_text_characters = other_object._count_text_characters;
    this->_reserved_count_text_characters = other_object._reserved_count_text_characters;
    // u have to setup glyph_pointer of "_text_characters" later;

    this->_word_wrap = other_object._word_wrap;
    this->_total_updated_columns = other_object._total_updated_columns;
    this->_total_updated_rows = other_object._total_updated_rows;

    this->_viewpoint = other_object._viewpoint;
    this->_total_lines_in_text = other_object._total_lines_in_text;
    this->_visible_bitmap_letters = other_object._visible_bitmap_letters;
    this->_visible_hitbox_letters = other_object._visible_hitbox_letters;
    this->_first_index_of_visible_hitbox = other_object._first_index_of_visible_hitbox;
    this->_first_index_of_visible_letter = other_object._first_index_of_visible_letter;
    this->_quality_draw = other_object._quality_draw;
    this->_color_mode = other_object._color_mode;
    this->_tab_width = other_object._tab_width;
    this->_treat_tab_width_as_spaces = other_object._treat_tab_width_as_spaces;
    this->_tab_index_inside_characters = other_object._tab_index_inside_characters;

    this->_count_of_characters = other_object._count_of_characters;
    if(this->_count_of_characters!=0)
    {
        this->_characters = (Character*)malloc(other_object._count_of_characters*sizeof(Character));
        memcpy(this->_characters,other_object._characters,this->_count_of_characters*sizeof(Character));
        for(size_t i = 0; i!=this->_count_of_characters; i++)
        {
            this->_characters[i].bitmap = (uint32_t*)malloc(this->_characters[i].bitmap_width*this->_characters[i].bitmap_height*4);
            for(size_t j = 0; j!=this->_characters[i].bitmap_height; j++)
            {
                memcpy(this->_characters[i].bitmap+(j*this->_characters[i].bitmap_width),other_object._characters[i].bitmap+(j*this->_characters[i].bitmap_width),this->_characters[i].bitmap_width*4);
            }
        }
    }
    else this->_characters = NULL;

    this->_extra_enter_character = other_object._extra_enter_character;

    this->_max_height = other_object._max_height;
    this->_max_y = other_object._max_y;
    this->_line_height = other_object._line_height;

// setup of "glyph_pointer" from "_text_characters";
    if(this->_count_of_characters!=0)
    {
        for(size_t i = 0; i!=this->_count_main_text_buffor; i++)
        {
            if(this->_main_text_buffor[i]==(uint32_t)'\n') this->_text_characters[i].gliph_pointer = &this->_extra_enter_character;
            else if(this->_main_text_buffor[i]==(uint32_t)'\t') this->_text_characters[i].gliph_pointer = &this->_characters[this->_tab_index_inside_characters];
            else
            {
                size_t j;
                for(j = 0; j!=this->_count_of_characters; j++)
                {
                    if(this->_characters[j].unicode==this->_main_text_buffor[i])
                    {
                        this->_text_characters[i].gliph_pointer = &this->_characters[j];
                        break;
                    }
                }
                if(j==this->_count_of_characters) this->_text_characters[i].gliph_pointer = &this->_characters[0];
            }
        }
    }

    return;
}

void cSDL_FontManager::_move_object_of_class(cSDL_FontManager& other_object)
{
    this->_error_code = other_object._error_code;
    if(this->_error_code!=0) {this->_destroy_class = 0; return;}
    other_object._destroy_class = 0;
    this->_destroy_class = 1;

    this->_filename_for_constructor = other_object._filename_for_constructor;
    this->_style_index = other_object._style_index;
    this->_external_library = other_object._external_library;
    this->_external_font = other_object._external_font;
    this->_library = other_object._library;
    this->_font = other_object._font;
// only for SDL version
//----------------------------------------------------------------------------------------
 #ifdef CSDL_FONTMANAGER_WITH_SDL
    this->_render = other_object._render;
    this->_main_texture = other_object._main_texture;
#endif
//----------------------------------------------------------------------------------------
    this->_main_bitmap = other_object._main_bitmap;
    this->_bitmap_w = other_object._bitmap_w;
    this->_bitmap_h = other_object._bitmap_h;
    this->_largest_bitmap_w = other_object._largest_bitmap_w;
    this->_largest_bitmap_h = other_object._largest_bitmap_h;
    this->_default_letters_color = other_object._default_letters_color;
    this->_default_background_color = other_object._default_background_color;
    this->_extra_offset_right = other_object._extra_offset_right;
    this->_extra_offset_up = other_object._extra_offset_up;

    this->_main_text_buffor = other_object._main_text_buffor;
    this->_count_main_text_buffor = other_object._count_main_text_buffor;
    this->_reserved_count_main_text_buffor = other_object._reserved_count_main_text_buffor;
    this->_text_characters = other_object._text_characters;
    this->_count_text_characters = other_object._count_text_characters;
    this->_reserved_count_text_characters = other_object._reserved_count_text_characters;

    this->_word_wrap = other_object._word_wrap;
    this->_total_updated_columns = other_object._total_updated_columns;
    this->_total_updated_rows = other_object._total_updated_rows;

    this->_viewpoint = other_object._viewpoint;
    this->_total_lines_in_text = other_object._total_lines_in_text;
    this->_visible_bitmap_letters = other_object._visible_bitmap_letters;
    this->_visible_hitbox_letters = other_object._visible_hitbox_letters;
    this->_first_index_of_visible_hitbox = other_object._first_index_of_visible_hitbox;
    this->_first_index_of_visible_letter = other_object._first_index_of_visible_letter;
    this->_quality_draw = other_object._quality_draw;
    this->_color_mode = other_object._color_mode;
    this->_tab_width = other_object._tab_width;
    this->_treat_tab_width_as_spaces = other_object._treat_tab_width_as_spaces;
    this->_tab_index_inside_characters = other_object._tab_index_inside_characters;

    this->_count_of_characters = other_object._count_of_characters;
    this->_characters = other_object._characters;
    this->_extra_enter_character = other_object._extra_enter_character;

    this->_max_height = other_object._max_height;
    this->_max_y = other_object._max_y;
    this->_line_height = other_object._line_height;

    return;
}


//  copying constructor; used like:
//      Class object0 = object1;
//      Class object0 = return_object();
//          [inside return_object]:
//          return object2;
cSDL_FontManager::cSDL_FontManager(const cSDL_FontManager& other_object)
{
    this->_copy_objects_of_class(other_object);

    return;
}

//  copying operator; used like:
//      object0 = object1;
//      object0 = return_object();
//          [inside return_object]:
//          return object2;
cSDL_FontManager& cSDL_FontManager::operator=(const cSDL_FontManager& other_object)
{
    if(this==&other_object) return *this;  //  check if you're not using '=' on the same object;

    this->_free_class(*this);
    this->_copy_objects_of_class(other_object);

    return *this;
}

//  moving constructor; used like:
//      Class object0 = Class();
//      Class object0 = std::move(object1);
//      Class object0 = return_object();
//          [inside return_object]:
//          return Class();
cSDL_FontManager::cSDL_FontManager(cSDL_FontManager&& other_object)
{
    this->_move_object_of_class(other_object);

    return;
}

//  moving operator; used like:
//      object0 = Class();
//      object0 = std::move(object1);
//      object0 = return_object();
//          [inside return_object]:
//          return Class();
cSDL_FontManager& cSDL_FontManager::operator=(cSDL_FontManager&& other_object)
{
    if(this==&other_object) return *this;  //  check if you're not using '=' on the same object;

    this->_free_class(*this);
    this->_move_object_of_class(other_object);

    return *this;
}

#endif
