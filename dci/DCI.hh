#ifndef _DCI_hh
#define _DCI_hh

#include <signal.h>
#include "event.h"
#include "msg.h"
#include "log.h"
#include "diag_lsm.h"
#include "diag_lsmi.h"
#include "diag_lsm_dci.h"
#include "diag_shared_i.h"
#include "DCIBase.hh"

class DCI: public DCIBase
{
	public:

		DCI();
		virtual ~DCI();

		virtual int setup_interface() override;

	protected:

		//unsigned char dci_rsp_pkt[4096];

		// Signal handler that handles the change in DCI channel
		static void _notify_handler(int signal, siginfo_t *info, void *unused);

		// Signal Handler that will be fired when we receive DCI data 
		static void _dci_data_handler(int signal);

		/*!
		 * Enable or disable Qualcomm DCI Logs by Log codes.
		 * Proxy function to real DCI interface.
		 * See diag_lsm_dci.c::diag_log_stream_config(...)
		 *
		 * @param	action	ENABLE / DISABLE (actually bool)
		 * @param	codes	- Log codes to add or remove
		 */
		virtual int _dci_configure_logs(int action, std::unordered_set<uint16_t> codes) override;
		
		/*!
		 * Enable or disable Qualcomm DCI Events by Log codes.
		 * See _configure_logs(...)
		 */
		virtual int _dci_configure_events(int action, std::unordered_set<int> codes) override;
		
		friend void proxy_log_handler(unsigned char *ptr, int len);
		friend void proxy_event_handler(unsigned char *ptr, int len);

		virtual void _handle_log(unsigned char *ptr, int len) override;
};

#endif
