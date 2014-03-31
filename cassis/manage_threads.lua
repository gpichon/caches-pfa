-- Copyright (c) 2014 Nicolas Dubois, Pierre Goudet, Nicolas Heng,
-- Alexandre Honorat, Gilles Marait, Grégoire Pichon.
--
-- This Source Code Form is subject to the terms of the Mozilla Public
-- License, v. 2.0. If a copy of the MPL was not distributed with this
-- file, You can obtain one at http://mozilla.org/MPL/2.0/.


sizeofblock = 10	
i = 0



function interweave (current, nb_thread)
	 i = i+1	 
    	 if i > sizeofblock then 
       	 i = 1 
       	 current = (current +1) % nb_thread
       	 end
       	 return current
	 end

