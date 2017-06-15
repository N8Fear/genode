/*
 * \brief   Thread facility
 * \author  Norman Feske
 * \date    2015-05-01
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/log.h>
#include <util/string.h>

/* core includes */
#include <platform_thread.h>
#include <platform_pd.h>

/* base-internal includes */
#include <base/internal/capability_space_sel4.h>
#include <base/internal/native_utcb.h>

#include <base/internal/kernel_debugger.h>

using namespace Genode;


/*****************************************************
 ** Implementation of the install_mapping interface **
 *****************************************************/

class Platform_thread_registry : Noncopyable
{
	private:

		List<Platform_thread> _threads;
		Lock                  _lock;

	public:

		void insert(Platform_thread &thread)
		{
			kernel_debugger_outstring("platform_thread.cc: in Ptr: insert()\n\n\n");
			Lock::Guard guard(_lock);
			_threads.insert(&thread);
		}

		void remove(Platform_thread &thread)
		{
			kernel_debugger_outstring("platform_thread.cc: in Ptr: remove()\n\n\n");
			Lock::Guard guard(_lock);
			_threads.remove(&thread);
		}

		void install_mapping(Mapping const &mapping, unsigned long pager_object_badge)
		{
			kernel_debugger_outstring("platform_thread.cc: in Ptr: install_mapping()\n\n\n");
			Lock::Guard guard(_lock);

			for (Platform_thread *t = _threads.first(); t; t = t->next()) {
				if (t->pager_object_badge() == pager_object_badge)
					t->install_mapping(mapping);
			}
		}
};


Platform_thread_registry &platform_thread_registry()
{
	kernel_debugger_outstring("platform_thread.cc: in Platform_thread_registry()\n\n\n");
	static Platform_thread_registry inst;
	return inst;
}


void Genode::install_mapping(Mapping const &mapping, unsigned long pager_object_badge)
{
	kernel_debugger_outstring("platform_thread.cc: in install_mapping()\n\n\n");
	platform_thread_registry().install_mapping(mapping, pager_object_badge);
}


/********************************************************
 ** Utilities to support the Platform_thread interface **
 ********************************************************/

static void prepopulate_ipc_buffer(addr_t ipc_buffer_phys, Cap_sel ep_sel,
                                   Cap_sel lock_sel)
{
	kernel_debugger_outstring("platform_thread.cc: in prepopulate_ipc_buffer()\n\n\n");
	/* IPC buffer is one page */
	size_t const page_rounded_size = get_page_size();

	/* allocate range in core's virtual address space */
	void *virt_addr;
	if (!platform()->region_alloc()->alloc(page_rounded_size, &virt_addr)) {
		error("could not allocate virtual address range in core of size ",
		      page_rounded_size);
		return;
	}

	/* map the IPC buffer to core-local virtual addresses */
	map_local(ipc_buffer_phys, (addr_t)virt_addr, 1);

	/* populate IPC buffer with thread information */
	Native_utcb &utcb = *(Native_utcb *)virt_addr;
	utcb.ep_sel = ep_sel.value();
	utcb.lock_sel = lock_sel.value();

	/* unmap IPC buffer from core */
	unmap_local((addr_t)virt_addr, 1);

	/* free core's virtual address space */
	platform()->region_alloc()->free(virt_addr, page_rounded_size);
}


/*******************************
 ** Platform_thread interface **
 *******************************/

int Platform_thread::start(void *ip, void *sp, unsigned int cpu_no)
{
	kernel_debugger_outstring("platform_thread.cc: in start()\n\n\n");
	ASSERT(_pd);
	ASSERT(_pager);

	/* pager endpoint in core */
	Cap_sel const pager_sel(Capability_space::ipc_cap_data(_pager->cap()).sel);

	/* install page-fault handler endpoint selector to the PD's CSpace */
	_pd->cspace_cnode(_fault_handler_sel).copy(platform_specific()->core_cnode(),
	                                           pager_sel, _fault_handler_sel);

	/* install the thread's endpoint selector to the PD's CSpace */
	_pd->cspace_cnode(_ep_sel).copy(platform_specific()->core_cnode(),
	                                _info.ep_sel, _ep_sel);

	/* install the thread's notification object to the PD's CSpace */
	_pd->cspace_cnode(_lock_sel).mint(platform_specific()->core_cnode(),
	                                  _info.lock_sel, _lock_sel);

	/*
	 * Populate the thread's IPC buffer with initial information about the
	 * thread. Once started, the thread picks up this information in the
	 * 'Thread::_thread_bootstrap' method.
	 */
	prepopulate_ipc_buffer(_info.ipc_buffer_phys, _ep_sel, _lock_sel);

	/* bind thread to PD and CSpace */
	seL4_CapData_t const guard_cap_data =
		seL4_CapData_Guard_new(0, 32 - _pd->cspace_size_log2());

	seL4_CapData_t const no_cap_data = { { 0 } };

	int const ret = seL4_TCB_SetSpace(_info.tcb_sel.value(), _fault_handler_sel.value(),
	                                  _pd->cspace_cnode_1st().sel().value(), guard_cap_data,
	                                  _pd->page_directory_sel().value(), no_cap_data);
	ASSERT(ret == 0);

	start_sel4_thread(_info.tcb_sel, (addr_t)ip, (addr_t)(sp));
	return 0;
}


