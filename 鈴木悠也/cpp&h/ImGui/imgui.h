// dear imgui, v1.75 WIP
// (headers)

// Help:
// - Read FAQ at http://dearimgui.org/faq
// - Newcomers, read 'Programmer guide' in imgui.cpp for notes on how to setup Dear ImGui in your codebase.
// - Call and read ImGui::ShowDemoWindow() in imgui_demo.cpp for demo code. All applications in examples/ are doing that.
// Read imgui.cpp for more details, documentation and comments.

// Resources:
// - FAQ                   http://dearimgui.org/faq
// - Homepage & latest     https://github.com/ocornut/imgui
// - Releases & changelog  https://github.com/ocornut/imgui/releases
// - Gallery               https://github.com/ocornut/imgui/issues/2847 (please post your screenshots/video there!)
// - Glossary              https://github.com/ocornut/imgui/wiki/Glossary
// - Wiki                  https://github.com/ocornut/imgui/wiki
// - Issues & support      https://github.com/ocornut/imgui/issues

/*

Index of this file:
// Header mess
// Forward declarations and basic types
// ImGui API (Dear ImGui end-user API)
// Flags & Enumerations
// Memory allocations macros
// ImVector<>
// ImGuiStyle
// ImGuiIO
// Misc data structures (ImGuiInputTextCallbackData, ImGuiSizeCallbackData, ImGuiPayload)
// Obsolete functions
// Helpers (ImGuiOnceUponAFrame, ImGuiTextFilter, ImGuiTextBuffer, ImGuiStorage, ImGuiListClipper, ImColor)
// Draw List API (ImDrawCallback, ImDrawCmd, ImDrawIdx, ImDrawVert, ImDrawChannel, ImDrawListSplitter, ImDrawListFlags, ImDrawList, ImDrawData)
// Font API (ImFontConfig, ImFontGlyph, ImFontGlyphRangesBuilder, ImFontAtlasFlags, ImFontAtlas, ImFont)

*/

#pragma once

// Configuration file with compile-time options (edit imconfig.h or #define IMGUI_USER_CONFIG to your own filename)
#ifdef IMGUI_USER_CONFIG
#include IMGUI_USER_CONFIG
#endif
#if !defined(IMGUI_DISABLE_INCLUDE_IMCONFIG_H) || defined(IMGUI_INCLUDE_IMCONFIG_H)
#include "imconfig.h"
#endif

//-----------------------------------------------------------------------------
// Header mess
//-----------------------------------------------------------------------------

// Includes
#include <float.h>                  // FLT_MIN, FLT_MAX
#include <stdarg.h>                 // va_list, va_start, va_end
#include <stddef.h>                 // ptrdiff_t, NULL
#include <string.h>                 // memset, memmove, memcpy, strlen, strchr, strcpy, strcmp

// Version
// (Integer encoded as XYYZZ for use in #if preprocessor conditionals. Work in progress versions typically starts at XYY99 then bounce up to XYY00, XYY01 etc. when release tagging happens)
#define IMGUI_VERSION               "1.75 WIP"
#define IMGUI_VERSION_NUM           17401
#define IMGUI_CHECKVERSION()        ImGui::DebugCheckVersionAndDataLayout(IMGUI_VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx))

// Define attributes of all API symbols declarations (e.g. for DLL under Windows)
// IMGUI_API is used for core imgui functions, IMGUI_IMPL_API is used for the default bindings files (imgui_impl_xxx.h)
// Using dear imgui via a shared library is not recommended, because of function call overhead and because we don't guarantee backward nor forward ABI compatibility.
#ifndef IMGUI_API
#define IMGUI_API
#endif
#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API              IMGUI_API
#endif

// Helper Macros
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h
#endif
#if !defined(IMGUI_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__))
#define IM_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1))) // To apply printf-style warnings to our functions.
#define IM_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*_ARR)))       // Size of a static C-style array. Don't use on pointers!
#define IM_UNUSED(_VAR)             ((void)_VAR)                                // Used to silence "unused variable warnings". Often useful as asserts may be stripped out from final builds.
#if (__cplusplus >= 201100)
#define IM_OFFSETOF(_TYPE,_MEMBER)  offsetof(_TYPE, _MEMBER)                    // Offset of _MEMBER within _TYPE. Standardized as offsetof() in C++11
#else
#define IM_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))           // Offset of _MEMBER within _TYPE. Old style macro.
#endif
#define IM_UNICODE_CODEPOINT_MAX     0xFFFF                                     // Last Unicode code point supported by this build.
#define IM_UNICODE_CODEPOINT_INVALID 0xFFFD                                     // Standard invalid Unicode code point.

// Warnings
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wclass-memaccess"          // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

//-----------------------------------------------------------------------------
// Forward declarations and basic types
//-----------------------------------------------------------------------------

struct ImDrawChannel;               // Temporary storage to output draw commands out of order, used by ImDrawListSplitter and ImDrawList::ChannelsSplit()
struct ImDrawCmd;                   // A single draw command within a parent ImDrawList (generally maps to 1 GPU draw call, unless it is a callback)
struct ImDrawData;                  // All draw command lists required to render the frame + pos/size coordinates to use for the projection matrix.
struct ImDrawList;                  // A single draw command list (generally one per window, conceptually you may see this as a dynamic "mesh" builder)
struct ImDrawListSharedData;        // Data shared among multiple draw lists (typically owned by parent ImGui context, but you may create one yourself)
struct ImDrawListSplitter;          // Helper to split a draw list into different layers which can be drawn into out of order, then flattened back.
struct ImDrawVert;                  // A single vertex (pos + uv + col = 20 bytes by default. Override layout with IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
struct ImFont;                      // Runtime data for a single font within a parent ImFontAtlas
struct ImFontAtlas;                 // Runtime data for multiple fonts, bake multiple fonts into a single texture, TTF/OTF font loader
struct ImFontConfig;                // Configuration data when adding a font or merging fonts
struct ImFontGlyph;                 // A single font glyph (code point + coordinates within in ImFontAtlas + offset)
struct ImFontGlyphRangesBuilder;    // Helper to build glyph ranges from text/string data
struct ImColor;                     // Helper functions to create a color that can be converted to either u32 or float4 (*OBSOLETE* please avoid using)
struct ImGuiContext;                // Dear ImGui context (opaque structure, unless including imgui_internal.h)
struct ImGuiIO;                     // Main configuration and I/O between your application and ImGui
struct ImGuiInputTextCallbackData;  // Shared state of InputText() when using custom ImGuiInputTextCallback (rare/advanced use)
struct ImGuiListClipper;            // Helper to manually clip large list of items
struct ImGuiOnceUponAFrame;         // Helper for running a block of code not more than once a frame, used by IMGUI_ONCE_UPON_A_FRAME macro
struct ImGuiPayload;                // User data payload for drag and drop operations
struct ImGuiSizeCallbackData;       // Callback data when using SetNextWindowSizeConstraints() (rare/advanced use)
struct ImGuiStorage;                // Helper for key->value storage
struct ImGuiStyle;                  // Runtime data for styling/colors
struct ImGuiTextBuffer;             // Helper to hold and append into a text buffer (~string builder)
struct ImGuiTextFilter;             // Helper to parse and apply text filters (e.g. "aaaaa[,bbbb][,ccccc]")

// Typedefs and Enums/Flags (declared as int for compatibility with old C++, to allow using as flags and to not pollute the top of this file)
// Use your programming IDE "Go to definition" facility on the names in the central column below to find the actual flags/enum lists.
#ifndef ImTextureID
typedef void* ImTextureID;          // User data to identify a texture (this is whatever to you want it to be! read the FAQ about ImTextureID in imgui.cpp)
#endif
typedef unsigned int ImGuiID;       // Unique ID used by widgets (typically hashed from a stack of string)
typedef unsigned short ImWchar;     // A single U16 character for keyboard input/display. We encode them as multi bytes UTF-8 when used in strings.
typedef int ImGuiCol;               // -> enum ImGuiCol_             // Enum: A color identifier for styling
typedef int ImGuiCond;              // -> enum ImGuiCond_            // Enum: A condition for many Set*() functions
typedef int ImGuiDataType;          // -> enum ImGuiDataType_        // Enum: A primary data type
typedef int ImGuiDir;               // -> enum ImGuiDir_             // Enum: A cardinal direction
typedef int ImGuiKey;               // -> enum ImGuiKey_             // Enum: A key identifier (ImGui-side enum)
typedef int ImGuiNavInput;          // -> enum ImGuiNavInput_        // Enum: An input identifier for navigation
typedef int ImGuiMouseButton;       // -> enum ImGuiMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
typedef int ImGuiMouseCursor;       // -> enum ImGuiMouseCursor_     // Enum: A mouse cursor identifier
typedef int ImGuiStyleVar;          // -> enum ImGuiStyleVar_        // Enum: A variable identifier for styling
typedef int ImDrawCornerFlags;      // -> enum ImDrawCornerFlags_    // Flags: for ImDrawList::AddRect(), AddRectFilled() etc.
typedef int ImDrawListFlags;        // -> enum ImDrawListFlags_      // Flags: for ImDrawList
typedef int ImFontAtlasFlags;       // -> enum ImFontAtlasFlags_     // Flags: for ImFontAtlas
typedef int ImGuiBackendFlags;      // -> enum ImGuiBackendFlags_    // Flags: for io.BackendFlags
typedef int ImGuiColorEditFlags;    // -> enum ImGuiColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
typedef int ImGuiConfigFlags;       // -> enum ImGuiConfigFlags_     // Flags: for io.ConfigFlags
typedef int ImGuiComboFlags;        // -> enum ImGuiComboFlags_      // Flags: for BeginCombo()
typedef int ImGuiDragDropFlags;     // -> enum ImGuiDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
typedef int ImGuiFocusedFlags;      // -> enum ImGuiFocusedFlags_    // Flags: for IsWindowFocused()
typedef int ImGuiHoveredFlags;      // -> enum ImGuiHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
typedef int ImGuiInputTextFlags;    // -> enum ImGuiInputTextFlags_  // Flags: for InputText(), InputTextMultiline()
typedef int ImGuiSelectableFlags;   // -> enum ImGuiSelectableFlags_ // Flags: for Selectable()
typedef int ImGuiTabBarFlags;       // -> enum ImGuiTabBarFlags_     // Flags: for BeginTabBar()
typedef int ImGuiTabItemFlags;      // -> enum ImGuiTabItemFlags_    // Flags: for BeginTabItem()
typedef int ImGuiTreeNodeFlags;     // -> enum ImGuiTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
typedef int ImGuiWindowFlags;       // -> enum ImGuiWindowFlags_     // Flags: for Begin(), BeginChild()
typedef int (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData *data);
typedef void (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);

// Scalar data types
typedef signed char         ImS8;   // 8-bit signed integer
typedef unsigned char       ImU8;   // 8-bit unsigned integer
typedef signed short        ImS16;  // 16-bit signed integer
typedef unsigned short      ImU16;  // 16-bit unsigned integer
typedef signed int          ImS32;  // 32-bit signed integer == int
typedef unsigned int        ImU32;  // 32-bit unsigned integer (often used to store packed colors)
#if defined(_MSC_VER) && !defined(__clang__)
typedef signed   __int64    ImS64;  // 64-bit signed integer (pre and post C++11 with Visual Studio)
typedef unsigned __int64    ImU64;  // 64-bit unsigned integer (pre and post C++11 with Visual Studio)
#elif (defined(__clang__) || defined(__GNUC__)) && (__cplusplus < 201100)
#include <stdint.h>
typedef int64_t             ImS64;  // 64-bit signed integer (pre C++11)
typedef uint64_t            ImU64;  // 64-bit unsigned integer (pre C++11)
#else
typedef signed   long long  ImS64;  // 64-bit signed integer (post C++11)
typedef unsigned long long  ImU64;  // 64-bit unsigned integer (post C++11)
#endif

// 2D vector (often used to store positions, sizes, etc.)
struct ImVec2
{
    float     x, y;
    ImVec2()  { x = y = 0.0f; }
    ImVec2(float _x, float _y) { x = _x; y = _y; }
    float  operator[] (size_t idx) const { IM_ASSERT(idx <= 1); return (&x)[idx]; }    // We very rarely use this [] operator, the assert overhead is fine.
    float& operator[] (size_t idx)       { IM_ASSERT(idx <= 1); return (&x)[idx]; }    // We very rarely use this [] operator, the assert overhead is fine.
#ifdef IM_VEC2_CLASS_EXTRA
    IM_VEC2_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imconfig.h to convert back and forth between your math types and ImVec2.
#endif
};

// 4D vector (often used to store floating-point colors)
struct ImVec4
{
    float     x, y, z, w;
    ImVec4()  { x = y = z = w = 0.0f; }
    ImVec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
#ifdef IM_VEC4_CLASS_EXTRA
    IM_VEC4_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imconfig.h to convert back and forth between your math types and ImVec4.
#endif
};

//-----------------------------------------------------------------------------
// ImGui: Dear ImGui end-user API
// (Inside a namespace so you can add extra functions in your own separate file. Please don't modify imgui source files!)
//-----------------------------------------------------------------------------

namespace ImGui
{
    // Context creation and access
    // Each context create its own ImFontAtlas by default. You may instance one yourself and pass it to CreateContext() to share a font atlas between imgui contexts.
    // None of those functions is reliant on the current context.
    IMGUI_API ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL);
    IMGUI_API void          DestroyContext(ImGuiContext* ctx = NULL);   // NULL = destroy current context
    IMGUI_API ImGuiContext* GetCurrentContext();
    IMGUI_API void          SetCurrentContext(ImGuiContext* ctx);
    IMGUI_API bool          DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx);

    // Main
    IMGUI_API ImGuiIO&      GetIO();                                    // access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)
    IMGUI_API ImGuiStyle&   GetStyle();                                 // access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame.
    IMGUI_API void          NewFrame();                                 // start a new Dear ImGui frame, you can submit any command from this point until Render()/EndFrame().
    IMGUI_API void          EndFrame();                                 // ends the Dear ImGui frame. automatically called by Render(), you likely don't need to call that yourself directly. If you don't need to render data (skipping rendering) you may call EndFrame() but you'll have wasted CPU already! If you don't need to render, better to not create any imgui windows and not call NewFrame() at all!
    IMGUI_API void          Render();                                   // ends the Dear ImGui frame, finalize the draw data. You can get call GetDrawData() to obtain it and run your rendering function. (Obsolete: this used to call io.RenderDrawListsFn(). Nowadays, we allow and prefer calling your render function yourself.)
    IMGUI_API ImDrawData*   GetDrawData();                              // valid after Render() and until the next call to NewFrame(). this is what you have to render.

    // Demo, Debug, Information
    IMGUI_API void          ShowDemoWindow(bool* p_open = NULL);        // create Demo window (previously called ShowTestWindow). demonstrate most ImGui features. call this to learn about the library! try to make it always available in your application!
    IMGUI_API void          ShowAboutWindow(bool* p_open = NULL);       // create About window. display Dear ImGui version, credits and build/system information.
    IMGUI_API void          ShowMetricsWindow(bool* p_open = NULL);     // create Metrics/Debug window. display Dear ImGui internals: draw commands (with individual draw calls and vertices), window list, basic internal state, etc.
    IMGUI_API void          ShowStyleEditor(ImGuiStyle* ref = NULL);    // add style editor block (not a window). you can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it uses the default style)
    IMGUI_API bool          ShowStyleSelector(const char* label);       // add style selector block (not a window), essentially a combo listing the default styles.
    IMGUI_API void          ShowFontSelector(const char* label);        // add font selector block (not a window), essentially a combo listing the loaded fonts.
    IMGUI_API void          ShowUserGuide();                            // add basic help/info block (not a window): how to manipulate ImGui as a end-user (mouse/keyboard controls).
    IMGUI_API const char*   GetVersion();                               // get the compiled version string e.g. "1.23" (essentially the compiled value for IMGUI_VERSION)

    // Styles
    IMGUI_API void          StyleColorsDark(ImGuiStyle* dst = NULL);    // new, recommended style (default)
    IMGUI_API void          StyleColorsClassic(ImGuiStyle* dst = NULL); // classic imgui style
    IMGUI_API void          StyleColorsLight(ImGuiStyle* dst = NULL);   // best used with borders and a custom, thicker font

    // Windows
    // - Begin() = push window to the stack and start appending to it. End() = pop window from the stack.
    // - You may append multiple times to the same window during the same frame.
    // - Passing 'bool* p_open != NULL' shows a window-closing widget in the upper-right corner of the window,
    //   which clicking will set the boolean to false when clicked.
    // - Begin() return false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting
    //   anything to the window. Always call a matching End() for each Begin() call, regardless of its return value!
    //   [Important: due to legacy reason, this is inconsistent with most other functions such as BeginMenu/EndMenu,
    //    BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding BeginXXX function
    //    returned true. Begin and BeginChild are the only odd ones out. Will be fixed in a future update.]
    // - Note that the bottom of window stack always contains a window called "Debug".
    IMGUI_API bool          Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
    IMGUI_API void          End();

    // Child Windows
    // - Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child.
    // - For each independent axis of 'size': ==0.0f: use remaining host window size / >0.0f: fixed size / <0.0f: use remaining window size minus abs(size) / Each axis can use a different mode, e.g. ImVec2(0,400).
    // - BeginChild() returns false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting anything to the window.
    //   Always call a matching EndChild() for each BeginChild() call, regardless of its return value [as with Begin: this is due to legacy reason and inconsistent with most BeginXXX functions apart from the regular Begin() which behaves like BeginChild().]
    IMGUI_API bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0,0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API bool          BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0,0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API void          EndChild();

    // Windows Utilities
    // - 'current window' = the window we are appending into while inside a Begin()/End() block. 'next window' = next window we will Begin() into.
    IMGUI_API bool          IsWindowAppearing();
    IMGUI_API bool          IsWindowCollapsed();
    IMGUI_API bool          IsWindowFocused(ImGuiFocusedFlags flags=0); // is current window focused? or its root/child, depending on flags. see flags for options.
    IMGUI_API bool          IsWindowHovered(ImGuiHoveredFlags flags=0); // is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!
    IMGUI_API ImDrawList*   GetWindowDrawList();                        // get draw list associated to the current window, to append your own drawing primitives
    IMGUI_API ImVec2        GetWindowPos();                             // get current window position in screen space (useful if you want to do your own drawing via the DrawList API)
    IMGUI_API ImVec2        GetWindowSize();                            // get current window size
    IMGUI_API float         GetWindowWidth();                           // get current window width (shortcut for GetWindowSize().x)
    IMGUI_API float         GetWindowHeight();                          // get current window height (shortcut for GetWindowSize().y)

    // Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
    IMGUI_API void          SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0,0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
    IMGUI_API void          SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);                  // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
    IMGUI_API void          SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL); // set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down. Use callback to apply non-trivial programmatic constraints.
    IMGUI_API void          SetNextWindowContentSize(const ImVec2& size);                               // set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. set an axis to 0.0f to leave it automatic. call before Begin()
    IMGUI_API void          SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                 // set next window collapsed state. call before Begin()
    IMGUI_API void          SetNextWindowFocus();                                                       // set next window to be focused / top-most. call before Begin()
    IMGUI_API void          SetNextWindowBgAlpha(float alpha);                                          // set next window background color alpha. helper to easily override the Alpha component of ImGuiCol_WindowBg/ChildBg/PopupBg. you may also use ImGuiWindowFlags_NoBackground.
    IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);                        // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
    IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiCond cond = 0);                      // (not recommended) set current window size - call within Begin()/End(). set to ImVec2(0,0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
    IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                     // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
    IMGUI_API void          SetWindowFocus();                                                           // (not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus().
    IMGUI_API void          SetWindowFontScale(float scale);                                            // set font scale. Adjust IO.FontGlobalScale if you want to scale all windows. This is an old API! For correct scaling, prefer to reload font + rebuild ImFontAtlas + call style.ScaleAllSizes().
    IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond = 0);      // set named window position.
    IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond = 0);    // set named window size. set axis to 0.0f to force an auto-fit on this axis.
    IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0);   // set named window collapsed state
    IMGUI_API void          SetWindowFocus(const char* name);                                           // set named window to be focused / top-most. use NULL to remove focus.

    // Content region
    // - Those functions are bound to be redesigned soon (they are confusing, incomplete and return values in local window coordinates which increases confusion)
    IMGUI_API ImVec2        GetContentRegionMax();                                          // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
    IMGUI_API ImVec2        GetContentRegionAvail();                                        // == GetContentRegionMax() - GetCursorPos()
    IMGUI_API ImVec2        GetWindowContentRegionMin();                                    // content boundaries min (roughly (0,0)-Scroll), in window coordinates
    IMGUI_API ImVec2        GetWindowContentRegionMax();                                    // content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
    IMGUI_API float         GetWindowContentRegionWidth();                                  //

    // Windows Scrolling
    IMGUI_API float         GetScrollX();                                                   // get scrolling amount [0..GetScrollMaxX()]
    IMGUI_API float         GetScrollY();                                                   // get scrolling amount [0..GetScrollMaxY()]
    IMGUI_API float         GetScrollMaxX();                                                // get maximum scrolling amount ~~ ContentSize.X - WindowSize.X
    IMGUI_API float         GetScrollMaxY();                                                // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
    IMGUI_API void          SetScrollX(float scroll_x);                                     // set scrolling amount [0..GetScrollMaxX()]
    IMGUI_API void          SetScrollY(float scroll_y);                                     // set scrolling amount [0..GetScrollMaxY()]
    IMGUI_API void          SetScrollHereX(float center_x_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    IMGUI_API void          SetScrollHereY(float center_y_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    IMGUI_API void          SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
    IMGUI_API void          SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.

    // Parameters stacks (shared)
    IMGUI_API void          PushFont(ImFont* font);                                         // use NULL as a shortcut to push default font
    IMGUI_API void          PopFont();
    IMGUI_API void          PushStyleColor(ImGuiCol idx, ImU32 col);
    IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);
    IMGUI_API void          PopStyleColor(int count = 1);
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, float val);
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
    IMGUI_API void          PopStyleVar(int count = 1);
    IMGUI_API const ImVec4& GetStyleColorVec4(ImGuiCol idx);                                // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.
    IMGUI_API ImFont*       GetFont();                                                      // get current font
    IMGUI_API float         GetFontSize();                                                  // get current font size (= height in pixels) of current font with current scale applied
    IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                       // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
    IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);              // retrieve given style color with style alpha applied and optional extra alpha multiplier
    IMGUI_API ImU32         GetColorU32(const ImVec4& col);                                 // retrieve given color with style alpha applied
    IMGUI_API ImU32         GetColorU32(ImU32 col);                                         // retrieve given color with style alpha applied

    // Parameters stacks (current window)
    IMGUI_API void          PushItemWidth(float item_width);                                // set width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side). 0.0f = default to ~2/3 of windows width,
    IMGUI_API void          PopItemWidth();
    IMGUI_API void          SetNextItemWidth(float item_width);                             // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side)
    IMGUI_API float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
    IMGUI_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // word-wrapping for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
    IMGUI_API void          PopTextWrapPos();
    IMGUI_API void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              // allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
    IMGUI_API void          PopAllowKeyboardFocus();
    IMGUI_API void          PushButtonRepeat(bool repeat);                                  // in 'repeat' mode, Button*() functions return repeated true in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
    IMGUI_API void          PopButtonRepeat();

    // Cursor / Layout
    // - By "cursor" we mean the current output position.
    // - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down.
    // - You can call SameLine() between widgets to undo the last carriage return and output at the right of the preceeding widget.
    IMGUI_API void          Separator();                                                    // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
    IMGUI_API void          SameLine(float offset_from_start_x=0.0f, float spacing=-1.0f);  // call between widgets or groups to layout them horizontally. X position given in window coordinates.
    IMGUI_API void          NewLine();                                                      // undo a SameLine() or force a new line when in an horizontal-layout context.
    IMGUI_API void          Spacing();                                                      // add vertical spacing.
    IMGUI_API void          Dummy(const ImVec2& size);                                      // add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
    IMGUI_API void          Indent(float indent_w = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
    IMGUI_API void          Unindent(float indent_w = 0.0f);                                // move content position back to the left, by style.IndentSpacing or indent_w if != 0
    IMGUI_API void          BeginGroup();                                                   // lock horizontal starting position
    IMGUI_API void          EndGroup();                                                     // unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
    IMGUI_API ImVec2        GetCursorPos();                                                 // cursor position in window coordinates (relative to window position)
    IMGUI_API float         GetCursorPosX();                                                //   (some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.
    IMGUI_API float         GetCursorPosY();                                                //    other functions such as GetCursorScreenPos or everything in ImDrawList::
    IMGUI_API void          SetCursorPos(const ImVec2& local_pos);                          //    are using the main, absolute coordinate system.
    IMGUI_API void          SetCursorPosX(float local_x);                                   //    GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.)
    IMGUI_API void          SetCursorPosY(float local_y);                                   //
    IMGUI_API ImVec2        GetCursorStartPos();                                            // initial cursor position in window coordinates
    IMGUI_API ImVec2        GetCursorScreenPos();                                           // cursor position in absolute screen coordinates [0..io.DisplaySize] (useful to work with ImDrawList API)
    IMGUI_API void          SetCursorScreenPos(const ImVec2& pos);                          // cursor position in absolute screen coordinates [0..io.DisplaySize]
    IMGUI_API void          AlignTextToFramePadding();                                      // vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
    IMGUI_API float         GetTextLineHeight();                                            // ~ FontSize
    IMGUI_API float         GetTextLineHeightWithSpacing();                                 // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
    IMGUI_API float         GetFrameHeight();                                               // ~ FontSize + style.FramePadding.y * 2
    IMGUI_API float         GetFrameHeightWithSpacing();                                    // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)

    // ID stack/scopes
    // - Read the FAQ for more details about how ID are handled in dear imgui. If you are creating widgets in a loop you most
    //   likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
    // - The resulting ID are hashes of the entire stack.
    // - You can also use the "Label##foobar" syntax within widget label to distinguish them from each others.
    // - In this header file we use the "label"/"name" terminology to denote a string that will be displayed and used as an ID,
    //   whereas "str_id" denote a string that is only used as an ID and not normally displayed.
    IMGUI_API void          PushID(const char* str_id);                                     // push string into the ID stack (will hash string).
    IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push string into the ID stack (will hash string).
    IMGUI_API void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
    IMGUI_API void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
    IMGUI_API void          PopID();                                                        // pop from the ID stack.
    IMGUI_API ImGuiID       GetID(const char* str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into ImGuiStorage yourself
    IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
    IMGUI_API ImGuiID       GetID(const void* ptr_id);

    // Widgets: Text
    IMGUI_API void          TextUnformatted(const char* text, const char* text_end = NULL); // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
    IMGUI_API void          Text(const char* fmt, ...)                                      IM_FMTARGS(1); // formatted text
    IMGUI_API void          TextV(const char* fmt, va_list args)                            IM_FMTLIST(1);
    IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...)            IM_FMTARGS(2); // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
    IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args)  IM_FMTLIST(2);
    IMGUI_API void          TextDisabled(const char* fmt, ...)                              IM_FMTARGS(1); // shortcut for PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
    IMGUI_API void          TextDisabledV(const char* fmt, va_list args)                    IM_FMTLIST(1);
    IMGUI_API void          TextWrapped(const char* fmt, ...)                               IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
    IMGUI_API void          TextWrappedV(const char* fmt, va_list args)                     IM_FMTLIST(1);
    IMGUI_API void          LabelText(const char* label, const char* fmt, ...)              IM_FMTARGS(2); // display text+label aligned the same way as value+label widgets
    IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_list args)    IM_FMTLIST(2);
    IMGUI_API void          BulletText(const char* fmt, ...)                                IM_FMTARGS(1); // shortcut for Bullet()+Text()
    IMGUI_API void          BulletTextV(const char* fmt, va_list args)                      IM_FMTLIST(1);

    // Widgets: Main
    // - Most widgets return true when the value has been changed or when pressed/selected
    // - You may also use one of the many IsItemXXX functions (e.g. IsItemActive, IsItemHovered, etc.) to query widget state.
    IMGUI_API bool          Button(const char* label, const ImVec2& size = ImVec2(0,0));    // button
    IMGUI_API bool          SmallButton(const char* label);                                 // button with FramePadding=(0,0) to easily embed within text
    IMGUI_API bool          InvisibleButton(const char* str_id, const ImVec2& size);        // button behavior without the visuals, frequently useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
    IMGUI_API bool          ArrowButton(const char* str_id, ImGuiDir dir);                  // square button with an arrow shape
    IMGUI_API void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1), const ImVec4& tint_col = ImVec4(1,1,1,1), const ImVec4& border_col = ImVec4(0,0,0,0));
    IMGUI_API bool          ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0),  const ImVec2& uv1 = ImVec2(1,1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0,0,0,0), const ImVec4& tint_col = ImVec4(1,1,1,1));    // <0 frame_padding uses default frame padding settings. 0 for no padding
    IMGUI_API bool          Checkbox(const char* label, bool* v);
    IMGUI_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
    IMGUI_API bool          RadioButton(const char* label, bool active);                    // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
    IMGUI_API bool          RadioButton(const char* label, int* v, int v_button);           // shortcut to handle the above pattern when value is an integer
    IMGUI_API void          ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-1,0), const char* overlay = NULL);
    IMGUI_API void          Bullet();                                                       // draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses

    // Widgets: Combo Box
    // - The BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
    // - The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose.
    IMGUI_API bool          BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    IMGUI_API void          EndCombo(); // only call EndCombo() if BeginCombo() returns true!
    IMGUI_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    IMGUI_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

    // Widgets: Drags
    // - CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
    // - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
    // - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    // - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For gamepad/keyboard navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
    // - Use v_min < v_max to clamp edits to given limits. Note that CTRL+Click manual input can override those limits.
    // - Use v_max = FLT_MAX / INT_MAX etc to avoid clamping to a maximum, same with v_min = -FLT_MAX / INT_MIN to avoid clamping to a minimum.
    // - Use v_min > v_max to lock edits.
    IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);     // If v_min >= v_max we have no bound
    IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, float power = 1.0f);
    IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");                                       // If v_min >= v_max we have no bound
    IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL);
    IMGUI_API bool          DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, float power = 1.0f);
    IMGUI_API bool          DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, float power = 1.0f);

    // Widgets: Sliders
    // - CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
    // - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
    IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg");
    IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, float power = 1.0f);
    IMGUI_API bool          SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, float power = 1.0f);
    IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d");
    IMGUI_API bool          VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, float power = 1.0f);

    // Widgets: Input with Keyboard
    // - If you want to use InputText() with std::string or any custom dynamic string type, see misc/cpp/imgui_stdlib.h and comments in imgui_demo.cpp.
    // - Most of the ImGuiInputTextFlags flags are only useful for InputText() and not for InputFloatX, InputIntX, InputDouble etc.
    IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0,0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);

    // Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little colored preview square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
    // - Note that in C++ a 'float v[X]' function argument is the _same_ as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible.
    // - You can pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
    IMGUI_API bool          ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    IMGUI_API bool          ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0,0));  // display a colored square/button, hover for details, return true when pressed.
    IMGUI_API void          SetColorEditOptions(ImGuiColorEditFlags flags);                     // initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.

    // Widgets: Trees
    // - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
    IMGUI_API bool          TreeNode(const char* label);
    IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   // helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
    IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   // "
    IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
    IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API void          TreePush(const char* str_id);                                       // ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
    IMGUI_API void          TreePush(const void* ptr_id = NULL);                                // "
    IMGUI_API void          TreePop();                                                          // ~ Unindent()+PopId()
    IMGUI_API float         GetTreeNodeToLabelSpacing();                                        // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
    IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);  // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
    IMGUI_API bool          CollapsingHeader(const char* label, bool* p_open, ImGuiTreeNodeFlags flags = 0); // when 'p_open' isn't NULL, display an additional small close button on upper right of the header
    IMGUI_API void          SetNextItemOpen(bool is_open, ImGuiCond cond = 0);                  // set next TreeNode/CollapsingHeader open state.

    // Widgets: Selectables
    // - A selectable highlights when hovered, and can display another color when selected.
    // - Neighbors selectable extend their highlight bounds in order to leave no gap between them. This is so a series of selected Selectable appear contiguous.
    IMGUI_API bool          Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));       // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

    // Widgets: List Boxes
    // - FIXME: To be consistent with all the newer API, ListBoxHeader/ListBoxFooter should in reality be called BeginListBox/EndListBox. Will rename them.
    IMGUI_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    IMGUI_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
    IMGUI_API bool          ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0,0)); // use if you want to reimplement ListBox() will custom data or interactions. if the function return true, you can output elements then call ListBoxFooter() afterwards.
    IMGUI_API bool          ListBoxHeader(const char* label, int items_count, int height_in_items = -1); // "
    IMGUI_API void          ListBoxFooter();                                                    // terminate the scrolling region. only call ListBoxFooter() if ListBoxHeader() returned true!

    // Widgets: Data Plotting
    IMGUI_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
    IMGUI_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void          PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

    // Widgets: Value() Helpers.
    // - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
    IMGUI_API void          Value(const char* prefix, bool b);
    IMGUI_API void          Value(const char* prefix, int v);
    IMGUI_API void          Value(const char* prefix, unsigned int v);
    IMGUI_API void          Value(const char* prefix, float v, const char* float_format = NULL);

    // Widgets: Menus
    // - Use BeginMenuBar() on a window ImGuiWindowFlags_MenuBar to append to its menu bar.
    // - Use BeginMainMenuBar() to create a menu bar at the top of the screen.
    IMGUI_API bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires ImGuiWindowFlags_MenuBar flag set on parent window).
    IMGUI_API void          EndMenuBar();                                                       // only call EndMenuBar() if BeginMenuBar() returns true!
    IMGUI_API bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
    IMGUI_API void          EndMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!
    IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
    IMGUI_API void          EndMenu();                                                          // only call EndMenu() if BeginMenu() returns true!
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL

    // Tooltips
    // - Tooltip are windows following the mouse which do not take focus away.
    IMGUI_API void          BeginTooltip();                                                     // begin/append a tooltip window. to create full-featured tooltip (with any kind of items).
    IMGUI_API void          EndTooltip();
    IMGUI_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);                     // set a text-only tooltip, typically use with ImGui::IsItemHovered(). override any previous call to SetTooltip().
    IMGUI_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Popups, Modals
    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by imgui instead of being held by the programmer as we are used to with regular Begin() calls.
    //   User can manipulate the visibility state by calling OpenPopup().
    // - We default to use the right mouse (ImGuiMouseButton_Right=1) for the Popup Context functions.
    // (*) You can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.
    IMGUI_API void          OpenPopup(const char* str_id);                                      // call to mark popup as open (don't call every frame!). popups are closed when user click outside, or if CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. By default, Selectable()/MenuItem() are calling CloseCurrentPopup(). Popup identifiers are relative to the current ID-stack (so OpenPopup and BeginPopup needs to be at the same level).
    IMGUI_API bool          BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);                                             // return true if the popup is open, and you can start outputting to it. only call EndPopup() if BeginPopup() returns true!
    IMGUI_API bool          BeginPopupContextItem(const char* str_id = NULL, ImGuiMouseButton mouse_button = 1);                    // helper to open and begin popup when clicked on last item. if you can pass a NULL str_id only if the previous item had an id. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
    IMGUI_API bool          BeginPopupContextWindow(const char* str_id = NULL, ImGuiMouseButton mouse_button = 1, bool also_over_items = true);  // helper to open and begin popup when clicked on current window.
    IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, ImGuiMouseButton mouse_button = 1);                    // helper to open and begin popup when clicked in void (where there are no imgui windows).
    IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);                     // modal dialog (regular window with title bar, block interactions behind the modal window, can't close the modal window by clicking outside)
    IMGUI_API void          EndPopup();                                                                                             // only call EndPopup() if BeginPopupXXX() returns true!
    IMGUI_API bool          OpenPopupOnItemClick(const char* str_id = NULL, ImGuiMouseButton mouse_button = 1);                     // helper to open popup when clicked on last item (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors). return true when just opened.
    IMGUI_API bool          IsPopupOpen(const char* str_id);                                    // return true if the popup is open at the current begin-ed level of the popup stack.
    IMGUI_API void          CloseCurrentPopup();                                                // close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup.

    // Columns
    // - You can also use SameLine(pos_x) to mimic simplified columns.
    // - The columns API is work-in-progress and rather lacking (columns are arguably the worst part of dear imgui at the moment!)
    // - There is a maximum of 64 columns.
    // - Currently working on new 'Tables' api which will replace columns (see GitHub #2957)
    IMGUI_API void          Columns(int count = 1, const char* id = NULL, bool border = true);
    IMGUI_API void          NextColumn();                                                       // next column, defaults to current row or next row if the current row is finished
    IMGUI_API int           GetColumnIndex();                                                   // get current column index
    IMGUI_API float         GetColumnWidth(int column_index = -1);                              // get column width (in pixels). pass -1 to use current column
    IMGUI_API void          SetColumnWidth(int column_index, float width);                      // set column width (in pixels). pass -1 to use current column
    IMGUI_API float         GetColumnOffset(int column_index = -1);                             // get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
    IMGUI_API void          SetColumnOffset(int column_index, float offset_x);                  // set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
    IMGUI_API int           GetColumnsCount();

    // Tab Bars, Tabs
    IMGUI_API bool          BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);        // create and append into a TabBar
    IMGUI_API void          EndTabBar();                                                        // only call EndTabBar() if BeginTabBar() returns true!
    IMGUI_API bool          BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);// create a Tab. Returns true if the Tab is selected.
    IMGUI_API void          EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
    IMGUI_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.

    // Logging/Capture
    // - All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
    IMGUI_API void          LogToTTY(int auto_open_depth = -1);                                 // start logging to tty (stdout)
    IMGUI_API void          LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // start logging to file
    IMGUI_API void          LogToClipboard(int auto_open_depth = -1);                           // start logging to OS clipboard
    IMGUI_API void          LogFinish();                                                        // stop logging (close file, etc.)
    IMGUI_API void          LogButtons();                                                       // helper to display buttons for logging to tty/file/clipboard
    IMGUI_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);                        // pass text data straight to log (without being displayed)

    // Drag and Drop
    // [BETA API] API may evolve!
    IMGUI_API bool          BeginDragDropSource(ImGuiDragDropFlags flags = 0);                                      // call when the current item is active. If this return true, you can call SetDragDropPayload() + EndDragDropSource()
    IMGUI_API bool          SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0);  // type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear imgui internal types. Data is copied and held by imgui.
    IMGUI_API void          EndDragDropSource();                                                                    // only call EndDragDropSource() if BeginDragDropSource() returns true!
    IMGUI_API bool                  BeginDragDropTarget();                                                          // call after submitting an item that may receive a payload. If this returns true, you can call AcceptDragDropPayload() + EndDragDropTarget()
    IMGUI_API const ImGuiPayload*   AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);          // accept contents of a given type. If ImGuiDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
    IMGUI_API void                  EndDragDropTarget();                                                            // only call EndDragDropTarget() if BeginDragDropTarget() returns true!
    IMGUI_API const ImGuiPayload*   GetDragDropPayload();                                                           // peek directly into the current payload from anywhere. may return NULL. use ImGuiPayload::IsDataType() to test for the payload type.

    // Clipping
    IMGUI_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
    IMGUI_API void          PopClipRect();

    // Focus, Activation
    // - Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHereY()" when applicable to signify "this is the default item"
    IMGUI_API void          SetItemDefaultFocus();                                              // make last item the default focused item of a window.
    IMGUI_API void          SetKeyboardFocusHere(int offset = 0);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.

    // Item/Widgets Utilities
    // - Most of the functions are referring to the last/previous item we submitted.
    // - See Demo Window under "Widgets->Querying Status" for an interactive visualization of most of those functions.
    IMGUI_API bool          IsItemHovered(ImGuiHoveredFlags flags = 0);                         // is the last item hovered? (and usable, aka not blocked by a popup, etc.). See ImGuiHoveredFlags for more options.
    IMGUI_API bool          IsItemActive();                                                     // is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
    IMGUI_API bool          IsItemFocused();                                                    // is the last item focused for keyboard/gamepad navigation?
    IMGUI_API bool          IsItemClicked(ImGuiMouseButton mouse_button = 0);                   // is the last item clicked? (e.g. button/node just clicked on) == IsMouseClicked(mouse_button) && IsItemHovered()
    IMGUI_API bool          IsItemVisible();                                                    // is the last item visible? (items may be out of sight because of clipping/scrolling)
    IMGUI_API bool          IsItemEdited();                                                     // did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets.
    IMGUI_API bool          IsItemActivated();                                                  // was the last item just made active (item was previously inactive).
    IMGUI_API bool          IsItemDeactivated();                                                // was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
    IMGUI_API bool          IsItemDeactivatedAfterEdit();                                       // was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
    IMGUI_API bool          IsItemToggledOpen();                                                // was the last item open state toggled? set by TreeNode().
    IMGUI_API bool          IsAnyItemHovered();                                                 // is any item hovered?
    IMGUI_API bool          IsAnyItemActive();                                                  // is any item active?
    IMGUI_API bool          IsAnyItemFocused();                                                 // is any item focused?
    IMGUI_API ImVec2        GetItemRectMin();                                                   // get upper-left bounding rectangle of the last item (screen space)
    IMGUI_API ImVec2        GetItemRectMax();                                                   // get lower-right bounding rectangle of the last item (screen space)
    IMGUI_API ImVec2        GetItemRectSize();                                                  // get size of last item
    IMGUI_API void          SetItemAllowOverlap();                                              // allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.

    // Miscellaneous Utilities
    IMGUI_API bool          IsRectVisible(const ImVec2& size);                                  // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
    IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);      // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
    IMGUI_API double        GetTime();                                                          // get global imgui time. incremented by io.DeltaTime every frame.
    IMGUI_API int           GetFrameCount();                                                    // get global imgui frame count. incremented by 1 every frame.
    IMGUI_API ImDrawList*   GetBackgroundDrawList();                                            // this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.
    IMGUI_API ImDrawList*   GetForegroundDrawList();                                            // this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.
    IMGUI_API ImDrawListSharedData* GetDrawListSharedData();                                    // you may use this when creating your own ImDrawList instances.
    IMGUI_API const char*   GetStyleColorName(ImGuiCol idx);                                    // get a string corresponding to the enum value (for display, saving, etc.).
    IMGUI_API void          SetStateStorage(ImGuiStorage* storage);                             // replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
    IMGUI_API ImGuiStorage* GetStateStorage();
    IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
    IMGUI_API void          CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);    // calculate coarse clipping for large list of evenly sized items. Prefer using the ImGuiListClipper higher-level helper if you can.
    IMGUI_API bool          BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags flags = 0); // helper to create a child window / scrolling region that looks like a normal widget frame
    IMGUI_API void          EndChildFrame();                                                    // always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)

    // Color Utilities
    IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
    IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
    IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    // Inputs Utilities: Keyboard
    // - For 'int user_key_index' you can use your own indices/enums according to how your backend/engine stored them in io.KeysDown[].
    // - We don't know the meaning of those value. You can use GetKeyIndex() to map a ImGuiKey_ value into the user index.
    IMGUI_API int           GetKeyIndex(ImGuiKey imgui_key);                                    // map ImGuiKey_* values into user's key index. == io.KeyMap[key]
    IMGUI_API bool          IsKeyDown(int user_key_index);                                      // is key being held. == io.KeysDown[user_key_index].
    IMGUI_API bool          IsKeyPressed(int user_key_index, bool repeat = true);               // was key pressed (went from !Down to Down)? if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
    IMGUI_API bool          IsKeyReleased(int user_key_index);                                  // was key released (went from Down to !Down)?
    IMGUI_API int           GetKeyPressedAmount(int key_index, float repeat_delay, float rate); // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
    IMGUI_API void          CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);    // attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after the next NewFrame() call.

    // Inputs Utilities: Mouse
    // - To refer to a mouse button, you may use named enums in your code e.g. ImGuiMouseButton_Left, ImGuiMouseButton_Right.
    // - You can also use regular integer: it is forever guaranteed that 0=Left, 1=Right, 2=Middle.
    // - Dragging operations are only reported after mouse has moved a certain distance away from the initial clicking position (see 'lock_threshold' and 'io.MouseDraggingThreshold')
    IMGUI_API bool          IsMouseDown(ImGuiMouseButton button);                               // is mouse button held?
    IMGUI_API bool          IsMouseClicked(ImGuiMouseButton button, bool repeat = false);       // did mouse button clicked? (went from !Down to Down)
    IMGUI_API bool          IsMouseReleased(ImGuiMouseButton button);                           // did mouse button released? (went from Down to !Down)
    IMGUI_API bool          IsMouseDoubleClicked(ImGuiMouseButton button);                      // did mouse button double-clicked? a double-click returns false in IsMouseClicked(). uses io.MouseDoubleClickTime.
    IMGUI_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);// is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
    IMGUI_API bool          IsMousePosValid(const ImVec2* mouse_pos = NULL);                    // by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse available
    IMGUI_API bool          IsAnyMouseDown();                                                   // is any mouse button held?
    IMGUI_API ImVec2        GetMousePos();                                                      // shortcut to ImGui::GetIO().MousePos provided by user, to be consistent with other calls
    IMGUI_API ImVec2        GetMousePosOnOpeningCurrentPopup();                                 // retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves)
    IMGUI_API bool          IsMouseDragging(ImGuiMouseButton button, float lock_threshold = -1.0f);         // is mouse dragging? (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
    IMGUI_API ImVec2        GetMouseDragDelta(ImGuiMouseButton button = 0, float lock_threshold = -1.0f);   // return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
    IMGUI_API void          ResetMouseDragDelta(ImGuiMouseButton button = 0);                   //
    IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                // get desired cursor type, reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
    IMGUI_API void          SetMouseCursor(ImGuiMouseCursor cursor_type);                       // set desired cursor type
    IMGUI_API void          CaptureMouseFromApp(bool want_capture_mouse_value = true);          // attention: misleading name! manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse_value;" after the next NewFrame() call.

    // Clipboard Utilities (also see the LogToClipboard() function to capture or output text data to the clipboard)
    IMGUI_API const char*   GetClipboardText();
    IMGUI_API void          SetClipboardText(const char* text);

    // Settings/.Ini Utilities
    // - The disk functions are automatically called if io.IniFilename != NULL (default is "imgui.ini").
    // - Set io.IniFilename to NULL to load/save manually. Read io.WantSaveIniSettings description about handling .ini saving manually.
    IMGUI_API void          LoadIniSettingsFromDisk(const char* ini_filename);                  // call after CreateContext() and before the first call to NewFrame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).
    IMGUI_API void          LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size=0); // call after CreateContext() and before the first call to NewFrame() to provide .ini data from your own data source.
    IMGUI_API void          SaveIniSettingsToDisk(const char* ini_filename);                    // this is automatically called (if io.IniFilename is not empty) a few seconds after any modification that should be reflected in the .ini file (and also by DestroyContext).
    IMGUI_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL);               // return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.

    // Memory Allocators
    // - All those functions are not reliant on the current context.
    // - If you reload the contents of imgui.cpp at runtime, you may need to call SetCurrentContext() + SetAllocatorFunctions() again because we use global storage for those.
    IMGUI_API void          SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = NULL);
    IMGUI_API void*         MemAlloc(size_t size);
    IMGUI_API void          MemFree(void* ptr);

} // namespace ImGui

