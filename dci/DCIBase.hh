#ifndef _DCIBase_hh
#define _DCIBase_hh

#include <cstdint>
#include <functional>
#include <set>
#include <vector>
#include <unordered_set>
#include "diag_lsm.h"
#include "diag_lsm_dci.h"

typedef  std::function<void(uint8_t *, int)> log_callback_t;

extern "C" void proxy_log_handler(unsigned char *ptr, int len);
extern "C" void proxy_event_handler(unsigned char *ptr, int len);

class DCIBase
{
	public:

		virtual ~DCIBase() {};

		virtual int setup_interface() { return 0; };

		/*!
		 * Set Log codes to catch.
		 * Previous log codes are removed.
		 *
		 * @param	log_codes	- list of Qualcomm Log codes
		 */
		int set_log_codes(const std::unordered_set<uint16_t> &log_codes); 

		/*!
		 * Add new Log codes to catch. See set_log_codes()
		 */
		int add_log_codes(const std::unordered_set<uint16_t> &log_codes); 

		/*!
		 * Disables Logs by removing all Log Codes to track
		 */
		int reset_log_codes();

		void add_log_handler(log_callback_t const & c);

		/*!
		 * Set Event codes to catch.
		 * Previous log codes are removed.
		 *
		 * @param	log_codes	- list of Qualcomm Event codes
		 */
		int set_event_codes(const std::unordered_set<int> &log_codes); 

	protected:

		int _client_id;		/// DCI Client ID

		diag_dci_peripherals _periph_list;	// uint16_t

		/// Modem Log codes to catch
		std::unordered_set<uint16_t> _log_codes;

		/// List of Log callbacks to call on each log arrival
		//std::set<log_callback_t> _log_handlers;
		std::vector<log_callback_t> _log_handlers;

		virtual int _dci_configure_logs(int, std::unordered_set<uint16_t>)
		{
			return 0;	// Success
		}

		virtual int _dci_configure_events(int, std::unordered_set<int>)
		{
			return 0;	// Success
		}

		friend void proxy_log_handler(unsigned char *ptr, int len);
		friend void proxy_event_handler(unsigned char *ptr, int len);

		virtual void _handle_log(unsigned char *ptr, int len);
		static void _handle_event(unsigned char *ptr, int len);

		/*!
		 * Sets or resets Log codes to catch.
		 * See diag_lsm_dci.h
		 *
		 * @param	action	0 - disable, 1 - enable (ENABLE/DISABLE constants may be used)
		 */
		int _manipulate_log_codes(const std::unordered_set<uint16_t> &log_codes, int action );

};

#endif
