sizeofblock = 3	
i = 0



function foutre (current, nb_thread)
	 i = i+1	 
    	 if i > sizeofblock then 
       	 print("on change core")
       	 i = 1 
       	 current = (current +1) % nb_thread
       	 end
       	 return current
	 end