//-----------------------------------------------------------------------------
// Flags & Enumerations
//-----------------------------------------------------------------------------

// Flags for ImGui::Begin()
enum ImGuiWindowFlags_
{
    ImGuiWindowFlags_None                   = 0,
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    ImGuiWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
    ImGuiWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    ImGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
    ImGuiWindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
    ImGuiWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    ImGuiWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
    ImGuiWindowFlags_NoNavInputs            = 1 << 18,  // No gamepad/keyboard navigation within the window
    ImGuiWindowFlags_NoNavFocus             = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
    ImGuiWindowFlags_UnsavedDocument        = 1 << 20,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
    ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    ImGuiWindowFlags_NoInputs               = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    // [Internal]
    ImGuiWindowFlags_NavFlattened           = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to cross over parent border to this child (only use on child that have no scrolling!)
    ImGuiWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
    ImGuiWindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
    ImGuiWindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
    ImGuiWindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
    ImGuiWindowFlags_ChildMenu              = 1 << 28   // Don't use! For internal use by BeginMenu()

    // [Obsolete]
    //ImGuiWindowFlags_ShowBorders          = 1 << 7,   // --> Set style.FrameBorderSize=1.0f or style.WindowBorderSize=1.0f to enable borders around items or windows.
    //ImGuiWindowFlags_ResizeFromAnySide    = 1 << 17,  // --> Set io.ConfigWindowsResizeFromEdges=true and make sure mouse cursors are supported by back-end (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)
};

// Flags for ImGui::InputText()
enum ImGuiInputTextFlags_
{
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-*/
    ImGuiInputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow 0123456789ABCDEFabcdef
    ImGuiInputTextFlags_CharsUppercase      = 1 << 2,   // Turn a..z into A..Z
    ImGuiInputTextFlags_CharsNoBlank        = 1 << 3,   // Filter out spaces, tabs
    ImGuiInputTextFlags_AutoSelectAll       = 1 << 4,   // Select entire text when first taking mouse focus
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
    ImGuiInputTextFlags_CallbackCompletion  = 1 << 6,   // Callback on pressing TAB (for completion handling)
    ImGuiInputTextFlags_CallbackHistory     = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
    ImGuiInputTextFlags_CallbackAlways      = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
    ImGuiInputTextFlags_CallbackCharFilter  = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
    ImGuiInputTextFlags_AllowTabInput       = 1 << 10,  // Pressing TAB input a '\t' character into the text field
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
    ImGuiInputTextFlags_NoHorizontalScroll  = 1 << 12,  // Disable following the cursor horizontally
    ImGuiInputTextFlags_AlwaysInsertMode    = 1 << 13,  // Insert mode
    ImGuiInputTextFlags_ReadOnly            = 1 << 14,  // Read-only mode
    ImGuiInputTextFlags_Password            = 1 << 15,  // Password mode, display all characters as '*'
    ImGuiInputTextFlags_NoUndoRedo          = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
    ImGuiInputTextFlags_CharsScientific     = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
    ImGuiInputTextFlags_CallbackResize      = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
    // [Internal]
    ImGuiInputTextFlags_Multiline           = 1 << 20,  // For internal use by InputTextMultiline()
    ImGuiInputTextFlags_NoMarkEdited        = 1 << 21   // For internal use by functions using InputText() before reformatting data
};

// Flags for ImGui::TreeNodeEx(), ImGui::CollapsingHeader*()
enum ImGuiTreeNodeFlags_
{
    ImGuiTreeNodeFlags_None                 = 0,
    ImGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
    ImGuiTreeNodeFlags_Framed               = 1 << 1,   // Full colored frame (e.g. for CollapsingHeader)
    ImGuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
    ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
    ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
    ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Need double-click to open node
    ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
    ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
    ImGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow
    ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
    ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
    ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (bypass the indented area).
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
    //ImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
    ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog
};

// Flags for ImGui::Selectable()
enum ImGuiSelectableFlags_
{
    ImGuiSelectableFlags_None               = 0,
    ImGuiSelectableFlags_DontClosePopups    = 1 << 0,   // Clicking this don't close parent popup window
    ImGuiSelectableFlags_SpanAllColumns     = 1 << 1,   // Selectable frame can span all columns (text will still fit in current column)
    ImGuiSelectableFlags_AllowDoubleClick   = 1 << 2,   // Generate press events on double clicks too
    ImGuiSelectableFlags_Disabled           = 1 << 3,   // Cannot be selected, display grayed out text
    ImGuiSelectableFlags_AllowItemOverlap   = 1 << 4    // (WIP) Hit testing to allow subsequent widgets to overlap this one
};

// Flags for ImGui::BeginCombo()
enum ImGuiComboFlags_
{
    ImGuiComboFlags_None                    = 0,
    ImGuiComboFlags_PopupAlignLeft          = 1 << 0,   // Align the popup toward the left by default
    ImGuiComboFlags_HeightSmall             = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    ImGuiComboFlags_HeightRegular           = 1 << 2,   // Max ~8 items visible (default)
    ImGuiComboFlags_HeightLarge             = 1 << 3,   // Max ~20 items visible
    ImGuiComboFlags_HeightLargest           = 1 << 4,   // As many fitting items as possible
    ImGuiComboFlags_NoArrowButton           = 1 << 5,   // Display on the preview box without the square arrow button
    ImGuiComboFlags_NoPreview               = 1 << 6,   // Display only a square arrow button
    ImGuiComboFlags_HeightMask_             = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest
};

// Flags for ImGui::BeginTabBar()
enum ImGuiTabBarFlags_
{
    ImGuiTabBarFlags_None                           = 0,
    ImGuiTabBarFlags_Reorderable                    = 1 << 0,   // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
    ImGuiTabBarFlags_AutoSelectNewTabs              = 1 << 1,   // Automatically select new tabs when they appear
    ImGuiTabBarFlags_TabListPopupButton             = 1 << 2,   // Disable buttons to open the tab list popup
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    ImGuiTabBarFlags_NoTabListScrollingButtons      = 1 << 4,   // Disable scrolling buttons (apply when fitting policy is ImGuiTabBarFlags_FittingPolicyScroll)
    ImGuiTabBarFlags_NoTooltip                      = 1 << 5,   // Disable tooltips when hovering a tab
    ImGuiTabBarFlags_FittingPolicyResizeDown        = 1 << 6,   // Resize tabs when they don't fit
    ImGuiTabBarFlags_FittingPolicyScroll            = 1 << 7,   // Add scroll buttons when tabs don't fit
    ImGuiTabBarFlags_FittingPolicyMask_             = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
    ImGuiTabBarFlags_FittingPolicyDefault_          = ImGuiTabBarFlags_FittingPolicyResizeDown
};

// Flags for ImGui::BeginTabItem()
enum ImGuiTabItemFlags_
{
    ImGuiTabItemFlags_None                          = 0,
    ImGuiTabItemFlags_UnsavedDocument               = 1 << 0,   // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. Also: tab is selected on closure and closure is deferred by one frame to allow code to undo it without flicker.
    ImGuiTabItemFlags_SetSelected                   = 1 << 1,   // Trigger flag to programmatically make the tab selected when calling BeginTabItem()
    ImGuiTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    ImGuiTabItemFlags_NoPushId                      = 1 << 3    // Don't call PushID(tab->ID)/PopID() on BeginTabItem()/EndTabItem()
};

// Flags for ImGui::IsWindowFocused()
enum ImGuiFocusedFlags_
{
    ImGuiFocusedFlags_None                          = 0,
    ImGuiFocusedFlags_ChildWindows                  = 1 << 0,   // IsWindowFocused(): Return true if any children of the window is focused
    ImGuiFocusedFlags_RootWindow                    = 1 << 1,   // IsWindowFocused(): Test from root window (top most parent of the current hierarchy)
    ImGuiFocusedFlags_AnyWindow                     = 1 << 2,   // IsWindowFocused(): Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use ImGui::GetIO().WantCaptureMouse instead.
    ImGuiFocusedFlags_RootAndChildWindows           = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows
};

// Flags for ImGui::IsItemHovered(), ImGui::IsWindowHovered()
// Note: if you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use the 'io.WantCaptureMouse' boolean for that. Please read the FAQ!
// Note: windows with the ImGuiWindowFlags_NoInputs flag are ignored by IsWindowHovered() calls.
enum ImGuiHoveredFlags_
{
    ImGuiHoveredFlags_None                          = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
    ImGuiHoveredFlags_ChildWindows                  = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
    ImGuiHoveredFlags_RootWindow                    = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
    ImGuiHoveredFlags_AnyWindow                     = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
    ImGuiHoveredFlags_AllowWhenBlockedByPopup       = 1 << 3,   // Return true even if a popup window is normally blocking access to this item/window
    //ImGuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 4,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 5,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
    ImGuiHoveredFlags_AllowWhenOverlapped           = 1 << 6,   // Return true even if the position is obstructed or overlapped by another window
    ImGuiHoveredFlags_AllowWhenDisabled             = 1 << 7,   // Return true even if the item is disabled
    ImGuiHoveredFlags_RectOnly                      = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows           = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows
};

// Flags for ImGui::BeginDragDropSource(), ImGui::AcceptDragDropPayload()
enum ImGuiDragDropFlags_
{
    ImGuiDragDropFlags_None                         = 0,
    // BeginDragDropSource() flags
    ImGuiDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,   // By default, a successful call to BeginDragDropSource opens a tooltip so you can display a preview or description of the source contents. This flag disable this behavior.
    ImGuiDragDropFlags_SourceNoDisableHover         = 1 << 1,   // By default, when dragging we clear data so that IsItemHovered() will return false, to avoid subsequent user code submitting tooltips. This flag disable this behavior so you can still call IsItemHovered() on the source item.
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,   // Disable the behavior that allows to open tree nodes and collapsing header by holding over them while dragging a source item.
    ImGuiDragDropFlags_SourceAllowNullID            = 1 << 3,   // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit.
    ImGuiDragDropFlags_SourceExtern                 = 1 << 4,   // External source (from outside of dear imgui), won't attempt to read current item/window info. Will always return true. Only one Extern source can be active simultaneously.
    ImGuiDragDropFlags_SourceAutoExpirePayload      = 1 << 5,   // Automatically expire the payload if the source cease to be submitted (otherwise payloads are persisting while being dragged)
    // AcceptDragDropPayload() flags
    ImGuiDragDropFlags_AcceptBeforeDelivery         = 1 << 10,  // AcceptDragDropPayload() will returns true even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,  // Do not draw the default highlight rectangle when hovering over target.
    ImGuiDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,  // Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.
    ImGuiDragDropFlags_AcceptPeekOnly               = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect  // For peeking ahead and inspecting the payload before delivery.
};

// Standard Drag and Drop payload types. You can define you own payload types using short strings. Types starting with '_' are defined by Dear ImGui.
#define IMGUI_PAYLOAD_TYPE_COLOR_3F     "_COL3F"    // float[3]: Standard type for colors, without alpha. User code may use this type.
#define IMGUI_PAYLOAD_TYPE_COLOR_4F     "_COL4F"    // float[4]: Standard type for colors. User code may use this type.

// A primary data type
enum ImGuiDataType_
{
    ImGuiDataType_S8,       // signed char / char (with sensible compilers)
    ImGuiDataType_U8,       // unsigned char
    ImGuiDataType_S16,      // short
    ImGuiDataType_U16,      // unsigned short
    ImGuiDataType_S32,      // int
    ImGuiDataType_U32,      // unsigned int
    ImGuiDataType_S64,      // long long / __int64
    ImGuiDataType_U64,      // unsigned long long / unsigned __int64
    ImGuiDataType_Float,    // float
    ImGuiDataType_Double,   // double
    ImGuiDataType_COUNT
};

// A cardinal direction
enum ImGuiDir_
{
    ImGuiDir_None    = -1,
    ImGuiDir_Left    = 0,
    ImGuiDir_Right   = 1,
    ImGuiDir_Up      = 2,
    ImGuiDir_Down    = 3,
    ImGuiDir_COUNT
};

// User fill ImGuiIO.KeyMap[] array with indices into the ImGuiIO.KeysDown[512] array
enum ImGuiKey_
{
    ImGuiKey_Tab,
    ImGuiKey_LeftArrow,
    ImGuiKey_RightArrow,
    ImGuiKey_UpArrow,
    ImGuiKey_DownArrow,
    ImGuiKey_PageUp,
    ImGuiKey_PageDown,
    ImGuiKey_Home,
    ImGuiKey_End,
    ImGuiKey_Insert,
    ImGuiKey_Delete,
    ImGuiKey_Backspace,
    ImGuiKey_Space,
    ImGuiKey_Enter,
    ImGuiKey_Escape,
    ImGuiKey_KeyPadEnter,
    ImGuiKey_A,                 // for text edit CTRL+A: select all
    ImGuiKey_C,                 // for text edit CTRL+C: copy
    ImGuiKey_V,                 // for text edit CTRL+V: paste
    ImGuiKey_X,                 // for text edit CTRL+X: cut
    ImGuiKey_Y,                 // for text edit CTRL+Y: redo
    ImGuiKey_Z,                 // for text edit CTRL+Z: undo
    ImGuiKey_COUNT
};

// Gamepad/Keyboard directional navigation
// Keyboard: Set io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard to enable. NewFrame() will automatically fill io.NavInputs[] based on your io.KeysDown[] + io.KeyMap[] arrays.
// Gamepad:  Set io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad to enable. Back-end: set ImGuiBackendFlags_HasGamepad and fill the io.NavInputs[] fields before calling NewFrame(). Note that io.NavInputs[] is cleared by EndFrame().
// Read instructions in imgui.cpp for more details. Download PNG/PSD at http://goo.gl/9LgVZW.
enum ImGuiNavInput_
{
    // Gamepad Mapping
    ImGuiNavInput_Activate,      // activate / open / toggle / tweak value       // e.g. Cross  (PS4), A (Xbox), A (Switch), Space (Keyboard)
    ImGuiNavInput_Cancel,        // cancel / close / exit                        // e.g. Circle (PS4), B (Xbox), B (Switch), Escape (Keyboard)
    ImGuiNavInput_Input,         // text input / on-screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
    ImGuiNavInput_Menu,          // tap: toggle menu / hold: focus, move, resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
    ImGuiNavInput_DpadLeft,      // move / tweak / resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
    ImGuiNavInput_DpadRight,     //
    ImGuiNavInput_DpadUp,        //
    ImGuiNavInput_DpadDown,      //
    ImGuiNavInput_LStickLeft,    // scroll / move window (w/ PadMenu)            // e.g. Left Analog Stick Left/Right/Up/Down
    ImGuiNavInput_LStickRight,   //
    ImGuiNavInput_LStickUp,      //
    ImGuiNavInput_LStickDown,    //
    ImGuiNavInput_FocusPrev,     // next window (w/ PadMenu)                     // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    ImGuiNavInput_FocusNext,     // prev window (w/ PadMenu)                     // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)
    ImGuiNavInput_TweakSlow,     // slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    ImGuiNavInput_TweakFast,     // faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)

    // [Internal] Don't use directly! This is used internally to differentiate keyboard from gamepad inputs for behaviors that require to differentiate them.
    // Keyboard behavior that have no corresponding gamepad mapping (e.g. CTRL+TAB) will be directly reading from io.KeysDown[] instead of io.NavInputs[].
    ImGuiNavInput_KeyMenu_,      // toggle menu                                  // = io.KeyAlt
    ImGuiNavInput_KeyLeft_,      // move left                                    // = Arrow keys
    ImGuiNavInput_KeyRight_,     // move right
    ImGuiNavInput_KeyUp_,        // move up
    ImGuiNavInput_KeyDown_,      // move down
    ImGuiNavInput_COUNT,
    ImGuiNavInput_InternalStart_ = ImGuiNavInput_KeyMenu_
};

// Configuration flags stored in io.ConfigFlags. Set by user/application.
enum ImGuiConfigFlags_
{
    ImGuiConfigFlags_None                   = 0,
    ImGuiConfigFlags_NavEnableKeyboard      = 1 << 0,   // Master keyboard navigation enable flag. NewFrame() will automatically fill io.NavInputs[] based on io.KeysDown[].
    ImGuiConfigFlags_NavEnableGamepad       = 1 << 1,   // Master gamepad navigation enable flag. This is mostly to instruct your imgui back-end to fill io.NavInputs[]. Back-end also needs to set ImGuiBackendFlags_HasGamepad.
    ImGuiConfigFlags_NavEnableSetMousePos   = 1 << 2,   // Instruct navigation to move the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is awkward. Will update io.MousePos and set io.WantSetMousePos=true. If enabled you MUST honor io.WantSetMousePos requests in your binding, otherwise ImGui will react as if the mouse is jumping around back and forth.
    ImGuiConfigFlags_NavNoCaptureKeyboard   = 1 << 3,   // Instruct navigation to not set the io.WantCaptureKeyboard flag when io.NavActive is set.
    ImGuiConfigFlags_NoMouse                = 1 << 4,   // Instruct imgui to clear mouse position/buttons in NewFrame(). This allows ignoring the mouse information set by the back-end.
    ImGuiConfigFlags_NoMouseCursorChange    = 1 << 5,   // Instruct back-end to not alter mouse cursor shape and visibility. Use if the back-end cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from imgui by reading GetMouseCursor() yourself instead.

    // User storage (to allow your back-end/engine to communicate to code that may be shared between multiple projects. Those flags are not used by core Dear ImGui)
    ImGuiConfigFlags_IsSRGB                 = 1 << 20,  // Application is SRGB-aware.
    ImGuiConfigFlags_IsTouchScreen          = 1 << 21   // Application is using a touch screen instead of a mouse.
};

// Back-end capabilities flags stored in io.BackendFlags. Set by imgui_impl_xxx or custom back-end.
enum ImGuiBackendFlags_
{
    ImGuiBackendFlags_None                  = 0,
    ImGuiBackendFlags_HasGamepad            = 1 << 0,   // Back-end Platform supports gamepad and currently has one connected.
    ImGuiBackendFlags_HasMouseCursors       = 1 << 1,   // Back-end Platform supports honoring GetMouseCursor() value to change the OS cursor shape.
    ImGuiBackendFlags_HasSetMousePos        = 1 << 2,   // Back-end Platform supports io.WantSetMousePos requests to reposition the OS mouse position (only used if ImGuiConfigFlags_NavEnableSetMousePos is set).
    ImGuiBackendFlags_RendererHasVtxOffset  = 1 << 3    // Back-end Renderer supports ImDrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bit indices.
};

// Enumeration for PushStyleColor() / PopStyleColor()
enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,              // Background of normal windows
    ImGuiCol_ChildBg,               // Background of child windows
    ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
    ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
    ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
    ImGuiCol_COUNT

    // Obsolete names (will be removed)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiCol_ModalWindowDarkening = ImGuiCol_ModalWindowDimBg                      // [renamed in 1.63]
    //, ImGuiCol_CloseButton, ImGuiCol_CloseButtonActive, ImGuiCol_CloseButtonHovered// [unused since 1.60+] the close button now uses regular button colors.
#endif
};

// Enumeration for PushStyleVar() / PopStyleVar() to temporarily modify the ImGuiStyle structure.
// NB: the enum only refers to fields of ImGuiStyle which makes sense to be pushed/popped inside UI code. During initialization, feel free to just poke into ImGuiStyle directly.
// NB: if changing this enum, you need to update the associated internal table GStyleVarInfo[] accordingly. This is where we link enum values to members offset/type.
enum ImGuiStyleVar_
{
    // Enum name --------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
    ImGuiStyleVar_Alpha,               // float     Alpha
    ImGuiStyleVar_WindowPadding,       // ImVec2    WindowPadding
    ImGuiStyleVar_WindowRounding,      // float     WindowRounding
    ImGuiStyleVar_WindowBorderSize,    // float     WindowBorderSize
    ImGuiStyleVar_WindowMinSize,       // ImVec2    WindowMinSize
    ImGuiStyleVar_WindowTitleAlign,    // ImVec2    WindowTitleAlign
    ImGuiStyleVar_ChildRounding,       // float     ChildRounding
    ImGuiStyleVar_ChildBorderSize,     // float     ChildBorderSize
    ImGuiStyleVar_PopupRounding,       // float     PopupRounding
    ImGuiStyleVar_PopupBorderSize,     // float     PopupBorderSize
    ImGuiStyleVar_FramePadding,        // ImVec2    FramePadding
    ImGuiStyleVar_FrameRounding,       // float     FrameRounding
    ImGuiStyleVar_FrameBorderSize,     // float     FrameBorderSize
    ImGuiStyleVar_ItemSpacing,         // ImVec2    ItemSpacing
    ImGuiStyleVar_ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
    ImGuiStyleVar_IndentSpacing,       // float     IndentSpacing
    ImGuiStyleVar_ScrollbarSize,       // float     ScrollbarSize
    ImGuiStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
    ImGuiStyleVar_GrabMinSize,         // float     GrabMinSize
    ImGuiStyleVar_GrabRounding,        // float     GrabRounding
    ImGuiStyleVar_TabRounding,         // float     TabRounding
    ImGuiStyleVar_ButtonTextAlign,     // ImVec2    ButtonTextAlign
    ImGuiStyleVar_SelectableTextAlign, // ImVec2    SelectableTextAlign
    ImGuiStyleVar_COUNT

    // Obsolete names (will be removed)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiStyleVar_Count_ = ImGuiStyleVar_COUNT                    // [renamed in 1.60]
#endif
};

// Flags for ColorEdit3() / ColorEdit4() / ColorPicker3() / ColorPicker4() / ColorButton()
enum ImGuiColorEditFlags_
{
    ImGuiColorEditFlags_None            = 0,
    ImGuiColorEditFlags_NoAlpha         = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
    ImGuiColorEditFlags_NoPicker        = 1 << 2,   //              // ColorEdit: disable picker when clicking on colored square.
    ImGuiColorEditFlags_NoOptions       = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
    ImGuiColorEditFlags_NoSmallPreview  = 1 << 4,   //              // ColorEdit, ColorPicker: disable colored square preview next to the inputs. (e.g. to show only the inputs)
    ImGuiColorEditFlags_NoInputs        = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview colored square).
    ImGuiColorEditFlags_NoTooltip       = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
    ImGuiColorEditFlags_NoLabel         = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
    ImGuiColorEditFlags_NoSidePreview   = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small colored square preview instead.
    ImGuiColorEditFlags_NoDragDrop      = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.

    // User Options (right-click on widget to change some of them).
    ImGuiColorEditFlags_AlphaBar        = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
    ImGuiColorEditFlags_AlphaPreview    = 1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
    ImGuiColorEditFlags_AlphaPreviewHalf= 1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
    ImGuiColorEditFlags_HDR             = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use ImGuiColorEditFlags_Float flag as well).
    ImGuiColorEditFlags_DisplayRGB      = 1 << 20,  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    ImGuiColorEditFlags_DisplayHSV      = 1 << 21,  // [Display]    // "
    ImGuiColorEditFlags_DisplayHex      = 1 << 22,  // [Display]    // "
    ImGuiColorEditFlags_Uint8           = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
    ImGuiColorEditFlags_Float           = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
    ImGuiColorEditFlags_PickerHueBar    = 1 << 25,  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
    ImGuiColorEditFlags_PickerHueWheel  = 1 << 26,  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
    ImGuiColorEditFlags_InputRGB        = 1 << 27,  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
    ImGuiColorEditFlags_InputHSV        = 1 << 28,  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.

    // Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to
    // override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.
    ImGuiColorEditFlags__OptionsDefault = ImGuiColorEditFlags_Uint8|ImGuiColorEditFlags_DisplayRGB|ImGuiColorEditFlags_InputRGB|ImGuiColorEditFlags_PickerHueBar,

    // [Internal] Masks
    ImGuiColorEditFlags__DisplayMask    = ImGuiColorEditFlags_DisplayRGB|ImGuiColorEditFlags_DisplayHSV|ImGuiColorEditFlags_DisplayHex,
    ImGuiColorEditFlags__DataTypeMask   = ImGuiColorEditFlags_Uint8|ImGuiColorEditFlags_Float,
    ImGuiColorEditFlags__PickerMask     = ImGuiColorEditFlags_PickerHueWheel|ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags__InputMask      = ImGuiColorEditFlags_InputRGB|ImGuiColorEditFlags_InputHSV

    // Obsolete names (will be removed)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiColorEditFlags_RGB = ImGuiColorEditFlags_DisplayRGB, ImGuiColorEditFlags_HSV = ImGuiColorEditFlags_DisplayHSV, ImGuiColorEditFlags_HEX = ImGuiColorEditFlags_DisplayHex  // [renamed in 1.69]
