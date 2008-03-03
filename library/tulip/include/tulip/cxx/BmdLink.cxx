/*
 * Authors: Jérémy Compostella, Jean Darracq, Benjamin Muller,
 *          Fabrice Rochambeau, Fabiani Simplice, Jyl Cristoff Zobeide
 * 
 * Email : jcompost@etu.u-bordeaux1.fr, jdarracq@etu.u-bordeaux1.fr,
 *         bmuller@etu.u-bordeaux1.fr, frochamb@etu.u-bordeaux1.fr,
 *         fsimplic@etu.u-bordeaux1.fr, jczobeid@etu.u-bordeaux1.fr.
 *
 * Last modification : $id $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

//================================================================
namespace tlp {

template <typename TYPE>
BmdLink<TYPE>::BmdLink(TYPE data, BmdLink<TYPE> *pre, BmdLink<TYPE> *suc)
  : data(data), pre(suc), suc(pre) {
}

}
//=================================================================
