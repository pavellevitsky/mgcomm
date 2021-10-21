/*!
 * @file DCI.cpp
 *
 * @brief Qualcomm DCI Interface communications - implementation
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <iostream>
#include <vector>
#include <errno.h>
#include "DCI.hh"

#define DIAG_SAMPLE_SIGNAL	SIGRTMIN + 15

/*!
 * C-code declares callback as:
 *	void (*func_ptr_logs)(unsigned char *ptr, int len)
 */
DCIBase *dci_object = NULL;
void proxy_log_handler(unsigned char *ptr, int len)
{
	if(dci_object)
		dci_object->_handle_log(ptr, len);
}

void proxy_event_handler(unsigned char *ptr, int len)
{
	if(dci_object)
		dci_object->_handle_event(ptr, len);
}

/*!
 * Modem LOGs handler
 */
void DCI::_handle_log(unsigned char *ptr, int len)
{
	for(auto handler: _log_handlers)
		handler(ptr, len);
}


DCI::DCI()
{
	if(dci_object)
		std::cout << "Couldn't create more than one DCI object in current implementation";
	dci_object = this;

	_client_id = -1;
	_periph_list = DIAG_CON_MPSS | DIAG_CON_APSS | DIAG_CON_LPASS;
}

DCI::~DCI()
{
	std::cout << '\n' << __func__ << ":\n\treleasing DCI interface\n";

	if(_client_id != -1)
	{
		diag_release_dci_client(&_client_id);
		Diag_LSM_DeInit();
	}
}

/**
 * \return	0 on success
 */
int DCI::setup_interface()
{
	/**
	 
	?
	 ○ MPSS - Modem Processor
	 ○ APPS - Application processor
	
	 ○ RPM - Resource and Power Management 
	 ○ WCNSS - Wireless Connectivity
	 ○ LPASS - Low Power Audio
	 https://www.slideshare.net/cisoplatform7/a-journey-into-hexagon-dissecting-qualcomm-basebands
	*/

	std::cout << '\n' << __func__ << ":\n\tSetting up DCI interface...";

	int signal_type = SIGCONT;

	// Signal handling to handle SSR
	struct sigaction notify_action;
	sigemptyset(&notify_action.sa_mask);
	notify_action.sa_sigaction = _notify_handler;
	 //Use SA_SIGINFO to denote we are expecting data with the signal 
	notify_action.sa_flags = SA_SIGINFO;
	sigaction(signal_type, &notify_action, NULL);

	/* Signal handling for DCI Data */
	struct sigaction dci_data_action;
	sigemptyset(&dci_data_action.sa_mask);
	dci_data_action.sa_handler = _dci_data_handler;
	sigaction(DIAG_SAMPLE_SIGNAL, &dci_data_action, NULL);

	// This should be done only once !
	/* Registering with Diag which gives the client a handle to the Diag driver */
	if(!Diag_LSM_Init(NULL))
	{
		DIAG_LOGE(" Couldn't register with Diag LSM, errno: %d\n", errno);
		return -1;
	}

	/* From dciscan, check this later:
	
    //The following 2 lines are for sending command to submodules other than diag
	DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY(128, 33, test_tbl_4);
    DIAGPKT_DISPATCH_TABLE_REGISTER_V2_DELAY(75, 33, test_tbl_4);
	*/

	// Register DCI client and get ID
	int channel = 0;
	int err;
	err = diag_register_dci_client(&_client_id, &_periph_list, channel, &signal_type);
	if (err != DIAG_DCI_NO_ERROR) {
		DIAG_LOGE(" Could not register with DCI, err: %d, errno: %d\n", err, errno);
		return -1;
	} else
		DIAG_LOGE(" Successfully registered with DCI, client ID = %d\n", _client_id);

	err = diag_register_dci_signal_data(_client_id, DIAG_SAMPLE_SIGNAL);
	if (err != DIAG_DCI_NO_ERROR) {
		DIAG_LOGE(" Could not register signal for DCI Data, err: %d, errno: %d\n", err, errno);
		return -1;
	}

	/* Getting supported Peripherals list*/
	DIAG_LOGE(" DCI Status on Processors:\n");
	err = diag_get_dci_support_list(&_periph_list);
	if (err != DIAG_DCI_NO_ERROR) {
		printf(" Could not get support list, err: %d, errno: %d\n", err, errno);
		return -1;
	}
	DIAG_LOGE("   MPSS:\t ");
	DIAG_LOGE((_periph_list & DIAG_CON_MPSS) ? "UP\n" : "DOWN\n");
	DIAG_LOGE("   LPASS:\t ");
	DIAG_LOGE((_periph_list & DIAG_CON_LPASS) ? "UP\n" : "DOWN\n");
	DIAG_LOGE("   WCNSS:\t ");
	DIAG_LOGE((_periph_list & DIAG_CON_WCNSS) ? "UP\n" : "DOWN\n");
	DIAG_LOGE("   APSS:\t ");
	DIAG_LOGE((_periph_list & DIAG_CON_APSS) ? "UP\n" : "DOWN\n");

	/* Initializing Log and Event streaming by registering
	   listeners - This is required to receive Logs and Events */
	DIAG_LOGE(" Opening Data Stream, registering listeners\n");

	err = diag_register_dci_stream(proxy_log_handler, proxy_event_handler);
	if (err != DIAG_DCI_NO_ERROR)
		DIAG_LOGE(" Unable to register DCI stream, err: %d, errno: %d\n", err, errno);

	printf("\n");

	return 0;
}