#endif
};

// Identify a mouse button.
// Those values are guaranteed to be stable and we frequently use 0/1 directly. Named enums provided for convenience.
enum ImGuiMouseButton_
{
    ImGuiMouseButton_Left = 0,
    ImGuiMouseButton_Right = 1,
    ImGuiMouseButton_Middle = 2,
    ImGuiMouseButton_COUNT = 5
};

// Enumeration for GetMouseCursor()
// User code may request binding to display given cursor by calling SetMouseCursor(), which is why we have some cursors that are marked unused here
enum ImGuiMouseCursor_
{
    ImGuiMouseCursor_None = -1,
    ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput,         // When hovering over InputText, etc.
    ImGuiMouseCursor_ResizeAll,         // (Unused by Dear ImGui functions)
    ImGuiMouseCursor_ResizeNS,          // When hovering over an horizontal border
    ImGuiMouseCursor_ResizeEW,          // When hovering over a vertical border or a column
    ImGuiMouseCursor_ResizeNESW,        // When hovering over the bottom-left corner of a window
    ImGuiMouseCursor_ResizeNWSE,        // When hovering over the bottom-right corner of a window
    ImGuiMouseCursor_Hand,              // (Unused by Dear ImGui functions. Use for e.g. hyperlinks)
    ImGuiMouseCursor_NotAllowed,        // When hovering something with disallowed interaction. Usually a crossed circle.
    ImGuiMouseCursor_COUNT

    // Obsolete names (will be removed)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiMouseCursor_Count_ = ImGuiMouseCursor_COUNT      // [renamed in 1.60]
#endif
};

// Enumeration for ImGui::SetWindow***(), SetNextWindow***(), SetNextItem***() functions
// Represent a condition.
// Important: Treat as a regular enum! Do NOT combine multiple values using binary operators! All the functions above treat 0 as a shortcut to ImGuiCond_Always.
enum ImGuiCond_
{
    ImGuiCond_Always        = 1 << 0,   // Set the variable
    ImGuiCond_Once          = 1 << 1,   // Set the variable once per runtime session (only the first call with succeed)
    ImGuiCond_FirstUseEver  = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    ImGuiCond_Appearing     = 1 << 3    // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};

//-----------------------------------------------------------------------------
// Helpers: Memory allocations macros
// IM_MALLOC(), IM_FREE(), IM_NEW(), IM_PLACEMENT_NEW(), IM_DELETE()
// We call C++ constructor on own allocated memory via the placement "new(ptr) Type()" syntax.
// Defining a custom placement new() with a dummy parameter allows us to bypass including <new> which on some platforms complains when user has disabled exceptions.
//-----------------------------------------------------------------------------

struct ImNewDummy {};
inline void* operator new(size_t, ImNewDummy, void* ptr) { return ptr; }
inline void  operator delete(void*, ImNewDummy, void*)   {} // This is only required so we can use the symmetrical new()
#define IM_ALLOC(_SIZE)                     ImGui::MemAlloc(_SIZE)
#define IM_FREE(_PTR)                       ImGui::MemFree(_PTR)
#define IM_PLACEMENT_NEW(_PTR)              new(ImNewDummy(), _PTR)
#define IM_NEW(_TYPE)                       new(ImNewDummy(), ImGui::MemAlloc(sizeof(_TYPE))) _TYPE
template<typename T> void IM_DELETE(T* p)   { if (p) { p->~T(); ImGui::MemFree(p); } }

//-----------------------------------------------------------------------------
// Helper: ImVector<>
// Lightweight std::vector<>-like class to avoid dragging dependencies (also, some implementations of STL with debug enabled are absurdly slow, we bypass it so our code runs fast in debug).
//-----------------------------------------------------------------------------
// - You generally do NOT need to care or use this ever. But we need to make it available in imgui.h because some of our public structures are relying on it.
// - We use std-like naming convention here, which is a little unusual for this codebase.
// - Important: clear() frees memory, resize(0) keep the allocated buffer. We use resize(0) a lot to intentionally recycle allocated buffers across frames and amortize our costs.
// - Important: our implementation does NOT call C++ constructors/destructors, we treat everything as raw data! This is intentional but be extra mindful of that,
//   Do NOT use this class as a std::vector replacement in your own code! Many of the structures used by dear imgui can be safely initialized by a zero-memset.
//-----------------------------------------------------------------------------

template<typename T>
struct ImVector
{
    int                 Size;
    int                 Capacity;
    T*                  Data;

    // Provide standard typedefs but we don't use them ourselves.
    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    // Constructors, destructor
    inline ImVector()                                       { Size = Capacity = 0; Data = NULL; }
    inline ImVector(const ImVector<T>& src)                 { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline ImVector<T>& operator=(const ImVector<T>& src)   { clear(); resize(src.Size); memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~ImVector()                                      { if (Data) IM_FREE(Data); }

    inline bool         empty() const                       { return Size == 0; }
    inline int          size() const                        { return Size; }
    inline int          size_in_bytes() const               { return Size * (int)sizeof(T); }
    inline int          capacity() const                    { return Capacity; }
    inline T&           operator[](int i)                   { IM_ASSERT(i < Size); return Data[i]; }
    inline const T&     operator[](int i) const             { IM_ASSERT(i < Size); return Data[i]; }

    inline void         clear()                             { if (Data) { Size = Capacity = 0; IM_FREE(Data); Data = NULL; } }
    inline T*           begin()                             { return Data; }
    inline const T*     begin() const                       { return Data; }
    inline T*           end()                               { return Data + Size; }
    inline const T*     end() const                         { return Data + Size; }
    inline T&           front()                             { IM_ASSERT(Size > 0); return Data[0]; }
    inline const T&     front() const                       { IM_ASSERT(Size > 0); return Data[0]; }
    inline T&           back()                              { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const T&     back() const                        { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void         swap(ImVector<T>& rhs)              { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const        { int new_capacity = Capacity ? (Capacity + Capacity/2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size)                { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         shrink(int new_size)                { IM_ASSERT(new_size <= Size); Size = new_size; } // Resize a vector to a smaller size, guaranteed not to cause a reallocation
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }

    // NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the ImVector data itself! e.g. v.push_back(v[10]) is forbidden.
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         pop_back()                          { IM_ASSERT(Size > 0); Size--; }
    inline void         push_front(const T& v)              { if (Size == 0) push_back(v); else insert(Data, v); }
    inline T*           erase(const T* it)                  { IM_ASSERT(it >= Data && it < Data+Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
    inline T*           erase(const T* it, const T* it_last){ IM_ASSERT(it >= Data && it < Data+Size && it_last > it && it_last <= Data+Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - count) * sizeof(T)); Size -= (int)count; return Data + off; }
    inline T*           erase_unsorted(const T* it)         { IM_ASSERT(it >= Data && it < Data+Size);  const ptrdiff_t off = it - Data; if (it < Data+Size-1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
    inline T*           insert(const T* it, const T& v)     { IM_ASSERT(it >= Data && it <= Data+Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const T& v) const          { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline T*           find(const T& v)                    { T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline const T*     find(const T& v) const              { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline bool         find_erase(const T& v)              { const T* it = find(v); if (it < Data + Size) { erase(it); return true; } return false; }
    inline bool         find_erase_unsorted(const T& v)     { const T* it = find(v); if (it < Data + Size) { erase_unsorted(it); return true; } return false; }
    inline int          index_from_ptr(const T* it) const   { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; return (int)off; }
};

//-----------------------------------------------------------------------------
// ImGuiStyle
// You may modify the ImGui::GetStyle() main instance during initialization and before NewFrame().
// During the frame, use ImGui::PushStyleVar(ImGuiStyleVar_XXXX)/PopStyleVar() to alter the main style values,
// and ImGui::PushStyleColor(ImGuiCol_XXX)/PopStyleColor() for colors.
//-----------------------------------------------------------------------------

struct ImGuiStyle
{
    float       Alpha;                      // Global alpha applies to everything in Dear ImGui.
    ImVec2      WindowPadding;              // Padding within a window.
    float       WindowRounding;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows.
    float       WindowBorderSize;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2      WindowMinSize;              // Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().
    ImVec2      WindowTitleAlign;           // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
    ImGuiDir    WindowMenuButtonPosition;   // Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to ImGuiDir_Left.
    float       ChildRounding;              // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
    float       ChildBorderSize;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    float       PopupRounding;              // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
    float       PopupBorderSize;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2      FramePadding;               // Padding within a framed rectangle (used by most widgets).
    float       FrameRounding;              // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    float       FrameBorderSize;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2      ItemSpacing;                // Horizontal and vertical spacing between widgets/lines.
    ImVec2      ItemInnerSpacing;           // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
    ImVec2      TouchExtraPadding;          // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    float       IndentSpacing;              // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    float       ColumnsMinSpacing;          // Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
    float       ScrollbarSize;              // Width of the vertical scrollbar, Height of the horizontal scrollbar.
    float       ScrollbarRounding;          // Radius of grab corners for scrollbar.
    float       GrabMinSize;                // Minimum width/height of a grab box for slider/scrollbar.
    float       GrabRounding;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    float       TabRounding;                // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
    float       TabBorderSize;              // Thickness of border around tabs.
    ImGuiDir    ColorButtonPosition;        // Side of the color button in the ColorEdit4 widget (left/right). Defaults to ImGuiDir_Right.
    ImVec2      ButtonTextAlign;            // Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
    ImVec2      SelectableTextAlign;        // Alignment of selectable text when selectable is larger than text. Defaults to (0.0f, 0.0f) (top-left aligned).
    ImVec2      DisplayWindowPadding;       // Window position are clamped to be visible within the display area by at least this amount. Only applies to regular windows.
    ImVec2      DisplaySafeAreaPadding;     // If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
    float       MouseCursorScale;           // Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
    bool        AntiAliasedLines;           // Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
    bool        AntiAliasedFill;            // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    float       CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    float       CircleSegmentMaxError;      // Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
    ImVec4      Colors[ImGuiCol_COUNT];

    IMGUI_API ImGuiStyle();
    IMGUI_API void ScaleAllSizes(float scale_factor);
};

//-----------------------------------------------------------------------------
// ImGuiIO
// Communicate most settings and inputs/outputs to Dear ImGui using this structure.
// Access via ImGui::GetIO(). Read 'Programmer guide' section in .cpp file for general usage.
//-----------------------------------------------------------------------------

struct ImGuiIO
{
    //------------------------------------------------------------------
    // Configuration (fill once)                // Default value
    //------------------------------------------------------------------

    ImGuiConfigFlags   ConfigFlags;             // = 0              // See ImGuiConfigFlags_ enum. Set by user/application. Gamepad/keyboard navigation options, etc.
    ImGuiBackendFlags  BackendFlags;            // = 0              // See ImGuiBackendFlags_ enum. Set by back-end (imgui_impl_xxx files or custom back-end) to communicate features supported by the back-end.
    ImVec2      DisplaySize;                    // <unset>          // Main display size, in pixels.
    float       DeltaTime;                      // = 1.0f/60.0f     // Time elapsed since last frame, in seconds.
    float       IniSavingRate;                  // = 5.0f           // Minimum time between saving positions/sizes to .ini file, in seconds.
    const char* IniFilename;                    // = "imgui.ini"    // Path to .ini file. Set NULL to disable automatic .ini loading/saving, if e.g. you want to manually load/save from memory.
    const char* LogFilename;                    // = "imgui_log.txt"// Path to .log file (default parameter to ImGui::LogToFile when no file is specified).
    float       MouseDoubleClickTime;           // = 0.30f          // Time for a double-click, in seconds.
    float       MouseDoubleClickMaxDist;        // = 6.0f           // Distance threshold to stay in to validate a double-click, in pixels.
    float       MouseDragThreshold;             // = 6.0f           // Distance threshold before considering we are dragging.
    int         KeyMap[ImGuiKey_COUNT];         // <unset>          // Map of indices into the KeysDown[512] entries array which represent your "native" keyboard state.
    float       KeyRepeatDelay;                 // = 0.250f         // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
    float       KeyRepeatRate;                  // = 0.050f         // When holding a key/button, rate at which it repeats, in seconds.
    void*       UserData;                       // = NULL           // Store your own data for retrieval by callbacks.

    ImFontAtlas*Fonts;                          // <auto>           // Font atlas: load, rasterize and pack one or more fonts into a single texture.
    float       FontGlobalScale;                // = 1.0f           // Global scale all fonts
    bool        FontAllowUserScaling;           // = false          // Allow user scaling text of individual window with CTRL+Wheel.
    ImFont*     FontDefault;                    // = NULL           // Font to use on NewFrame(). Use NULL to uses Fonts->Fonts[0].
    ImVec2      DisplayFramebufferScale;        // = (1, 1)         // For retina display or other situations where window coordinates are different from framebuffer coordinates. This generally ends up in ImDrawData::FramebufferScale.

    // Miscellaneous options
    bool        MouseDrawCursor;                // = false          // Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor). Cannot be easily renamed to 'io.ConfigXXX' because this is frequently used by back-end implementations.
    bool        ConfigMacOSXBehaviors;          // = defined(__APPLE__) // OS X style: Text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl (was called io.OptMacOSXBehaviors prior to 1.63)
    bool        ConfigInputTextCursorBlink;     // = true           // Set to false to disable blinking cursor, for users who consider it distracting. (was called: io.OptCursorBlink prior to 1.63)
    bool        ConfigWindowsResizeFromEdges;   // = true           // Enable resizing of windows from their edges and from the lower-left corner. This requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback. (This used to be a per-window ImGuiWindowFlags_ResizeFromAnySide flag)
    bool        ConfigWindowsMoveFromTitleBarOnly; // = false       // [BETA] Set to true to only allow moving windows when clicked+dragged from the title bar. Windows without a title bar are not affected.
    float       ConfigWindowsMemoryCompactTimer;// = 60.0f          // [BETA] Compact window memory usage when unused. Set to -1.0f to disable.

    //------------------------------------------------------------------
    // Platform Functions
    // (the imgui_impl_xxxx back-end files are setting those up for you)
    //------------------------------------------------------------------

    // Optional: Platform/Renderer back-end name (informational only! will be displayed in About Window) + User data for back-end/wrappers to store their own stuff.
    const char* BackendPlatformName;            // = NULL
    const char* BackendRendererName;            // = NULL
    void*       BackendPlatformUserData;        // = NULL           // User data for platform back-end
    void*       BackendRendererUserData;        // = NULL           // User data for renderer back-end
    void*       BackendLanguageUserData;        // = NULL           // User data for non C++ programming language back-end

    // Optional: Access OS clipboard
    // (default to use native Win32 clipboard on Windows, otherwise uses a private clipboard. Override to access OS clipboard on other architectures)
    const char* (*GetClipboardTextFn)(void* user_data);
    void        (*SetClipboardTextFn)(void* user_data, const char* text);
    void*       ClipboardUserData;

    // Optional: Notify OS Input Method Editor of the screen position of your cursor for text input position (e.g. when using Japanese/Chinese IME on Windows)
    // (default to use native imm32 api on Windows)
    void        (*ImeSetInputScreenPosFn)(int x, int y);
    void*       ImeWindowHandle;                // = NULL           // (Windows) Set this to your HWND to get automatic IME cursor positioning.

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    // [OBSOLETE since 1.60+] Rendering function, will be automatically called in Render(). Please call your rendering function yourself now!
    // You can obtain the ImDrawData* by calling ImGui::GetDrawData() after Render(). See example applications if you are unsure of how to implement this.
    void        (*RenderDrawListsFn)(ImDrawData* data);
#else
    // This is only here to keep ImGuiIO the same size/layout, so that IMGUI_DISABLE_OBSOLETE_FUNCTIONS can exceptionally be used outside of imconfig.h.
    void*       RenderDrawListsFnUnused;
#endif

    //------------------------------------------------------------------
    // Input - Fill before calling NewFrame()
    //------------------------------------------------------------------

    ImVec2      MousePos;                       // Mouse position, in pixels. Set to ImVec2(-FLT_MAX,-FLT_MAX) if mouse is unavailable (on another screen, etc.)
    bool        MouseDown[5];                   // Mouse buttons: 0=left, 1=right, 2=middle + extras. ImGui itself mostly only uses left button (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
    float       MouseWheel;                     // Mouse wheel Vertical: 1 unit scrolls about 5 lines text.
    float       MouseWheelH;                    // Mouse wheel Horizontal. Most users don't have a mouse with an horizontal wheel, may not be filled by all back-ends.
    bool        KeyCtrl;                        // Keyboard modifier pressed: Control
    bool        KeyShift;                       // Keyboard modifier pressed: Shift
    bool        KeyAlt;                         // Keyboard modifier pressed: Alt
    bool        KeySuper;                       // Keyboard modifier pressed: Cmd/Super/Windows
    bool        KeysDown[512];                  // Keyboard keys that are pressed (ideally left in the "native" order your engine has access to keyboard keys, so you can use your own defines/enums for keys).
    float       NavInputs[ImGuiNavInput_COUNT]; // Gamepad inputs. Cleared back to zero by EndFrame(). Keyboard keys will be auto-mapped and be written here by NewFrame().

    // Functions
    IMGUI_API void  AddInputCharacter(unsigned int c);          // Queue new character input
    IMGUI_API void  AddInputCharactersUTF8(const char* str);    // Queue new characters input from an UTF-8 string
    IMGUI_API void  ClearInputCharacters();                     // Clear the text input buffer manually

    //------------------------------------------------------------------
    // Output - Retrieve after calling NewFrame()
    //------------------------------------------------------------------

    bool        WantCaptureMouse;               // When io.WantCaptureMouse is true, imgui will use the mouse inputs, do not dispatch them to your main game/application (in both cases, always pass on mouse inputs to imgui). (e.g. unclicked mouse is hovering over an imgui window, widget is active, mouse was clicked over an imgui window, etc.).
    bool        WantCaptureKeyboard;            // When io.WantCaptureKeyboard is true, imgui will use the keyboard inputs, do not dispatch them to your main game/application (in both cases, always pass keyboard inputs to imgui). (e.g. InputText active, or an imgui window is focused and navigation is enabled, etc.).
    bool        WantTextInput;                  // Mobile/console: when io.WantTextInput is true, you may display an on-screen keyboard. This is set by ImGui when it wants textual keyboard input to happen (e.g. when a InputText widget is active).
    bool        WantSetMousePos;                // MousePos has been altered, back-end should reposition mouse on next frame. Set only when ImGuiConfigFlags_NavEnableSetMousePos flag is enabled.
    bool        WantSaveIniSettings;            // When manual .ini load/save is active (io.IniFilename == NULL), this will be set to notify your application that you can call SaveIniSettingsToMemory() and save yourself. IMPORTANT: You need to clear io.WantSaveIniSettings yourself.
    bool        NavActive;                      // Directional navigation is currently allowed (will handle ImGuiKey_NavXXX events) = a window is focused and it doesn't use the ImGuiWindowFlags_NoNavInputs flag.
    bool        NavVisible;                     // Directional navigation is visible and allowed (will handle ImGuiKey_NavXXX events).
    float       Framerate;                      // Application framerate estimation, in frame per second. Solely for convenience. Rolling average estimation based on IO.DeltaTime over 120 frames
    int         MetricsRenderVertices;          // Vertices output during last call to Render()
    int         MetricsRenderIndices;           // Indices output during last call to Render() = number of triangles * 3
    int         MetricsRenderWindows;           // Number of visible windows
    int         MetricsActiveWindows;           // Number of active windows
    int         MetricsActiveAllocations;       // Number of active allocations, updated by MemAlloc/MemFree based on current context. May be off if you have multiple imgui contexts.
    ImVec2      MouseDelta;                     // Mouse delta. Note that this is zero if either current or previous position are invalid (-FLT_MAX,-FLT_MAX), so a disappearing/reappearing mouse won't have a huge delta.

    //------------------------------------------------------------------
    // [Internal] Dear ImGui will maintain those fields. Forward compatibility not guaranteed!
    //------------------------------------------------------------------

    ImVec2      MousePosPrev;                   // Previous mouse position (note that MouseDelta is not necessary == MousePos-MousePosPrev, in case either position is invalid)
    ImVec2      MouseClickedPos[5];             // Position at time of clicking
    double      MouseClickedTime[5];            // Time of last click (used to figure out double-click)
    bool        MouseClicked[5];                // Mouse button went from !Down to Down
    bool        MouseDoubleClicked[5];          // Has mouse button been double-clicked?
    bool        MouseReleased[5];               // Mouse button went from Down to !Down
    bool        MouseDownOwned[5];              // Track if button was clicked inside a dear imgui window. We don't request mouse capture from the application if click started outside ImGui bounds.
    bool        MouseDownWasDoubleClick[5];     // Track if button down was a double-click
    float       MouseDownDuration[5];           // Duration the mouse button has been down (0.0f == just clicked)
    float       MouseDownDurationPrev[5];       // Previous time the mouse button has been down
    ImVec2      MouseDragMaxDistanceAbs[5];     // Maximum distance, absolute, on each axis, of how much mouse has traveled from the clicking point
    float       MouseDragMaxDistanceSqr[5];     // Squared maximum distance of how much mouse has traveled from the clicking point
    float       KeysDownDuration[512];          // Duration the keyboard key has been down (0.0f == just pressed)
    float       KeysDownDurationPrev[512];      // Previous duration the key has been down
    float       NavInputsDownDuration[ImGuiNavInput_COUNT];
    float       NavInputsDownDurationPrev[ImGuiNavInput_COUNT];
    ImVector<ImWchar> InputQueueCharacters;     // Queue of _characters_ input (obtained by platform back-end). Fill using AddInputCharacter() helper.

    IMGUI_API   ImGuiIO();
};

//-----------------------------------------------------------------------------
// Misc data structures
//-----------------------------------------------------------------------------

// Shared state of InputText(), passed as an argument to your callback when a ImGuiInputTextFlags_Callback* flag is used.
// The callback function should return 0 by default.
// Callbacks (follow a flag name and see comments in ImGuiInputTextFlags_ declarations for more details)
// - ImGuiInputTextFlags_CallbackCompletion:  Callback on pressing TAB
// - ImGuiInputTextFlags_CallbackHistory:     Callback on pressing Up/Down arrows
// - ImGuiInputTextFlags_CallbackAlways:      Callback on each iteration
// - ImGuiInputTextFlags_CallbackCharFilter:  Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
// - ImGuiInputTextFlags_CallbackResize:      Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow.
struct ImGuiInputTextCallbackData
{
    ImGuiInputTextFlags EventFlag;      // One ImGuiInputTextFlags_Callback*    // Read-only
    ImGuiInputTextFlags Flags;          // What user passed to InputText()      // Read-only
    void*               UserData;       // What user passed to InputText()      // Read-only

    // Arguments for the different callback events
    // - To modify the text buffer in a callback, prefer using the InsertChars() / DeleteChars() function. InsertChars() will take care of calling the resize callback if necessary.
    // - If you know your edits are not going to resize the underlying buffer allocation, you may modify the contents of 'Buf[]' directly. You need to update 'BufTextLen' accordingly (0 <= BufTextLen < BufSize) and set 'BufDirty'' to true so InputText can update its internal state.
    ImWchar             EventChar;      // Character input                      // Read-write   // [CharFilter] Replace character with another one, or set to zero to drop. return 1 is equivalent to setting EventChar=0;
    ImGuiKey            EventKey;       // Key pressed (Up/Down/TAB)            // Read-only    // [Completion,History]
    char*               Buf;            // Text buffer                          // Read-write   // [Resize] Can replace pointer / [Completion,History,Always] Only write to pointed data, don't replace the actual pointer!
    int                 BufTextLen;     // Text length (in bytes)               // Read-write   // [Resize,Completion,History,Always] Exclude zero-terminator storage. In C land: == strlen(some_text), in C++ land: string.length()
    int                 BufSize;        // Buffer size (in bytes) = capacity+1  // Read-only    // [Resize,Completion,History,Always] Include zero-terminator storage. In C land == ARRAYSIZE(my_char_array), in C++ land: string.capacity()+1
    bool                BufDirty;       // Set if you modify Buf/BufTextLen!    // Write        // [Completion,History,Always]
    int                 CursorPos;      //                                      // Read-write   // [Completion,History,Always]
    int                 SelectionStart; //                                      // Read-write   // [Completion,History,Always] == to SelectionEnd when no selection)
    int                 SelectionEnd;   //                                      // Read-write   // [Completion,History,Always]

    // Helper functions for text manipulation.
    // Use those function to benefit from the CallbackResize behaviors. Calling those function reset the selection.
    IMGUI_API ImGuiInputTextCallbackData();
    IMGUI_API void      DeleteChars(int pos, int bytes_count);
    IMGUI_API void      InsertChars(int pos, const char* text, const char* text_end = NULL);
    bool                HasSelection() const { return SelectionStart != SelectionEnd; }
};

// Resizing callback data to apply custom constraint. As enabled by SetNextWindowSizeConstraints(). Callback is called during the next Begin().
// NB: For basic min/max size constraint on each axis you don't need to use the callback! The SetNextWindowSizeConstraints() parameters are enough.
struct ImGuiSizeCallbackData
{
    void*   UserData;       // Read-only.   What user passed to SetNextWindowSizeConstraints()
    ImVec2  Pos;            // Read-only.   Window position, for reference.
    ImVec2  CurrentSize;    // Read-only.   Current window size.
    ImVec2  DesiredSize;    // Read-write.  Desired size, based on user's mouse position. Write to this field to restrain resizing.
};

// Data payload for Drag and Drop operations: AcceptDragDropPayload(), GetDragDropPayload()
struct ImGuiPayload
{
    // Members
    void*           Data;               // Data (copied and owned by dear imgui)
    int             DataSize;           // Data size

    // [Internal]
    ImGuiID         SourceId;           // Source item id
    ImGuiID         SourceParentId;     // Source parent id (if available)
    int             DataFrameCount;     // Data timestamp
    char            DataType[32+1];     // Data type tag (short user-supplied string, 32 characters max)
    bool            Preview;            // Set when AcceptDragDropPayload() was called and mouse has been hovering the target item (nb: handle overlapping drag targets)
    bool            Delivery;           // Set when AcceptDragDropPayload() was called and mouse button is released over the target item.

    ImGuiPayload()  { Clear(); }
    void Clear()    { SourceId = SourceParentId = 0; Data = NULL; DataSize = 0; memset(DataType, 0, sizeof(DataType)); DataFrameCount = -1; Preview = Delivery = false; }
    bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
    bool IsPreview() const                  { return Preview; }
    bool IsDelivery() const                 { return Delivery; }
};

//-----------------------------------------------------------------------------
// Obsolete functions (Will be removed! Read 'API BREAKING CHANGES' section in imgui.cpp for details)
// Please keep your copy of dear imgui up to date! Occasionally set '#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS' in imconfig.h to stay ahead.
//-----------------------------------------------------------------------------

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
namespace ImGui
{
    // OBSOLETED in 1.72 (from July 2019)
    static inline void  TreeAdvanceToLabelPos()               { SetCursorPosX(GetCursorPosX() + GetTreeNodeToLabelSpacing()); }
    // OBSOLETED in 1.71 (from June 2019)
    static inline void  SetNextTreeNodeOpen(bool open, ImGuiCond cond = 0) { SetNextItemOpen(open, cond); }
    // OBSOLETED in 1.70 (from May 2019)
    static inline float GetContentRegionAvailWidth()          { return GetContentRegionAvail().x; }
    // OBSOLETED in 1.69 (from Mar 2019)
    static inline ImDrawList* GetOverlayDrawList()            { return GetForegroundDrawList(); }
    // OBSOLETED in 1.66 (from Sep 2018)
    static inline void  SetScrollHere(float center_ratio=0.5f){ SetScrollHereY(center_ratio); }
    // OBSOLETED in 1.63 (between Aug 2018 and Sept 2018)
    static inline bool  IsItemDeactivatedAfterChange()        { return IsItemDeactivatedAfterEdit(); }
    // OBSOLETED in 1.61 (between Apr 2018 and Aug 2018)
    IMGUI_API bool      InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, ImGuiInputTextFlags flags = 0); // Use the 'const char* format' version instead of 'decimal_precision'!
    IMGUI_API bool      InputFloat2(const char* label, float v[2], int decimal_precision, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool      InputFloat3(const char* label, float v[3], int decimal_precision, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool      InputFloat4(const char* label, float v[4], int decimal_precision, ImGuiInputTextFlags flags = 0);
    // OBSOLETED in 1.60 (between Dec 2017 and Apr 2018)
    static inline bool  IsAnyWindowFocused()                  { return IsWindowFocused(ImGuiFocusedFlags_AnyWindow); }
    static inline bool  IsAnyWindowHovered()                  { return IsWindowHovered(ImGuiHoveredFlags_AnyWindow); }
    static inline ImVec2 CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge = false, float outward = 0.f) { IM_UNUSED(on_edge); IM_UNUSED(outward); IM_ASSERT(0); return pos; }
}
typedef ImGuiInputTextCallback      ImGuiTextEditCallback;    // OBSOLETED in 1.63 (from Aug 2018): made the names consistent
typedef ImGuiInputTextCallbackData  ImGuiTextEditCallbackData;
#endif

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------

// Helper: Execute a block of code at maximum once a frame. Convenient if you want to quickly create an UI within deep-nested code that runs multiple times every frame.
// Usage: static ImGuiOnceUponAFrame oaf; if (oaf) ImGui::Text("This will be called only once per frame");
struct ImGuiOnceUponAFrame
{
    ImGuiOnceUponAFrame() { RefFrame = -1; }
    mutable int RefFrame;
    operator bool() const { int current_frame = ImGui::GetFrameCount(); if (RefFrame == current_frame) return false; RefFrame = current_frame; return true; }
};

// Helper: Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
struct ImGuiTextFilter
{
    IMGUI_API           ImGuiTextFilter(const char* default_filter = "");
    IMGUI_API bool      Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);  // Helper calling InputText+Build
    IMGUI_API bool      PassFilter(const char* text, const char* text_end = NULL) const;
    IMGUI_API void      Build();
    void                Clear()          { InputBuf[0] = 0; Build(); }
    bool                IsActive() const { return !Filters.empty(); }

    // [Internal]
    struct ImGuiTextRange
    {
        const char*     b;
        const char*     e;

        ImGuiTextRange()                                { b = e = NULL; }
        ImGuiTextRange(const char* _b, const char* _e)  { b = _b; e = _e; }
        bool            empty() const                   { return b == e; }
        IMGUI_API void  split(char separator, ImVector<ImGuiTextRange>* out) const;
    };
    char                    InputBuf[256];
    ImVector<ImGuiTextRange>Filters;
    int                     CountGrep;
};

// Helper: Growable text buffer for logging/accumulating text
// (this could be called 'ImGuiTextBuilder' / 'ImGuiStringBuilder')
struct ImGuiTextBuffer
{
    ImVector<char>      Buf;
    IMGUI_API static char EmptyString[1];

    ImGuiTextBuffer()   { }
    inline char         operator[](int i) const { IM_ASSERT(Buf.Data != NULL); return Buf.Data[i]; }
    const char*         begin() const           { return Buf.Data ? &Buf.front() : EmptyString; }
    const char*         end() const             { return Buf.Data ? &Buf.back() : EmptyString; }   // Buf is zero-terminated, so end() will point on the zero-terminator
    int                 size() const            { return Buf.Size ? Buf.Size - 1 : 0; }
    bool                empty() const           { return Buf.Size <= 1; }
    void                clear()                 { Buf.clear(); }
    void                reserve(int capacity)   { Buf.reserve(capacity); }
    const char*         c_str() const           { return Buf.Data ? Buf.Data : EmptyString; }
    IMGUI_API void      append(const char* str, const char* str_end = NULL);
    IMGUI_API void      appendf(const char* fmt, ...) IM_FMTARGS(2);
    IMGUI_API void      appendfv(const char* fmt, va_list args) IM_FMTLIST(2);
};

// Helper: Key->Value storage
// Typically you don't have to worry about this since a storage is held within each Window.
// We use it to e.g. store collapse state for a tree (Int 0/1)
// This is optimized for efficient lookup (dichotomy into a contiguous buffer) and rare insertion (typically tied to user interactions aka max once a frame)
// You can use it as custom user storage for temporary values. Declare your own storage if, for example:
// - You want to manipulate the open/close state of a particular sub-tree in your interface (tree node uses Int 0/1 to store their state).
// - You want to store custom debug data easily without adding or editing structures in your code (probably not efficient, but convenient)
// Types are NOT stored, so it is up to you to make sure your Key don't collide with different types.
struct ImGuiStorage
{
    // [Internal]
    struct ImGuiStoragePair
    {
        ImGuiID key;
        union { int val_i; float val_f; void* val_p; };
        ImGuiStoragePair(ImGuiID _key, int _val_i)      { key = _key; val_i = _val_i; }
        ImGuiStoragePair(ImGuiID _key, float _val_f)    { key = _key; val_f = _val_f; }
        ImGuiStoragePair(ImGuiID _key, void* _val_p)    { key = _key; val_p = _val_p; }
    };

    ImVector<ImGuiStoragePair>      Data;

    // - Get***() functions find pair, never add/allocate. Pairs are sorted so a query is O(log N)
    // - Set***() functions find pair, insertion on demand if missing.
    // - Sorted insertion is costly, paid once. A typical frame shouldn't need to insert any new pair.
    void                Clear() { Data.clear(); }
    IMGUI_API int       GetInt(ImGuiID key, int default_val = 0) const;
    IMGUI_API void      SetInt(ImGuiID key, int val);
    IMGUI_API bool      GetBool(ImGuiID key, bool default_val = false) const;
    IMGUI_API void      SetBool(ImGuiID key, bool val);
    IMGUI_API float     GetFloat(ImGuiID key, float default_val = 0.0f) const;
    IMGUI_API void      SetFloat(ImGuiID key, float val);
    IMGUI_API void*     GetVoidPtr(ImGuiID key) const; // default_val is NULL
    IMGUI_API void      SetVoidPtr(ImGuiID key, void* val);

    // - Get***Ref() functions finds pair, insert on demand if missing, return pointer. Useful if you intend to do Get+Set.
    // - References are only valid until a new value is added to the storage. Calling a Set***() function or a Get***Ref() function invalidates the pointer.
    // - A typical use case where this is convenient for quick hacking (e.g. add storage during a live Edit&Continue session if you can't modify existing struct)
    //      float* pvar = ImGui::GetFloatRef(key); ImGui::SliderFloat("var", pvar, 0, 100.0f); some_var += *pvar;
    IMGUI_API int*      GetIntRef(ImGuiID key, int default_val = 0);
    IMGUI_API bool*     GetBoolRef(ImGuiID key, bool default_val = false);
    IMGUI_API float*    GetFloatRef(ImGuiID key, float default_val = 0.0f);
    IMGUI_API void**    GetVoidPtrRef(ImGuiID key, void* default_val = NULL);

    // Use on your own storage if you know only integer are being stored (open/close all tree nodes)
    IMGUI_API void      SetAllInt(int val);

    // For quicker full rebuild of a storage (instead of an incremental one), you may add all your contents and then sort once.
    IMGUI_API void      BuildSortByKey();
};

// Helper: Manually clip large list of items.
// If you are submitting lots of evenly spaced items and you have a random access to the list, you can perform coarse clipping based on visibility to save yourself from processing those items at all.
// The clipper calculates the range of visible items and advance the cursor to compensate for the non-visible items we have skipped.
// ImGui already clip items based on their bounds but it needs to measure text size to do so. Coarse clipping before submission makes this cost and your own data fetching/submission cost null.
// Usage:
//     ImGuiListClipper clipper(1000);  // we have 1000 elements, evenly spaced.
//     while (clipper.Step())
//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
//             ImGui::Text("line number %d", i);
// - Step 0: the clipper let you process the first element, regardless of it being visible or not, so we can measure the element height (step skipped if we passed a known height as second arg to constructor).
// - Step 1: the clipper infer height from first element, calculate the actual range of elements to display, and position the cursor before the first element.
// - (Step 2: dummy step only required if an explicit items_height was passed to constructor or Begin() and user call Step(). Does nothing and switch to Step 3.)
// - Step 3: the clipper validate that we have reached the expected Y position (corresponding to element DisplayEnd), advance the cursor to the end of the list and then returns 'false' to end the loop.
struct ImGuiListClipper
{
    int     DisplayStart, DisplayEnd;
    int     ItemsCount;

    // [Internal]
    int     StepNo;
    float   ItemsHeight;
    float   StartPosY;

    // items_count:  Use -1 to ignore (you can call Begin later). Use INT_MAX if you don't know how many items you have (in which case the cursor won't be advanced in the final step).
    // items_height: Use -1.0f to be calculated automatically on first step. Otherwise pass in the distance between your items, typically GetTextLineHeightWithSpacing() or GetFrameHeightWithSpacing().
    // If you don't specify an items_height, you NEED to call Step(). If you specify items_height you may call the old Begin()/End() api directly, but prefer calling Step().
    ImGuiListClipper(int items_count = -1, float items_height = -1.0f)  { Begin(items_count, items_height); } // NB: Begin() initialize every fields (as we allow user to call Begin/End multiple times on a same instance if they want).
    ~ImGuiListClipper()                                                 { IM_ASSERT(ItemsCount == -1); }      // Assert if user forgot to call End() or Step() until false.

    IMGUI_API bool Step();                                              // Call until it returns false. The DisplayStart/DisplayEnd fields will be set and you can process/draw those items.
    IMGUI_API void Begin(int items_count, float items_height = -1.0f);  // Automatically called by constructor if you passed 'items_count' or by Step() in Step 1.
    IMGUI_API void End();                                               // Automatically called on the last call of Step() that returns false.
};

// Helpers macros to generate 32-bit encoded colors
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define IM_COL32_R_SHIFT    16
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    0
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#else
#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#endif
#define IM_COL32(R,G,B,A)    (((ImU32)(A)<<IM_COL32_A_SHIFT) | ((ImU32)(B)<<IM_COL32_B_SHIFT) | ((ImU32)(G)<<IM_COL32_G_SHIFT) | ((ImU32)(R)<<IM_COL32_R_SHIFT))
#define IM_COL32_WHITE       IM_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define IM_COL32_BLACK       IM_COL32(0,0,0,255)        // Opaque black
#define IM_COL32_BLACK_TRANS IM_COL32(0,0,0,0)          // Transparent black = 0x00000000

// Helper: ImColor() implicitly converts colors to either ImU32 (packed 4x1 byte) or ImVec4 (4x1 float)
// Prefer using IM_COL32() macros if you want a guaranteed compile-time ImU32 for usage with ImDrawList API.
// **Avoid storing ImColor! Store either u32 of ImVec4. This is not a full-featured color class. MAY OBSOLETE.
// **None of the ImGui API are using ImColor directly but you can use it as a convenience to pass colors in either ImU32 or ImVec4 formats. Explicitly cast to ImU32 or ImVec4 if needed.
struct ImColor
{
    ImVec4              Value;

    ImColor()                                                       { Value.x = Value.y = Value.z = Value.w = 0.0f; }
    ImColor(int r, int g, int b, int a = 255)                       { float sc = 1.0f/255.0f; Value.x = (float)r * sc; Value.y = (float)g * sc; Value.z = (float)b * sc; Value.w = (float)a * sc; }
    ImColor(ImU32 rgba)                                             { float sc = 1.0f/255.0f; Value.x = (float)((rgba>>IM_COL32_R_SHIFT)&0xFF) * sc; Value.y = (float)((rgba>>IM_COL32_G_SHIFT)&0xFF) * sc; Value.z = (float)((rgba>>IM_COL32_B_SHIFT)&0xFF) * sc; Value.w = (float)((rgba>>IM_COL32_A_SHIFT)&0xFF) * sc; }
    ImColor(float r, float g, float b, float a = 1.0f)              { Value.x = r; Value.y = g; Value.z = b; Value.w = a; }
    ImColor(const ImVec4& col)                                      { Value = col; }
    inline operator ImU32() const                                   { return ImGui::ColorConvertFloat4ToU32(Value); }
    inline operator ImVec4() const                                  { return Value; }

    // FIXME-OBSOLETE: May need to obsolete/cleanup those helpers.
    inline void    SetHSV(float h, float s, float v, float a = 1.0f){ ImGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z); Value.w = a; }
    static ImColor HSV(float h, float s, float v, float a = 1.0f)   { float r,g,b; ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b); return ImColor(r,g,b,a); }
};

//-----------------------------------------------------------------------------
// Draw List API (ImDrawCmd, ImDrawIdx, ImDrawVert, ImDrawChannel, ImDrawListSplitter, ImDrawListFlags, ImDrawList, ImDrawData)
// Hold a series of drawing commands. The user provides a renderer for ImDrawData which essentially contains an array of ImDrawList.
//-----------------------------------------------------------------------------

// Draw callbacks for advanced uses.
// NB: You most likely do NOT need to use draw callbacks just to create your own widget or customized UI rendering,
// you can poke into the draw list for that! Draw callback may be useful for example to:
//  A) Change your GPU render state,
//  B) render a complex 3D scene inside a UI element without an intermediate texture/render target, etc.
// The expected behavior from your rendering function is 'if (cmd.UserCallback != NULL) { cmd.UserCallback(parent_list, cmd); } else { RenderTriangles() }'
// If you want to override the signature of ImDrawCallback, you can simply use e.g. '#define ImDrawCallback MyDrawCallback' (in imconfig.h) + update rendering back-end accordingly.
#ifndef ImDrawCallback
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);
#endif

// Special Draw callback value to request renderer back-end to reset the graphics/render state.
// The renderer back-end needs to handle this special value, otherwise it will crash trying to call a function at this address.
// This is useful for example if you submitted callbacks which you know have altered the render state and you want it to be restored.
// It is not done by default because they are many perfectly useful way of altering render state for imgui contents (e.g. changing shader/blending settings before an Image call).
#define ImDrawCallback_ResetRenderState     (ImDrawCallback)(-1)

// Typically, 1 command = 1 GPU draw call (unless command is a callback)
// Pre 1.71 back-ends will typically ignore the VtxOffset/IdxOffset fields. When 'io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset'
// is enabled, those fields allow us to render meshes larger than 64K vertices while keeping 16-bit indices.
struct ImDrawCmd
{
    unsigned int    ElemCount;              // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee ImDrawList's vtx_buffer[] array, indices in idx_buffer[].
    ImVec4          ClipRect;               // Clipping rectangle (x1, y1, x2, y2). Subtract ImDrawData->DisplayPos to get clipping rectangle in "viewport" coordinates
    ImTextureID     TextureId;              // User-provided texture ID. Set by user in ImfontAtlas::SetTexID() for fonts or passed to Image*() functions. Ignore if never using images or multiple fonts atlas.
    unsigned int    VtxOffset;              // Start offset in vertex buffer. Pre-1.71 or without ImGuiBackendFlags_RendererHasVtxOffset: always 0. With ImGuiBackendFlags_RendererHasVtxOffset: may be >0 to support meshes larger than 64K vertices with 16-bit indices.
    unsigned int    IdxOffset;              // Start offset in index buffer. Always equal to sum of ElemCount drawn so far.
    ImDrawCallback  UserCallback;           // If != NULL, call the function instead of rendering the vertices. clip_rect and texture_id will be set normally.
    void*           UserCallbackData;       // The draw callback code can access this.

    ImDrawCmd() { ElemCount = 0; TextureId = (ImTextureID)NULL; VtxOffset = IdxOffset = 0;  UserCallback = NULL; UserCallbackData = NULL; }
};

// Vertex index
// (to allow large meshes with 16-bit indices: set 'io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset' and handle ImDrawCmd::VtxOffset in the renderer back-end)
// (to use 32-bit indices: override with '#define ImDrawIdx unsigned int' in imconfig.h)
#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;
#endif

// Vertex layout
#ifndef IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert
{
    ImVec2  pos;
    ImVec2  uv;
    ImU32   col;
};
#else
// You can override the vertex format layout by defining IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT in imconfig.h
// The code expect ImVec2 pos (8 bytes), ImVec2 uv (8 bytes), ImU32 col (4 bytes), but you can re-order them or add other fields as needed to simplify integration in your engine.
// The type has to be described within the macro (you can either declare the struct or use a typedef). This is because ImVec2/ImU32 are likely not declared a the time you'd want to set your type up.
// NOTE: IMGUI DOESN'T CLEAR THE STRUCTURE AND DOESN'T CALL A CONSTRUCTOR SO ANY CUSTOM FIELD WILL BE UNINITIALIZED. IF YOU ADD EXTRA FIELDS (SUCH AS A 'Z' COORDINATES) YOU WILL NEED TO CLEAR THEM DURING RENDER OR TO IGNORE THEM.
IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

// For use by ImDrawListSplitter.
struct ImDrawChannel
{
    ImVector<ImDrawCmd>         _CmdBuffer;
    ImVector<ImDrawIdx>         _IdxBuffer;
};

// Split/Merge functions are used to split the draw list into different layers which can be drawn into out of order.
// This is used by the Columns api, so items of each column can be batched together in a same draw call.
struct ImDrawListSplitter
{
    int                         _Current;    // Current channel number (0)
    int                         _Count;      // Number of active channels (1+)
    ImVector<ImDrawChannel>     _Channels;   // Draw channels (not resized down so _Count might be < Channels.Size)

    inline ImDrawListSplitter()  { Clear(); }
    inline ~ImDrawListSplitter() { ClearFreeMemory(); }
    inline void                 Clear() { _Current = 0; _Count = 1; } // Do not clear Channels[] so our allocations are reused next frame
    IMGUI_API void              ClearFreeMemory();
    IMGUI_API void              Split(ImDrawList* draw_list, int count);
    IMGUI_API void              Merge(ImDrawList* draw_list);
    IMGUI_API void              SetCurrentChannel(ImDrawList* draw_list, int channel_idx);
};

enum ImDrawCornerFlags_
{
    ImDrawCornerFlags_None      = 0,
    ImDrawCornerFlags_TopLeft   = 1 << 0, // 0x1
    ImDrawCornerFlags_TopRight  = 1 << 1, // 0x2
    ImDrawCornerFlags_BotLeft   = 1 << 2, // 0x4
    ImDrawCornerFlags_BotRight  = 1 << 3, // 0x8
    ImDrawCornerFlags_Top       = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight,   // 0x3
    ImDrawCornerFlags_Bot       = ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight,   // 0xC
    ImDrawCornerFlags_Left      = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft,    // 0x5
    ImDrawCornerFlags_Right     = ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight,  // 0xA
    ImDrawCornerFlags_All       = 0xF     // In your function calls you may use ~0 (= all bits sets) instead of ImDrawCornerFlags_All, as a convenience
};

enum ImDrawListFlags_
{
    ImDrawListFlags_None             = 0,
    ImDrawListFlags_AntiAliasedLines = 1 << 0,  // Lines are anti-aliased (*2 the number of triangles for 1.0f wide line, otherwise *3 the number of triangles)
    ImDrawListFlags_AntiAliasedFill  = 1 << 1,  // Filled shapes have anti-aliased edges (*2 the number of vertices)
    ImDrawListFlags_AllowVtxOffset   = 1 << 2   // Can emit 'VtxOffset > 0' to allow large meshes. Set when 'ImGuiBackendFlags_RendererHasVtxOffset' is enabled.
};

// Draw command list
// This is the low-level list of polygons that ImGui:: functions are filling. At the end of the frame,
// all command lists are passed to your ImGuiIO::RenderDrawListFn function for rendering.
// Each dear imgui window contains its own ImDrawList. You can use ImGui::GetWindowDrawList() to
// access the current window draw list and draw custom primitives.
// You can interleave normal ImGui:: calls and adding primitives to the current draw list.
// All positions are generally in pixel coordinates (top-left at (0,0), bottom-right at io.DisplaySize), but you are totally free to apply whatever transformation matrix to want to the data (if you apply such transformation you'll want to apply it to ClipRect as well)
// Important: Primitives are always added to the list and not culled (culling is done at higher-level by ImGui:: functions), if you use this API a lot consider coarse culling your drawn objects.
struct ImDrawList
{
    // This is what you have to render
    ImVector<ImDrawCmd>     CmdBuffer;          // Draw commands. Typically 1 command = 1 GPU draw call, unless the command is a callback.
    ImVector<ImDrawIdx>     IdxBuffer;          // Index buffer. Each command consume ImDrawCmd::ElemCount of those
    ImVector<ImDrawVert>    VtxBuffer;          // Vertex buffer.
    ImDrawListFlags         Flags;              // Flags, you may poke into these to adjust anti-aliasing settings per-primitive.

    // [Internal, used while building lists]
    const ImDrawListSharedData* _Data;          // Pointer to shared draw data (you can use ImGui::GetDrawListSharedData() to get the one from current ImGui context)
    const char*             _OwnerName;         // Pointer to owner window's name for debugging
    unsigned int            _VtxCurrentOffset;  // [Internal] Always 0 unless 'Flags & ImDrawListFlags_AllowVtxOffset'.
    unsigned int            _VtxCurrentIdx;     // [Internal] Generally == VtxBuffer.Size unless we are past 64K vertices, in which case this gets reset to 0.
    ImDrawVert*             _VtxWritePtr;       // [Internal] point within VtxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
    ImDrawIdx*              _IdxWritePtr;       // [Internal] point within IdxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
    ImVector<ImVec4>        _ClipRectStack;     // [Internal]
    ImVector<ImTextureID>   _TextureIdStack;    // [Internal]
    ImVector<ImVec2>        _Path;              // [Internal] current path building
    ImDrawListSplitter      _Splitter;          // [Internal] for channels api

    // If you want to create ImDrawList instances, pass them ImGui::GetDrawListSharedData() or create and use your own ImDrawListSharedData (so you can use ImDrawList without ImGui)
    ImDrawList(const ImDrawListSharedData* shared_data) { _Data = shared_data; _OwnerName = NULL; Clear(); }
    ~ImDrawList() { ClearFreeMemory(); }
    IMGUI_API void  PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false);  // Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)
    IMGUI_API void  PushClipRectFullScreen();
    IMGUI_API void  PopClipRect();
    IMGUI_API void  PushTextureID(ImTextureID texture_id);
    IMGUI_API void  PopTextureID();
    inline ImVec2   GetClipRectMin() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.x, cr.y); }
    inline ImVec2   GetClipRectMax() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.z, cr.w); }

    // Primitives
    // - For rectangular primitives, "p_min" and "p_max" represent the upper-left and lower-right corners.
    // - For circle primitives, use "num_segments == 0" to automatically calculate tessellation (preferred). 
    //   In future versions we will use textures to provide cheaper and higher-quality circles. 
    //   Use AddNgon() and AddNgonFilled() functions if you need to guaranteed a specific number of sides.
    IMGUI_API void  AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All, float thickness = 1.0f);   // a: upper-left, b: lower-right (== upper-left + size), rounding_corners_flags: 4 bits corresponding to which corner to round
    IMGUI_API void  AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All);                     // a: upper-left, b: lower-right (== upper-left + size)
    IMGUI_API void  AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
    IMGUI_API void  AddQuad(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddQuadFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col);
    IMGUI_API void  AddTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);
    IMGUI_API void  AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
    IMGUI_API void  AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 12);
    IMGUI_API void  AddNgon(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness = 1.0f);
    IMGUI_API void  AddNgonFilled(const ImVec2& center, float radius, ImU32 col, int num_segments);
    IMGUI_API void  AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
    IMGUI_API void  AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
    IMGUI_API void  AddPolyline(const ImVec2* points, int num_points, ImU32 col, bool closed, float thickness);
    IMGUI_API void  AddConvexPolyFilled(const ImVec2* points, int num_points, ImU32 col); // Note: Anti-aliased filling requires points to be in clockwise order.
    IMGUI_API void  AddBezierCurve(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0);

