#ifndef __PIXEL_H__
#define __PIXEL_H__

/*
 * The followig codes are from the following discussion.
 * https://stackoverflow.com/questions/34991908/describing-pixel-format-information-in-c-in-a-way-that-is-usable-at-both-compi
 */

enum class color_space : std::uint8_t { rgb, cymk, gray, other };
    
// PIXEL LAYOUT
// Can be created/modified at runtime, but a predefined set of pixel_layouts
// exists for compile-time use.
struct pixel_layout {
    color_space space;
    size_t   channels;
};
    
constexpr bool operator==(const pixel_layout& a, const pixel_layout& b) {
    return (a.space == b.space) && (a.channels == b.channels);
}

constexpr bool operator!=(const pixel_layout& a, const pixel_layout& b) {
    return (a.space != b.space) || (a.channels != b.channels);
}
    
// Predefined pixel_layout instances, for use as template arguments
// As static constexpr members of class, to make sure they have external linkage,
// required for use as reference template arguments.
struct default_pixel_layouts {
    static constexpr pixel_layout rgb{ color_space::rgb, 3 };
    static constexpr pixel_layout cymk{ color_space::cymk, 4 };
    static constexpr pixel_layout gray{ color_space::gray, 1 };
};
    
// Definitions for the pixel_layouts
constexpr pixel_layout default_pixel_layouts::rgb;
constexpr pixel_layout default_pixel_layouts::cymk;
    
// PIXEL TYPE
// Takes pixel_layout reference as non-type template argument.
template<const pixel_layout& Layout>
struct pixel {
    static constexpr const pixel_layout& layout = Layout;
    static constexpr const size_t channels = Layout.channels;

    // Because layout is constexpr, can use its members (e.g. channels),
    // for example as template argument.
    // Here size of pixel depends on number of channels in pixel_layout
    std::array<std::uint32_t, layout.channels> data;

    uint32_t& operator[](size_t channel) {
        return data[channel];
    }
};

// RGB and CYMK pixel_types as type aliases
using rgb  = pixel<default_pixel_layouts::rgb>;
using cymk = pixel<default_pixel_layouts::cymk>;
using gray = pixel<default_pixel_layouts::gray>;

template <typename To, typename From>
inline void transform(To &to, const From& from) {
    // TBD
}

template<>
inline void transform(gray &to, const rgb& from) {
    to.data[0] = (from.data[0] + from.data[1] + from.data[2]) / 3;
}

#endif  // __PIXEL_H__
