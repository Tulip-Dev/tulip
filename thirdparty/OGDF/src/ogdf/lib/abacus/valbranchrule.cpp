/*!\file
 * \author Matthias Elf
 *
 * \par License:
 * This file is part of ABACUS - A Branch And CUt System
 * Copyright (C) 1995 - 2003
 * University of Cologne, Germany
 *
 * \par
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * \par
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * \see http://www.gnu.org/copyleft/gpl.html
 */

#include <ogdf/lib/abacus/valbranchrule.h>
#include <ogdf/lib/abacus/master.h>
#include <ogdf/lib/abacus/fsvarstat.h>
#include <ogdf/lib/abacus/sub.h>

namespace abacus {


std::ostream &operator<<(std::ostream &out, const ValBranchRule &rhs)
{
	return out << "x" << rhs.variable_ << " = " << rhs.value_;
}


int ValBranchRule::extract(Sub *sub)
{
	if (sub->fsVarStat(variable_)->contradiction(FSVarStat::Set, value_))
		return 1;

	sub->fsVarStat(variable_)->status(FSVarStat::Set, value_);
	return 0;
}


void ValBranchRule::extract(LpSub *lp)
{
	oldLpLBound_ = lp->lBound(variable_);
	oldLpUBound_ = lp->uBound(variable_);

	lp->changeLBound(variable_, value_);
	lp->changeUBound(variable_, value_);
}


void ValBranchRule::unExtract(LpSub *lp)
{
	lp->changeLBound(variable_, oldLpLBound_);
	lp->changeUBound(variable_, oldLpUBound_);
}
}