    // Image primitives
    // - Read FAQ to understand what ImTextureID is.
    // - "p_min" and "p_max" represent the upper-left and lower-right corners of the rectangle.
    // - "uv_min" and "uv_max" represent the normalized texture coordinates to use for those corners. Using (0,0)->(1,1) texture coordinates will generally display the entire texture.
    IMGUI_API void  AddImage(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE);
    IMGUI_API void  AddImageQuad(ImTextureID user_texture_id, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& uv1 = ImVec2(0, 0), const ImVec2& uv2 = ImVec2(1, 0), const ImVec2& uv3 = ImVec2(1, 1), const ImVec2& uv4 = ImVec2(0, 1), ImU32 col = IM_COL32_WHITE);
    IMGUI_API void  AddImageRounded(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col, float rounding, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All);

    // Stateful path API, add points then finish with PathFillConvex() or PathStroke()
    inline    void  PathClear()                                                 { _Path.Size = 0; }
    inline    void  PathLineTo(const ImVec2& pos)                               { _Path.push_back(pos); }
    inline    void  PathLineToMergeDuplicate(const ImVec2& pos)                 { if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size-1], &pos, 8) != 0) _Path.push_back(pos); }
    inline    void  PathFillConvex(ImU32 col)                                   { AddConvexPolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0; }  // Note: Anti-aliased filling requires points to be in clockwise order.
    inline    void  PathStroke(ImU32 col, bool closed, float thickness = 1.0f)  { AddPolyline(_Path.Data, _Path.Size, col, closed, thickness); _Path.Size = 0; }
    IMGUI_API void  PathArcTo(const ImVec2& center, float radius, float a_min, float a_max, int num_segments = 10);
    IMGUI_API void  PathArcToFast(const ImVec2& center, float radius, int a_min_of_12, int a_max_of_12);                                            // Use precomputed angles for a 12 steps circle
    IMGUI_API void  PathBezierCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments = 0);
    IMGUI_API void  PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All);

    // Advanced
    IMGUI_API void  AddCallback(ImDrawCallback callback, void* callback_data);  // Your rendering function must check for 'UserCallback' in ImDrawCmd and call the function instead of rendering triangles.
    IMGUI_API void  AddDrawCmd();                                               // This is useful if you need to forcefully create a new draw call (to allow for dependent rendering / blending). Otherwise primitives are merged into the same draw-call as much as possible
    IMGUI_API ImDrawList* CloneOutput() const;                                  // Create a clone of the CmdBuffer/IdxBuffer/VtxBuffer.

    // Advanced: Channels
    // - Use to split render into layers. By switching channels to can render out-of-order (e.g. submit FG primitives before BG primitives)
    // - Use to minimize draw calls (e.g. if going back-and-forth between multiple clipping rectangles, prefer to append into separate channels then merge at the end)
    // - FIXME-OBSOLETE: This API shouldn't have been in ImDrawList in the first place!
    //   Prefer using your own persistent copy of ImDrawListSplitter as you can stack them.
    //   Using the ImDrawList::ChannelsXXXX you cannot stack a split over another.
    inline void     ChannelsSplit(int count)    { _Splitter.Split(this, count); }
    inline void     ChannelsMerge()             { _Splitter.Merge(this); }
    inline void     ChannelsSetCurrent(int n)   { _Splitter.SetCurrentChannel(this, n); }

    // Internal helpers
    // NB: all primitives needs to be reserved via PrimReserve() beforehand!
    IMGUI_API void  Clear();
    IMGUI_API void  ClearFreeMemory();
    IMGUI_API void  PrimReserve(int idx_count, int vtx_count);
    IMGUI_API void  PrimUnreserve(int idx_count, int vtx_count);
    IMGUI_API void  PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);      // Axis aligned rectangle (composed of two triangles)
    IMGUI_API void  PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
    IMGUI_API void  PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
    inline    void  PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col){ _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
    inline    void  PrimWriteIdx(ImDrawIdx idx)                                 { *_IdxWritePtr = idx; _IdxWritePtr++; }
    inline    void  PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)     { PrimWriteIdx((ImDrawIdx)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); }
    IMGUI_API void  UpdateClipRect();
    IMGUI_API void  UpdateTextureID();
};

// All draw data to render a Dear ImGui frame
// (NB: the style and the naming convention here is a little inconsistent, we currently preserve them for backward compatibility purpose,
// as this is one of the oldest structure exposed by the library! Basically, ImDrawList == CmdList)
struct ImDrawData
{
    bool            Valid;                  // Only valid after Render() is called and before the next NewFrame() is called.
    ImDrawList**    CmdLists;               // Array of ImDrawList* to render. The ImDrawList are owned by ImGuiContext and only pointed to from here.
    int             CmdListsCount;          // Number of ImDrawList* to render
    int             TotalIdxCount;          // For convenience, sum of all ImDrawList's IdxBuffer.Size
    int             TotalVtxCount;          // For convenience, sum of all ImDrawList's VtxBuffer.Size
    ImVec2          DisplayPos;             // Upper-left position of the viewport to render (== upper-left of the orthogonal projection matrix to use)
    ImVec2          DisplaySize;            // Size of the viewport to render (== io.DisplaySize for the main viewport) (DisplayPos + DisplaySize == lower-right of the orthogonal projection matrix to use)
    ImVec2          FramebufferScale;       // Amount of pixels for each unit of DisplaySize. Based on io.DisplayFramebufferScale. Generally (1,1) on normal display, (2,2) on OSX with Retina display.

    // Functions
    ImDrawData()    { Valid = false; Clear(); }
    ~ImDrawData()   { Clear(); }
    void Clear()    { Valid = false; CmdLists = NULL; CmdListsCount = TotalVtxCount = TotalIdxCount = 0; DisplayPos = DisplaySize = FramebufferScale = ImVec2(0.f, 0.f); } // The ImDrawList are owned by ImGuiContext!
    IMGUI_API void  DeIndexAllBuffers();                    // Helper to convert all buffers from indexed to non-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
    IMGUI_API void  ScaleClipRects(const ImVec2& fb_scale); // Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than Dear ImGui expects, or if there is a difference between your window resolution and framebuffer resolution.
};

//-----------------------------------------------------------------------------
// Font API (ImFontConfig, ImFontGlyph, ImFontAtlasFlags, ImFontAtlas, ImFontGlyphRangesBuilder, ImFont)
//-----------------------------------------------------------------------------

struct ImFontConfig
{
    void*           FontData;               //          // TTF/OTF data
    int             FontDataSize;           //          // TTF/OTF data size
    bool            FontDataOwnedByAtlas;   // true     // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
    int             FontNo;                 // 0        // Index of font within TTF/OTF file
    float           SizePixels;             //          // Size in pixels for rasterizer (more or less maps to the resulting font height).
    int             OversampleH;            // 3        // Rasterize at higher quality for sub-pixel positioning. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
    int             OversampleV;            // 1        // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
    bool            PixelSnapH;             // false    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
    ImVec2          GlyphExtraSpacing;      // 0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
    ImVec2          GlyphOffset;            // 0, 0     // Offset all glyphs from this font input.
    const ImWchar*  GlyphRanges;            // NULL     // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
    float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
    float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
    bool            MergeMode;              // false    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
    unsigned int    RasterizerFlags;        // 0x00     // Settings for custom font rasterizer (e.g. ImGuiFreeType). Leave as zero if you aren't using one.
    float           RasterizerMultiply;     // 1.0f     // Brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable.
    ImWchar         EllipsisChar;           // -1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.

    // [Internal]
    char            Name[40];               // Name (strictly to ease debugging)
    ImFont*         DstFont;

    IMGUI_API ImFontConfig();
};

struct ImFontGlyph
{
    ImWchar         Codepoint;          // 0x0000..0xFFFF
    float           AdvanceX;           // Distance to next character (= data from font + ImFontConfig::GlyphExtraSpacing.x baked in)
    float           X0, Y0, X1, Y1;     // Glyph corners
    float           U0, V0, U1, V1;     // Texture coordinates
};

// Helper to build glyph ranges from text/string data. Feed your application strings/characters to it then call BuildRanges().
// This is essentially a tightly packed of vector of 64k booleans = 8KB storage.
struct ImFontGlyphRangesBuilder
{
    ImVector<ImU32> UsedChars;            // Store 1-bit per Unicode code point (0=unused, 1=used)

    ImFontGlyphRangesBuilder()          { Clear(); }
    inline void     Clear()             { int size_in_bytes = (IM_UNICODE_CODEPOINT_MAX+1) / 8; UsedChars.resize(size_in_bytes / (int)sizeof(ImU32)); memset(UsedChars.Data, 0, (size_t)size_in_bytes); }
    inline bool     GetBit(int n) const { int off = (n >> 5); ImU32 mask = 1u << (n & 31); return (UsedChars[off] & mask) != 0; }  // Get bit n in the array
    inline void     SetBit(int n)       { int off = (n >> 5); ImU32 mask = 1u << (n & 31); UsedChars[off] |= mask; }               // Set bit n in the array
    inline void     AddChar(ImWchar c)  { SetBit(c); }                          // Add character
    IMGUI_API void  AddText(const char* text, const char* text_end = NULL);     // Add string (each character of the UTF-8 string are added)
    IMGUI_API void  AddRanges(const ImWchar* ranges);                           // Add ranges, e.g. builder.AddRanges(ImFontAtlas::GetGlyphRangesDefault()) to force add all of ASCII/Latin+Ext
    IMGUI_API void  BuildRanges(ImVector<ImWchar>* out_ranges);                 // Output new ranges
};

// See ImFontAtlas::AddCustomRectXXX functions.
struct ImFontAtlasCustomRect
{
    unsigned int    ID;             // Input    // User ID. Use < 0x110000 to map into a font glyph, >= 0x110000 for other/internal/custom texture data.
    unsigned short  Width, Height;  // Input    // Desired rectangle dimension
    unsigned short  X, Y;           // Output   // Packed position in Atlas
    float           GlyphAdvanceX;  // Input    // For custom font glyphs only (ID < 0x110000): glyph xadvance
    ImVec2          GlyphOffset;    // Input    // For custom font glyphs only (ID < 0x110000): glyph display offset
    ImFont*         Font;           // Input    // For custom font glyphs only (ID < 0x110000): target font
    ImFontAtlasCustomRect()         { ID = 0xFFFFFFFF; Width = Height = 0; X = Y = 0xFFFF; GlyphAdvanceX = 0.0f; GlyphOffset = ImVec2(0,0); Font = NULL; }
    bool IsPacked() const           { return X != 0xFFFF; }
};

enum ImFontAtlasFlags_
{
    ImFontAtlasFlags_None               = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don't round the height to next power of two
    ImFontAtlasFlags_NoMouseCursors     = 1 << 1    // Don't build software mouse cursors into the atlas
};

// Load and rasterize multiple TTF/OTF fonts into a same texture. The font atlas will build a single texture holding:
//  - One or more fonts.
//  - Custom graphics data needed to render the shapes needed by Dear ImGui.
//  - Mouse cursor shapes for software cursor rendering (unless setting 'Flags |= ImFontAtlasFlags_NoMouseCursors' in the font atlas).
// It is the user-code responsibility to setup/build the atlas, then upload the pixel data into a texture accessible by your graphics api.
//  - Optionally, call any of the AddFont*** functions. If you don't call any, the default font embedded in the code will be loaded for you.
//  - Call GetTexDataAsAlpha8() or GetTexDataAsRGBA32() to build and retrieve pixels data.
//  - Upload the pixels data into a texture within your graphics system (see imgui_impl_xxxx.cpp examples)
//  - Call SetTexID(my_tex_id); and pass the pointer/identifier to your texture in a format natural to your graphics API.
//    This value will be passed back to you during rendering to identify the texture. Read FAQ entry about ImTextureID for more details.
// Common pitfalls:
// - If you pass a 'glyph_ranges' array to AddFont*** functions, you need to make sure that your array persist up until the
//   atlas is build (when calling GetTexData*** or Build()). We only copy the pointer, not the data.
// - Important: By default, AddFontFromMemoryTTF() takes ownership of the data. Even though we are not writing to it, we will free the pointer on destruction.
//   You can set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed,
// - Even though many functions are suffixed with "TTF", OTF data is supported just as well.
// - This is an old API and it is currently awkward for those and and various other reasons! We will address them in the future!
struct ImFontAtlas
{
    IMGUI_API ImFontAtlas();
    IMGUI_API ~ImFontAtlas();
    IMGUI_API ImFont*           AddFont(const ImFontConfig* font_cfg);
    IMGUI_API ImFont*           AddFontDefault(const ImFontConfig* font_cfg = NULL);
    IMGUI_API ImFont*           AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    IMGUI_API ImFont*           AddFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // Note: Transfer ownership of 'ttf_data' to ImFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed.
    IMGUI_API ImFont*           AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // 'compressed_font_data' still owned by caller. Compress with binary_to_compressed_c.cpp.
    IMGUI_API ImFont*           AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);              // 'compressed_font_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 parameter.
    IMGUI_API void              ClearInputData();           // Clear input data (all ImFontConfig structures including sizes, TTF data, glyph ranges, etc.) = all the data used to build the texture and fonts.
    IMGUI_API void              ClearTexData();             // Clear output texture data (CPU side). Saves RAM once the texture has been copied to graphics memory.
    IMGUI_API void              ClearFonts();               // Clear output font data (glyphs storage, UV coordinates).
    IMGUI_API void              Clear();                    // Clear all input and output.

    // Build atlas, retrieve pixel data.
    // User is in charge of copying the pixels into graphics memory (e.g. create a texture with your engine). Then store your texture handle with SetTexID().
    // The pitch is always = Width * BytesPerPixels (1 or 4)
    // Building in RGBA32 format is provided for convenience and compatibility, but note that unless you manually manipulate or copy color data into
    // the texture (e.g. when using the AddCustomRect*** api), then the RGB pixels emitted will always be white (~75% of memory/bandwidth waste.
    IMGUI_API bool              Build();                    // Build pixels data. This is called automatically for you by the GetTexData*** functions.
    IMGUI_API void              GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 1 byte per-pixel
    IMGUI_API void              GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 4 bytes-per-pixel
    bool                        IsBuilt() const             { return Fonts.Size > 0 && (TexPixelsAlpha8 != NULL || TexPixelsRGBA32 != NULL); }
    void                        SetTexID(ImTextureID id)    { TexID = id; }

    //-------------------------------------------
    // Glyph Ranges
    //-------------------------------------------

    // Helpers to retrieve list of common Unicode ranges (2 value per range, values are inclusive, zero-terminated list)
    // NB: Make sure that your string are UTF-8 and NOT in your local code page. In C++11, you can create UTF-8 string literal using the u8"Hello world" syntax. See FAQ for details.
    // NB: Consider using ImFontGlyphRangesBuilder to build glyph ranges from textual data.
    IMGUI_API const ImWchar*    GetGlyphRangesDefault();                // Basic Latin, Extended Latin
    IMGUI_API const ImWchar*    GetGlyphRangesKorean();                 // Default + Korean characters
    IMGUI_API const ImWchar*    GetGlyphRangesJapanese();               // Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
    IMGUI_API const ImWchar*    GetGlyphRangesChineseFull();            // Default + Half-Width + Japanese Hiragana/Katakana + full set of about 21000 CJK Unified Ideographs
    IMGUI_API const ImWchar*    GetGlyphRangesChineseSimplifiedCommon();// Default + Half-Width + Japanese Hiragana/Katakana + set of 2500 CJK Unified Ideographs for common simplified Chinese
    IMGUI_API const ImWchar*    GetGlyphRangesCyrillic();               // Default + about 400 Cyrillic characters
    IMGUI_API const ImWchar*    GetGlyphRangesThai();                   // Default + Thai characters
    IMGUI_API const ImWchar*    GetGlyphRangesVietnamese();             // Default + Vietnamese characters

    //-------------------------------------------
    // [BETA] Custom Rectangles/Glyphs API
    //-------------------------------------------

    // You can request arbitrary rectangles to be packed into the atlas, for your own purposes.
    // After calling Build(), you can query the rectangle position and render your pixels.
    // You can also request your rectangles to be mapped as font glyph (given a font + Unicode point),
    // so you can render e.g. custom colorful icons and use them as regular glyphs.
    // Read docs/FONTS.txt for more details about using colorful icons.
    IMGUI_API int               AddCustomRectRegular(unsigned int id, int width, int height);                                                                   // Id needs to be >= 0x110000. Id >= 0x80000000 are reserved for ImGui and ImDrawList
    IMGUI_API int               AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset = ImVec2(0,0));   // Id needs to be < 0x110000 to register a rectangle to map into a specific font.
    const ImFontAtlasCustomRect*GetCustomRectByIndex(int index) const { if (index < 0) return NULL; return &CustomRects[index]; }

    // [Internal]
    IMGUI_API void              CalcCustomRectUV(const ImFontAtlasCustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max) const;
    IMGUI_API bool              GetMouseCursorTexData(ImGuiMouseCursor cursor, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2]);

    //-------------------------------------------
    // Members
    //-------------------------------------------

    bool                        Locked;             // Marked as Locked by ImGui::NewFrame() so attempt to modify the atlas will assert.
    ImFontAtlasFlags            Flags;              // Build flags (see ImFontAtlasFlags_)
    ImTextureID                 TexID;              // User data to refer to the texture once it has been uploaded to user's graphic systems. It is passed back to you during rendering via the ImDrawCmd structure.
    int                         TexDesiredWidth;    // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
    int                         TexGlyphPadding;    // Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0.

    // [Internal]
    // NB: Access texture data via GetTexData*() calls! Which will setup a default font for you.
    unsigned char*              TexPixelsAlpha8;    // 1 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight
    unsigned int*               TexPixelsRGBA32;    // 4 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
    int                         TexWidth;           // Texture width calculated during Build().
    int                         TexHeight;          // Texture height calculated during Build().
    ImVec2                      TexUvScale;         // = (1.0f/TexWidth, 1.0f/TexHeight)
    ImVec2                      TexUvWhitePixel;    // Texture coordinates to a white pixel
    ImVector<ImFont*>           Fonts;              // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling ImGui::NewFrame(), use ImGui::PushFont()/PopFont() to change the current font.
    ImVector<ImFontAtlasCustomRect> CustomRects;    // Rectangles for packing custom texture data into the atlas.
    ImVector<ImFontConfig>      ConfigData;         // Internal data
    int                         CustomRectIds[1];   // Identifiers of custom texture rectangle used by ImFontAtlas/ImDrawList

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    typedef ImFontAtlasCustomRect    CustomRect;         // OBSOLETED in 1.72+
    typedef ImFontGlyphRangesBuilder GlyphRangesBuilder; // OBSOLETED in 1.67+
