/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

// Declaration
#include "Image.hpp"

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
// stb_image_write
#  define STB_IMAGE_WRITE_IMPLEMENTATION
#  define STBI_WRITE_NO_STDIO
#  include "stb_image_write.h"
#endif

// CeCe
#include "cece/core/FileStream.hpp"
#include "cece/core/Exception.hpp"

/* ************************************************************************ */

namespace cece {
namespace cli {

/* ************************************************************************ */

#ifdef CECE_CLI_ENABLE_IMAGE_CAPTURE
void storeImage(const FilePath& filename, DynamicArray<unsigned char> data,
    unsigned int width, unsigned int height)
{
    constexpr int CHANNEL_COUNT = 3;

    OutFileStream file(filename.c_str(), OutFileStream::binary);

    if (!file.is_open())
        throw InvalidArgumentException("Cannot open output file: " + filename.toString());

    // Image is vertically flipped
    DynamicArray<unsigned char> data_flip(data.size());

    const unsigned int line_size = width * CHANNEL_COUNT;

    // Vertically flip lines
    for (unsigned int i = 0; i < height; ++i)
    {
        std::memcpy(
            data_flip.data() + i * line_size,
            data.data() + (height - i - 1) * line_size,
            line_size
        );
    }

    // Write function
    stbi_write_func* func = [] (void* context, void* data, int size) {
        reinterpret_cast<OutFileStream*>(context)->write(reinterpret_cast<const char*>(data), size);
    };

    // Copy data
    if (!stbi_write_png_to_func(func, &file, width, height, CHANNEL_COUNT, data_flip.data(), 0))
        throw RuntimeException("Unable to write a picture");
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
