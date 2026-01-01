#pragma once

/*
MIT License

Copyright (c) 2024-2026 Philippe Schmouker, ph.schmouker (at) gmail.com

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
* function  as  well  when  compiled  with  standard  c++11  since  no   c++14
* specificities have been used there, but it has not been tested as such.
*/


#include <cstdint>
#include <vector>


namespace bmpl
{
    namespace bmpf
    {
        //===========================================================================
        class BitfieldMaskBase
        {
        public:
            inline BitfieldMaskBase(const std::uint32_t bits_count) noexcept
                : _bits_count(bits_count)
            {}

            inline BitfieldMaskBase() noexcept = default;
            virtual ~BitfieldMaskBase() noexcept = default;

            BitfieldMaskBase(const BitfieldMaskBase&) noexcept = delete;
            BitfieldMaskBase(BitfieldMaskBase&&) noexcept = delete;

            BitfieldMaskBase& operator=(const BitfieldMaskBase&) noexcept = delete;
            BitfieldMaskBase& operator=(BitfieldMaskBase&&) noexcept = delete;


            [[nodiscard]]
            const std::uint32_t get_component_value(const std::uint32_t pixel_value) const noexcept;


        protected:
            std::uint32_t _bits_count{ 0 };

            [[nodiscard]]
            virtual inline const std::uint32_t _evaluate_component(const std::uint32_t pixel_value) const noexcept
            {
                return 0;
            }

        };


        //===========================================================================
        class BitfieldMask : public BitfieldMaskBase
        {
        public:
            inline BitfieldMask(const std::vector<std::uint8_t>& bits_indices, const std::uint32_t bits_count) noexcept
                : BitfieldMaskBase(bits_count)
                , _bits_indexes(bits_indices)
            {}


        protected:
            [[nodiscard]]
            virtual const std::uint32_t _evaluate_component(const std::uint32_t pixel_value) const noexcept;


        private:
            std::vector<std::uint8_t> _bits_indexes;

        };


        //===========================================================================
        class BitfieldContiguousMask : public BitfieldMaskBase
        {
        public:
            inline BitfieldContiguousMask(
                const std::uint32_t mask,
                const std::uint32_t shift_count,
                const std::uint32_t bits_count
            ) noexcept
                : BitfieldMaskBase(bits_count)
                , _mask(mask)
                , _shift(shift_count)
            {}


        protected:
            [[nodiscard]]
            virtual inline const std::uint32_t _evaluate_component(const std::uint32_t pixel_value) const noexcept
            {
                return (pixel_value & this->_mask) >> this->_shift;
            }


        private:
            const std::uint32_t _mask{ 0 };
            const std::uint32_t _shift{ 0 };

        };


        //===========================================================================
        const BitfieldMaskBase* create_bitfield_mask(const std::uint32_t mask) noexcept;

    }
}