#endif
};

// Font runtime data and rendering
// ImFontAtlas automatically loads a default embedded font for you when you call GetTexDataAsAlpha8() or GetTexDataAsRGBA32().
struct ImFont
{
    // Members: Hot ~20/24 bytes (for CalcTextSize)
    ImVector<float>             IndexAdvanceX;      // 12-16 // out //            // Sparse. Glyphs->AdvanceX in a directly indexable way (cache-friendly for CalcTextSize functions which only this this info, and are often bottleneck in large UI).
    float                       FallbackAdvanceX;   // 4     // out // = FallbackGlyph->AdvanceX
    float                       FontSize;           // 4     // in  //            // Height of characters/line, set during loading (don't change after loading)

    // Members: Hot ~36/48 bytes (for CalcTextSize + render loop)
    ImVector<ImWchar>           IndexLookup;        // 12-16 // out //            // Sparse. Index glyphs by Unicode code-point.
    ImVector<ImFontGlyph>       Glyphs;             // 12-16 // out //            // All glyphs.
    const ImFontGlyph*          FallbackGlyph;      // 4-8   // out // = FindGlyph(FontFallbackChar)
    ImVec2                      DisplayOffset;      // 8     // in  // = (0,0)    // Offset font rendering by xx pixels

    // Members: Cold ~32/40 bytes
    ImFontAtlas*                ContainerAtlas;     // 4-8   // out //            // What we has been loaded into
    const ImFontConfig*         ConfigData;         // 4-8   // in  //            // Pointer within ContainerAtlas->ConfigData
    short                       ConfigDataCount;    // 2     // in  // ~ 1        // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
    ImWchar                     FallbackChar;       // 2     // in  // = '?'      // Replacement character if a glyph isn't found. Only set via SetFallbackChar()
    ImWchar                     EllipsisChar;       // 2     // out // = -1       // Character used for ellipsis rendering.
    bool                        DirtyLookupTables;  // 1     // out //
    float                       Scale;              // 4     // in  // = 1.f      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
    float                       Ascent, Descent;    // 4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
    int                         MetricsTotalSurface;// 4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)

    // Methods
    IMGUI_API ImFont();
    IMGUI_API ~ImFont();
    IMGUI_API const ImFontGlyph*FindGlyph(ImWchar c) const;
    IMGUI_API const ImFontGlyph*FindGlyphNoFallback(ImWchar c) const;
    float                       GetCharAdvance(ImWchar c) const     { return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; }
    bool                        IsLoaded() const                    { return ContainerAtlas != NULL; }
    const char*                 GetDebugName() const                { return ConfigData ? ConfigData->Name : "<unknown>"; }

    // 'max_width' stops rendering after a certain width (could be turned into a 2d size). FLT_MAX to disable.
    // 'wrap_width' enable automatic word-wrapping across multiple lines to fit into given width. 0.0f to disable.
    IMGUI_API ImVec2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8
    IMGUI_API const char*       CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
    IMGUI_API void              RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, ImWchar c) const;
    IMGUI_API void              RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

    // [Internal] Don't use!
    IMGUI_API void              BuildLookupTable();
    IMGUI_API void              ClearOutputData();
    IMGUI_API void              GrowIndex(int new_size);
    IMGUI_API void              AddGlyph(ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
    IMGUI_API void              AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst = true); // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.
    IMGUI_API void              SetFallbackChar(ImWchar c);
};