void Platform_thread::pause()
{
	kernel_debugger_outstring("platform_thread.cc: in pause()\n\n\n");
	int const ret = seL4_TCB_Suspend(_info.tcb_sel.value());
	if (ret != seL4_NoError)
		error("pausing thread failed with ", ret);
}


void Platform_thread::resume()
{
	kernel_debugger_outstring("platform_thread.cc: in resume()\n\n\n");
	int const ret = seL4_TCB_Resume(_info.tcb_sel.value());
	if (ret != seL4_NoError)
		error("pausing thread failed with ", ret);
}


void Platform_thread::state(Thread_state s)
{
	kernel_debugger_outstring("platform_thread.cc: Pt in state()\n\n\n");
	warning(__PRETTY_FUNCTION__, " not implemented");
	throw Cpu_thread::State_access_failed();
}


Thread_state Platform_thread::state()
{
	kernel_debugger_outstring("platform_thread.cc: in state()\n\n\n");
	seL4_TCB   const thread         = _info.tcb_sel.value();
	seL4_Bool  const suspend_source = false;
	seL4_Uint8 const arch_flags     = 0;
	seL4_UserContext registers;
	seL4_Word  const register_count = sizeof(registers) / sizeof(registers.pc);

	int const ret = seL4_TCB_ReadRegisters(thread, suspend_source, arch_flags,
	                                       register_count, &registers);
	if (ret != seL4_NoError) {
		error("reading thread state ", ret);
		throw Cpu_thread::State_access_failed();
	}

	Thread_state state;

	/* fame registers */
	state.ip     = registers.pc;
	state.sp     = registers.sp;
	state.cpsr   = registers.cpsr;
	state.r0     = registers.r0;
	state.r1     = registers.r1;
	state.r8     = registers.r8;
	state.r9     = registers.r9;
	state.r10    = registers.r10;
	state.r11    = registers.r11;
	state.r12    = registers.r12;

	/* other integer frame registers */
	state.r2     = registers.r2;
	state.r3     = registers.r3;
	state.r4     = registers.r4;
	state.r5     = registers.r5;
	state.r6     = registers.r6;
	state.r7     = registers.r7;
  /* is in seL4_UserContext but not in genode's Cpu_state */
	//state.r14    = registers.r14;

	return state;
}


void Platform_thread::cancel_blocking()
{
	kernel_debugger_outstring("platform_thread.cc: in cancel_blocking()\n\n\n");
	seL4_Signal(_info.lock_sel.value());
}


Weak_ptr<Address_space> Platform_thread::address_space()
{
	kernel_debugger_outstring("platform_thread.cc: in address_space()\n\n\n");
	ASSERT(_pd);
	return _pd->weak_ptr();
}


void Platform_thread::install_mapping(Mapping const &mapping)
{
	kernel_debugger_outstring("platform_thread.cc: in instal_mapping()\n\n\n");
	_pd->install_mapping(mapping);
}


Platform_thread::Platform_thread(size_t, const char *name, unsigned priority,
                                 Affinity::Location, addr_t utcb)
:
	_name(name),
	_utcb(utcb),
	_pager_obj_sel(platform_specific()->core_sel_alloc().alloc())

{
	_info.init(_utcb ? _utcb : INITIAL_IPC_BUFFER_VIRT);
	platform_thread_registry().insert(*this);
}


Platform_thread::~Platform_thread()
{
	kernel_debugger_outstring("platform_thread.cc: in ~Platform_thread()\n\n\n");
	if (_pd) {
		seL4_TCB_Suspend(_info.tcb_sel.value());
		_pd->unbind_thread(this);
	}

	if (_pager) {
		Cap_sel const pager_sel(Capability_space::ipc_cap_data(_pager->cap()).sel);
		seL4_CNode_Revoke(seL4_CapInitThreadCNode, pager_sel.value(), 32);
	}

	seL4_CNode_Revoke(seL4_CapInitThreadCNode, _info.lock_sel.value(), 32);
	seL4_CNode_Revoke(seL4_CapInitThreadCNode, _info.ep_sel.value(), 32);

	_info.destruct();

	platform_thread_registry().remove(*this);
	platform_specific()->core_sel_alloc().free(_pager_obj_sel);
}