int DCI::_dci_configure_logs(int action, std::unordered_set<uint16_t> codes)
{
	// Make a C array of Log codes
	std::vector<uint16_t> log_codes_vector(codes.begin(), codes.end());

	int result =  diag_log_stream_config(
			_client_id, action,
			log_codes_vector.data(), log_codes_vector.size());

	if (result != DIAG_DCI_NO_ERROR)
	{
		DIAG_LOGE(" Error configuring LOG list, err: %d, errno: %d\n", 
				result, errno);
		return 1;
	}

	return 0;
}

int DCI::_dci_configure_events(int action, std::unordered_set<int> codes)
{
	// Make a C array of Event codes
	std::vector<int> event_codes_vector(codes.begin(), codes.end());


	int result = diag_event_stream_config(
			_client_id, action,
			event_codes_vector.data(), event_codes_vector.size());

	if (result != DIAG_DCI_NO_ERROR)
	{
		DIAG_LOGE(" Error configuring EVENT list, err: %d, errno: %d\n", 
				result, errno);
		return 1;
	}

	return 0;
}

// Signal handler that handles the change in DCI channel
//   static
void DCI::_notify_handler(int signal, siginfo_t *info, void *unused)
{
	(void)unused;

	if (info) {
		int err;
		diag_dci_peripherals list = 0;

		DIAG_LOGE("diag: In %s, signal %d received from kernel, data is: %x\n",
			__func__, signal, info->si_int);

		if (info->si_int & DIAG_STATUS_OPEN) {
			if (info->si_int & DIAG_CON_MPSS) {
				DIAG_LOGE("diag: DIAG_STATUS_OPEN on DIAG_CON_MPSS\n");
			} else if (info->si_int & DIAG_CON_LPASS) {
				DIAG_LOGE("diag: DIAG_STATUS_OPEN on DIAG_CON_LPASS\n");
			} else {
				DIAG_LOGE("diag: DIAG_STATUS_OPEN on unknown peripheral\n");
			}
		} else if (info->si_int & DIAG_STATUS_CLOSED) {
			if (info->si_int & DIAG_CON_MPSS) {
				DIAG_LOGE("diag: DIAG_STATUS_CLOSED on DIAG_CON_MPSS\n");
			} else if (info->si_int & DIAG_CON_LPASS) {
				DIAG_LOGE("diag: DIAG_STATUS_CLOSED on DIAG_CON_LPASS\n");
			} else {
				DIAG_LOGE("diag: DIAG_STATUS_CLOSED on unknown peripheral\n");
			}
		}
		err = diag_get_dci_support_list(&list);
		if(err != DIAG_DCI_NO_ERROR) {
			DIAG_LOGE("diag: could not get support list, err: %d\n", err);
		}
		/* This will print out all peripherals supporting DCI */
		if (list & DIAG_CON_MPSS)
			DIAG_LOGE("diag: Modem supports DCI\n");
		if (list & DIAG_CON_LPASS)
			DIAG_LOGE("diag: LPASS supports DCI\n");
		if (list & DIAG_CON_WCNSS)
			DIAG_LOGE("diag: RIVA supports DCI\n");
		if (list & DIAG_CON_APSS)
			DIAG_LOGE("diag: APSS supports DCI\n");
		if (!list)
			DIAG_LOGE("diag: No current dci support\n");
	} else {
		DIAG_LOGE("diag: In %s, signal %d received from kernel, but no info value, info: 0x%p\n",
			__func__, signal, info);
	}
}

/*!
 * Singal Handler that will be fired when we receive DCI data 
 *
 * @note	static member
 */
void DCI::_dci_data_handler(int signal)
{
	(void)signal;

	std::cout << '\n' << __func__ ;

	/* Do something here when you receive DCI data. */

	/* This is usually for holding wakelocks when the
	   clients are running in Diag Non Real Time mode
	   or when they know the Apps processor is in deep
	   sleep but they still need to process DCI data.

	   Please Note: Wakelocks must be released
	   after processing the data in the respective
	   response/log/event handler. Failure to do so
	   will affect the power consumption of the Apps
	   processor. */
}