// 日本語対応するためのコード
static  const ImWchar glyphRangesJapanese[] = 
{	0x0020, 0x007E, 0x00A2, 0x00A3, 0x00A7, 0x00A8, 0x00AC, 0x00AC, 0x00B0, 0x00B1, 0x00B4, 0x00B4, 0x00B6, 0x00B6, 0x00D7, 0x00D7,
	0x00F7, 0x00F7, 0x0391, 0x03A1, 0x03A3, 0x03A9, 0x03B1, 0x03C1, 0x03C3, 0x03C9, 0x0401, 0x0401, 0x0410, 0x044F, 0x0451, 0x0451,
	0x2010, 0x2010, 0x2015, 0x2016, 0x2018, 0x2019, 0x201C, 0x201D, 0x2020, 0x2021, 0x2025, 0x2026, 0x2030, 0x2030, 0x2032, 0x2033,
	0x203B, 0x203B, 0x2103, 0x2103, 0x212B, 0x212B, 0x2190, 0x2193, 0x21D2, 0x21D2, 0x21D4, 0x21D4, 0x2200, 0x2200, 0x2202, 0x2203,
	0x2207, 0x2208, 0x220B, 0x220B, 0x2212, 0x2212, 0x221A, 0x221A, 0x221D, 0x221E, 0x2220, 0x2220, 0x2227, 0x222C, 0x2234, 0x2235,
	0x223D, 0x223D, 0x2252, 0x2252, 0x2260, 0x2261, 0x2266, 0x2267, 0x226A, 0x226B, 0x2282, 0x2283, 0x2286, 0x2287, 0x22A5, 0x22A5,
	0x2312, 0x2312, 0x2500, 0x2503, 0x250C, 0x250C, 0x250F, 0x2510, 0x2513, 0x2514, 0x2517, 0x2518, 0x251B, 0x251D, 0x2520, 0x2520,
	0x2523, 0x2525, 0x2528, 0x2528, 0x252B, 0x252C, 0x252F, 0x2530, 0x2533, 0x2534, 0x2537, 0x2538, 0x253B, 0x253C, 0x253F, 0x253F,
	0x2542, 0x2542, 0x254B, 0x254B, 0x25A0, 0x25A1, 0x25B2, 0x25B3, 0x25BC, 0x25BD, 0x25C6, 0x25C7, 0x25CB, 0x25CB, 0x25CE, 0x25CF,
	0x25EF, 0x25EF, 0x2605, 0x2606, 0x2640, 0x2640, 0x2642, 0x2642, 0x266A, 0x266A, 0x266D, 0x266D, 0x266F, 0x266F, 0x3000, 0x3003,
	0x3005, 0x3015, 0x301C, 0x301C, 0x3041, 0x3093, 0x309B, 0x309E, 0x30A1, 0x30F6, 0x30FB, 0x30FE, 0x4E00, 0x4E01, 0x4E03, 0x4E03,
	0x4E07, 0x4E0B, 0x4E0D, 0x4E0E, 0x4E10, 0x4E11, 0x4E14, 0x4E19, 0x4E1E, 0x4E1E, 0x4E21, 0x4E21, 0x4E26, 0x4E26, 0x4E2A, 0x4E2A,
	0x4E2D, 0x4E2D, 0x4E31, 0x4E32, 0x4E36, 0x4E36, 0x4E38, 0x4E39, 0x4E3B, 0x4E3C, 0x4E3F, 0x4E3F, 0x4E42, 0x4E43, 0x4E45, 0x4E45,
	0x4E4B, 0x4E4B, 0x4E4D, 0x4E4F, 0x4E55, 0x4E59, 0x4E5D, 0x4E5F, 0x4E62, 0x4E62, 0x4E71, 0x4E71, 0x4E73, 0x4E73, 0x4E7E, 0x4E7E,
	0x4E80, 0x4E80, 0x4E82, 0x4E82, 0x4E85, 0x4E86, 0x4E88, 0x4E8C, 0x4E8E, 0x4E8E, 0x4E91, 0x4E92, 0x4E94, 0x4E95, 0x4E98, 0x4E99,
	0x4E9B, 0x4E9C, 0x4E9E, 0x4EA2, 0x4EA4, 0x4EA6, 0x4EA8, 0x4EA8, 0x4EAB, 0x4EAE, 0x4EB0, 0x4EB0, 0x4EB3, 0x4EB3, 0x4EB6, 0x4EB6,
	0x4EBA, 0x4EBA, 0x4EC0, 0x4EC2, 0x4EC4, 0x4EC4, 0x4EC6, 0x4EC7, 0x4ECA, 0x4ECB, 0x4ECD, 0x4ECF, 0x4ED4, 0x4ED9, 0x4EDD, 0x4EDF,
	0x4EE3, 0x4EE5, 0x4EED, 0x4EEE, 0x4EF0, 0x4EF0, 0x4EF2, 0x4EF2, 0x4EF6, 0x4EF7, 0x4EFB, 0x4EFB, 0x4F01, 0x4F01, 0x4F09, 0x4F0A,
	0x4F0D, 0x4F11, 0x4F1A, 0x4F1A, 0x4F1C, 0x4F1D, 0x4F2F, 0x4F30, 0x4F34, 0x4F34, 0x4F36, 0x4F36, 0x4F38, 0x4F38, 0x4F3A, 0x4F3A,
	0x4F3C, 0x4F3D, 0x4F43, 0x4F43, 0x4F46, 0x4F47, 0x4F4D, 0x4F51, 0x4F53, 0x4F53, 0x4F55, 0x4F55, 0x4F57, 0x4F57, 0x4F59, 0x4F5E,
	0x4F69, 0x4F69, 0x4F6F, 0x4F70, 0x4F73, 0x4F73, 0x4F75, 0x4F76, 0x4F7B, 0x4F7C, 0x4F7F, 0x4F7F, 0x4F83, 0x4F83, 0x4F86, 0x4F86,
	0x4F88, 0x4F88, 0x4F8B, 0x4F8B, 0x4F8D, 0x4F8D, 0x4F8F, 0x4F8F, 0x4F91, 0x4F91, 0x4F96, 0x4F96, 0x4F98, 0x4F98, 0x4F9B, 0x4F9B,
	0x4F9D, 0x4F9D, 0x4FA0, 0x4FA1, 0x4FAB, 0x4FAB, 0x4FAD, 0x4FAF, 0x4FB5, 0x4FB6, 0x4FBF, 0x4FBF, 0x4FC2, 0x4FC4, 0x4FCA, 0x4FCA,
	0x4FCE, 0x4FCE, 0x4FD0, 0x4FD1, 0x4FD4, 0x4FD4, 0x4FD7, 0x4FD8, 0x4FDA, 0x4FDB, 0x4FDD, 0x4FDD, 0x4FDF, 0x4FDF, 0x4FE1, 0x4FE1,
	0x4FE3, 0x4FE5, 0x4FEE, 0x4FEF, 0x4FF3, 0x4FF3, 0x4FF5, 0x4FF6, 0x4FF8, 0x4FF8, 0x4FFA, 0x4FFA, 0x4FFE, 0x4FFE, 0x5005, 0x5006,
	0x5009, 0x5009, 0x500B, 0x500B, 0x500D, 0x500D, 0x500F, 0x500F, 0x5011, 0x5012, 0x5014, 0x5014, 0x5016, 0x5016, 0x5019, 0x501A,
	0x501F, 0x501F, 0x5021, 0x5021, 0x5023, 0x5026, 0x5028, 0x502D, 0x5036, 0x5036, 0x5039, 0x5039, 0x5043, 0x5043, 0x5047, 0x5049,
	0x504F, 0x5050, 0x5055, 0x5056, 0x505A, 0x505A, 0x505C, 0x505C, 0x5065, 0x5065, 0x506C, 0x506C, 0x5072, 0x5072, 0x5074, 0x5076,
	0x5078, 0x5078, 0x507D, 0x507D, 0x5080, 0x5080, 0x5085, 0x5085, 0x508D, 0x508D, 0x5091, 0x5091, 0x5098, 0x509A, 0x50AC, 0x50AD,
	0x50B2, 0x50B5, 0x50B7, 0x50B7, 0x50BE, 0x50BE, 0x50C2, 0x50C2, 0x50C5, 0x50C5, 0x50C9, 0x50CA, 0x50CD, 0x50CD, 0x50CF, 0x50CF,
	0x50D1, 0x50D1, 0x50D5, 0x50D6, 0x50DA, 0x50DA, 0x50DE, 0x50DE, 0x50E3, 0x50E3, 0x50E5, 0x50E5, 0x50E7, 0x50E7, 0x50ED, 0x50EE,
	0x50F5, 0x50F5, 0x50F9, 0x50F9, 0x50FB, 0x50FB, 0x5100, 0x5102, 0x5104, 0x5104, 0x5109, 0x5109, 0x5112, 0x5112, 0x5114, 0x5116,
	0x5118, 0x5118, 0x511A, 0x511A, 0x511F, 0x511F, 0x5121, 0x5121, 0x512A, 0x512A, 0x5132, 0x5132, 0x5137, 0x5137, 0x513A, 0x513C,
	0x513F, 0x5141, 0x5143, 0x5149, 0x514B, 0x514E, 0x5150, 0x5150, 0x5152, 0x5152, 0x5154, 0x5154, 0x515A, 0x515A, 0x515C, 0x515C,
	0x5162, 0x5162, 0x5165, 0x5165, 0x5168, 0x516E, 0x5171, 0x5171, 0x5175, 0x5178, 0x517C, 0x517C, 0x5180, 0x5180, 0x5182, 0x5182,
	0x5185, 0x5186, 0x5189, 0x518A, 0x518C, 0x518D, 0x518F, 0x5193, 0x5195, 0x5197, 0x5199, 0x5199, 0x51A0, 0x51A0, 0x51A2, 0x51A2,
	0x51A4, 0x51A6, 0x51A8, 0x51AC, 0x51B0, 0x51B7, 0x51BD, 0x51BD, 0x51C4, 0x51C6, 0x51C9, 0x51C9, 0x51CB, 0x51CD, 0x51D6, 0x51D6,
	0x51DB, 0x51DD, 0x51E0, 0x51E1, 0x51E6, 0x51E7, 0x51E9, 0x51EA, 0x51ED, 0x51ED, 0x51F0, 0x51F1, 0x51F5, 0x51F6, 0x51F8, 0x51FA,
	0x51FD, 0x51FE, 0x5200, 0x5200, 0x5203, 0x5204, 0x5206, 0x5208, 0x520A, 0x520B, 0x520E, 0x520E, 0x5211, 0x5211, 0x5214, 0x5214,
	0x5217, 0x5217, 0x521D, 0x521D, 0x5224, 0x5225, 0x5227, 0x5227, 0x5229, 0x522A, 0x522E, 0x522E, 0x5230, 0x5230, 0x5233, 0x5233,
	0x5236, 0x523B, 0x5243, 0x5244, 0x5247, 0x5247, 0x524A, 0x524D, 0x524F, 0x524F, 0x5254, 0x5254, 0x5256, 0x5256, 0x525B, 0x525B,
	0x525E, 0x525E, 0x5263, 0x5265, 0x5269, 0x526A, 0x526F, 0x5275, 0x527D, 0x527D, 0x527F, 0x527F, 0x5283, 0x5283, 0x5287, 0x5289,
	0x528D, 0x528D, 0x5291, 0x5292, 0x5294, 0x5294, 0x529B, 0x529B, 0x529F, 0x52A0, 0x52A3, 0x52A3, 0x52A9, 0x52AD, 0x52B1, 0x52B1,
	0x52B4, 0x52B5, 0x52B9, 0x52B9, 0x52BC, 0x52BC, 0x52BE, 0x52BE, 0x52C1, 0x52C1, 0x52C3, 0x52C3, 0x52C5, 0x52C5, 0x52C7, 0x52C7,
	0x52C9, 0x52C9, 0x52CD, 0x52CD, 0x52D2, 0x52D2, 0x52D5, 0x52D5, 0x52D7, 0x52D9, 0x52DD, 0x52E0, 0x52E2, 0x52E4, 0x52E6, 0x52E7,
	0x52F2, 0x52F3, 0x52F5, 0x52F5, 0x52F8, 0x52FA, 0x52FE, 0x52FF, 0x5301, 0x5302, 0x5305, 0x5306, 0x5308, 0x5308, 0x530D, 0x530D,
	0x530F, 0x5310, 0x5315, 0x5317, 0x5319, 0x531A, 0x531D, 0x531D, 0x5320, 0x5321, 0x5323, 0x5323, 0x532A, 0x532A, 0x532F, 0x532F,
	0x5331, 0x5331, 0x5333, 0x5333, 0x5338, 0x533B, 0x533F, 0x5341, 0x5343, 0x5343, 0x5345, 0x534A, 0x534D, 0x534D, 0x5351, 0x5354,
	0x5357, 0x5358, 0x535A, 0x535A, 0x535C, 0x535C, 0x535E, 0x535E, 0x5360, 0x5360, 0x5366, 0x5366, 0x5369, 0x5369, 0x536E, 0x5371,
	0x5373, 0x5375, 0x5377, 0x5378, 0x537B, 0x537B, 0x537F, 0x537F, 0x5382, 0x5382, 0x5384, 0x5384, 0x5396, 0x5396, 0x5398, 0x5398,
	0x539A, 0x539A, 0x539F, 0x53A0, 0x53A5, 0x53A6, 0x53A8, 0x53A9, 0x53AD, 0x53AE, 0x53B0, 0x53B0, 0x53B3, 0x53B3, 0x53B6, 0x53B6,
	0x53BB, 0x53BB, 0x53C2, 0x53C3, 0x53C8, 0x53CE, 0x53D4, 0x53D4, 0x53D6, 0x53D7, 0x53D9, 0x53D9, 0x53DB, 0x53DB, 0x53DF, 0x53DF,
	0x53E1, 0x53E5, 0x53E8, 0x53F3, 0x53F6, 0x53F8, 0x53FA, 0x53FA, 0x5401, 0x5401, 0x5403, 0x5404, 0x5408, 0x5411, 0x541B, 0x541B,
	0x541D, 0x541D, 0x541F, 0x5420, 0x5426, 0x5426, 0x5429, 0x5429, 0x542B, 0x542E, 0x5436, 0x5436, 0x5438, 0x5439, 0x543B, 0x543E,
	0x5440, 0x5440, 0x5442, 0x5442, 0x5446, 0x5446, 0x5448, 0x544A, 0x544E, 0x544E, 0x5451, 0x5451, 0x545F, 0x545F, 0x5468, 0x5468,
	0x546A, 0x546A, 0x5470, 0x5471, 0x5473, 0x5473, 0x5475, 0x5477, 0x547B, 0x547D, 0x5480, 0x5480, 0x5484, 0x5484, 0x5486, 0x5486,
	0x548B, 0x548C, 0x548E, 0x5490, 0x5492, 0x5492, 0x54A2, 0x54A2, 0x54A4, 0x54A5, 0x54A8, 0x54A8, 0x54AB, 0x54AC, 0x54AF, 0x54AF,
	0x54B2, 0x54B3, 0x54B8, 0x54B8, 0x54BC, 0x54BE, 0x54C0, 0x54C2, 0x54C4, 0x54C4, 0x54C7, 0x54C9, 0x54D8, 0x54D8, 0x54E1, 0x54E2,
	0x54E5, 0x54E6, 0x54E8, 0x54E9, 0x54ED, 0x54EE, 0x54F2, 0x54F2, 0x54FA, 0x54FA, 0x54FD, 0x54FD, 0x5504, 0x5504, 0x5506, 0x5507,
	0x550F, 0x5510, 0x5514, 0x5514, 0x5516, 0x5516, 0x552E, 0x552F, 0x5531, 0x5531, 0x5533, 0x5533, 0x5538, 0x5539, 0x553E, 0x553E,
	0x5540, 0x5540, 0x5544, 0x5546, 0x554C, 0x554C, 0x554F, 0x554F, 0x5553, 0x5553, 0x5556, 0x5557, 0x555C, 0x555D, 0x5563, 0x5563,
	0x557B, 0x557C, 0x557E, 0x557E, 0x5580, 0x5580, 0x5583, 0x5584, 0x5587, 0x5587, 0x5589, 0x558B, 0x5598, 0x559A, 0x559C, 0x559F,
	0x55A7, 0x55AC, 0x55AE, 0x55AE, 0x55B0, 0x55B0, 0x55B6, 0x55B6, 0x55C4, 0x55C5, 0x55C7, 0x55C7, 0x55D4, 0x55D4, 0x55DA, 0x55DA,
	0x55DC, 0x55DC, 0x55DF, 0x55DF, 0x55E3, 0x55E4, 0x55F7, 0x55F7, 0x55F9, 0x55F9, 0x55FD, 0x55FE, 0x5606, 0x5606, 0x5609, 0x5609,
	0x5614, 0x5614, 0x5616, 0x5618, 0x561B, 0x561B, 0x5629, 0x5629, 0x562F, 0x562F, 0x5631, 0x5632, 0x5634, 0x5634, 0x5636, 0x5636,
	0x5638, 0x5638, 0x5642, 0x5642, 0x564C, 0x564C, 0x564E, 0x564E, 0x5650, 0x5650, 0x565B, 0x565B, 0x5664, 0x5664, 0x5668, 0x5668,
	0x566A, 0x566C, 0x5674, 0x5674, 0x5678, 0x5678, 0x567A, 0x567A, 0x5680, 0x5680, 0x5686, 0x5687, 0x568A, 0x568A, 0x568F, 0x568F,
	0x5694, 0x5694, 0x56A0, 0x56A0, 0x56A2, 0x56A2, 0x56A5, 0x56A5, 0x56AE, 0x56AE, 0x56B4, 0x56B4, 0x56B6, 0x56B6, 0x56BC, 0x56BC,
	0x56C0, 0x56C3, 0x56C8, 0x56C8, 0x56CE, 0x56CE, 0x56D1, 0x56D1, 0x56D3, 0x56D3, 0x56D7, 0x56D8, 0x56DA, 0x56DB, 0x56DE, 0x56DE,
	0x56E0, 0x56E0, 0x56E3, 0x56E3, 0x56EE, 0x56EE, 0x56F0, 0x56F0, 0x56F2, 0x56F3, 0x56F9, 0x56FA, 0x56FD, 0x56FD, 0x56FF, 0x5700,
	0x5703, 0x5704, 0x5708, 0x5709, 0x570B, 0x570B, 0x570D, 0x570D, 0x570F, 0x570F, 0x5712, 0x5713, 0x5716, 0x5716, 0x5718, 0x5718,
	0x571C, 0x571C, 0x571F, 0x571F, 0x5726, 0x5728, 0x572D, 0x572D, 0x5730, 0x5730, 0x5737, 0x5738, 0x573B, 0x573B, 0x5740, 0x5740,
	0x5742, 0x5742, 0x5747, 0x5747, 0x574A, 0x574A, 0x574E, 0x5751, 0x5761, 0x5761, 0x5764, 0x5764, 0x5766, 0x5766, 0x5769, 0x576A,
	0x577F, 0x577F, 0x5782, 0x5782, 0x5788, 0x5789, 0x578B, 0x578B, 0x5793, 0x5793, 0x57A0, 0x57A0, 0x57A2, 0x57A4, 0x57AA, 0x57AA,
	0x57B0, 0x57B0, 0x57B3, 0x57B3, 0x57C0, 0x57C0, 0x57C3, 0x57C3, 0x57C6, 0x57C6, 0x57CB, 0x57CB, 0x57CE, 0x57CE, 0x57D2, 0x57D4,
	0x57D6, 0x57D6, 0x57DC, 0x57DC, 0x57DF, 0x57E0, 0x57E3, 0x57E3, 0x57F4, 0x57F4, 0x57F7, 0x57F7, 0x57F9, 0x57FA, 0x57FC, 0x57FC,
	0x5800, 0x5800, 0x5802, 0x5802, 0x5805, 0x5806, 0x580A, 0x580B, 0x5815, 0x5815, 0x5819, 0x5819, 0x581D, 0x581D, 0x5821, 0x5821,
	0x5824, 0x5824, 0x582A, 0x582A, 0x582F, 0x5831, 0x5834, 0x5835, 0x583A, 0x583A, 0x583D, 0x583D, 0x5840, 0x5841, 0x584A, 0x584B,
	0x5851, 0x5852, 0x5854, 0x5854, 0x5857, 0x585A, 0x585E, 0x585E, 0x5862, 0x5862, 0x5869, 0x5869, 0x586B, 0x586B, 0x5870, 0x5870,
	0x5872, 0x5872, 0x5875, 0x5875, 0x5879, 0x5879, 0x587E, 0x587E, 0x5883, 0x5883, 0x5885, 0x5885, 0x5893, 0x5893, 0x5897, 0x5897,
	0x589C, 0x589C, 0x589F, 0x589F, 0x58A8, 0x58A8, 0x58AB, 0x58AB, 0x58AE, 0x58AE, 0x58B3, 0x58B3, 0x58B8, 0x58BB, 0x58BE, 0x58BE,
	0x58C1, 0x58C1, 0x58C5, 0x58C5, 0x58C7, 0x58C7, 0x58CA, 0x58CA, 0x58CC, 0x58CC, 0x58D1, 0x58D1, 0x58D3, 0x58D3, 0x58D5, 0x58D5,
	0x58D7, 0x58D9, 0x58DC, 0x58DC, 0x58DE, 0x58DF, 0x58E4, 0x58E5, 0x58EB, 0x58EC, 0x58EE, 0x58F2, 0x58F7, 0x58F7, 0x58F9, 0x58FD,
	0x5902, 0x5902, 0x5909, 0x590A, 0x590F, 0x5910, 0x5915, 0x5916, 0x5918, 0x591C, 0x5922, 0x5922, 0x5925, 0x5925, 0x5927, 0x5927,
	0x5929, 0x592E, 0x5931, 0x5932, 0x5937, 0x5938, 0x593E, 0x593E, 0x5944, 0x5944, 0x5947, 0x5949, 0x594E, 0x5951, 0x5954, 0x5955,
	0x5957, 0x5958, 0x595A, 0x595A, 0x5960, 0x5960, 0x5962, 0x5962, 0x5965, 0x5965, 0x5967, 0x596A, 0x596C, 0x596C, 0x596E, 0x596E,
	0x5973, 0x5974, 0x5978, 0x5978, 0x597D, 0x597D, 0x5981, 0x5984, 0x598A, 0x598A, 0x598D, 0x598D, 0x5993, 0x5993, 0x5996, 0x5996,
	0x5999, 0x5999, 0x599B, 0x599B, 0x599D, 0x599D, 0x59A3, 0x59A3, 0x59A5, 0x59A5, 0x59A8, 0x59A8, 0x59AC, 0x59AC, 0x59B2, 0x59B2,
	0x59B9, 0x59B9, 0x59BB, 0x59BB, 0x59BE, 0x59BE, 0x59C6, 0x59C6, 0x59C9, 0x59C9, 0x59CB, 0x59CB, 0x59D0, 0x59D1, 0x59D3, 0x59D4,
	0x59D9, 0x59DA, 0x59DC, 0x59DC, 0x59E5, 0x59E6, 0x59E8, 0x59E8, 0x59EA, 0x59EB, 0x59F6, 0x59F6, 0x59FB, 0x59FB, 0x59FF, 0x59FF,
	0x5A01, 0x5A01, 0x5A03, 0x5A03, 0x5A09, 0x5A09, 0x5A11, 0x5A11, 0x5A18, 0x5A18, 0x5A1A, 0x5A1A, 0x5A1C, 0x5A1C, 0x5A1F, 0x5A20,
	0x5A25, 0x5A25, 0x5A29, 0x5A29, 0x5A2F, 0x5A2F, 0x5A35, 0x5A36, 0x5A3C, 0x5A3C, 0x5A40, 0x5A41, 0x5A46, 0x5A46, 0x5A49, 0x5A49,
	0x5A5A, 0x5A5A, 0x5A62, 0x5A62, 0x5A66, 0x5A66, 0x5A6A, 0x5A6A, 0x5A6C, 0x5A6C, 0x5A7F, 0x5A7F, 0x5A92, 0x5A92, 0x5A9A, 0x5A9B,
	0x5ABC, 0x5ABE, 0x5AC1, 0x5AC2, 0x5AC9, 0x5AC9, 0x5ACB, 0x5ACC, 0x5AD0, 0x5AD0, 0x5AD6, 0x5AD7, 0x5AE1, 0x5AE1, 0x5AE3, 0x5AE3,
	0x5AE6, 0x5AE6, 0x5AE9, 0x5AE9, 0x5AFA, 0x5AFB, 0x5B09, 0x5B09, 0x5B0B, 0x5B0C, 0x5B16, 0x5B16, 0x5B22, 0x5B22, 0x5B2A, 0x5B2A,
	0x5B2C, 0x5B2C, 0x5B30, 0x5B30, 0x5B32, 0x5B32, 0x5B36, 0x5B36, 0x5B3E, 0x5B3E, 0x5B40, 0x5B40, 0x5B43, 0x5B43, 0x5B45, 0x5B45,
	0x5B50, 0x5B51, 0x5B54, 0x5B55, 0x5B57, 0x5B58, 0x5B5A, 0x5B5D, 0x5B5F, 0x5B5F, 0x5B63, 0x5B66, 0x5B69, 0x5B69, 0x5B6B, 0x5B6B,
	0x5B70, 0x5B71, 0x5B73, 0x5B73, 0x5B75, 0x5B75, 0x5B78, 0x5B78, 0x5B7A, 0x5B7A, 0x5B80, 0x5B80, 0x5B83, 0x5B83, 0x5B85, 0x5B85,
	0x5B87, 0x5B89, 0x5B8B, 0x5B8D, 0x5B8F, 0x5B8F, 0x5B95, 0x5B95, 0x5B97, 0x5B9D, 0x5B9F, 0x5B9F, 0x5BA2, 0x5BA6, 0x5BAE, 0x5BAE,
	0x5BB0, 0x5BB0, 0x5BB3, 0x5BB6, 0x5BB8, 0x5BB9, 0x5BBF, 0x5BBF, 0x5BC2, 0x5BC7, 0x5BC9, 0x5BC9, 0x5BCC, 0x5BCC, 0x5BD0, 0x5BD0,
	0x5BD2, 0x5BD4, 0x5BDB, 0x5BDB, 0x5BDD, 0x5BDF, 0x5BE1, 0x5BE2, 0x5BE4, 0x5BE9, 0x5BEB, 0x5BEB, 0x5BEE, 0x5BEE, 0x5BF0, 0x5BF0,
	0x5BF3, 0x5BF3, 0x5BF5, 0x5BF6, 0x5BF8, 0x5BF8, 0x5BFA, 0x5BFA, 0x5BFE, 0x5BFF, 0x5C01, 0x5C02, 0x5C04, 0x5C0B, 0x5C0D, 0x5C0F,
	0x5C11, 0x5C11, 0x5C13, 0x5C13, 0x5C16, 0x5C16, 0x5C1A, 0x5C1A, 0x5C20, 0x5C20, 0x5C22, 0x5C22, 0x5C24, 0x5C24, 0x5C28, 0x5C28,
	0x5C2D, 0x5C2D, 0x5C31, 0x5C31, 0x5C38, 0x5C41, 0x5C45, 0x5C46, 0x5C48, 0x5C48, 0x5C4A, 0x5C4B, 0x5C4D, 0x5C51, 0x5C53, 0x5C53,
	0x5C55, 0x5C55, 0x5C5E, 0x5C5E, 0x5C60, 0x5C61, 0x5C64, 0x5C65, 0x5C6C, 0x5C6C, 0x5C6E, 0x5C6F, 0x5C71, 0x5C71, 0x5C76, 0x5C76,
	0x5C79, 0x5C79, 0x5C8C, 0x5C8C, 0x5C90, 0x5C91, 0x5C94, 0x5C94, 0x5CA1, 0x5CA1, 0x5CA8, 0x5CA9, 0x5CAB, 0x5CAC, 0x5CB1, 0x5CB1,
	0x5CB3, 0x5CB3, 0x5CB6, 0x5CB8, 0x5CBB, 0x5CBC, 0x5CBE, 0x5CBE, 0x5CC5, 0x5CC5, 0x5CC7, 0x5CC7, 0x5CD9, 0x5CD9, 0x5CE0, 0x5CE1,
	0x5CE8, 0x5CEA, 0x5CED, 0x5CED, 0x5CEF, 0x5CF0, 0x5CF6, 0x5CF6, 0x5CFA, 0x5CFB, 0x5CFD, 0x5CFD, 0x5D07, 0x5D07, 0x5D0B, 0x5D0B,
	0x5D0E, 0x5D0E, 0x5D11, 0x5D11, 0x5D14, 0x5D1B, 0x5D1F, 0x5D1F, 0x5D22, 0x5D22, 0x5D29, 0x5D29, 0x5D4B, 0x5D4C, 0x5D4E, 0x5D4E,
	0x5D50, 0x5D50, 0x5D52, 0x5D52, 0x5D5C, 0x5D5C, 0x5D69, 0x5D69, 0x5D6C, 0x5D6C, 0x5D6F, 0x5D6F, 0x5D73, 0x5D73, 0x5D76, 0x5D76,
	0x5D82, 0x5D82, 0x5D84, 0x5D84, 0x5D87, 0x5D87, 0x5D8B, 0x5D8C, 0x5D90, 0x5D90, 0x5D9D, 0x5D9D, 0x5DA2, 0x5DA2, 0x5DAC, 0x5DAC,
	0x5DAE, 0x5DAE, 0x5DB7, 0x5DB7, 0x5DBA, 0x5DBA, 0x5DBC, 0x5DBD, 0x5DC9, 0x5DC9, 0x5DCC, 0x5DCD, 0x5DD2, 0x5DD3, 0x5DD6, 0x5DD6,
	0x5DDB, 0x5DDB, 0x5DDD, 0x5DDE, 0x5DE1, 0x5DE1, 0x5DE3, 0x5DE3, 0x5DE5, 0x5DE8, 0x5DEB, 0x5DEB, 0x5DEE, 0x5DEE, 0x5DF1, 0x5DF5,
	0x5DF7, 0x5DF7, 0x5DFB, 0x5DFB, 0x5DFD, 0x5DFE, 0x5E02, 0x5E03, 0x5E06, 0x5E06, 0x5E0B, 0x5E0C, 0x5E11, 0x5E11, 0x5E16, 0x5E16,
	0x5E19, 0x5E1B, 0x5E1D, 0x5E1D, 0x5E25, 0x5E25, 0x5E2B, 0x5E2B, 0x5E2D, 0x5E2D, 0x5E2F, 0x5E30, 0x5E33, 0x5E33, 0x5E36, 0x5E38,
	0x5E3D, 0x5E3D, 0x5E40, 0x5E40, 0x5E43, 0x5E45, 0x5E47, 0x5E47, 0x5E4C, 0x5E4C, 0x5E4E, 0x5E4E, 0x5E54, 0x5E55, 0x5E57, 0x5E57,
	0x5E5F, 0x5E5F, 0x5E61, 0x5E64, 0x5E72, 0x5E76, 0x5E78, 0x5E7F, 0x5E81, 0x5E81, 0x5E83, 0x5E84, 0x5E87, 0x5E87, 0x5E8A, 0x5E8A,
	0x5E8F, 0x5E8F, 0x5E95, 0x5E97, 0x5E9A, 0x5E9A, 0x5E9C, 0x5E9C, 0x5EA0, 0x5EA0, 0x5EA6, 0x5EA7, 0x5EAB, 0x5EAB, 0x5EAD, 0x5EAD,
	0x5EB5, 0x5EB8, 0x5EC1, 0x5EC3, 0x5EC8, 0x5ECA, 0x5ECF, 0x5ED0, 0x5ED3, 0x5ED3, 0x5ED6, 0x5ED6, 0x5EDA, 0x5EDB, 0x5EDD, 0x5EDD,
	0x5EDF, 0x5EE3, 0x5EE8, 0x5EE9, 0x5EEC, 0x5EEC, 0x5EF0, 0x5EF1, 0x5EF3, 0x5EF4, 0x5EF6, 0x5EF8, 0x5EFA, 0x5EFC, 0x5EFE, 0x5EFF,
	0x5F01, 0x5F01, 0x5F03, 0x5F04, 0x5F09, 0x5F0D, 0x5F0F, 0x5F11, 0x5F13, 0x5F18, 0x5F1B, 0x5F1B, 0x5F1F, 0x5F1F, 0x5F25, 0x5F27,
	0x5F29, 0x5F29, 0x5F2D, 0x5F2D, 0x5F2F, 0x5F2F, 0x5F31, 0x5F31, 0x5F35, 0x5F35, 0x5F37, 0x5F38, 0x5F3C, 0x5F3C, 0x5F3E, 0x5F3E,
	0x5F41, 0x5F41, 0x5F48, 0x5F48, 0x5F4A, 0x5F4A, 0x5F4C, 0x5F4C, 0x5F4E, 0x5F4E, 0x5F51, 0x5F51, 0x5F53, 0x5F53, 0x5F56, 0x5F57,
	0x5F59, 0x5F59, 0x5F5C, 0x5F5D, 0x5F61, 0x5F62, 0x5F66, 0x5F66, 0x5F69, 0x5F6D, 0x5F70, 0x5F71, 0x5F73, 0x5F73, 0x5F77, 0x5F77,
	0x5F79, 0x5F79, 0x5F7C, 0x5F7C, 0x5F7F, 0x5F85, 0x5F87, 0x5F88, 0x5F8A, 0x5F8C, 0x5F90, 0x5F93, 0x5F97, 0x5F99, 0x5F9E, 0x5F9E,
	0x5FA0, 0x5FA1, 0x5FA8, 0x5FAA, 0x5FAD, 0x5FAE, 0x5FB3, 0x5FB4, 0x5FB9, 0x5FB9, 0x5FBC, 0x5FBD, 0x5FC3, 0x5FC3, 0x5FC5, 0x5FC5,
	0x5FCC, 0x5FCD, 0x5FD6, 0x5FD9, 0x5FDC, 0x5FDD, 0x5FE0, 0x5FE0, 0x5FE4, 0x5FE4, 0x5FEB, 0x5FEB, 0x5FF0, 0x5FF1, 0x5FF5, 0x5FF5,
	0x5FF8, 0x5FF8, 0x5FFB, 0x5FFB, 0x5FFD, 0x5FFD, 0x5FFF, 0x5FFF, 0x600E, 0x6010, 0x6012, 0x6012, 0x6015, 0x6016, 0x6019, 0x6019,
	0x601B, 0x601D, 0x6020, 0x6021, 0x6025, 0x602B, 0x602F, 0x602F, 0x6031, 0x6031, 0x603A, 0x603A, 0x6041, 0x6043, 0x6046, 0x6046,
	0x604A, 0x604B, 0x604D, 0x604D, 0x6050, 0x6050, 0x6052, 0x6052, 0x6055, 0x6055, 0x6059, 0x605A, 0x605F, 0x6060, 0x6062, 0x6065,
	0x6068, 0x606D, 0x606F, 0x6070, 0x6075, 0x6075, 0x6077, 0x6077, 0x6081, 0x6081, 0x6083, 0x6084, 0x6089, 0x6089, 0x608B, 0x608D,
	0x6092, 0x6092, 0x6094, 0x6094, 0x6096, 0x6097, 0x609A, 0x609B, 0x609F, 0x60A0, 0x60A3, 0x60A3, 0x60A6, 0x60A7, 0x60A9, 0x60AA,
	0x60B2, 0x60B6, 0x60B8, 0x60B8, 0x60BC, 0x60BD, 0x60C5, 0x60C7, 0x60D1, 0x60D1, 0x60D3, 0x60D3, 0x60D8, 0x60D8, 0x60DA, 0x60DA,
	0x60DC, 0x60DC, 0x60DF, 0x60E1, 0x60E3, 0x60E3, 0x60E7, 0x60E8, 0x60F0, 0x60F1, 0x60F3, 0x60F4, 0x60F6, 0x60F7, 0x60F9, 0x60FB,
	0x6100, 0x6101, 0x6103, 0x6103, 0x6106, 0x6106, 0x6108, 0x6109, 0x610D, 0x610F, 0x6115, 0x6115, 0x611A, 0x611B, 0x611F, 0x611F,
	0x6121, 0x6121, 0x6127, 0x6128, 0x612C, 0x612C, 0x6134, 0x6134, 0x613C, 0x613F, 0x6142, 0x6142, 0x6144, 0x6144, 0x6147, 0x6148,
	0x614A, 0x614E, 0x6153, 0x6153, 0x6155, 0x6155, 0x6158, 0x615A, 0x615D, 0x615D, 0x615F, 0x615F, 0x6162, 0x6163, 0x6165, 0x6165,
	0x6167, 0x6168, 0x616B, 0x616B, 0x616E, 0x6171, 0x6173, 0x6177, 0x617E, 0x617E, 0x6182, 0x6182, 0x6187, 0x6187, 0x618A, 0x618A,
	0x618E, 0x618E, 0x6190, 0x6191, 0x6194, 0x6194, 0x6196, 0x6196, 0x6199, 0x619A, 0x61A4, 0x61A4, 0x61A7, 0x61A7, 0x61A9, 0x61A9,
	0x61AB, 0x61AC, 0x61AE, 0x61AE, 0x61B2, 0x61B2, 0x61B6, 0x61B6, 0x61BA, 0x61BA, 0x61BE, 0x61BE, 0x61C3, 0x61C3, 0x61C6, 0x61CD,
	0x61D0, 0x61D0, 0x61E3, 0x61E3, 0x61E6, 0x61E6, 0x61F2, 0x61F2, 0x61F4, 0x61F4, 0x61F6, 0x61F8, 0x61FA, 0x61FA, 0x61FC, 0x6200,
	0x6208, 0x620A, 0x620C, 0x620E, 0x6210, 0x6212, 0x6214, 0x6214, 0x6216, 0x6216, 0x621A, 0x621B, 0x621D, 0x621F, 0x6221, 0x6221,
	0x6226, 0x6226, 0x622A, 0x622A, 0x622E, 0x6230, 0x6232, 0x6234, 0x6238, 0x6238, 0x623B, 0x623B, 0x623F, 0x6241, 0x6247, 0x6249,
	0x624B, 0x624B, 0x624D, 0x624E, 0x6253, 0x6253, 0x6255, 0x6255, 0x6258, 0x6258, 0x625B, 0x625B, 0x625E, 0x625E, 0x6260, 0x6260,
	0x6263, 0x6263, 0x6268, 0x6268, 0x626E, 0x626E, 0x6271, 0x6271, 0x6276, 0x6276, 0x6279, 0x6279, 0x627C, 0x627C, 0x627E, 0x6280,
	0x6282, 0x6284, 0x6289, 0x628A, 0x6291, 0x6298, 0x629B, 0x629C, 0x629E, 0x629E, 0x62AB, 0x62AC, 0x62B1, 0x62B1, 0x62B5, 0x62B5,
	0x62B9, 0x62B9, 0x62BB, 0x62BD, 0x62C2, 0x62C2, 0x62C5, 0x62CA, 0x62CC, 0x62CD, 0x62CF, 0x62D4, 0x62D7, 0x62D9, 0x62DB, 0x62DD,
	0x62E0, 0x62E1, 0x62EC, 0x62EF, 0x62F1, 0x62F1, 0x62F3, 0x62F3, 0x62F5, 0x62F7, 0x62FE, 0x62FF, 0x6301, 0x6302, 0x6307, 0x6309,
	0x630C, 0x630C, 0x6311, 0x6311, 0x6319, 0x6319, 0x631F, 0x631F, 0x6327, 0x6328, 0x632B, 0x632B, 0x632F, 0x632F, 0x633A, 0x633A,
	0x633D, 0x633F, 0x6349, 0x6349, 0x634C, 0x634D, 0x634F, 0x6350, 0x6355, 0x6355, 0x6357, 0x6357, 0x635C, 0x635C, 0x6367, 0x6369,
	0x636B, 0x636B, 0x636E, 0x636E, 0x6372, 0x6372, 0x6376, 0x6377, 0x637A, 0x637B, 0x6380, 0x6380, 0x6383, 0x6383, 0x6388, 0x6389,
	0x638C, 0x638C, 0x638E, 0x638F, 0x6392, 0x6392, 0x6396, 0x6396, 0x6398, 0x6398, 0x639B, 0x639B, 0x639F, 0x63A3, 0x63A5, 0x63A5,
	0x63A7, 0x63AC, 0x63B2, 0x63B2, 0x63B4, 0x63B5, 0x63BB, 0x63BB, 0x63BE, 0x63BE, 0x63C0, 0x63C0, 0x63C3, 0x63C4, 0x63C6, 0x63C6,
	0x63C9, 0x63C9, 0x63CF, 0x63D0, 0x63D2, 0x63D2, 0x63D6, 0x63D6, 0x63DA, 0x63DB, 0x63E1, 0x63E1, 0x63E3, 0x63E3, 0x63E9, 0x63E9,
	0x63EE, 0x63EE, 0x63F4, 0x63F4, 0x63F6, 0x63F6, 0x63FA, 0x63FA, 0x6406, 0x6406, 0x640D, 0x640D, 0x640F, 0x640F, 0x6413, 0x6413,
	0x6416, 0x6417, 0x641C, 0x641C, 0x6426, 0x6426, 0x6428, 0x6428, 0x642C, 0x642D, 0x6434, 0x6434, 0x6436, 0x6436, 0x643A, 0x643A,
	0x643E, 0x643E, 0x6442, 0x6442, 0x644E, 0x644E, 0x6458, 0x6458, 0x6467, 0x6467, 0x6469, 0x6469, 0x646F, 0x646F, 0x6476, 0x6476,
	0x6478, 0x6478, 0x647A, 0x647A, 0x6483, 0x6483, 0x6488, 0x6488, 0x6492, 0x6493, 0x6495, 0x6495, 0x649A, 0x649A, 0x649E, 0x649E,
	0x64A4, 0x64A5, 0x64A9, 0x64A9, 0x64AB, 0x64AB, 0x64AD, 0x64AE, 0x64B0, 0x64B0, 0x64B2, 0x64B2, 0x64B9, 0x64B9, 0x64BB, 0x64BC,
	0x64C1, 0x64C2, 0x64C5, 0x64C5, 0x64C7, 0x64C7, 0x64CD, 0x64CD, 0x64D2, 0x64D2, 0x64D4, 0x64D4, 0x64D8, 0x64D8, 0x64DA, 0x64DA,
	0x64E0, 0x64E3, 0x64E6, 0x64E7, 0x64EC, 0x64EC, 0x64EF, 0x64EF, 0x64F1, 0x64F2, 0x64F4, 0x64F4, 0x64F6, 0x64F6, 0x64FA, 0x64FA,
	0x64FD, 0x64FE, 0x6500, 0x6500, 0x6505, 0x6505, 0x6518, 0x6518, 0x651C, 0x651D, 0x6523, 0x6524, 0x652A, 0x652C, 0x652F, 0x652F,
	0x6534, 0x6539, 0x653B, 0x653B, 0x653E, 0x653F, 0x6545, 0x6545, 0x6548, 0x6548, 0x654D, 0x654D, 0x654F, 0x654F, 0x6551, 0x6551,
	0x6555, 0x6559, 0x655D, 0x655E, 0x6562, 0x6563, 0x6566, 0x6566, 0x656C, 0x656C, 0x6570, 0x6570, 0x6572, 0x6572, 0x6574, 0x6575,
	0x6577, 0x6578, 0x6582, 0x6583, 0x6587, 0x6589, 0x658C, 0x658C, 0x658E, 0x658E, 0x6590, 0x6591, 0x6597, 0x6597, 0x6599, 0x6599,
	0x659B, 0x659C, 0x659F, 0x659F, 0x65A1, 0x65A1, 0x65A4, 0x65A5, 0x65A7, 0x65A7, 0x65AB, 0x65AD, 0x65AF, 0x65B0, 0x65B7, 0x65B7,
	0x65B9, 0x65B9, 0x65BC, 0x65BD, 0x65C1, 0x65C1, 0x65C3, 0x65C6, 0x65CB, 0x65CC, 0x65CF, 0x65CF, 0x65D2, 0x65D2, 0x65D7, 0x65D7,
	0x65D9, 0x65D9, 0x65DB, 0x65DB, 0x65E0, 0x65E2, 0x65E5, 0x65E9, 0x65EC, 0x65ED, 0x65F1, 0x65F1, 0x65FA, 0x65FB, 0x6602, 0x6603,
	0x6606, 0x6607, 0x660A, 0x660A, 0x660C, 0x660C, 0x660E, 0x660F, 0x6613, 0x6614, 0x661C, 0x661C, 0x661F, 0x6620, 0x6625, 0x6625,
	0x6627, 0x6628, 0x662D, 0x662D, 0x662F, 0x662F, 0x6634, 0x6636, 0x663C, 0x663C, 0x663F, 0x663F, 0x6641, 0x6644, 0x6649, 0x6649,
	0x664B, 0x664B, 0x664F, 0x664F, 0x6652, 0x6652, 0x665D, 0x665F, 0x6662, 0x6662, 0x6664, 0x6664, 0x6666, 0x6669, 0x666E, 0x6670,
	0x6674, 0x6674, 0x6676, 0x6676, 0x667A, 0x667A, 0x6681, 0x6681, 0x6683, 0x6684, 0x6687, 0x6689, 0x668E, 0x668E, 0x6691, 0x6691,
	0x6696, 0x6698, 0x669D, 0x669D, 0x66A2, 0x66A2, 0x66A6, 0x66A6, 0x66AB, 0x66AB, 0x66AE, 0x66AE, 0x66B4, 0x66B4, 0x66B8, 0x66B9,
	0x66BC, 0x66BC, 0x66BE, 0x66BE, 0x66C1, 0x66C1, 0x66C4, 0x66C4, 0x66C7, 0x66C7, 0x66C9, 0x66C9, 0x66D6, 0x66D6, 0x66D9, 0x66DA,
	0x66DC, 0x66DD, 0x66E0, 0x66E0, 0x66E6, 0x66E6, 0x66E9, 0x66E9, 0x66F0, 0x66F0, 0x66F2, 0x66F5, 0x66F7, 0x66F9, 0x66FC, 0x6700,
	0x6703, 0x6703, 0x6708, 0x6709, 0x670B, 0x670B, 0x670D, 0x670D, 0x670F, 0x670F, 0x6714, 0x6717, 0x671B, 0x671B, 0x671D, 0x671F,
	0x6726, 0x6728, 0x672A, 0x672E, 0x6731, 0x6731, 0x6734, 0x6734, 0x6736, 0x6738, 0x673A, 0x673A, 0x673D, 0x673D, 0x673F, 0x673F,
	0x6741, 0x6741, 0x6746, 0x6746, 0x6749, 0x6749, 0x674E, 0x6751, 0x6753, 0x6753, 0x6756, 0x6756, 0x6759, 0x6759, 0x675C, 0x675C,
	0x675E, 0x6765, 0x676A, 0x676A, 0x676D, 0x676D, 0x676F, 0x6773, 0x6775, 0x6775, 0x6777, 0x6777, 0x677C, 0x677C, 0x677E, 0x677F,
	0x6785, 0x6785, 0x6787, 0x6787, 0x6789, 0x6789, 0x678B, 0x678C, 0x6790, 0x6790, 0x6795, 0x6795, 0x6797, 0x6797, 0x679A, 0x679A,
	0x679C, 0x679D, 0x67A0, 0x67A2, 0x67A6, 0x67A6, 0x67A9, 0x67A9, 0x67AF, 0x67AF, 0x67B3, 0x67B4, 0x67B6, 0x67B9, 0x67C1, 0x67C1,
	0x67C4, 0x67C4, 0x67C6, 0x67C6, 0x67CA, 0x67CA, 0x67CE, 0x67D1, 0x67D3, 0x67D4, 0x67D8, 0x67D8, 0x67DA, 0x67DA, 0x67DD, 0x67DE,
	0x67E2, 0x67E2, 0x67E4, 0x67E4, 0x67E7, 0x67E7, 0x67E9, 0x67E9, 0x67EC, 0x67EC, 0x67EE, 0x67EF, 0x67F1, 0x67F1, 0x67F3, 0x67F5,
	0x67FB, 0x67FB, 0x67FE, 0x67FF, 0x6802, 0x6804, 0x6813, 0x6813, 0x6816, 0x6817, 0x681E, 0x681E, 0x6821, 0x6822, 0x6829, 0x682B,
	0x6832, 0x6832, 0x6834, 0x6834, 0x6838, 0x6839, 0x683C, 0x683D, 0x6840, 0x6843, 0x6846, 0x6846, 0x6848, 0x6848, 0x684D, 0x684E,
	0x6850, 0x6851, 0x6853, 0x6854, 0x6859, 0x6859, 0x685C, 0x685D, 0x685F, 0x685F, 0x6863, 0x6863, 0x6867, 0x6867, 0x6874, 0x6874,
	0x6876, 0x6877, 0x687E, 0x687F, 0x6881, 0x6881, 0x6883, 0x6883, 0x6885, 0x6885, 0x688D, 0x688D, 0x688F, 0x688F, 0x6893, 0x6894,
	0x6897, 0x6897, 0x689B, 0x689B, 0x689D, 0x689D, 0x689F, 0x68A0, 0x68A2, 0x68A2, 0x68A6, 0x68A8, 0x68AD, 0x68AD, 0x68AF, 0x68B1,
	0x68B3, 0x68B3, 0x68B5, 0x68B6, 0x68B9, 0x68BA, 0x68BC, 0x68BC, 0x68C4, 0x68C4, 0x68C6, 0x68C6, 0x68C9, 0x68CB, 0x68CD, 0x68CD,
	0x68D2, 0x68D2, 0x68D4, 0x68D5, 0x68D7, 0x68D8, 0x68DA, 0x68DA, 0x68DF, 0x68E1, 0x68E3, 0x68E3, 0x68E7, 0x68E7, 0x68EE, 0x68EF,
	0x68F2, 0x68F2, 0x68F9, 0x68FA, 0x6900, 0x6901, 0x6904, 0x6905, 0x6908, 0x6908, 0x690B, 0x690F, 0x6912, 0x6912, 0x6919, 0x691C,
	0x6921, 0x6923, 0x6925, 0x6926, 0x6928, 0x6928, 0x692A, 0x692A, 0x6930, 0x6930, 0x6934, 0x6934, 0x6936, 0x6936, 0x6939, 0x6939,
	0x693D, 0x693D, 0x693F, 0x693F, 0x694A, 0x694A, 0x6953, 0x6955, 0x6959, 0x695A, 0x695C, 0x695E, 0x6960, 0x6962, 0x696A, 0x696B,
	0x696D, 0x696F, 0x6973, 0x6975, 0x6977, 0x6979, 0x697C, 0x697E, 0x6981, 0x6982, 0x698A, 0x698A, 0x698E, 0x698E, 0x6991, 0x6991,
	0x6994, 0x6995, 0x699B, 0x699C, 0x69A0, 0x69A0, 0x69A7, 0x69A7, 0x69AE, 0x69AE, 0x69B1, 0x69B2, 0x69B4, 0x69B4, 0x69BB, 0x69BB,
	0x69BE, 0x69BF, 0x69C1, 0x69C1, 0x69C3, 0x69C3, 0x69C7, 0x69C7, 0x69CA, 0x69CE, 0x69D0, 0x69D0, 0x69D3, 0x69D3, 0x69D8, 0x69D9,
	0x69DD, 0x69DE, 0x69E7, 0x69E8, 0x69EB, 0x69EB, 0x69ED, 0x69ED, 0x69F2, 0x69F2, 0x69F9, 0x69F9, 0x69FB, 0x69FB, 0x69FD, 0x69FD,
	0x69FF, 0x69FF, 0x6A02, 0x6A02, 0x6A05, 0x6A05, 0x6A0A, 0x6A0C, 0x6A12, 0x6A14, 0x6A17, 0x6A17, 0x6A19, 0x6A19, 0x6A1B, 0x6A1B,
	0x6A1E, 0x6A1F, 0x6A21, 0x6A23, 0x6A29, 0x6A2B, 0x6A2E, 0x6A2E, 0x6A35, 0x6A36, 0x6A38, 0x6A3A, 0x6A3D, 0x6A3D, 0x6A44, 0x6A44,
	0x6A47, 0x6A48, 0x6A4B, 0x6A4B, 0x6A58, 0x6A59, 0x6A5F, 0x6A5F, 0x6A61, 0x6A62, 0x6A66, 0x6A66, 0x6A72, 0x6A72, 0x6A78, 0x6A78,
	0x6A7F, 0x6A80, 0x6A84, 0x6A84, 0x6A8D, 0x6A8E, 0x6A90, 0x6A90, 0x6A97, 0x6A97, 0x6A9C, 0x6A9C, 0x6AA0, 0x6AA0, 0x6AA2, 0x6AA3,
	0x6AAA, 0x6AAA, 0x6AAC, 0x6AAC, 0x6AAE, 0x6AAE, 0x6AB3, 0x6AB3, 0x6AB8, 0x6AB8, 0x6ABB, 0x6ABB, 0x6AC1, 0x6AC3, 0x6AD1, 0x6AD1,
	0x6AD3, 0x6AD3, 0x6ADA, 0x6ADB, 0x6ADE, 0x6ADF, 0x6AE8, 0x6AE8, 0x6AEA, 0x6AEA, 0x6AFA, 0x6AFB, 0x6B04, 0x6B05, 0x6B0A, 0x6B0A,
	0x6B12, 0x6B12, 0x6B16, 0x6B16, 0x6B1D, 0x6B1D, 0x6B1F, 0x6B21, 0x6B23, 0x6B23, 0x6B27, 0x6B27, 0x6B32, 0x6B32, 0x6B37, 0x6B3A,
	0x6B3D, 0x6B3E, 0x6B43, 0x6B43, 0x6B47, 0x6B47, 0x6B49, 0x6B49, 0x6B4C, 0x6B4C, 0x6B4E, 0x6B4E, 0x6B50, 0x6B50, 0x6B53, 0x6B54,
	0x6B59, 0x6B59, 0x6B5B, 0x6B5B, 0x6B5F, 0x6B5F, 0x6B61, 0x6B64, 0x6B66, 0x6B66, 0x6B69, 0x6B6A, 0x6B6F, 0x6B6F, 0x6B73, 0x6B74,
	0x6B78, 0x6B79, 0x6B7B, 0x6B7B, 0x6B7F, 0x6B80, 0x6B83, 0x6B84, 0x6B86, 0x6B86, 0x6B89, 0x6B8B, 0x6B8D, 0x6B8D, 0x6B95, 0x6B96,
	0x6B98, 0x6B98, 0x6B9E, 0x6B9E, 0x6BA4, 0x6BA4, 0x6BAA, 0x6BAB, 0x6BAF, 0x6BAF, 0x6BB1, 0x6BB5, 0x6BB7, 0x6BB7, 0x6BBA, 0x6BBC,
	0x6BBF, 0x6BC0, 0x6BC5, 0x6BC6, 0x6BCB, 0x6BCB, 0x6BCD, 0x6BCE, 0x6BD2, 0x6BD4, 0x6BD8, 0x6BD8, 0x6BDB, 0x6BDB, 0x6BDF, 0x6BDF,
	0x6BEB, 0x6BEC, 0x6BEF, 0x6BEF, 0x6BF3, 0x6BF3, 0x6C08, 0x6C08, 0x6C0F, 0x6C0F, 0x6C11, 0x6C11, 0x6C13, 0x6C14, 0x6C17, 0x6C17,
	0x6C1B, 0x6C1B, 0x6C23, 0x6C24, 0x6C34, 0x6C34, 0x6C37, 0x6C38, 0x6C3E, 0x6C3E, 0x6C40, 0x6C42, 0x6C4E, 0x6C4E, 0x6C50, 0x6C50,
	0x6C55, 0x6C55, 0x6C57, 0x6C57, 0x6C5A, 0x6C5A, 0x6C5D, 0x6C60, 0x6C62, 0x6C62, 0x6C68, 0x6C68, 0x6C6A, 0x6C6A, 0x6C70, 0x6C70,
	0x6C72, 0x6C73, 0x6C7A, 0x6C7A, 0x6C7D, 0x6C7E, 0x6C81, 0x6C83, 0x6C88, 0x6C88, 0x6C8C, 0x6C8D, 0x6C90, 0x6C90, 0x6C92, 0x6C93,
	0x6C96, 0x6C96, 0x6C99, 0x6C9B, 0x6CA1, 0x6CA2, 0x6CAB, 0x6CAB, 0x6CAE, 0x6CAE, 0x6CB1, 0x6CB1, 0x6CB3, 0x6CB3, 0x6CB8, 0x6CBF,
	0x6CC1, 0x6CC1, 0x6CC4, 0x6CC5, 0x6CC9, 0x6CCA, 0x6CCC, 0x6CCC, 0x6CD3, 0x6CD3, 0x6CD5, 0x6CD5, 0x6CD7, 0x6CD7, 0x6CD9, 0x6CD9,
	0x6CDB, 0x6CDB, 0x6CDD, 0x6CDD, 0x6CE1, 0x6CE3, 0x6CE5, 0x6CE5, 0x6CE8, 0x6CE8, 0x6CEA, 0x6CEA, 0x6CEF, 0x6CF1, 0x6CF3, 0x6CF3,
	0x6D0B, 0x6D0C, 0x6D12, 0x6D12, 0x6D17, 0x6D17, 0x6D19, 0x6D19, 0x6D1B, 0x6D1B, 0x6D1E, 0x6D1F, 0x6D25, 0x6D25, 0x6D29, 0x6D2B,
	0x6D32, 0x6D33, 0x6D35, 0x6D36, 0x6D38, 0x6D38, 0x6D3B, 0x6D3B, 0x6D3D, 0x6D3E, 0x6D41, 0x6D41, 0x6D44, 0x6D45, 0x6D59, 0x6D5A,
	0x6D5C, 0x6D5C, 0x6D63, 0x6D64, 0x6D66, 0x6D66, 0x6D69, 0x6D6A, 0x6D6C, 0x6D6C, 0x6D6E, 0x6D6E, 0x6D74, 0x6D74, 0x6D77, 0x6D79,
	0x6D85, 0x6D85, 0x6D88, 0x6D88, 0x6D8C, 0x6D8C, 0x6D8E, 0x6D8E, 0x6D93, 0x6D93, 0x6D95, 0x6D95, 0x6D99, 0x6D99, 0x6D9B, 0x6D9C,
	0x6DAF, 0x6DAF, 0x6DB2, 0x6DB2, 0x6DB5, 0x6DB5, 0x6DB8, 0x6DB8, 0x6DBC, 0x6DBC, 0x6DC0, 0x6DC0, 0x6DC5, 0x6DC7, 0x6DCB, 0x6DCC,
	0x6DD1, 0x6DD2, 0x6DD5, 0x6DD5, 0x6DD8, 0x6DD9, 0x6DDE, 0x6DDE, 0x6DE1, 0x6DE1, 0x6DE4, 0x6DE4, 0x6DE6, 0x6DE6, 0x6DE8, 0x6DE8,
	0x6DEA, 0x6DEC, 0x6DEE, 0x6DEE, 0x6DF1, 0x6DF1, 0x6DF3, 0x6DF3, 0x6DF5, 0x6DF5, 0x6DF7, 0x6DF7, 0x6DF9, 0x6DFB, 0x6E05, 0x6E05,
	0x6E07, 0x6E0B, 0x6E13, 0x6E13, 0x6E15, 0x6E15, 0x6E19, 0x6E1B, 0x6E1D, 0x6E1D, 0x6E1F, 0x6E21, 0x6E23, 0x6E26, 0x6E29, 0x6E29,
	0x6E2B, 0x6E2F, 0x6E38, 0x6E38, 0x6E3A, 0x6E3A, 0x6E3E, 0x6E3E, 0x6E43, 0x6E43, 0x6E4A, 0x6E4A, 0x6E4D, 0x6E4E, 0x6E56, 0x6E56,
	0x6E58, 0x6E58, 0x6E5B, 0x6E5B, 0x6E5F, 0x6E5F, 0x6E67, 0x6E67, 0x6E6B, 0x6E6B, 0x6E6E, 0x6E6F, 0x6E72, 0x6E72, 0x6E76, 0x6E76,
	0x6E7E, 0x6E80, 0x6E82, 0x6E82, 0x6E8C, 0x6E8C, 0x6E8F, 0x6E90, 0x6E96, 0x6E96, 0x6E98, 0x6E98, 0x6E9C, 0x6E9D, 0x6E9F, 0x6E9F,
	0x6EA2, 0x6EA2, 0x6EA5, 0x6EA5, 0x6EAA, 0x6EAA, 0x6EAF, 0x6EAF, 0x6EB2, 0x6EB2, 0x6EB6, 0x6EB7, 0x6EBA, 0x6EBA, 0x6EBD, 0x6EBD,
	0x6EC2, 0x6EC2, 0x6EC4, 0x6EC5, 0x6EC9, 0x6EC9, 0x6ECB, 0x6ECC, 0x6ED1, 0x6ED1, 0x6ED3, 0x6ED5, 0x6EDD, 0x6EDE, 0x6EEC, 0x6EEC,
	0x6EEF, 0x6EEF, 0x6EF2, 0x6EF2, 0x6EF4, 0x6EF4, 0x6EF7, 0x6EF8, 0x6EFE, 0x6EFF, 0x6F01, 0x6F02, 0x6F06, 0x6F06, 0x6F09, 0x6F09,
	0x6F0F, 0x6F0F, 0x6F11, 0x6F11, 0x6F13, 0x6F15, 0x6F20, 0x6F20, 0x6F22, 0x6F23, 0x6F2B, 0x6F2C, 0x6F31, 0x6F32, 0x6F38, 0x6F38,
	0x6F3E, 0x6F3F, 0x6F41, 0x6F41, 0x6F45, 0x6F45, 0x6F54, 0x6F54, 0x6F58, 0x6F58, 0x6F5B, 0x6F5C, 0x6F5F, 0x6F5F, 0x6F64, 0x6F64,
	0x6F66, 0x6F66, 0x6F6D, 0x6F70, 0x6F74, 0x6F74, 0x6F78, 0x6F78, 0x6F7A, 0x6F7A, 0x6F7C, 0x6F7C, 0x6F80, 0x6F82, 0x6F84, 0x6F84,
	0x6F86, 0x6F86, 0x6F8E, 0x6F8E, 0x6F91, 0x6F91, 0x6F97, 0x6F97, 0x6FA1, 0x6FA1, 0x6FA3, 0x6FA4, 0x6FAA, 0x6FAA, 0x6FB1, 0x6FB1,
	0x6FB3, 0x6FB3, 0x6FB9, 0x6FB9, 0x6FC0, 0x6FC3, 0x6FC6, 0x6FC6, 0x6FD4, 0x6FD5, 0x6FD8, 0x6FD8, 0x6FDB, 0x6FDB, 0x6FDF, 0x6FE1,
	0x6FE4, 0x6FE4, 0x6FEB, 0x6FEC, 0x6FEE, 0x6FEF, 0x6FF1, 0x6FF1, 0x6FF3, 0x6FF3, 0x6FF6, 0x6FF6, 0x6FFA, 0x6FFA, 0x6FFE, 0x6FFE,
	0x7001, 0x7001, 0x7009, 0x7009, 0x700B, 0x700B, 0x700F, 0x700F, 0x7011, 0x7011, 0x7015, 0x7015, 0x7018, 0x7018, 0x701A, 0x701B,
	0x701D, 0x701F, 0x7026, 0x7027, 0x702C, 0x702C, 0x7030, 0x7030, 0x7032, 0x7032, 0x703E, 0x703E, 0x704C, 0x704C, 0x7051, 0x7051,
	0x7058, 0x7058, 0x7063, 0x7063, 0x706B, 0x706B, 0x706F, 0x7070, 0x7078, 0x7078, 0x707C, 0x707D, 0x7089, 0x708A, 0x708E, 0x708E,
	0x7092, 0x7092, 0x7099, 0x7099, 0x70AC, 0x70AF, 0x70B3, 0x70B3, 0x70B8, 0x70BA, 0x70C8, 0x70C8, 0x70CB, 0x70CB, 0x70CF, 0x70CF,
	0x70D9, 0x70D9, 0x70DD, 0x70DD, 0x70DF, 0x70DF, 0x70F1, 0x70F1, 0x70F9, 0x70F9, 0x70FD, 0x70FD, 0x7109, 0x7109, 0x7114, 0x7114,
	0x7119, 0x711A, 0x711C, 0x711C, 0x7121, 0x7121, 0x7126, 0x7126, 0x7136, 0x7136, 0x713C, 0x713C, 0x7149, 0x7149, 0x714C, 0x714C,
	0x714E, 0x714E, 0x7155, 0x7156, 0x7159, 0x7159, 0x7162, 0x7162, 0x7164, 0x7167, 0x7169, 0x7169, 0x716C, 0x716C, 0x716E, 0x716E,
	0x717D, 0x717D, 0x7184, 0x7184, 0x7188, 0x7188, 0x718A, 0x718A, 0x718F, 0x718F, 0x7194, 0x7195, 0x7199, 0x7199, 0x719F, 0x719F,
	0x71A8, 0x71A8, 0x71AC, 0x71AC, 0x71B1, 0x71B1, 0x71B9, 0x71B9, 0x71BE, 0x71BE, 0x71C3, 0x71C3, 0x71C8, 0x71C9, 0x71CE, 0x71CE,
	0x71D0, 0x71D0, 0x71D2, 0x71D2, 0x71D4, 0x71D5, 0x71D7, 0x71D7, 0x71DF, 0x71E0, 0x71E5, 0x71E7, 0x71EC, 0x71EE, 0x71F5, 0x71F5,
	0x71F9, 0x71F9, 0x71FB, 0x71FC, 0x71FF, 0x71FF, 0x7206, 0x7206, 0x720D, 0x720D, 0x7210, 0x7210, 0x721B, 0x721B, 0x7228, 0x7228,
	0x722A, 0x722A, 0x722C, 0x722D, 0x7230, 0x7230, 0x7232, 0x7232, 0x7235, 0x7236, 0x723A, 0x7240, 0x7246, 0x7248, 0x724B, 0x724C,
	0x7252, 0x7252, 0x7258, 0x7259, 0x725B, 0x725B, 0x725D, 0x725D, 0x725F, 0x725F, 0x7261, 0x7262, 0x7267, 0x7267, 0x7269, 0x7269,
	0x7272, 0x7272, 0x7274, 0x7274, 0x7279, 0x7279, 0x727D, 0x727E, 0x7280, 0x7282, 0x7287, 0x7287, 0x7292, 0x7292, 0x7296, 0x7296,
	0x72A0, 0x72A0, 0x72A2, 0x72A2, 0x72A7, 0x72A7, 0x72AC, 0x72AC, 0x72AF, 0x72AF, 0x72B2, 0x72B2, 0x72B6, 0x72B6, 0x72B9, 0x72B9,
	0x72C2, 0x72C4, 0x72C6, 0x72C6, 0x72CE, 0x72CE, 0x72D0, 0x72D0, 0x72D2, 0x72D2, 0x72D7, 0x72D7, 0x72D9, 0x72D9, 0x72DB, 0x72DB,
	0x72E0, 0x72E2, 0x72E9, 0x72E9, 0x72EC, 0x72ED, 0x72F7, 0x72F9, 0x72FC, 0x72FD, 0x730A, 0x730A, 0x7316, 0x7317, 0x731B, 0x731D,
	0x731F, 0x731F, 0x7325, 0x7325, 0x7329, 0x732B, 0x732E, 0x732F, 0x7334, 0x7334, 0x7336, 0x7337, 0x733E, 0x733F, 0x7344, 0x7345,
	0x734E, 0x734F, 0x7357, 0x7357, 0x7363, 0x7363, 0x7368, 0x7368, 0x736A, 0x736A, 0x7370, 0x7370, 0x7372, 0x7372, 0x7375, 0x7375,
	0x7378, 0x7378, 0x737A, 0x737B, 0x7384, 0x7384, 0x7387, 0x7387, 0x7389, 0x7389, 0x738B, 0x738B, 0x7396, 0x7396, 0x73A9, 0x73A9,
	0x73B2, 0x73B3, 0x73BB, 0x73BB, 0x73C0, 0x73C0, 0x73C2, 0x73C2, 0x73C8, 0x73C8, 0x73CA, 0x73CA, 0x73CD, 0x73CE, 0x73DE, 0x73DE,
	0x73E0, 0x73E0, 0x73E5, 0x73E5, 0x73EA, 0x73EA, 0x73ED, 0x73EE, 0x73F1, 0x73F1, 0x73F8, 0x73F8, 0x73FE, 0x73FE, 0x7403, 0x7403,
	0x7405, 0x7406, 0x7409, 0x7409, 0x7422, 0x7422, 0x7425, 0x7425, 0x7432, 0x7436, 0x743A, 0x743A, 0x743F, 0x743F, 0x7441, 0x7441,
	0x7455, 0x7455, 0x7459, 0x745C, 0x745E, 0x7460, 0x7463, 0x7464, 0x7469, 0x746A, 0x746F, 0x7470, 0x7473, 0x7473, 0x7476, 0x7476,
	0x747E, 0x747E, 0x7483, 0x7483, 0x748B, 0x748B, 0x749E, 0x749E, 0x74A2, 0x74A2, 0x74A7, 0x74A7, 0x74B0, 0x74B0, 0x74BD, 0x74BD,
	0x74CA, 0x74CA, 0x74CF, 0x74CF, 0x74D4, 0x74D4, 0x74DC, 0x74DC, 0x74E0, 0x74E0, 0x74E2, 0x74E3, 0x74E6, 0x74E7, 0x74E9, 0x74E9,
	0x74EE, 0x74EE, 0x74F0, 0x74F2, 0x74F6, 0x74F8, 0x7503, 0x7505, 0x750C, 0x750E, 0x7511, 0x7511, 0x7513, 0x7513, 0x7515, 0x7515,
	0x7518, 0x7518, 0x751A, 0x751A, 0x751C, 0x751C, 0x751E, 0x751F, 0x7523, 0x7523, 0x7525, 0x7526, 0x7528, 0x7528, 0x752B, 0x752C,
	0x7530, 0x7533, 0x7537, 0x7538, 0x753A, 0x753C, 0x7544, 0x7544, 0x7546, 0x7546, 0x7549, 0x754D, 0x754F, 0x754F, 0x7551, 0x7551,
	0x7554, 0x7554, 0x7559, 0x755D, 0x7560, 0x7560, 0x7562, 0x7562, 0x7564, 0x7567, 0x7569, 0x756B, 0x756D, 0x756D, 0x7570, 0x7570,
	0x7573, 0x7574, 0x7576, 0x7578, 0x757F, 0x757F, 0x7582, 0x7582, 0x7586, 0x7587, 0x7589, 0x758B, 0x758E, 0x758F, 0x7591, 0x7591,
	0x7594, 0x7594, 0x759A, 0x759A, 0x759D, 0x759D, 0x75A3, 0x75A3, 0x75A5, 0x75A5, 0x75AB, 0x75AB, 0x75B1, 0x75B3, 0x75B5, 0x75B5,
	0x75B8, 0x75B9, 0x75BC, 0x75BE, 0x75C2, 0x75C3, 0x75C5, 0x75C5, 0x75C7, 0x75C7, 0x75CA, 0x75CA, 0x75CD, 0x75CD, 0x75D2, 0x75D2,
	0x75D4, 0x75D5, 0x75D8, 0x75D9, 0x75DB, 0x75DB, 0x75DE, 0x75DE, 0x75E2, 0x75E3, 0x75E9, 0x75E9, 0x75F0, 0x75F0, 0x75F2, 0x75F4,
	0x75FA, 0x75FA, 0x75FC, 0x75FC, 0x75FE, 0x75FF, 0x7601, 0x7601, 0x7609, 0x7609, 0x760B, 0x760B, 0x760D, 0x760D, 0x761F, 0x7622,
	0x7624, 0x7624, 0x7627, 0x7627, 0x7630, 0x7630, 0x7634, 0x7634, 0x763B, 0x763B, 0x7642, 0x7642, 0x7646, 0x7648, 0x764C, 0x764C,
	0x7652, 0x7652, 0x7656, 0x7656, 0x7658, 0x7658, 0x765C, 0x765C, 0x7661, 0x7662, 0x7667, 0x766A, 0x766C, 0x766C, 0x7670, 0x7670,
	0x7672, 0x7672, 0x7676, 0x7676, 0x7678, 0x7678, 0x767A, 0x767E, 0x7680, 0x7680, 0x7683, 0x7684, 0x7686, 0x7688, 0x768B, 0x768B,
	0x768E, 0x768E, 0x7690, 0x7690, 0x7693, 0x7693, 0x7696, 0x7696, 0x7699, 0x769A, 0x76AE, 0x76AE, 0x76B0, 0x76B0, 0x76B4, 0x76B4,
	0x76B7, 0x76BA, 0x76BF, 0x76BF, 0x76C2, 0x76C3, 0x76C6, 0x76C6, 0x76C8, 0x76C8, 0x76CA, 0x76CA, 0x76CD, 0x76CD, 0x76D2, 0x76D2,
	0x76D6, 0x76D7, 0x76DB, 0x76DC, 0x76DE, 0x76DF, 0x76E1, 0x76E1, 0x76E3, 0x76E5, 0x76E7, 0x76E7, 0x76EA, 0x76EA, 0x76EE, 0x76EE,
	0x76F2, 0x76F2, 0x76F4, 0x76F4, 0x76F8, 0x76F8, 0x76FB, 0x76FB, 0x76FE, 0x76FE, 0x7701, 0x7701, 0x7704, 0x7704, 0x7707, 0x7709,
	0x770B, 0x770C, 0x771B, 0x771B, 0x771E, 0x7720, 0x7724, 0x7726, 0x7729, 0x7729, 0x7737, 0x7738, 0x773A, 0x773A, 0x773C, 0x773C,
	0x7740, 0x7740, 0x7747, 0x7747, 0x775A, 0x775B, 0x7761, 0x7761, 0x7763, 0x7763, 0x7765, 0x7766, 0x7768, 0x7768, 0x776B, 0x776B,
	0x7779, 0x7779, 0x777E, 0x777F, 0x778B, 0x778B, 0x778E, 0x778E, 0x7791, 0x7791, 0x779E, 0x779E, 0x77A0, 0x77A0, 0x77A5, 0x77A5,
	0x77AC, 0x77AD, 0x77B0, 0x77B0, 0x77B3, 0x77B3, 0x77B6, 0x77B6, 0x77B9, 0x77B9, 0x77BB, 0x77BD, 0x77BF, 0x77BF, 0x77C7, 0x77C7,
	0x77CD, 0x77CD, 0x77D7, 0x77D7, 0x77DA, 0x77DC, 0x77E2, 0x77E3, 0x77E5, 0x77E5, 0x77E7, 0x77E7, 0x77E9, 0x77E9, 0x77ED, 0x77EF,
	0x77F3, 0x77F3, 0x77FC, 0x77FC, 0x7802, 0x7802, 0x780C, 0x780C, 0x7812, 0x7812, 0x7814, 0x7815, 0x7820, 0x7820, 0x7825, 0x7827,
	0x7832, 0x7832, 0x7834, 0x7834, 0x783A, 0x783A, 0x783F, 0x783F, 0x7845, 0x7845, 0x785D, 0x785D, 0x786B, 0x786C, 0x786F, 0x786F,
	0x7872, 0x7872, 0x7874, 0x7874, 0x787C, 0x787C, 0x7881, 0x7881, 0x7886, 0x7887, 0x788C, 0x788E, 0x7891, 0x7891, 0x7893, 0x7893,
	0x7895, 0x7895, 0x7897, 0x7897, 0x789A, 0x789A, 0x78A3, 0x78A3, 0x78A7, 0x78A7, 0x78A9, 0x78AA, 0x78AF, 0x78AF, 0x78B5, 0x78B5,
	0x78BA, 0x78BA, 0x78BC, 0x78BC, 0x78BE, 0x78BE, 0x78C1, 0x78C1, 0x78C5, 0x78C6, 0x78CA, 0x78CB, 0x78D0, 0x78D1, 0x78D4, 0x78D4,
	0x78DA, 0x78DA, 0x78E7, 0x78E8, 0x78EC, 0x78EC, 0x78EF, 0x78EF, 0x78F4, 0x78F4, 0x78FD, 0x78FD, 0x7901, 0x7901, 0x7907, 0x7907,
	0x790E, 0x790E, 0x7911, 0x7912, 0x7919, 0x7919, 0x7926, 0x7926, 0x792A, 0x792C, 0x793A, 0x793A, 0x793C, 0x793C, 0x793E, 0x793E,
	0x7940, 0x7941, 0x7947, 0x7949, 0x7950, 0x7950, 0x7953, 0x7953, 0x7955, 0x7957, 0x795A, 0x795A, 0x795D, 0x7960, 0x7962, 0x7962,
	0x7965, 0x7965, 0x7968, 0x7968, 0x796D, 0x796D, 0x7977, 0x7977, 0x797A, 0x797A, 0x797F, 0x7981, 0x7984, 0x7985, 0x798A, 0x798A,
	0x798D, 0x798F, 0x799D, 0x799D, 0x79A6, 0x79A7, 0x79AA, 0x79AA, 0x79AE, 0x79AE, 0x79B0, 0x79B0, 0x79B3, 0x79B3, 0x79B9, 0x79BA,
	0x79BD, 0x79C1, 0x79C9, 0x79C9, 0x79CB, 0x79CB, 0x79D1, 0x79D2, 0x79D5, 0x79D5, 0x79D8, 0x79D8, 0x79DF, 0x79DF, 0x79E1, 0x79E1,
	0x79E3, 0x79E4, 0x79E6, 0x79E7, 0x79E9, 0x79E9, 0x79EC, 0x79EC, 0x79F0, 0x79F0, 0x79FB, 0x79FB, 0x7A00, 0x7A00, 0x7A08, 0x7A08,
	0x7A0B, 0x7A0B, 0x7A0D, 0x7A0E, 0x7A14, 0x7A14, 0x7A17, 0x7A1A, 0x7A1C, 0x7A1C, 0x7A1F, 0x7A20, 0x7A2E, 0x7A2E, 0x7A31, 0x7A32,
	0x7A37, 0x7A37, 0x7A3B, 0x7A40, 0x7A42, 0x7A43, 0x7A46, 0x7A46, 0x7A49, 0x7A49, 0x7A4D, 0x7A50, 0x7A57, 0x7A57, 0x7A61, 0x7A63,
	0x7A69, 0x7A69, 0x7A6B, 0x7A6B, 0x7A70, 0x7A70, 0x7A74, 0x7A74, 0x7A76, 0x7A76, 0x7A79, 0x7A7A, 0x7A7D, 0x7A7D, 0x7A7F, 0x7A7F,
	0x7A81, 0x7A81, 0x7A83, 0x7A84, 0x7A88, 0x7A88, 0x7A92, 0x7A93, 0x7A95, 0x7A98, 0x7A9F, 0x7A9F, 0x7AA9, 0x7AAA, 0x7AAE, 0x7AB0,
	0x7AB6, 0x7AB6, 0x7ABA, 0x7ABA, 0x7ABF, 0x7ABF, 0x7AC3, 0x7AC5, 0x7AC7, 0x7AC8, 0x7ACA, 0x7ACB, 0x7ACD, 0x7ACD, 0x7ACF, 0x7ACF,
	0x7AD2, 0x7AD3, 0x7AD5, 0x7AD5, 0x7AD9, 0x7ADA, 0x7ADC, 0x7ADD, 0x7ADF, 0x7AE3, 0x7AE5, 0x7AE6, 0x7AEA, 0x7AEA, 0x7AED, 0x7AED,
	0x7AEF, 0x7AF0, 0x7AF6, 0x7AF6, 0x7AF8, 0x7AFA, 0x7AFF, 0x7AFF, 0x7B02, 0x7B02, 0x7B04, 0x7B04, 0x7B06, 0x7B06, 0x7B08, 0x7B08,
	0x7B0A, 0x7B0B, 0x7B0F, 0x7B0F, 0x7B11, 0x7B11, 0x7B18, 0x7B19, 0x7B1B, 0x7B1B, 0x7B1E, 0x7B1E, 0x7B20, 0x7B20, 0x7B25, 0x7B26,
	0x7B28, 0x7B28, 0x7B2C, 0x7B2C, 0x7B33, 0x7B33, 0x7B35, 0x7B36, 0x7B39, 0x7B39, 0x7B45, 0x7B46, 0x7B48, 0x7B49, 0x7B4B, 0x7B4D,
	0x7B4F, 0x7B52, 0x7B54, 0x7B54, 0x7B56, 0x7B56, 0x7B5D, 0x7B5D, 0x7B65, 0x7B65, 0x7B67, 0x7B67, 0x7B6C, 0x7B6C, 0x7B6E, 0x7B6E,
	0x7B70, 0x7B71, 0x7B74, 0x7B75, 0x7B7A, 0x7B7A, 0x7B86, 0x7B87, 0x7B8B, 0x7B8B, 0x7B8D, 0x7B8D, 0x7B8F, 0x7B8F, 0x7B92, 0x7B92,
	0x7B94, 0x7B95, 0x7B97, 0x7B9A, 0x7B9C, 0x7B9D, 0x7B9F, 0x7B9F, 0x7BA1, 0x7BA1, 0x7BAA, 0x7BAA, 0x7BAD, 0x7BAD, 0x7BB1, 0x7BB1,
	0x7BB4, 0x7BB4, 0x7BB8, 0x7BB8, 0x7BC0, 0x7BC1, 0x7BC4, 0x7BC4, 0x7BC6, 0x7BC7, 0x7BC9, 0x7BC9, 0x7BCB, 0x7BCC, 0x7BCF, 0x7BCF,
	0x7BDD, 0x7BDD, 0x7BE0, 0x7BE0, 0x7BE4, 0x7BE6, 0x7BE9, 0x7BE9, 0x7BED, 0x7BED, 0x7BF3, 0x7BF3, 0x7BF6, 0x7BF7, 0x7C00, 0x7C00,
	0x7C07, 0x7C07, 0x7C0D, 0x7C0D, 0x7C11, 0x7C14, 0x7C17, 0x7C17, 0x7C1F, 0x7C1F, 0x7C21, 0x7C21, 0x7C23, 0x7C23, 0x7C27, 0x7C27,
	0x7C2A, 0x7C2B, 0x7C37, 0x7C38, 0x7C3D, 0x7C40, 0x7C43, 0x7C43, 0x7C4C, 0x7C4D, 0x7C4F, 0x7C50, 0x7C54, 0x7C54, 0x7C56, 0x7C56,
	0x7C58, 0x7C58, 0x7C5F, 0x7C60, 0x7C64, 0x7C65, 0x7C6C, 0x7C6C, 0x7C73, 0x7C73, 0x7C75, 0x7C75, 0x7C7E, 0x7C7E, 0x7C81, 0x7C83,
	0x7C89, 0x7C89, 0x7C8B, 0x7C8B, 0x7C8D, 0x7C8D, 0x7C90, 0x7C90, 0x7C92, 0x7C92, 0x7C95, 0x7C95, 0x7C97, 0x7C98, 0x7C9B, 0x7C9B,
	0x7C9F, 0x7C9F, 0x7CA1, 0x7CA2, 0x7CA4, 0x7CA5, 0x7CA7, 0x7CA8, 0x7CAB, 0x7CAB, 0x7CAD, 0x7CAE, 0x7CB1, 0x7CB3, 0x7CB9, 0x7CB9,
	0x7CBD, 0x7CBE, 0x7CC0, 0x7CC0, 0x7CC2, 0x7CC2, 0x7CC5, 0x7CC5, 0x7CCA, 0x7CCA, 0x7CCE, 0x7CCE, 0x7CD2, 0x7CD2, 0x7CD6, 0x7CD6,
	0x7CD8, 0x7CD8, 0x7CDC, 0x7CDC, 0x7CDE, 0x7CE0, 0x7CE2, 0x7CE2, 0x7CE7, 0x7CE7, 0x7CEF, 0x7CEF, 0x7CF2, 0x7CF2, 0x7CF4, 0x7CF4,
	0x7CF6, 0x7CF6, 0x7CF8, 0x7CF8, 0x7CFA, 0x7CFB, 0x7CFE, 0x7CFE, 0x7D00, 0x7D00, 0x7D02, 0x7D02, 0x7D04, 0x7D06, 0x7D0A, 0x7D0B,
	0x7D0D, 0x7D0D, 0x7D10, 0x7D10, 0x7D14, 0x7D15, 0x7D17, 0x7D1C, 0x7D20, 0x7D22, 0x7D2B, 0x7D2C, 0x7D2E, 0x7D30, 0x7D32, 0x7D33,
	0x7D35, 0x7D35, 0x7D39, 0x7D3A, 0x7D3F, 0x7D3F, 0x7D42, 0x7D46, 0x7D4B, 0x7D4C, 0x7D4E, 0x7D50, 0x7D56, 0x7D56, 0x7D5B, 0x7D5B,
	0x7D5E, 0x7D5E, 0x7D61, 0x7D63, 0x7D66, 0x7D66, 0x7D68, 0x7D68, 0x7D6E, 0x7D6E, 0x7D71, 0x7D73, 0x7D75, 0x7D76, 0x7D79, 0x7D79,
	0x7D7D, 0x7D7D, 0x7D89, 0x7D89, 0x7D8F, 0x7D8F, 0x7D93, 0x7D93, 0x7D99, 0x7D9C, 0x7D9F, 0x7D9F, 0x7DA2, 0x7DA3, 0x7DAB, 0x7DB2,
	0x7DB4, 0x7DB5, 0x7DB8, 0x7DB8, 0x7DBA, 0x7DBB, 0x7DBD, 0x7DBF, 0x7DC7, 0x7DC7, 0x7DCA, 0x7DCB, 0x7DCF, 0x7DCF, 0x7DD1, 0x7DD2,
	0x7DD5, 0x7DD5, 0x7DD8, 0x7DD8, 0x7DDA, 0x7DDA, 0x7DDC, 0x7DDE, 0x7DE0, 0x7DE1, 0x7DE4, 0x7DE4, 0x7DE8, 0x7DE9, 0x7DEC, 0x7DEC,
	0x7DEF, 0x7DEF, 0x7DF2, 0x7DF2, 0x7DF4, 0x7DF4, 0x7DFB, 0x7DFB, 0x7E01, 0x7E01, 0x7E04, 0x7E05, 0x7E09, 0x7E0B, 0x7E12, 0x7E12,
	0x7E1B, 0x7E1B, 0x7E1E, 0x7E1F, 0x7E21, 0x7E23, 0x7E26, 0x7E26, 0x7E2B, 0x7E2B, 0x7E2E, 0x7E2E, 0x7E31, 0x7E32, 0x7E35, 0x7E35,
	0x7E37, 0x7E37, 0x7E39, 0x7E3B, 0x7E3D, 0x7E3E, 0x7E41, 0x7E41, 0x7E43, 0x7E43, 0x7E46, 0x7E46, 0x7E4A, 0x7E4B, 0x7E4D, 0x7E4D,
	0x7E54, 0x7E56, 0x7E59, 0x7E5A, 0x7E5D, 0x7E5E, 0x7E66, 0x7E67, 0x7E69, 0x7E6A, 0x7E6D, 0x7E6D, 0x7E70, 0x7E70, 0x7E79, 0x7E79,
	0x7E7B, 0x7E7D, 0x7E7F, 0x7E7F, 0x7E82, 0x7E83, 0x7E88, 0x7E89, 0x7E8C, 0x7E8C, 0x7E8E, 0x7E90, 0x7E92, 0x7E94, 0x7E96, 0x7E96,
	0x7E9B, 0x7E9C, 0x7F36, 0x7F36, 0x7F38, 0x7F38, 0x7F3A, 0x7F3A, 0x7F45, 0x7F45, 0x7F4C, 0x7F4E, 0x7F50, 0x7F51, 0x7F54, 0x7F55,
	0x7F58, 0x7F58, 0x7F5F, 0x7F60, 0x7F67, 0x7F6B, 0x7F6E, 0x7F6E, 0x7F70, 0x7F70, 0x7F72, 0x7F72, 0x7F75, 0x7F75, 0x7F77, 0x7F79,
	0x7F82, 0x7F83, 0x7F85, 0x7F88, 0x7F8A, 0x7F8A, 0x7F8C, 0x7F8C, 0x7F8E, 0x7F8E, 0x7F94, 0x7F94, 0x7F9A, 0x7F9A, 0x7F9D, 0x7F9E,
	0x7FA3, 0x7FA4, 0x7FA8, 0x7FA9, 0x7FAE, 0x7FAF, 0x7FB2, 0x7FB2, 0x7FB6, 0x7FB6, 0x7FB8, 0x7FB9, 0x7FBD, 0x7FBD, 0x7FC1, 0x7FC1,
	0x7FC5, 0x7FC6, 0x7FCA, 0x7FCA, 0x7FCC, 0x7FCC, 0x7FD2, 0x7FD2, 0x7FD4, 0x7FD5, 0x7FE0, 0x7FE1, 0x7FE6, 0x7FE6, 0x7FE9, 0x7FE9,
	0x7FEB, 0x7FEB, 0x7FF0, 0x7FF0, 0x7FF3, 0x7FF3, 0x7FF9, 0x7FF9, 0x7FFB, 0x7FFC, 0x8000, 0x8001, 0x8003, 0x8006, 0x800B, 0x800C,
	0x8010, 0x8010, 0x8012, 0x8012, 0x8015, 0x8015, 0x8017, 0x8019, 0x801C, 0x801C, 0x8021, 0x8021, 0x8028, 0x8028, 0x8033, 0x8033,
	0x8036, 0x8036, 0x803B, 0x803B, 0x803D, 0x803D, 0x803F, 0x803F, 0x8046, 0x8046, 0x804A, 0x804A, 0x8052, 0x8052, 0x8056, 0x8056,
	0x8058, 0x8058, 0x805A, 0x805A, 0x805E, 0x805F, 0x8061, 0x8062, 0x8068, 0x8068, 0x806F, 0x8070, 0x8072, 0x8074, 0x8076, 0x8077,
	0x8079, 0x8079, 0x807D, 0x807F, 0x8084, 0x8087, 0x8089, 0x8089, 0x808B, 0x808C, 0x8093, 0x8093, 0x8096, 0x8096, 0x8098, 0x8098,
	0x809A, 0x809B, 0x809D, 0x809D, 0x80A1, 0x80A2, 0x80A5, 0x80A5, 0x80A9, 0x80AA, 0x80AC, 0x80AD, 0x80AF, 0x80AF, 0x80B1, 0x80B2,
	0x80B4, 0x80B4, 0x80BA, 0x80BA, 0x80C3, 0x80C4, 0x80C6, 0x80C6, 0x80CC, 0x80CC, 0x80CE, 0x80CE, 0x80D6, 0x80D6, 0x80D9, 0x80DB,
	0x80DD, 0x80DE, 0x80E1, 0x80E1, 0x80E4, 0x80E5, 0x80EF, 0x80EF, 0x80F1, 0x80F1, 0x80F4, 0x80F4, 0x80F8, 0x80F8, 0x80FC, 0x80FD,
	0x8102, 0x8102, 0x8105, 0x810A, 0x811A, 0x811B, 0x8123, 0x8123, 0x8129, 0x8129, 0x812F, 0x812F, 0x8131, 0x8131, 0x8133, 0x8133,
	0x8139, 0x8139, 0x813E, 0x813E, 0x8146, 0x8146, 0x814B, 0x814B, 0x814E, 0x814E, 0x8150, 0x8151, 0x8153, 0x8155, 0x815F, 0x815F,
	0x8165, 0x8166, 0x816B, 0x816B, 0x816E, 0x816E, 0x8170, 0x8171, 0x8174, 0x8174, 0x8178, 0x817A, 0x817F, 0x8180, 0x8182, 0x8183,
	0x8188, 0x8188, 0x818A, 0x818A, 0x818F, 0x818F, 0x8193, 0x8193, 0x8195, 0x8195, 0x819A, 0x819A, 0x819C, 0x819D, 0x81A0, 0x81A0,
	0x81A3, 0x81A4, 0x81A8, 0x81A9, 0x81B0, 0x81B0, 0x81B3, 0x81B3, 0x81B5, 0x81B5, 0x81B8, 0x81B8, 0x81BA, 0x81BA, 0x81BD, 0x81C0,
	0x81C2, 0x81C2, 0x81C6, 0x81C6, 0x81C8, 0x81C9, 0x81CD, 0x81CD, 0x81D1, 0x81D1, 0x81D3, 0x81D3, 0x81D8, 0x81DA, 0x81DF, 0x81E0,
	0x81E3, 0x81E3, 0x81E5, 0x81E5, 0x81E7, 0x81E8, 0x81EA, 0x81EA, 0x81ED, 0x81ED, 0x81F3, 0x81F4, 0x81FA, 0x81FC, 0x81FE, 0x81FE,
	0x8201, 0x8202, 0x8205, 0x8205, 0x8207, 0x820A, 0x820C, 0x820E, 0x8210, 0x8210, 0x8212, 0x8212, 0x8216, 0x8218, 0x821B, 0x821C,
	0x821E, 0x821F, 0x8229, 0x822C, 0x822E, 0x822E, 0x8233, 0x8233, 0x8235, 0x8239, 0x8240, 0x8240, 0x8247, 0x8247, 0x8258, 0x825A,
	0x825D, 0x825D, 0x825F, 0x825F, 0x8262, 0x8262, 0x8264, 0x8264, 0x8266, 0x8266, 0x8268, 0x8268, 0x826A, 0x826B, 0x826E, 0x826F,
	0x8271, 0x8272, 0x8276, 0x8278, 0x827E, 0x827E, 0x828B, 0x828B, 0x828D, 0x828D, 0x8292, 0x8292, 0x8299, 0x8299, 0x829D, 0x829D,
	0x829F, 0x829F, 0x82A5, 0x82A6, 0x82AB, 0x82AD, 0x82AF, 0x82AF, 0x82B1, 0x82B1, 0x82B3, 0x82B3, 0x82B8, 0x82B9, 0x82BB, 0x82BB,
	0x82BD, 0x82BD, 0x82C5, 0x82C5, 0x82D1, 0x82D4, 0x82D7, 0x82D7, 0x82D9, 0x82D9, 0x82DB, 0x82DC, 0x82DE, 0x82DF, 0x82E1, 0x82E1,
	0x82E3, 0x82E3, 0x82E5, 0x82E7, 0x82EB, 0x82EB, 0x82F1, 0x82F1, 0x82F3, 0x82F4, 0x82F9, 0x82FB, 0x8302, 0x8306, 0x8309, 0x8309,
	0x830E, 0x830E, 0x8316, 0x8318, 0x831C, 0x831C, 0x8323, 0x8323, 0x8328, 0x8328, 0x832B, 0x832B, 0x832F, 0x832F, 0x8331, 0x8332,
	0x8334, 0x8336, 0x8338, 0x8339, 0x8340, 0x8340, 0x8345, 0x8345, 0x8349, 0x834A, 0x834F, 0x8350, 0x8352, 0x8352, 0x8358, 0x8358,
	0x8373, 0x8373, 0x8375, 0x8375, 0x8377, 0x8377, 0x837B, 0x837C, 0x8385, 0x8385, 0x8387, 0x8387, 0x8389, 0x838A, 0x838E, 0x838E,
	0x8393, 0x8393, 0x8396, 0x8396, 0x839A, 0x839A, 0x839E, 0x83A0, 0x83A2, 0x83A2, 0x83A8, 0x83A8, 0x83AA, 0x83AB, 0x83B1, 0x83B1,
	0x83B5, 0x83B5, 0x83BD, 0x83BD, 0x83C1, 0x83C1, 0x83C5, 0x83C5, 0x83CA, 0x83CA, 0x83CC, 0x83CC, 0x83CE, 0x83CE, 0x83D3, 0x83D3,
	0x83D6, 0x83D6, 0x83D8, 0x83D8, 0x83DC, 0x83DC, 0x83DF, 0x83E0, 0x83E9, 0x83E9, 0x83EB, 0x83EB, 0x83EF, 0x83F2, 0x83F4, 0x83F4,
	0x83F7, 0x83F7, 0x83FB, 0x83FB, 0x83FD, 0x83FD, 0x8403, 0x8404, 0x8407, 0x8407, 0x840B, 0x840E, 0x8413, 0x8413, 0x8420, 0x8420,
	0x8422, 0x8422, 0x8429, 0x842A, 0x842C, 0x842C, 0x8431, 0x8431, 0x8435, 0x8435, 0x8438, 0x8438, 0x843C, 0x843D, 0x8446, 0x8446,
	0x8449, 0x8449, 0x844E, 0x844E, 0x8457, 0x8457, 0x845B, 0x845B, 0x8461, 0x8463, 0x8466, 0x8466, 0x8469, 0x8469, 0x846B, 0x846F,
	0x8471, 0x8471, 0x8475, 0x8475, 0x8477, 0x8477, 0x8479, 0x847A, 0x8482, 0x8482, 0x8484, 0x8484, 0x848B, 0x848B, 0x8490, 0x8490,
	0x8494, 0x8494, 0x8499, 0x8499, 0x849C, 0x849C, 0x849F, 0x849F, 0x84A1, 0x84A1, 0x84AD, 0x84AD, 0x84B2, 0x84B2, 0x84B8, 0x84B9,
	0x84BB, 0x84BC, 0x84BF, 0x84BF, 0x84C1, 0x84C1, 0x84C4, 0x84C4, 0x84C6, 0x84C6, 0x84C9, 0x84CB, 0x84CD, 0x84CD, 0x84D0, 0x84D1,
	0x84D6, 0x84D6, 0x84D9, 0x84DA, 0x84EC, 0x84EC, 0x84EE, 0x84EE, 0x84F4, 0x84F4, 0x84FC, 0x84FC, 0x84FF, 0x8500, 0x8506, 0x8506,
	0x8511, 0x8511, 0x8513, 0x8515, 0x8517, 0x8518, 0x851A, 0x851A, 0x851F, 0x851F, 0x8521, 0x8521, 0x8526, 0x8526, 0x852C, 0x852D,
	0x8535, 0x8535, 0x853D, 0x853D, 0x8540, 0x8541, 0x8543, 0x8543, 0x8548, 0x854B, 0x854E, 0x854E, 0x8555, 0x8555, 0x8557, 0x8558,
	0x855A, 0x855A, 0x8563, 0x8563, 0x8568, 0x856A, 0x856D, 0x856D, 0x8577, 0x8577, 0x857E, 0x857E, 0x8580, 0x8580, 0x8584, 0x8584,
	0x8587, 0x8588, 0x858A, 0x858A, 0x8590, 0x8591, 0x8594, 0x8594, 0x8597, 0x8597, 0x8599, 0x8599, 0x859B, 0x859C, 0x85A4, 0x85A4,
	0x85A6, 0x85A6, 0x85A8, 0x85AC, 0x85AE, 0x85AF, 0x85B9, 0x85BA, 0x85C1, 0x85C1, 0x85C9, 0x85C9, 0x85CD, 0x85CD, 0x85CF, 0x85D0,
	0x85D5, 0x85D5, 0x85DC, 0x85DD, 0x85E4, 0x85E5, 0x85E9, 0x85EA, 0x85F7, 0x85F7, 0x85F9, 0x85FB, 0x85FE, 0x85FE, 0x8602, 0x8602,
	0x8606, 0x8607, 0x860A, 0x860B, 0x8613, 0x8613, 0x8616, 0x8617, 0x861A, 0x861A, 0x8622, 0x8622, 0x862D, 0x862D, 0x862F, 0x8630,
	0x863F, 0x863F, 0x864D, 0x864E, 0x8650, 0x8650, 0x8654, 0x8655, 0x865A, 0x865A, 0x865C, 0x865C, 0x865E, 0x865F, 0x8667, 0x8667,
	0x866B, 0x866B, 0x8671, 0x8671, 0x8679, 0x8679, 0x867B, 0x867B, 0x868A, 0x868C, 0x8693, 0x8693, 0x8695, 0x8695, 0x86A3, 0x86A4,
	0x86A9, 0x86AB, 0x86AF, 0x86B0, 0x86B6, 0x86B6, 0x86C4, 0x86C4, 0x86C6, 0x86C7, 0x86C9, 0x86C9, 0x86CB, 0x86CB, 0x86CD, 0x86CE,
	0x86D4, 0x86D4, 0x86D9, 0x86D9, 0x86DB, 0x86DB, 0x86DE, 0x86DF, 0x86E4, 0x86E4, 0x86E9, 0x86E9, 0x86EC, 0x86EF, 0x86F8, 0x86F9,
	0x86FB, 0x86FB, 0x86FE, 0x86FE, 0x8700, 0x8700, 0x8702, 0x8703, 0x8706, 0x8706, 0x8708, 0x870A, 0x870D, 0x870D, 0x8711, 0x8712,
	0x8718, 0x8718, 0x871A, 0x871A, 0x871C, 0x871C, 0x8725, 0x8725, 0x8729, 0x8729, 0x8734, 0x8734, 0x8737, 0x8737, 0x873B, 0x873B,
	0x873F, 0x873F, 0x8749, 0x8749, 0x874B, 0x874C, 0x874E, 0x874E, 0x8753, 0x8753, 0x8755, 0x8755, 0x8757, 0x8757, 0x8759, 0x8759,
	0x875F, 0x8760, 0x8763, 0x8763, 0x8766, 0x8766, 0x8768, 0x8768, 0x876A, 0x876A, 0x876E, 0x876E, 0x8774, 0x8774, 0x8776, 0x8776,
	0x8778, 0x8778, 0x877F, 0x877F, 0x8782, 0x8782, 0x878D, 0x878D, 0x879F, 0x879F, 0x87A2, 0x87A2, 0x87AB, 0x87AB, 0x87AF, 0x87AF,
	0x87B3, 0x87B3, 0x87BA, 0x87BB, 0x87BD, 0x87BD, 0x87C0, 0x87C0, 0x87C4, 0x87C4, 0x87C6, 0x87C7, 0x87CB, 0x87CB, 0x87D0, 0x87D0,
	0x87D2, 0x87D2, 0x87E0, 0x87E0, 0x87EF, 0x87EF, 0x87F2, 0x87F2, 0x87F6, 0x87F7, 0x87F9, 0x87F9, 0x87FB, 0x87FB, 0x87FE, 0x87FE,
	0x8805, 0x8805, 0x880D, 0x880F, 0x8811, 0x8811, 0x8815, 0x8816, 0x8821, 0x8823, 0x8827, 0x8827, 0x8831, 0x8831, 0x8836, 0x8836,
	0x8839, 0x8839, 0x883B, 0x883B, 0x8840, 0x8840, 0x8842, 0x8842, 0x8844, 0x8844, 0x8846, 0x8846, 0x884C, 0x884D, 0x8852, 0x8853,
	0x8857, 0x8857, 0x8859, 0x8859, 0x885B, 0x885B, 0x885D, 0x885E, 0x8861, 0x8863, 0x8868, 0x8868, 0x886B, 0x886B, 0x8870, 0x8870,
	0x8872, 0x8872, 0x8875, 0x8875, 0x8877, 0x8877, 0x887D, 0x887F, 0x8881, 0x8882, 0x8888, 0x8888, 0x888B, 0x888B, 0x888D, 0x888D,
	0x8892, 0x8892, 0x8896, 0x8897, 0x8899, 0x8899, 0x889E, 0x889E, 0x88A2, 0x88A2, 0x88A4, 0x88A4, 0x88AB, 0x88AB, 0x88AE, 0x88AE,
	0x88B0, 0x88B1, 0x88B4, 0x88B5, 0x88B7, 0x88B7, 0x88BF, 0x88BF, 0x88C1, 0x88C5, 0x88CF, 0x88CF, 0x88D4, 0x88D5, 0x88D8, 0x88D9,
	0x88DC, 0x88DD, 0x88DF, 0x88DF, 0x88E1, 0x88E1, 0x88E8, 0x88E8, 0x88F2, 0x88F4, 0x88F8, 0x88F9, 0x88FC, 0x88FE, 0x8902, 0x8902,
	0x8904, 0x8904, 0x8907, 0x8907, 0x890A, 0x890A, 0x890C, 0x890C, 0x8910, 0x8910, 0x8912, 0x8913, 0x891D, 0x891E, 0x8925, 0x8925,
	0x892A, 0x892B, 0x8936, 0x8936, 0x8938, 0x8938, 0x893B, 0x893B, 0x8941, 0x8941, 0x8943, 0x8944, 0x894C, 0x894D, 0x8956, 0x8956,
	0x895E, 0x8960, 0x8964, 0x8964, 0x8966, 0x8966, 0x896A, 0x896A, 0x896D, 0x896D, 0x896F, 0x896F, 0x8972, 0x8972, 0x8974, 0x8974,
	0x8977, 0x8977, 0x897E, 0x897F, 0x8981, 0x8981, 0x8983, 0x8983, 0x8986, 0x8988, 0x898A, 0x898B, 0x898F, 0x898F, 0x8993, 0x8993,
	0x8996, 0x8998, 0x899A, 0x899A, 0x89A1, 0x89A1, 0x89A6, 0x89A7, 0x89A9, 0x89AA, 0x89AC, 0x89AC, 0x89AF, 0x89AF, 0x89B2, 0x89B3,
	0x89BA, 0x89BA, 0x89BD, 0x89BD, 0x89BF, 0x89C0, 0x89D2, 0x89D2, 0x89DA, 0x89DA, 0x89DC, 0x89DD, 0x89E3, 0x89E3, 0x89E6, 0x89E7,
	0x89F4, 0x89F4, 0x89F8, 0x89F8, 0x8A00, 0x8A00, 0x8A02, 0x8A03, 0x8A08, 0x8A08, 0x8A0A, 0x8A0A, 0x8A0C, 0x8A0C, 0x8A0E, 0x8A0E,
	0x8A10, 0x8A10, 0x8A13, 0x8A13, 0x8A16, 0x8A18, 0x8A1B, 0x8A1B, 0x8A1D, 0x8A1D, 0x8A1F, 0x8A1F, 0x8A23, 0x8A23, 0x8A25, 0x8A25,
	0x8A2A, 0x8A2A, 0x8A2D, 0x8A2D, 0x8A31, 0x8A31, 0x8A33, 0x8A34, 0x8A36, 0x8A36, 0x8A3A, 0x8A3C, 0x8A41, 0x8A41, 0x8A46, 0x8A46,
	0x8A48, 0x8A48, 0x8A50, 0x8A52, 0x8A54, 0x8A55, 0x8A5B, 0x8A5B, 0x8A5E, 0x8A5E, 0x8A60, 0x8A60, 0x8A62, 0x8A63, 0x8A66, 0x8A66,
	0x8A69, 0x8A69, 0x8A6B, 0x8A6E, 0x8A70, 0x8A73, 0x8A7C, 0x8A7C, 0x8A82, 0x8A82, 0x8A84, 0x8A85, 0x8A87, 0x8A87, 0x8A89, 0x8A89,
	0x8A8C, 0x8A8D, 0x8A91, 0x8A91, 0x8A93, 0x8A93, 0x8A95, 0x8A95, 0x8A98, 0x8A98, 0x8A9A, 0x8A9A, 0x8A9E, 0x8A9E, 0x8AA0, 0x8AA1,
	0x8AA3, 0x8AA6, 0x8AA8, 0x8AA8, 0x8AAC, 0x8AAD, 0x8AB0, 0x8AB0, 0x8AB2, 0x8AB2, 0x8AB9, 0x8AB9, 0x8ABC, 0x8ABC, 0x8ABF, 0x8ABF,
	0x8AC2, 0x8AC2, 0x8AC4, 0x8AC4, 0x8AC7, 0x8AC7, 0x8ACB, 0x8ACD, 0x8ACF, 0x8ACF, 0x8AD2, 0x8AD2, 0x8AD6, 0x8AD6, 0x8ADA, 0x8ADC,
	0x8ADE, 0x8ADE, 0x8AE0, 0x8AE2, 0x8AE4, 0x8AE4, 0x8AE6, 0x8AE7, 0x8AEB, 0x8AEB, 0x8AED, 0x8AEE, 0x8AF1, 0x8AF1, 0x8AF3, 0x8AF3,
	0x8AF7, 0x8AF8, 0x8AFA, 0x8AFA, 0x8AFE, 0x8AFE, 0x8B00, 0x8B02, 0x8B04, 0x8B04, 0x8B07, 0x8B07, 0x8B0C, 0x8B0C, 0x8B0E, 0x8B0E,
	0x8B10, 0x8B10, 0x8B14, 0x8B14, 0x8B16, 0x8B17, 0x8B19, 0x8B1B, 0x8B1D, 0x8B1D, 0x8B20, 0x8B21, 0x8B26, 0x8B26, 0x8B28, 0x8B28,
	0x8B2B, 0x8B2C, 0x8B33, 0x8B33, 0x8B39, 0x8B39, 0x8B3E, 0x8B3E, 0x8B41, 0x8B41, 0x8B49, 0x8B49, 0x8B4C, 0x8B4C, 0x8B4E, 0x8B4F,
	0x8B56, 0x8B56, 0x8B58, 0x8B58, 0x8B5A, 0x8B5C, 0x8B5F, 0x8B5F, 0x8B66, 0x8B66, 0x8B6B, 0x8B6C, 0x8B6F, 0x8B72, 0x8B74, 0x8B74,
	0x8B77, 0x8B77, 0x8B7D, 0x8B7D, 0x8B80, 0x8B80, 0x8B83, 0x8B83, 0x8B8A, 0x8B8A, 0x8B8C, 0x8B8C, 0x8B8E, 0x8B8E, 0x8B90, 0x8B90,
	0x8B92, 0x8B93, 0x8B96, 0x8B96, 0x8B99, 0x8B9A, 0x8C37, 0x8C37, 0x8C3A, 0x8C3A, 0x8C3F, 0x8C3F, 0x8C41, 0x8C41, 0x8C46, 0x8C46,
	0x8C48, 0x8C48, 0x8C4A, 0x8C4A, 0x8C4C, 0x8C4C, 0x8C4E, 0x8C4E, 0x8C50, 0x8C50, 0x8C55, 0x8C55, 0x8C5A, 0x8C5A, 0x8C61, 0x8C62,
	0x8C6A, 0x8C6C, 0x8C78, 0x8C7A, 0x8C7C, 0x8C7C, 0x8C82, 0x8C82, 0x8C85, 0x8C85, 0x8C89, 0x8C8A, 0x8C8C, 0x8C8E, 0x8C94, 0x8C94,
	0x8C98, 0x8C98, 0x8C9D, 0x8C9E, 0x8CA0, 0x8CA2, 0x8CA7, 0x8CB0, 0x8CB2, 0x8CB4, 0x8CB6, 0x8CB8, 0x8CBB, 0x8CBD, 0x8CBF, 0x8CC4,
	0x8CC7, 0x8CC8, 0x8CCA, 0x8CCA, 0x8CCD, 0x8CCE, 0x8CD1, 0x8CD1, 0x8CD3, 0x8CD3, 0x8CDA, 0x8CDC, 0x8CDE, 0x8CDE, 0x8CE0, 0x8CE0,
	0x8CE2, 0x8CE4, 0x8CE6, 0x8CE6, 0x8CEA, 0x8CEA, 0x8CED, 0x8CED, 0x8CFA, 0x8CFD, 0x8D04, 0x8D05, 0x8D07, 0x8D08, 0x8D0A, 0x8D0B,
	0x8D0D, 0x8D0D, 0x8D0F, 0x8D10, 0x8D13, 0x8D14, 0x8D16, 0x8D16, 0x8D64, 0x8D64, 0x8D66, 0x8D67, 0x8D6B, 0x8D6B, 0x8D6D, 0x8D6D,
	0x8D70, 0x8D71, 0x8D73, 0x8D74, 0x8D77, 0x8D77, 0x8D81, 0x8D81, 0x8D85, 0x8D85, 0x8D8A, 0x8D8A, 0x8D99, 0x8D99, 0x8DA3, 0x8DA3,
	0x8DA8, 0x8DA8, 0x8DB3, 0x8DB3, 0x8DBA, 0x8DBA, 0x8DBE, 0x8DBE, 0x8DC2, 0x8DC2, 0x8DCB, 0x8DCC, 0x8DCF, 0x8DCF, 0x8DD6, 0x8DD6,
	0x8DDA, 0x8DDB, 0x8DDD, 0x8DDD, 0x8DDF, 0x8DDF, 0x8DE1, 0x8DE1, 0x8DE3, 0x8DE3, 0x8DE8, 0x8DE8, 0x8DEA, 0x8DEB, 0x8DEF, 0x8DEF,
	0x8DF3, 0x8DF3, 0x8DF5, 0x8DF5, 0x8DFC, 0x8DFC, 0x8DFF, 0x8DFF, 0x8E08, 0x8E0A, 0x8E0F, 0x8E10, 0x8E1D, 0x8E1F, 0x8E2A, 0x8E2A,
	0x8E30, 0x8E30, 0x8E34, 0x8E35, 0x8E42, 0x8E42, 0x8E44, 0x8E44, 0x8E47, 0x8E4A, 0x8E4C, 0x8E4C, 0x8E50, 0x8E50, 0x8E55, 0x8E55,
	0x8E59, 0x8E59, 0x8E5F, 0x8E60, 0x8E63, 0x8E64, 0x8E72, 0x8E72, 0x8E74, 0x8E74, 0x8E76, 0x8E76, 0x8E7C, 0x8E7C, 0x8E81, 0x8E81,
	0x8E84, 0x8E85, 0x8E87, 0x8E87, 0x8E8A, 0x8E8B, 0x8E8D, 0x8E8D, 0x8E91, 0x8E91, 0x8E93, 0x8E94, 0x8E99, 0x8E99, 0x8EA1, 0x8EA1,
	0x8EAA, 0x8EAC, 0x8EAF, 0x8EB1, 0x8EBE, 0x8EBE, 0x8EC5, 0x8EC6, 0x8EC8, 0x8EC8, 0x8ECA, 0x8ECD, 0x8ED2, 0x8ED2, 0x8EDB, 0x8EDB,
	0x8EDF, 0x8EDF, 0x8EE2, 0x8EE3, 0x8EEB, 0x8EEB, 0x8EF8, 0x8EF8, 0x8EFB, 0x8EFE, 0x8F03, 0x8F03, 0x8F05, 0x8F05, 0x8F09, 0x8F0A,
	0x8F0C, 0x8F0C, 0x8F12, 0x8F15, 0x8F19, 0x8F19, 0x8F1B, 0x8F1D, 0x8F1F, 0x8F1F, 0x8F26, 0x8F26, 0x8F29, 0x8F2A, 0x8F2F, 0x8F2F,
	0x8F33, 0x8F33, 0x8F38, 0x8F39, 0x8F3B, 0x8F3B, 0x8F3E, 0x8F3F, 0x8F42, 0x8F42, 0x8F44, 0x8F46, 0x8F49, 0x8F49, 0x8F4C, 0x8F4E,
	0x8F57, 0x8F57, 0x8F5C, 0x8F5C, 0x8F5F, 0x8F5F, 0x8F61, 0x8F64, 0x8F9B, 0x8F9C, 0x8F9E, 0x8F9F, 0x8FA3, 0x8FA3, 0x8FA7, 0x8FA8,
	0x8FAD, 0x8FB2, 0x8FB7, 0x8FB7, 0x8FBA, 0x8FBC, 0x8FBF, 0x8FBF, 0x8FC2, 0x8FC2, 0x8FC4, 0x8FC5, 0x8FCE, 0x8FCE, 0x8FD1, 0x8FD1,
	0x8FD4, 0x8FD4, 0x8FDA, 0x8FDA, 0x8FE2, 0x8FE2, 0x8FE5, 0x8FE6, 0x8FE9, 0x8FEB, 0x8FED, 0x8FED, 0x8FEF, 0x8FF0, 0x8FF4, 0x8FF4,
	0x8FF7, 0x8FFA, 0x8FFD, 0x8FFD, 0x9000, 0x9001, 0x9003, 0x9003, 0x9005, 0x9006, 0x900B, 0x900B, 0x900D, 0x9011, 0x9013, 0x9017,
	0x9019, 0x901A, 0x901D, 0x9023, 0x9027, 0x9027, 0x902E, 0x902E, 0x9031, 0x9032, 0x9035, 0x9036, 0x9038, 0x9039, 0x903C, 0x903C,
	0x903E, 0x903E, 0x9041, 0x9042, 0x9045, 0x9045, 0x9047, 0x9047, 0x9049, 0x904B, 0x904D, 0x9056, 0x9058, 0x9059, 0x905C, 0x905C,
	0x905E, 0x905E, 0x9060, 0x9061, 0x9063, 0x9063, 0x9065, 0x9065, 0x9068, 0x9069, 0x906D, 0x906F, 0x9072, 0x9072, 0x9075, 0x9078,
	0x907A, 0x907A, 0x907C, 0x907D, 0x907F, 0x9084, 0x9087, 0x9087, 0x9089, 0x908A, 0x908F, 0x908F, 0x9091, 0x9091, 0x90A3, 0x90A3,
	0x90A6, 0x90A6, 0x90A8, 0x90A8, 0x90AA, 0x90AA, 0x90AF, 0x90AF, 0x90B1, 0x90B1, 0x90B5, 0x90B5, 0x90B8, 0x90B8, 0x90C1, 0x90C1,
	0x90CA, 0x90CA, 0x90CE, 0x90CE, 0x90DB, 0x90DB, 0x90E1, 0x90E2, 0x90E4, 0x90E4, 0x90E8, 0x90E8, 0x90ED, 0x90ED, 0x90F5, 0x90F5,
	0x90F7, 0x90F7, 0x90FD, 0x90FD, 0x9102, 0x9102, 0x9112, 0x9112, 0x9119, 0x9119, 0x912D, 0x912D, 0x9130, 0x9130, 0x9132, 0x9132,
	0x9149, 0x914E, 0x9152, 0x9152, 0x9154, 0x9154, 0x9156, 0x9156, 0x9158, 0x9158, 0x9162, 0x9163, 0x9165, 0x9165, 0x9169, 0x916A,
	0x916C, 0x916C, 0x9172, 0x9173, 0x9175, 0x9175, 0x9177, 0x9178, 0x9182, 0x9182, 0x9187, 0x9187, 0x9189, 0x9189, 0x918B, 0x918B,
	0x918D, 0x918D, 0x9190, 0x9190, 0x9192, 0x9192, 0x9197, 0x9197, 0x919C, 0x919C, 0x91A2, 0x91A2, 0x91A4, 0x91A4, 0x91AA, 0x91AB,
	0x91AF, 0x91AF, 0x91B4, 0x91B5, 0x91B8, 0x91B8, 0x91BA, 0x91BA, 0x91C0, 0x91C1, 0x91C6, 0x91C9, 0x91CB, 0x91D1, 0x91D6, 0x91D6,
	0x91D8, 0x91D8, 0x91DB, 0x91DD, 0x91DF, 0x91DF, 0x91E1, 0x91E1, 0x91E3, 0x91E3, 0x91E6, 0x91E7, 0x91F5, 0x91F6, 0x91FC, 0x91FC,
	0x91FF, 0x91FF, 0x920D, 0x920E, 0x9211, 0x9211, 0x9214, 0x9215, 0x921E, 0x921E, 0x9229, 0x9229, 0x922C, 0x922C, 0x9234, 0x9234,
	0x9237, 0x9237, 0x923F, 0x923F, 0x9244, 0x9245, 0x9248, 0x9249, 0x924B, 0x924B, 0x9250, 0x9250, 0x9257, 0x9257, 0x925A, 0x925B,
	0x925E, 0x925E, 0x9262, 0x9262, 0x9264, 0x9264, 0x9266, 0x9266, 0x9271, 0x9271, 0x927E, 0x927E, 0x9280, 0x9280, 0x9283, 0x9283,
	0x9285, 0x9285, 0x9291, 0x9291, 0x9293, 0x9293, 0x9295, 0x9296, 0x9298, 0x9298, 0x929A, 0x929C, 0x92AD, 0x92AD, 0x92B7, 0x92B7,
	0x92B9, 0x92B9, 0x92CF, 0x92CF, 0x92D2, 0x92D2, 0x92E4, 0x92E4, 0x92E9, 0x92EA, 0x92ED, 0x92ED, 0x92F2, 0x92F3, 0x92F8, 0x92F8,
	0x92FA, 0x92FA, 0x92FC, 0x92FC, 0x9306, 0x9306, 0x930F, 0x9310, 0x9318, 0x931A, 0x9320, 0x9320, 0x9322, 0x9323, 0x9326, 0x9326,
	0x9328, 0x9328, 0x932B, 0x932C, 0x932E, 0x932F, 0x9332, 0x9332, 0x9335, 0x9335, 0x933A, 0x933B, 0x9344, 0x9344, 0x934B, 0x934B,
	0x934D, 0x934D, 0x9354, 0x9354, 0x9356, 0x9356, 0x935B, 0x935C, 0x9360, 0x9360, 0x936C, 0x936C, 0x936E, 0x936E, 0x9375, 0x9375,
	0x937C, 0x937C, 0x937E, 0x937E, 0x938C, 0x938C, 0x9394, 0x9394, 0x9396, 0x9397, 0x939A, 0x939A, 0x93A7, 0x93A7, 0x93AC, 0x93AE,
	0x93B0, 0x93B0, 0x93B9, 0x93B9, 0x93C3, 0x93C3, 0x93C8, 0x93C8, 0x93D0, 0x93D1, 0x93D6, 0x93D8, 0x93DD, 0x93DD, 0x93E1, 0x93E1,
	0x93E4, 0x93E5, 0x93E8, 0x93E8, 0x9403, 0x9403, 0x9407, 0x9407, 0x9410, 0x9410, 0x9413, 0x9414, 0x9418, 0x941A, 0x9421, 0x9421,
	0x942B, 0x942B, 0x9435, 0x9436, 0x9438, 0x9438, 0x943A, 0x943A, 0x9441, 0x9441, 0x9444, 0x9444, 0x9451, 0x9453, 0x945A, 0x945B,
	0x945E, 0x945E, 0x9460, 0x9460, 0x9462, 0x9462, 0x946A, 0x946A, 0x9470, 0x9470, 0x9475, 0x9475, 0x9477, 0x9477, 0x947C, 0x947F,
	0x9481, 0x9481, 0x9577, 0x9577, 0x9580, 0x9580, 0x9582, 0x9583, 0x9587, 0x9587, 0x9589, 0x958B, 0x958F, 0x958F, 0x9591, 0x9591,
	0x9593, 0x9594, 0x9596, 0x9596, 0x9598, 0x9599, 0x95A0, 0x95A0, 0x95A2, 0x95A5, 0x95A7, 0x95A8, 0x95AD, 0x95AD, 0x95B2, 0x95B2,
	0x95B9, 0x95B9, 0x95BB, 0x95BC, 0x95BE, 0x95BE, 0x95C3, 0x95C3, 0x95C7, 0x95C7, 0x95CA, 0x95CA, 0x95CC, 0x95CD, 0x95D4, 0x95D6,
	0x95D8, 0x95D8, 0x95DC, 0x95DC, 0x95E1, 0x95E2, 0x95E5, 0x95E5, 0x961C, 0x961C, 0x9621, 0x9621, 0x9628, 0x9628, 0x962A, 0x962A,
	0x962E, 0x962F, 0x9632, 0x9632, 0x963B, 0x963B, 0x963F, 0x9640, 0x9642, 0x9642, 0x9644, 0x9644, 0x964B, 0x964D, 0x964F, 0x9650,
	0x965B, 0x965F, 0x9662, 0x9666, 0x966A, 0x966A, 0x966C, 0x966C, 0x9670, 0x9670, 0x9672, 0x9673, 0x9675, 0x9678, 0x967A, 0x967A,
	0x967D, 0x967D, 0x9685, 0x9686, 0x9688, 0x9688, 0x968A, 0x968B, 0x968D, 0x968F, 0x9694, 0x9695, 0x9697, 0x9699, 0x969B, 0x969C,
	0x96A0, 0x96A0, 0x96A3, 0x96A3, 0x96A7, 0x96A8, 0x96AA, 0x96AA, 0x96B0, 0x96B2, 0x96B4, 0x96B4, 0x96B6, 0x96B9, 0x96BB, 0x96BC,
	0x96C0, 0x96C1, 0x96C4, 0x96C7, 0x96C9, 0x96C9, 0x96CB, 0x96CE, 0x96D1, 0x96D1, 0x96D5, 0x96D6, 0x96D9, 0x96D9, 0x96DB, 0x96DC,
	0x96E2, 0x96E3, 0x96E8, 0x96E8, 0x96EA, 0x96EB, 0x96F0, 0x96F0, 0x96F2, 0x96F2, 0x96F6, 0x96F7, 0x96F9, 0x96F9, 0x96FB, 0x96FB,
	0x9700, 0x9700, 0x9704, 0x9704, 0x9706, 0x9708, 0x970A, 0x970A, 0x970D, 0x970F, 0x9711, 0x9711, 0x9713, 0x9713, 0x9716, 0x9716,
	0x9719, 0x9719, 0x971C, 0x971C, 0x971E, 0x971E, 0x9724, 0x9724, 0x9727, 0x9727, 0x972A, 0x972A, 0x9730, 0x9730, 0x9732, 0x9732,
	0x9738, 0x9739, 0x973D, 0x973E, 0x9742, 0x9742, 0x9744, 0x9744, 0x9746, 0x9746, 0x9748, 0x9749, 0x9752, 0x9752, 0x9756, 0x9756,
	0x9759, 0x9759, 0x975C, 0x975C, 0x975E, 0x975E, 0x9760, 0x9762, 0x9764, 0x9764, 0x9766, 0x9766, 0x9768, 0x9769, 0x976B, 0x976B,
	0x976D, 0x976D, 0x9771, 0x9771, 0x9774, 0x9774, 0x9779, 0x977A, 0x977C, 0x977C, 0x9781, 0x9781, 0x9784, 0x9786, 0x978B, 0x978B,
	0x978D, 0x978D, 0x978F, 0x9790, 0x9798, 0x9798, 0x979C, 0x979C, 0x97A0, 0x97A0, 0x97A3, 0x97A3, 0x97A6, 0x97A6, 0x97A8, 0x97A8,
	0x97AB, 0x97AB, 0x97AD, 0x97AD, 0x97B3, 0x97B4, 0x97C3, 0x97C3, 0x97C6, 0x97C6, 0x97C8, 0x97C8, 0x97CB, 0x97CB, 0x97D3, 0x97D3,
	0x97DC, 0x97DC, 0x97ED, 0x97EE, 0x97F2, 0x97F3, 0x97F5, 0x97F6, 0x97FB, 0x97FB, 0x97FF, 0x97FF, 0x9801, 0x9803, 0x9805, 0x9806,
	0x9808, 0x9808, 0x980C, 0x980C, 0x980F, 0x9813, 0x9817, 0x9818, 0x981A, 0x981A, 0x9821, 0x9821, 0x9824, 0x9824, 0x982C, 0x982D,
	0x9834, 0x9834, 0x9837, 0x9838, 0x983B, 0x983D, 0x9846, 0x9846, 0x984B, 0x984F, 0x9854, 0x9855, 0x9858, 0x9858, 0x985B, 0x985B,
	0x985E, 0x985E, 0x9867, 0x9867, 0x986B, 0x986B, 0x986F, 0x9871, 0x9873, 0x9874, 0x98A8, 0x98A8, 0x98AA, 0x98AA, 0x98AF, 0x98AF,
	0x98B1, 0x98B1, 0x98B6, 0x98B6, 0x98C3, 0x98C4, 0x98C6, 0x98C6, 0x98DB, 0x98DC, 0x98DF, 0x98DF, 0x98E2, 0x98E2, 0x98E9, 0x98E9,
	0x98EB, 0x98EB, 0x98ED, 0x98EF, 0x98F2, 0x98F2, 0x98F4, 0x98F4, 0x98FC, 0x98FE, 0x9903, 0x9903, 0x9905, 0x9905, 0x9909, 0x990A,
	0x990C, 0x990C, 0x9910, 0x9910, 0x9912, 0x9914, 0x9918, 0x9918, 0x991D, 0x991E, 0x9920, 0x9921, 0x9924, 0x9924, 0x9928, 0x9928,
	0x992C, 0x992C, 0x992E, 0x992E, 0x993D, 0x993E, 0x9942, 0x9942, 0x9945, 0x9945, 0x9949, 0x9949, 0x994B, 0x994C, 0x9950, 0x9952,
	0x9955, 0x9955, 0x9957, 0x9957, 0x9996, 0x9999, 0x99A5, 0x99A5, 0x99A8, 0x99A8, 0x99AC, 0x99AE, 0x99B3, 0x99B4, 0x99BC, 0x99BC,
	0x99C1, 0x99C1, 0x99C4, 0x99C6, 0x99C8, 0x99C8, 0x99D0, 0x99D2, 0x99D5, 0x99D5, 0x99D8, 0x99D8, 0x99DB, 0x99DB, 0x99DD, 0x99DD,
	0x99DF, 0x99DF, 0x99E2, 0x99E2, 0x99ED, 0x99EE, 0x99F1, 0x99F2, 0x99F8, 0x99F8, 0x99FB, 0x99FB, 0x99FF, 0x99FF, 0x9A01, 0x9A01,
	0x9A05, 0x9A05, 0x9A0E, 0x9A0F, 0x9A12, 0x9A13, 0x9A19, 0x9A19, 0x9A28, 0x9A28, 0x9A2B, 0x9A2B, 0x9A30, 0x9A30, 0x9A37, 0x9A37,
	0x9A3E, 0x9A3E, 0x9A40, 0x9A40, 0x9A42, 0x9A43, 0x9A45, 0x9A45, 0x9A4D, 0x9A4D, 0x9A55, 0x9A55, 0x9A57, 0x9A57, 0x9A5A, 0x9A5B,
	0x9A5F, 0x9A5F, 0x9A62, 0x9A62, 0x9A64, 0x9A65, 0x9A69, 0x9A6B, 0x9AA8, 0x9AA8, 0x9AAD, 0x9AAD, 0x9AB0, 0x9AB0, 0x9AB8, 0x9AB8,
	0x9ABC, 0x9ABC, 0x9AC0, 0x9AC0, 0x9AC4, 0x9AC4, 0x9ACF, 0x9ACF, 0x9AD1, 0x9AD1, 0x9AD3, 0x9AD4, 0x9AD8, 0x9AD8, 0x9ADE, 0x9ADF,
	0x9AE2, 0x9AE3, 0x9AE6, 0x9AE6, 0x9AEA, 0x9AEB, 0x9AED, 0x9AEF, 0x9AF1, 0x9AF1, 0x9AF4, 0x9AF4, 0x9AF7, 0x9AF7, 0x9AFB, 0x9AFB,
	0x9B06, 0x9B06, 0x9B18, 0x9B18, 0x9B1A, 0x9B1A, 0x9B1F, 0x9B1F, 0x9B22, 0x9B23, 0x9B25, 0x9B25, 0x9B27, 0x9B2A, 0x9B2E, 0x9B2F,
	0x9B31, 0x9B32, 0x9B3B, 0x9B3C, 0x9B41, 0x9B45, 0x9B4D, 0x9B4F, 0x9B51, 0x9B51, 0x9B54, 0x9B54, 0x9B58, 0x9B58, 0x9B5A, 0x9B5A,
	0x9B6F, 0x9B6F, 0x9B74, 0x9B74, 0x9B83, 0x9B83, 0x9B8E, 0x9B8E, 0x9B91, 0x9B93, 0x9B96, 0x9B97, 0x9B9F, 0x9BA0, 0x9BA8, 0x9BA8,
	0x9BAA, 0x9BAB, 0x9BAD, 0x9BAE, 0x9BB4, 0x9BB4, 0x9BB9, 0x9BB9, 0x9BC0, 0x9BC0, 0x9BC6, 0x9BC6, 0x9BC9, 0x9BCA, 0x9BCF, 0x9BCF,
	0x9BD1, 0x9BD2, 0x9BD4, 0x9BD4, 0x9BD6, 0x9BD6, 0x9BDB, 0x9BDB, 0x9BE1, 0x9BE4, 0x9BE8, 0x9BE8, 0x9BF0, 0x9BF2, 0x9BF5, 0x9BF5,
	0x9C04, 0x9C04, 0x9C06, 0x9C06, 0x9C08, 0x9C0A, 0x9C0C, 0x9C0D, 0x9C10, 0x9C10, 0x9C12, 0x9C15, 0x9C1B, 0x9C1B, 0x9C21, 0x9C21,
	0x9C24, 0x9C25, 0x9C2D, 0x9C30, 0x9C32, 0x9C32, 0x9C39, 0x9C3B, 0x9C3E, 0x9C3E, 0x9C46, 0x9C48, 0x9C52, 0x9C52, 0x9C57, 0x9C57,
	0x9C5A, 0x9C5A, 0x9C60, 0x9C60, 0x9C67, 0x9C67, 0x9C76, 0x9C76, 0x9C78, 0x9C78, 0x9CE5, 0x9CE5, 0x9CE7, 0x9CE7, 0x9CE9, 0x9CE9,
	0x9CEB, 0x9CEC, 0x9CF0, 0x9CF0, 0x9CF3, 0x9CF4, 0x9CF6, 0x9CF6, 0x9D03, 0x9D03, 0x9D06, 0x9D09, 0x9D0E, 0x9D0E, 0x9D12, 0x9D12,
	0x9D15, 0x9D15, 0x9D1B, 0x9D1B, 0x9D1F, 0x9D1F, 0x9D23, 0x9D23, 0x9D26, 0x9D26, 0x9D28, 0x9D28, 0x9D2A, 0x9D2C, 0x9D3B, 0x9D3B,
	0x9D3E, 0x9D3F, 0x9D41, 0x9D41, 0x9D44, 0x9D44, 0x9D46, 0x9D46, 0x9D48, 0x9D48, 0x9D50, 0x9D51, 0x9D59, 0x9D59, 0x9D5C, 0x9D5E,
	0x9D60, 0x9D61, 0x9D64, 0x9D64, 0x9D6C, 0x9D6C, 0x9D6F, 0x9D6F, 0x9D72, 0x9D72, 0x9D7A, 0x9D7A, 0x9D87, 0x9D87, 0x9D89, 0x9D89,
	0x9D8F, 0x9D8F, 0x9D9A, 0x9D9A, 0x9DA4, 0x9DA4, 0x9DA9, 0x9DA9, 0x9DAB, 0x9DAB, 0x9DAF, 0x9DAF, 0x9DB2, 0x9DB2, 0x9DB4, 0x9DB4,
	0x9DB8, 0x9DB8, 0x9DBA, 0x9DBB, 0x9DC1, 0x9DC2, 0x9DC4, 0x9DC4, 0x9DC6, 0x9DC6, 0x9DCF, 0x9DCF, 0x9DD3, 0x9DD3, 0x9DD9, 0x9DD9,
	0x9DE6, 0x9DE6, 0x9DED, 0x9DED, 0x9DEF, 0x9DEF, 0x9DF2, 0x9DF2, 0x9DF8, 0x9DFA, 0x9DFD, 0x9DFD, 0x9E1A, 0x9E1B, 0x9E1E, 0x9E1E,
	0x9E75, 0x9E75, 0x9E78, 0x9E79, 0x9E7D, 0x9E7D, 0x9E7F, 0x9E7F, 0x9E81, 0x9E81, 0x9E88, 0x9E88, 0x9E8B, 0x9E8C, 0x9E91, 0x9E93,
	0x9E95, 0x9E95, 0x9E97, 0x9E97, 0x9E9D, 0x9E9D, 0x9E9F, 0x9E9F, 0x9EA5, 0x9EA6, 0x9EA9, 0x9EAA, 0x9EAD, 0x9EAD, 0x9EB8, 0x9EBC,
	0x9EBE, 0x9EBF, 0x9EC4, 0x9EC4, 0x9ECC, 0x9ED0, 0x9ED2, 0x9ED2, 0x9ED4, 0x9ED4, 0x9ED8, 0x9ED9, 0x9EDB, 0x9EDE, 0x9EE0, 0x9EE0,
	0x9EE5, 0x9EE5, 0x9EE8, 0x9EE8, 0x9EEF, 0x9EEF, 0x9EF4, 0x9EF4, 0x9EF6, 0x9EF7, 0x9EF9, 0x9EF9, 0x9EFB, 0x9EFD, 0x9F07, 0x9F08,
	0x9F0E, 0x9F0E, 0x9F13, 0x9F13, 0x9F15, 0x9F15, 0x9F20, 0x9F21, 0x9F2C, 0x9F2C, 0x9F3B, 0x9F3B, 0x9F3E, 0x9F3E, 0x9F4A, 0x9F4B,
	0x9F4E, 0x9F4F, 0x9F52, 0x9F52, 0x9F54, 0x9F54, 0x9F5F, 0x9F63, 0x9F66, 0x9F67, 0x9F6A, 0x9F6A, 0x9F6C, 0x9F6C, 0x9F72, 0x9F72,
	0x9F76, 0x9F77, 0x9F8D, 0x9F8D, 0x9F95, 0x9F95, 0x9F9C, 0x9F9D, 0x9FA0, 0x9FA0, 0xFF01, 0xFF01, 0xFF03, 0xFF06, 0xFF08, 0xFF0C,
	0xFF0E, 0xFF3B, 0xFF3D, 0xFF5D, 0xFF61, 0xFF9F, 0xFFE3, 0xFFE3, 0xFFE5, 0xFFE5, 0xFFFF, 0xFFFF, 0, };

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// Include imgui_user.h at the end of imgui.h (convenient for user to only explicitly include vanilla imgui.h)
#ifdef IMGUI_INCLUDE_IMGUI_USER_H
#include "imgui_user.h"
#endif
