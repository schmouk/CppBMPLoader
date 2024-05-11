/*
MIT License

Copyright (c) 2024 Philippe Schmouker, ph.schmouker (at) gmail.com

Permission is hereby granted,  free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"),  to deal
in the Software without restriction,  including without limitation the  rights
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell
copies of the Software,  and  to  permit  persons  to  whom  the  Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL  THE
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  ARISING FROM,
OUT  OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/*
* NOTICE: code here is implemented according to the c++14 standard.  It should
* function  as  well  when  compiled  with  standard  c++11  because  no c++14
* specificities have been used there, but it has not been tested as such.
*/

#include "bitfield_mask.h"


namespace bmpl
{
    namespace utils
    {


        const std::uint32_t BitfieldMaskBase::get_component_value(const std::uint32_t pixel_value) const noexcept
        {
            std::uint32_t color_component{ this->_evaluate_component(pixel_value) };

            if (color_component == 0)
                return 0;

            switch (_bits_count)
            {
            case 0:
                return 0;
            case 1:
                return color_component * 0b1111'1111;
            case 2:
                return color_component * 0b0101'0101;
            case 3:
                return (color_component * 0b0010'0100) | (color_component >> 1);
            case 4:
                return color_component * 0b1000'1000;
            case 5:
                return (color_component << 3) | (color_component >> 2);
            case 6:
                return (color_component << 2) | (color_component >> 4);
            case 7:
                return (color_component << 1) | (color_component >> 6);
            case 8:
                return color_component;
            default:
                return color_component >> (_bits_count - 8);
            }

        }


        const std::uint32_t BitfieldMask::_evaluate_component(const std::uint32_t pixel_value) const noexcept
        {
            std::uint32_t color_component{ 0 };

            // notice: this algorithm is ok since bits indexes are ordered from greatest to smallest value
            for (auto index : this->_bits_indexes) {
                color_component <<= 1;
                color_component |= (pixel_value & (1 << index)) != 0;
            }

            return color_component;
        }


        const BitfieldMaskBase* create_bitfield_mask(const std::uint32_t mask) noexcept
        {
            // are there set bits in the mask?
            if (mask == 0) {
                // well, no so let's return a pointer to the base class, the
                // one that always returns 0 as the pixel component value
                return new BitfieldMaskBase();
            }

            // let's evaluate the set bits indexes in the mask
            std::uint32_t _mask{ mask };
            std::uint32_t bits_count{ 0 };
            std::vector<std::uint8_t> bits_indexes;
            for (int index = 31; _mask != 0 && index >= 0; --index) {
                if (_mask & 0x8000'0000) {
                    bits_indexes.push_back(index);
                    ++bits_count;
                }
                _mask <<= 1;
            }

            // are the set bits contiguous?
            for (auto ndx_it = bits_indexes.cbegin(), ndx1_it = ndx_it + 1;
                ndx1_it != bits_indexes.cend();
                ++ndx_it, ++ndx1_it)
            {
                if (*ndx_it != *ndx1_it + 1) {
                    // the set bits are not contiguous
                    return new BitfieldMask(bits_indexes, bits_count);
                }
            }

            // once here, the set bits finally were contiguous
            return new BitfieldContiguousMask(mask, bits_indexes.back(), bits_count);
        }

    }
}