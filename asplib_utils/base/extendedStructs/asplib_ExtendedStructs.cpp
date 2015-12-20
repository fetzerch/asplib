/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include "asplib_utils/base/extendedStructs/asplib_ExtendedStructs.hpp"


namespace asplib
{
// helper function prototypes
uint32_t assignID(uint32_t &ID);


// create extended struct with Size and ID
CExtendedStructs::CExtendedStructs(size_t Size, uint32_t &StructID) : 
  size(Size - sizeof(CExtendedStructs)),
  byteOffset(sizeof(CExtendedStructs)),
  ID(assignID(StructID))
{
}


//-------------------------------------------------
// helper funtions implementations
//-------------------------------------------------
// A valid ID is greater than 0, if ID is equal or less than 0, the system will assign a new ID.
uint32_t assignID(uint32_t &ID)
{
  static uint32_t sID = 0;

  if (ID > 0)
  {
    return ID;
  }

  ID = ++sID;
  return ID;
}
}
