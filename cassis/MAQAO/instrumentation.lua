---
--  Copyright (C) 2004 - 2012 Université de Versailles Saint-Quentin-en-Yvelines (UVSQ)
--
-- This file is part of MAQAO.
--
-- MAQAO is free software; you can redistribute it and/or
--  modify it under the terms of the GNU Lesser General Public License
--  as published by the Free Software Foundation; either version 3
--  of the License, or (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU Lesser General Public License for more details.
--
--  You should have received a copy of the GNU Lesser General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
--
local fct_name = arg[3];
local binary = arg[2];

local event,model = "LI";
local trace_path =".";
local innermost_loopid_list = nil
local asmf;
local pname = "proj";
local libmt = "";

local loops =nil;

if(type(binary) ~= "string") then
   Message:critical("Required argument : binary not defined or invalid string");
end
if(type(loops) ~= "string" and type(fct_name) ~= "string") then
   Message:critical("Required argument : either loops or fct must be defined.");
end

model = "gomp";
libmt = "libmt-gomp.so";
Message:info("Using multicore model (OpenMP)");

if(fs.exists(binary) == false) then
   Message:critical("Binary "..binary.." could not be loaded.");
end

  p    = project.new(pname);
  asmf = p:load(binary,0);
  asmh = asmf:get_hash();
  sid  = 1;	
  
  local innermost_loop_list = Table:new();
  local instructions_tab = Table:new({insns = Table:new(),iinsns = Table:new(),loops = Table:new()});
  local instru_count = 1;
  local nb_insns     = {};
  local total_insns  = 0;
  
  if(type(loops) == "string") then 
     loopid_list = loadstring('return Table:new({'..loops..'})')();  
     
     if(type(loopid_list) == "table" and loopid_list:get_size() == 0) then
	Message:critical("Given loop list is invalid (no loop ids found).");
     else
	for fct in asmf:functions() do
	   for loop in fct:innermost_loops(fct) do
	      if(loopid_list:containsv(loop:get_id())) then
		 innermost_loop_list[loop:get_id()] = loop;
	      end
	   end
	end
     end
     if(innermost_loop_list:get_size() == 0) then
	Message:critical("Given loops were not found. Did you provide the correct ids ?");
     end 		
  elseif(loopid_list == nil and type(fct_name) == "string") then
     print("function "..fct_name);
     for fct in asmf:functions()	do
	if(string.find(fct:get_name(),fct_name)) then  	
	   print("Ok for fct_name = "..fct:get_name())
	   print("CFG : "..fct:get_CFG_file_path())							
	   for loop in fct:loops() do
	      print("Loop "..loop:get_id())
	      innermost_loop_list[loop:get_id()] = loop;
	   end
	end
     end
     
     if(innermost_loop_list:get_size() == 0) then 
	Message:critical("Given function is invalid or have no innermost loops.");
     end		
  elseif(loopid_list == nil and type(fct_name) ~= "string") then
     Message:critical("Neither loop list nor function were defined.");		
  else
     Message:critical("Given loop list is invalid (no loop ids found).");	
  end
  print("fct name "..fct_name)
  instru = madras.new(binary);
  instru:modifs_init(MDSAPI.STACK_SHIFT,512);
  --instru:modifs_init(MDSAPI.STACK_KEEP,0);
  instru:traceon();
  instru:modifs_addopt(MDSAPI.PATCHOPT_MOV1INSN);

  instructions_tab.groups    = Table:new();
  ---instructions_tab.polytopes = Table:new();
  instructions_tab.groupstxt = Table:new();
  
  insn_tab = Table:new();
  
  for loopid,lptr in pairs(innermost_loop_list) do
     local groups    = Table:new();
     ---local polytopes = Table:new();
     local groupstxt = Table:new();
     local gid = 1;
     
     ---lptr:get_function():analyze_polytopes();
     lptr:get_function():analyze_groups();
     lptr:get_function():analyze_groups_extend(Consts.x86_64.SSE_OFF);
     instructions_tab.groupstxt[lptr:get_id()] = lptr:get_group_lines();
     
     for group in lptr:groups() do
    	local ginfo    = group:totable();
	
	for _,insn in pairs(ginfo.insns) do
	   groups[insn.insn:get_address()] = gid;  			
	end
	gid = gid + 1;
     end  	
     
     ---polytopes = lptr:get_polytopes();
     ---Table:new(polytopes):tostring();
     --groups:tostring();
     instructions_tab.groups[lptr:get_id()] = Table:new(groups);  	
     
     Debug:info("Instrumenting Loop id "..loopid);
     --local t = Table:new(lptr:get_light_instru());
     local deduc_addrs = Table:new();
     nb_insns[loopid] = 0;
     
     for block in lptr:blocks() do
	Debug:info("\tBlock "..block:get_id());
	for ins in block:instructions() do
	   local mnemo = ins:get_name();
	   local lino = ins:get_address();
	   local linesrc = ins:get_src_line();
	   local addr = ins:get_address();
	   local nb_params = ins:get_noprnds();
	   local memsize;
	   local opr = {};
	   local last_insn_type = "";
	   
	   --Debug:info("Instruction "..mnemo.." line ASM (@"..(addr)..") : "..lino.." Line SRC : "..linesrc);
	   for p=1,nb_params do
	      opr[p] = ins:get_oprnd_type(p-1);
	   end
	   --Table:tostring(opr);
	   --if((opr[1] == MDSAPI.MEMORY or opr[2] == MDSAPI.MEMORY) and mnemo ~= "LEA" and instru_count < 2) then
	   
	   if((ins:is_load() or ins:is_store()) and mnemo ~= "LEA" --[[and ins:get_address() == 0x401557--]]) then
	      if(ins:is_load()) then
		 last_insn_type = "load";
	      elseif(opr[2] == MDSAPI.MEMORY) then
		 last_insn_type = "store";				
	      end
	      
	      memsize = ins:get_first_mem_oprnd().size;				  
	      
	      
	      -- AAdd by G. Vaumourin 
	      -- Check if the instruction is not instrumented yet 
	      local instruction_present = false;
	      
	      
	      for i,v in pairs(insn_tab) do
		 if(v == addr) then 
		    instruction_present = true; 
		 end
	      end
	      
	      if instruction_present == false then 
		 
		 ---
		 instructions_tab.insns[total_insns] = 
		    { 	
		    address = addr,sstride = nil,rstide = nil,type = last_insn_type,
		    asmline = lino,srcline = linesrc,loopid = loopid,computed = false,
		    memsize = memsize,txt = ins:tostring()
		    };
		    
		    insn_tab[total_insns] =addr;
		    
		    --instructions_tab.loops[loopid].insns:insert(total_insns);
		    
		    if(not deduc_addrs:containsv(addr)  ) then
		       
		       
		       if(last_insn_type == "load") then
			  
			  instru:fctcall_new("mt_load_from_mem",libmt,addr,0,0);
			  instru:fctcall_addparam_imm(instru_count-1);
			  print("\t\t"..instru_count.." - Intrumented LOAD ("..mnemo..")\t@"..
				string.format("%x",addr).." SRC@"..linesrc.." "..lptr:get_function():get_name());
			  if(ins:get_operand_src_index() == -1) then
			     Message:critical("invalid source index for LOAD instruction \t@"..
					      string.format("%x",addr).." - ASM@"..lino.." SRC@"..linesrc);
			  end
			  instru:fctcall_addparam_frominsn(ins:get_operand_src_index(),addr,0);
			  
		       elseif(last_insn_type == "store") then
			  
			  instru:fctcall_new("mt_store",libmt,addr,0,0);
			  instru:fctcall_addparam_imm(instru_count-1);					  	
			  print("\t\t"..instru_count.." - Intrumented STORE ("..mnemo..")\t@"..
				string.format("%x",addr).." SRC@"..linesrc.." "..lptr:get_function():get_name());
			  if(ins:get_operand_dest_index() == -1) then
			     Message:critical("invalid destination index for STORE instruction \t@"..
					      string.format("%x",addr).." - ASM@"..lino.." SRC@"..linesrc);
			  end								
			  instru:fctcall_addparam_frominsn(ins:get_operand_dest_index(),addr,0);
		       end
		       instructions_tab.iinsns[instru_count] = {type = "insn",value = total_insns};
		       instru_count = instru_count + 1;
		       instructions_tab.insns[total_insns].computed = false;
		    else
		       --TODO : IP and EIP should also be tested and not only RIP
		       local rip_enum  = instructions_tab.enum_regs:get_index_valueof("RIP");
		       local rip_value = ins:get_address() + ins:get_bitsize()/(64/8);
		       local sreg,ereg;
		       local insnt = instructions_tab.insns[total_insns];
		       --If Instruction pointer present, substitute value
		       insnt.form = instructions_tab.loops[loopid].addresses[addr];
		       insnt.form = string.gsub(insnt.form,"R "..rip_enum,"C "..rip_value);
		       --Debug:info(insnt.form);
		       --Table:new(instructions_tab.loops[loopid]):tostring();
		       sreg		= string.gsub(instructions_tab.loops[loopid].reg_counter,"R","R ");
		       insnt.form 	= insnt.form.." S "..sreg.." N N";
		       ereg		= string.gsub(instructions_tab.loops[loopid].reg_ending,"R","R ");
		       insnt.form 	= insnt.form.." E "..ereg.." N N";
		       insnt.form 	= insnt.form.." Z";
		       insnt.computed = true;
		       Debug:info("Instruction "..mnemo.." line ASM (@"..string.format("%x",addr)..",L"..linesrc..") can be computed afterwards : "..instructions_tab.insns[total_insns].form);
		    end
		    total_insns = total_insns + 1;
		    nb_insns[loopid] = nb_insns[loopid] + 1;
	      end
	   end
	end	
	
     end
     --[
     instructions_tab.insns_count = total_insns;
     Debug:info("Partial : "..(instru_count-1).." instructions instrumented");
     --THIS IF CODE WILL BE REMOVED WHEN THIS SCRIPT WILL MIGRATE TO MIL (and replaced by a probe) 
     if(instru_count-1 > 0) then
	--Add new instance event to all instructions	  	
	local lentries = Table:new(lptr:get_entries());
	local lid = lptr:get_id();
	local function condition_probe_before_branch(branch_addr)
	   local ins_opb,cond,position;
	   
	   position = mil.POSITIONT.BEFORE;
	   ins_opb,cond = instru:get_oppositebranch(branch_addr);
	   
	   if(type(ins_opb) == "userdata") then
	      Debug:info("Using conditional branch @"..string.format("%x",branch_addr));		
	      instru:linkbranch_toaddr(ins_opb,branch_addr);	
	      instru:add_insn(ins_opb,branch_addr,position);			
	   elseif(ins_opb == WRN_BRANCHINSN_REVERSEISCOND) then
	      --Nothing TODO here but after calling insert_call()	
	      Debug:info("Using conditional branch with fixed destination");
	   else 
	      --When get_oppositebranch failed
	      Message:critical("Exit loop - Error on opposite branch");
	   end	
	   
	   return ins_opb,cond;
	end	  
	
	if(event == "LO" or event == "LI") then
	   --Patch outermost loop
	   local olptr  = lptr;
	   local levent = "LO";
	   
	   while(olptr:is_outermost() == false) do
	      olptr = olptr:get_parent();
	      if(event == "LI") then 
		 levent = "LI";
		 break;
	      end
	   end
	   lid = olptr:get_id();
	   lentries = Table:new(olptr:get_entries());		
	   
	   for _,entry in pairs(lentries) do
	      --Patch preds NOT IN THE LOOP for time start and count (instances)		
	      for _,pred in pairs(entry:get_predecessors()) do
		 local last_insn     = pred:get_last_insn();
		 local ins_opb,cond;
		 
		 if(pred:get_loop() == nil or pred:get_loop():get_id() ~= lid) then 			
		    if(last_insn:is_branch_cond() == true) then
		       --At this point we are sure that pred has 2 successors (branch_cond = 2)
		       local target_block = last_insn:get_branch_target():get_block();
		       --if entry is the destination of the branch => insertion is before and we must condition the probe
		       if(target_block:get_id() == entry:get_id()) then --outer edge comming to entry	
			  position = mil.POSITIONT.BEFORE;
			  ins_opb,cond = condition_probe_before_branch(last_insn:get_address());	
		       else 
			  position = mil.POSITIONT.AFTER;
		       end
		    elseif(last_insn:is_branch_uncond() == true) then				
		       position   = mil.POSITIONT.BEFORE;
		    elseif(last_insn:is_call() == true) then			
		       position   = mil.POSITIONT.AFTER;	
		    else--Implicit edge (contiguous machine code)
		       position   = mil.POSITIONT.AFTER;
		    end
		    
		    Debug:info("Added call to fct mt_event_loopinstance @ "..string.format("0x%x",last_insn:get_address())
			       .." (block "..last_insn:get_block():get_id().." loop "..lid.." "..lptr:get_function():get_CFG_file_path()..")");
			       --instru:fctcall_new("mt_event_loopinstance",libmt,last_insn:get_address(),position);
			       --instru:fctcall_addparam_imm(instru_count-1);
			       --TODO add param to specify the type of event
		 end
	      end
	   end
	end
     end		
  end
  
	if(event == "LO") then
		Message:info("Using LO event (Loop outermost)");
	elseif(event == "LI") then
		Message:info("Using LI event (Loop innermost)");
	end
  Message:info((instru_count-1).." instructions instrumented");
  instructions_tab.nb_iinsns = instru_count-1;

  binary = string.gsub(binary,".*%/(.*)","%1");
  instructions_tab.asmfn = binary;
  --instructions_tab:tostring();
--  instructions_tab:save_output(trace_path.."/instru_meta_"..binary..".lua","instru_meta");

  --Provide the number of instrumented instructions to the runtime
  --FIXME : USE MIL and remove this section
  main_bin_init = nil;
  for fct in asmf:functions() do
	  if(fct:get_name() == "_init") then
	  	main_bin_init = fct:get_first_insn(); 		
	  end
  end
  if(type(main_bin_init) ~= "userdata") then
	  Message:critical("cannot find entry and/or exit points of the current main binary. Stopping instrumentation...");
  end   
  instru:fctcall_new("mt_load",libmt,main_bin_init:get_address(),0,0);
  instru:fctcall_addparam_imm(instructions_tab.nb_iinsns);
  Message:info("Writting instrumented binary to "..trace_path.."/"..binary.."_".."i_mtl");
  instru:modifs_commit(trace_path.."/"..binary.."_".."i_mtl");
  instru:terminate();
