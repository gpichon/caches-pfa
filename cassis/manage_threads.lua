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

